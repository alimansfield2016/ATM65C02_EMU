#pragma once

#include <personal/olcPixelGameEngine.h>
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;

class Bus;
class atmKeyboard;

class atm6522
{
private:
	/* data */
	olc::Sprite LEDs = olc::Sprite(6*8+1, 16*8+1);
public:
	atm6522(/* args */);
	~atm6522();

	void reset();
	void write(u16 addr, u8 data);
	void read(u16 addr, u8& data);
	void clock();
	u16 getOutput();
	void redraw();
	olc::Sprite& GetScreen();
	void ConnectBus(Bus* b);
	void ConnectKeyboard(atmKeyboard* kb);

	void cb1(bool cb2);
	void setA(u8 in);
	void clrA(u8 in);
	uint8_t getA();
	void setB(u8 in);
	void clrB(u8 in);
	uint8_t getB();

	void setCA(bool ca1, bool ca2);
	void setCB(bool cb1, bool cb2);

private:
	enum IFR_FLAG{
		CA2 = 1<<0,
		CA1 = 1<<1,
		C8S = 1<<2,
		CB2 = 1<<3,
		CB1 = 1<<4,
		T2TO= 1<<5,
		T1TO= 1<<6,
		IRQ = 1<<7,
	};
	Bus* bus;
	atmKeyboard* kb;

	u8 DDRA, DDRB, PORTA, PORTB;
	u16 T1C, T1L;
	u16 T2C;
	u16 T2C_prev;
	u8 T2L;
	u8 SR;
	u8 sr_count;
	u8 ca, cb;
	u8 ACR, PCR, IFR, IER = 0x80;
	olc::Pixel ON  = olc::Pixel(255, 0, 0);
	olc::Pixel OFF = olc::Pixel( 15, 0, 0);
};

