
#include "include/atm6522.h"
#include "include/Bus.h"
#include "include/atmKeyboard.h"

#define a atm6522

a::a(/* args */)
{
}

a::~a()
{
}

u16 a::getOutput(){
	return PORTA | PORTB<<8;
}

void a::reset(){
	DDRA = DDRB = PORTA = PORTB = T1C = T1L = T2C = T2L = T2C_prev = SR = sr_count = ACR = PCR = IFR = IER = 0;
}

void a::clock(){
	//deal with the timers/counters
	bool interrupt = false;
	u8 prev_ifr = IFR;
	bool t1z = false, t2z = false;
	if(T1C > 0){
		T1C--;
		if(T1C == 0)
			t1z = true;
	}
	if(!(ACR & 0x20)){	//if in one shot mode, decrement then roll over
		T2C--;
	}
	if(T2C == 0 && T2C != T2C_prev)
		t2z = true;
	if(t1z){
		IFR |= T1TO; //set IFR_6
		switch (ACR>>6)
		{
		case 0x00:
			break;
		case 0x01:
			T1C = T1L;
			break;
		case 0x02:
			PORTB |= 0x80;
			break;
		case 0x03:
			PORTB ^= 0x80;
			T1C = T1L;
			break;
		
		default:
			break;
		}
	}

	if(t2z)
		IFR |= T2TO; //set IFR5
	
	if((IER & IFR)&~0x80){
		//interrupt!!!
		IFR |= 0x80;
	}else{
		//no interrupt :(
		IFR &= ~0x80;
	}
	if(IFR&0x80)
		bus->IRQ();

	T2C_prev = T2C;
}	

void a::redraw(){
	for(int i = 0; i < 6*8+1; i++){
		for(int j = 0; j < 16*8+1; j++){
			LEDs.SetPixel(i, j, olc::DARK_BLUE);
		}
	}
	u8 a_out = PORTA & DDRA;
	u8 b_out = PORTB & DDRB;
	u8 a_in = PORTA & ~DDRA;
	u8 b_in = PORTB & ~DDRB;
	u16 out  = a_out | b_out<<8;
	u16 in  = a_in | b_in<<8;
	u16 ddr = DDRA | DDRB << 8;
	for(int i = 0; i < 1*8+1; i++){
		for(int j = 0; j < 8*16+1; j++){
			if(!(i%8) || !(j%8)){
				LEDs.SetPixel(i, j, olc::DARK_BLUE);
			}else if(i%8 == 1){
				LEDs.SetPixel(i, j, ddr&(1<<(j>>3))?olc::GREEN:olc::BLUE);
			}else{
				if(out&(1<<(j>>3)))
					LEDs.SetPixel(i, j, ON);
				else if(in&(1<<(j>>3)))
					LEDs.SetPixel(i, j, olc::YELLOW);
				else
					LEDs.SetPixel(i, j, OFF);
			}
		}
	}
	for(int i = 1*8+1; i < 2*8+1; i++){
		for(int j = 0; j < 8*16+1; j++){
			if(!(i%8) || !(j%8)){
				LEDs.SetPixel(i, j, olc::DARK_BLUE);
			}else if(i%8 == 1){
				LEDs.SetPixel(i, j, T1L&(1<<(15-(j>>3)))?olc::GREEN:olc::BLUE);
			}else{
				LEDs.SetPixel(i, j, T1C&(1<<(15-(j>>3)))?ON:OFF);
			}
		}
	}
	for(int i = 2*8+1; i < 3*8+1; i++){
		for(int j = 0; j < 16*8+1; j++){
			if(!(i%8) || !(j%8)){
				LEDs.SetPixel(i, j, olc::DARK_BLUE);
			}else if(i%8 == 1 && j >= 8*8){
				LEDs.SetPixel(i, j, T2L&(1<<(15-(j>>3)))?olc::GREEN:olc::BLUE);
			}else{
				LEDs.SetPixel(i, j, T2C&(1<<(15-(j>>3)))?olc::GREEN:OFF);
			}
		}
	}
	for(int i = 3*8+1; i < 4*8+1; i++){
		for(int j = 0; j < 8*8+1; j++){
			if(!(i%8) || !(j%8)){
				LEDs.SetPixel(i, j, olc::DARK_BLUE);
			// }else if(i%8 == 1 && j >= 8*8){
			// 	LEDs.SetPixel(i, j, SR&(1<<(7-(j>>3)))?olc::GREEN:olc::BLUE);
			}else{
				LEDs.SetPixel(i, j, PCR&(1<<(7-(j>>3)))?olc::BLUE:OFF);
			}
		}
	}
	for(int i = 3*8+1; i < 4*8+1; i++){
		for(int j = 8*8+1; j < 16*8+1; j++){
			if(!(i%8) || !(j%8)){
				LEDs.SetPixel(i, j, olc::DARK_BLUE);
			// }else if(i%8 == 1 && j >= 8*8){
			// 	LEDs.SetPixel(i, j, SR&(1<<(7-(j>>3)))?olc::GREEN:olc::BLUE);
			}else{
				LEDs.SetPixel(i, j, ACR&(1<<(15-(j>>3)))?olc::BLUE:OFF);
			}
		}
	}
	for(int i = 4*8+1; i < 5*8+1; i++){
		for(int j = 8*8+1; j < 16*8+1; j++){
			if(!(i%8) || !(j%8)){
				LEDs.SetPixel(i, j, olc::DARK_BLUE);
			// }else if(i%8 == 1 && j >= 8*8){
			// 	LEDs.SetPixel(i, j, SR&(1<<(7-(j>>3)))?olc::GREEN:olc::BLUE);
			}else{
				LEDs.SetPixel(i, j, SR&(1<<(15-(j>>3)))?olc::GREEN:OFF);
			}
		}
	}
	for(int i = 4*8+1; i < 5*8+1; i++){
		for(int j = 0; j < 8*8+1; j++){
			if(!(i%8) || !(j%8)){
				LEDs.SetPixel(i, j, olc::DARK_BLUE);
			}else if(i%8 == 1){
				LEDs.SetPixel(i, j, IER&(1<<(7-(j>>3)))?olc::GREEN:olc::RED);
			}else{
				LEDs.SetPixel(i, j, IFR&(1<<(7-(j>>3)))?olc::GREEN:OFF);
			}
		}
	}
}

olc::Sprite& a::GetScreen(){
	return LEDs;
}

void a::read(u16 addr, u8& data){
	u8 new_addr = addr & 0x0F;
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
	case 0x04:
		data = T1C&0x00FF;
		IFR &= ~(1<<6);	//clear IFR6
		break;
	case 0x05:
		data = T1C>>8;
		break;
	case 0x06:
		data = T1L&0x00FF;
		break;
	case 0x07:
		data = T1L>>8;
		break;
	case 0x08:
		data = T2C&0x00FF;
		IFR &= ~(1<<5);	//clear IFR5
		break;
	case 0x09:
		data = T2C>>8;
		break;
	case 0x0A:
		data = SR;
		break;
	case 0x0B:
		data = ACR;
		break;
	case 0x0C:
		data = PCR;
		break;
	case 0x0D:
		data = IFR;
		break;
	case 0x0E:
		data = IER|0x80;
		break;
	case 0x0F:
		data = PORTA;
		break;
	default:
		break;
	}
}

void a::write(u16 addr, u8 data){
	u8 new_addr = addr & 0x0F;
	switch (new_addr)
	{
	case 0x00:
		PORTB = data&DDRB;
		break;
	case 0x01:
		PORTA = data&DDRA;//clone list, not reference
		break;
	case 0x02:
		DDRB = data;
		break;
	case 0x03:
		DDRA = data;
		break;
	case 0x04:
		T1L = (T1L&0xFF00)|data;
		break;
	case 0x05:
		T1L = (T1L&0x00FF)|(data<<8);
		T1C = T1L;
		IFR &= ~(1<<6);	//clear IFR6
		if(ACR & 0x80)
			PORTB &= ~0x80;
		break;
	case 0x06:
		T1L = (T1L&0xFF00)|data;
		break;
	case 0x07:
		T1L = (T1L&0x00FF)|(data<<8);
		IFR &= ~(1<<6);	//clear IFR6
		break;
	case 0x08:
		T2L = data;
		break;
	case 0x09:
		T2C = (data<<8)|T2L;
		IFR &= ~(1<<5); //clear IFR5
		break;
	case 0x0A:
		SR = data;
		break;
	case 0x0B:
		ACR = data;
		break;
	case 0x0C:
		PCR = data;
		break;
	case 0x0D:
		IFR &= (~data|0x80);
		break;
	case 0x0E:
		if(data&0x80){
			//set
			IER |= data|0x80;
		}else{
			//clear
			IER &= ~data|0x80;
		}
		break;
	case 0x0F:
		PORTA = data;
	
	default:
		break;
	}
}

void a::ConnectBus(Bus* b){
	bus = b;
}
void a::ConnectKeyboard(atmKeyboard* _kb){
	kb = _kb;
}

void a::cb1(bool cb2 = false){
	if(PCR & 1<<4){
		cb = 0x01 | cb2<<1;
	}else{
		cb = cb2<<1;
	}
	if((ACR & 0x1C) == 0x0C){
		//shift in under control of CB1
		SR = SR << 1 | cb2;
		sr_count++;
		if(sr_count == 8){
			sr_count = 0;
			IFR |= C8S;
		}
	}
	// sr_count++;

}

void a::setA(u8 in){
	PORTA |= in&~DDRA;
}
void a::clrA(u8 in){
	PORTA &= ~(in&~DDRA);
}
void a::setB(u8 in){
	PORTB |= in&~DDRB;
}
void a::clrB(u8 in){
	PORTB &= ~(in&~DDRB);
}


void a::setCA(bool ca1, bool ca2){

	if(PCR&0x01){
		//rising edge triggered
		if(ca1 && !(ca&0x01))
			IFR |= CA1;
	}else{
		//falling edge triggered
		if(!ca1 && (ca&0x01))
			IFR |= CA1;
	}
	ca = ca1 | ca2<<1;
}
void a::setCB(bool cb1, bool cb2){
	if((ACR & 0x1C) == 0x0C && cb1 && !(cb&0x01)){
		//shift in under control of CB1
		SR = SR << 1 | cb2;
		sr_count++;
		if(sr_count == 8){
			sr_count = 0;
			IFR |= C8S;
		}
	}
	if(PCR&0x10){
		//rising edge triggered
		if(cb1 && !(cb&0x01))
			IFR |= CB1;
	}else{
		//falling edge triggered
		if(!cb1 && (cb&0x01))
			IFR |= CB1;
	}
	cb = cb1 | cb2<<1;
}