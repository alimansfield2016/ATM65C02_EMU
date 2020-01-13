#include "include/LCD.h"
#include "include/cgrom.h"
#ifdef LCD_LOG
#include <stdio.h>
#endif
LCD::LCD(){
	for(int i = 1; i < n_w*6; i++){
		for(int j = 1; j < n_h*9; j++){
			lcd_display.SetPixel(i-1, j-1, (!(i%6)|!(j%9))?olc::Pixel(0, 0, 0):(j/9)%2?BG:FG);
		}
	}
}
LCD::~LCD(){

}

uint8_t LCD::read(uint16_t addr, uint8_t &data){
	
}
uint8_t LCD::write(uint16_t addr, uint8_t data){
	// if(!(addr >= 0x6400 && addr < 0x6800))
	// 	return 0;
	uint8_t rs = addr%2;
	uint8_t disp_sel = (addr>>1)&0x3;
	for(int disp = 0; disp < n_disp; disp++){
		if(disp_sel&(1<<disp)){
			if(rs){	// data
				if(b_ddram_cgram[disp]){
					//ddram
					n_ddram[disp][n_ddram_addr[disp]] = data;
					//handle increment mode
					if(b_addr_inc[disp]){
						n_ddram_addr[disp]++;
						if(b_display_shift[disp])
							n_disp_shift[disp]++;
					}else{
						n_ddram_addr[disp]--;
						if(b_display_shift[disp])
							n_disp_shift[disp]--;
					}
					if(n_disp_shift[disp]>0x28)	
						n_disp_shift[disp]-=0x28;
					if(n_disp_shift[disp]<0)	
						n_disp_shift[disp]+=0x28;
					if((n_ddram_addr[disp]&~0xC0) >= 0x28 && b_addr_inc[disp]){
						n_ddram_addr[disp] = (n_ddram_addr[disp]&0xC0) + 0x40;
					}
					if((n_ddram_addr[disp]&~0xC0) >= 0x28 && !b_addr_inc[disp]){
						n_ddram_addr[disp] = (n_ddram_addr[disp]&0xC0)+0x27;
					}
					n_ddram_addr[disp] &= ~0x80;
				}
			}else{ // instruction
				bool msb_found = 0;
				uint8_t command = data;
				for(int n = 0x80; n > 0; n>>=1){
					if(msb_found)
						command &= ~n;
					if(command&n)
						msb_found = true;
				}
				switch (command)
				{
				case clear_disp:
					for(int i = 0; i < 128; i++)
						n_ddram[disp][i] = 0;

				case ret_home:
					n_ddram_addr[disp] = 0;
					n_disp_shift[disp] = 0;
					b_ddram_cgram[disp] = true;
					break;

				case entry_mode:
					b_addr_inc[disp] = data&(1<<1);
					b_display_shift[disp] = data&(1<<0);
					break;

				case display_control:
					b_disp_on[disp] = data&(1<<2);
					b_cursor_on[disp] = data&(1<<1);
					b_cursor_blink[disp] = data&(1<<0);
					break;

				case cursor_shift:
					if(data&(1<<3)){
						//shift screen
						if(data&(1<<2)){
							//right
							n_disp_shift[disp]++;
							if(n_disp_shift[disp]>0x28)	
								n_disp_shift[disp]-=0x28;
						}else{
							//left
							n_disp_shift[disp]--;
							if(n_disp_shift[disp]<0)	
								n_disp_shift[disp]+=0x28;
						}
						
					}else{
						//shift cursor
						if(data&(1<<2)){
							n_ddram_addr[disp]++;
							if((n_ddram_addr[disp]&~0xC0) >= 0x28){
								n_ddram_addr[disp] = (n_ddram_addr[disp]&0xC0) + 0x40;
							}
						}else{
							n_ddram_addr[disp]--;
							if((n_ddram_addr[disp]&~0xC0) >= 0x28){
								n_ddram_addr[disp] = (n_ddram_addr[disp]&0xC0)+0x27;
							}
						}
					}

					break;

				case func_set:
					b_initialised[disp] = true;
					break;

				case set_cgram_addr:
					n_cgram_addr[disp] = data&0x3F;
					b_ddram_cgram[disp] = false;
					break;

				case set_ddram_addr:
					n_ddram_addr[disp] = data&0x7F;
					b_ddram_cgram[disp] = true;
					break;
				
				default:
					break;
				}
			}
		}
	}
	#ifdef LCD_LOG
	printf("LCD Written to $%4x:%2x, ", addr, data);
	fflush(stdout);
	printf("Display: %c%c, ", disp_sel&1?'1':'0', disp_sel&2?'1':'0');
	printf("%s, ", rs?"Data":"Instruction");
	rs?printf("%c", data):0;
	printf("\n");
	#endif
}

olc::Sprite& LCD::GetScreen(){
	return lcd_display;
}

void LCD::redraw(float f_time){
	for(int i = 0; i < n_h; i++){
		uint8_t disp = (i>>1);
		for(int j = 0; j < n_w; j++){
			for(int n = 0; n < 8; n++){
				uint8_t chr = CGROM[n_ddram[disp][0x40*(i%2)+ (j + n_disp_shift[disp])%0x28]*0x10 + n];

				for(int k = 0; k < 5; k++){
					if(n==7 && b_initialised[disp] && !b_cursor_blink[disp] &&  b_cursor_on[disp] && n_ddram_addr[disp] == (i%2)*0x40+j)
						lcd_display.SetPixel(j*6+4-k, i*9+n, FG);
						#ifdef CURSOR_FULL_BLINK
					else if((fmod(f_time,1.0) < 0.5) && b_initialised[disp] && b_cursor_blink[disp] &&b_cursor_on[disp] && n_ddram_addr[disp] == i*0x40+j)
						#else
					else if(n==7 && (fmod(f_time,1.0) < 0.5) && b_initialised[disp] && b_cursor_blink[disp] &&b_cursor_on[disp] && n_ddram_addr[disp] == (i%2)*0x40+j)
						#endif	
						lcd_display.SetPixel(j*6+4-k, i*9+n, FG);
					else
						lcd_display.SetPixel(j*6+4-k, i*9+n,b_initialised[i/n_disp]?chr&0x01?FG:BG:(i%2)?BG:FG );
					chr>>=1;
				}
			}
		}
	}
}

void LCD::unInit(){
	for(auto &b : b_initialised){
		b = false;
	}
}