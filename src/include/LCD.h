#include <stdint.h>
#include <stdbool.h>
#include <personal/olcPixelGameEngine.h>

class LCD{
public:
	LCD();
	~LCD();

public:
	uint8_t read(uint16_t addr, uint8_t &data);
	uint8_t write(uint16_t addr, uint8_t data);
	olc::Sprite& GetScreen();//return lcd_display	
	void redraw(float f_time);
	void unInit();

private:
	enum FUNCTION{
		clear_disp = 0x01,
		ret_home = 0x02,
		entry_mode = 0x04,
		display_control = 0x08,
		cursor_shift = 0x10,
		func_set = 0x20,
		set_cgram_addr = 0x40,
		set_ddram_addr = 0x80
	};

	static const int n_w = 40, n_h = 4;
	static const int n_disp = 2;	//one display per two lines
	
	uint8_t n_ddram_addr[n_disp], n_cgram_addr[n_disp];
	uint8_t n_ddram[n_disp][128], n_cgram[n_disp][64];
	uint8_t n_disp_shift[n_disp];

	bool b_ddram_cgram[n_disp] = {true};
	bool b_initialised[n_disp] = {false};
	bool b_addr_inc[n_disp] = {false};
	bool b_display_shift[n_disp] = {false};
	bool b_disp_on[n_disp] = {false};
	bool b_cursor_on[n_disp] = {false};
	bool b_cursor_blink[n_disp] = {false};

	olc::Sprite lcd_display = olc::Sprite(n_w*6-1, n_h*9-1); 
	olc::Pixel BG = olc::Pixel(0, 0, 255);
	olc::Pixel FG = olc::Pixel(255, 255, 255);

};