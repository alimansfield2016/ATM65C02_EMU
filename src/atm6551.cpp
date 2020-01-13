#include "include/atm6551.h"
#include "include/Bus.h"

#define a atm6551

a::a(){
	STAT = 0x10;
	pSTAT = STAT;
	RCount = 0;
	TCount = 0;
}

a::~a(){

}

void a::clock(){
	DivCounter--;
	RDivCounter--;
	if(DivCounter == 0){
		if(CTRL & RCS){	//Rx clock source same as Tx

		}

		if(TSR | TCount){
			if(TCount == 0){
				//start bit
			}else if(TCount > 0){
				switch (CTRL&WL>>5)
				{
					case 0x00:/* code */
						//1, 2, 3, 4, 5, 6, 7, 8 
						TSR >>= 1;
						if(TCount == 8){
							TSR = 0;
							CTRL&WL?TCount = -2 : TCount = -1;
							TCount--;
						}
						break;
					case 0x01:
						//1, 2, 3, 4, 5, 6, 7
						TSR >>= 1;
						if(TCount == 7){
							TSR = 0;
							CTRL&WL?TCount = -2 : TCount = -1;
							TCount--;
						}
						break;
					case 0x02:
						//1, 2, 3, 4, 5, 6
						TSR >>= 1;
						if(TCount == 6){
							TSR = 0;
							CTRL&WL?TCount = -2 : TCount = -1;
							TCount--;
						}
						break;
					case 0x03:
						//1, 2, 3, 4, 5
						TSR >>= 1;
						if(TCount == 5){
							TSR = 0;
							CTRL&WL?TCount = -2 : TCount = -1;
							TCount--;
						}
						break;
				
					default:
						break;
				}
			}else{
				//stop bit(s)
			}
			TCount++;
		}
		switch (CTRL & 0x0F){
			case 0x00:
				DivCounter = 16;
				break;
			case 0x01:
				DivCounter = 36864;
				break;
			case 0x02:
				DivCounter = 24576;
				break;
			case 0x03:
				DivCounter = 16769;
				break;
			case 0x04:
				DivCounter = 13704;
				break;
			case 0x05:
				DivCounter = 12288;
				break;
			case 0x06:
				DivCounter = 6144;
				break;
			case 0x07:
				DivCounter = 3072;
				break;
			case 0x08:
				DivCounter = 1536;
				break;
			case 0x09:
				DivCounter = 1024;
				break;
			case 0x0A:
				DivCounter = 768;
				break;
			case 0x0B:
				DivCounter = 512;
				break;
			case 0x0C:
				DivCounter = 384;
				break;
			case 0x0D:
				DivCounter = 256;
				break;
			case 0x0E:
				DivCounter = 192;
				break;
			case 0x0F:
				DivCounter = 96;
				break;
			
			default:
				break;
		}
	}
	if(!RDivCounter&0x0F && !CTRL&RCS){	// Rx acting on external 1/16 clock

	}

	if((STAT&~pSTAT)&INT){
		bus->IRQ();
		STAT |= IRQ;
	}else{
		STAT &= ~IRQ;
	}
	pSTAT = STAT;
}

void a::ConnectBus(Bus* b){
	bus = b;
}

void a::RxData(u8 data){
	if(STAT & RDRF){
		//data still unread
		STAT |= OVR;
	}
	RDR = data;
}

void a::write(u16 addr, u8 data){
	u8 new_addr = addr&0x03;
	switch (new_addr)
	{
	case 0x00:
		TDR = data;
		TSR = TDR;		//Bug with the 65c51N TSR immediately populated
		STAT |= TDRE;	//Bug with the 65c51N TSR immediately populated
		TDR = 0;		//Bug with the 65c51N TSR immediately populated
		STAT &= ~TDRE;	//Bug with the 65c51N TSR immediately populated
		break;
	case 0x01:
		//Programmed reset
		break;
	case 0x02:
		CMD = data;
		break;
	case 0x03:
		CTRL = data;
	
	default:
		break;
	}
}

void a::read(u16 addr, u8 &data){
	u8 new_addr = addr&0x03;
	switch (new_addr)
	{
	case 0x00:
		data = RDR;
		STAT &= ~(RDRF|OVR|FRE|PAR);
		RDR = 0;
		break;
	case 0x01:
		data = STAT;
		STAT &= ~IRQ; //clear interrupt flag
		break;
	case 0x02:
		data = CMD;
		break;
	case 0x03:
		data = CTRL;
		break;
	
	default:
		break;
	}
}