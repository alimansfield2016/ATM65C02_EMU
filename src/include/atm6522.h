#pragma once

#include <personal/olcPixelGameEngine.h>

class atm6522
{
private:
	/* data */
	olc::Sprite LEDs = olc::Sprite(16*1+1, 8*16+1);
public:
	atm6522(/* args */);
	~atm6522();

	void write(uint16_t addr, uint8_t data);
	void read(uint16_t addr, uint8_t& data);

	uint16_t getOutput();
	void redraw();
	olc::Sprite& GetScreen();

private:
	uint8_t DDRA, DDRB, PORTA, PORTB;
	olc::Pixel ON  = olc::Pixel(255, 0, 0);
	olc::Pixel OFF = olc::Pixel( 15, 0, 0);
};

