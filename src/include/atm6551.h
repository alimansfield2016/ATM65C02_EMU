#pragma once
#include <stdint.h>
typedef uint8_t u8;
typedef int8_t i8;
typedef uint16_t u16;
class Bus;

class atm6551{
public:
	atm6551();
	~atm6551();

public:
	void write(u16 addr, u8 data);
	void read(u16 addr, u8 &data);
	void clock();
	// void xtalClock();
	void ConnectBus(Bus* b);

	void RxData(u8 data);
	u8 TxData();
private:
	enum FLAG{
		IRQ = 1<<7,
		DSR = 1<<6,
		DCB = 1<<5,
		TDRE= 1<<4,
		RDRF= 1<<3,
		OVR = 1<<2,
		FRE = 1<<1,
		PAR = 1<<0,
	};
	enum MASK{
		INT = 0x78,
	};
	enum CTRL_MASK{
		SBR0 = 1<<0,
		SBR1 = 1<<1,
		SBR2 = 1<<2,
		SBR3 = 1<<3,
		SBR  = 0x0F,
		RCS  = 1<<4,
		WL0  = 1<<5,
		WL1  = 1<<6,
		WL   = 0x60,
		SBN  = 1<<6,
	};
	u8 TDR, TSR, RDR, RSR, pSTAT, STAT, CMD, CTRL;
	i8 RCount, TCount;
	u16 DivCounter;
	u8 RDivCounter;
	Bus* bus;
};