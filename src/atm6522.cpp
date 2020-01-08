
#include "include/atm6522.h"

atm6522::atm6522(/* args */)
{
}

atm6522::~atm6522()
{
}

uint16_t atm6522::getOutput(){
	return PORTA | PORTB<<8;
}

void atm6522::redraw(){
	uint8_t a_out = PORTA & DDRA;
	uint8_t b_out = PORTB & DDRB;
	uint16_t out  = a_out | b_out<<8;
	uint16_t ddr = DDRA | DDRB << 8;
	for(int i = 0; i < 16*1+1; i++){
		for(int j = 0; j < 8*16+1; j++){
			if(!(i%16) || !(j%8)){
				LEDs.SetPixel(i, j, olc::DARK_BLUE);
			}else if(i == 1){
				LEDs.SetPixel(i, j, ddr&(1<<(j>>3))?olc::GREEN:olc::BLUE);
			}else{
				LEDs.SetPixel(i, j, out&(1<<(j>>3))?ON:OFF);
			}
		}
	}
}

olc::Sprite& atm6522::GetScreen(){
	return LEDs;
}

void atm6522::read(uint16_t addr, uint8_t& data){
	uint8_t new_addr = addr & 0x0F;
	switch (new_addr)
	{
	case 0x00:
		data = PORTB;
		break;
	case 0x01:
		data = PORTA;
		break;
	case 0x02:
		data = DDRB;
		break;
	case 0x03:
		data = DDRA;
		break;
	
	default:
		break;
	}
}

void atm6522::write(uint16_t addr, uint8_t data){
	uint8_t new_addr = addr & 0x0F;
	switch (new_addr)
	{
	case 0x00:
		PORTB = data;
		break;
	case 0x01:
		PORTA = data;
		break;
	case 0x02:
		DDRB = data;
		break;
	case 0x03:
		DDRA = data;
		break;
	
	default:
		break;
	}
}