
#include "include/atmKeyboard.h"
#include "include/atm6522.h"
#include "include/olcPixelGameEngine.h"

#include <stdio.h>

extern olc::PixelGameEngine demo;


atmKeyboard::atmKeyboard(/* args */){
	//yes i know this is bad to have so many maps, but what can you do?
	ScanCodeMap[olc::ESCAPE] = 110;												ScanCodeMap[olc::F1] = 112;		ScanCodeMap[olc::F2] = 113;		ScanCodeMap[olc::F3] = 114;		ScanCodeMap[olc::F4] = 115;							ScanCodeMap[olc::F5] = 116;		ScanCodeMap[olc::F6] = 117;		ScanCodeMap[olc::F7] = 118;		ScanCodeMap[olc::F8] = 119;					ScanCodeMap[olc::F9] = 120;		ScanCodeMap[olc::F10] = 121;		ScanCodeMap[olc::F11] = 122;		ScanCodeMap[olc::F12] = 123;				ScanCodeMap[olc::Key::PRNTSCR] = 124;	ScanCodeMap[olc::SCROLL] = 125;		ScanCodeMap[olc::PAUSE] = 126;
	ScanCodeMap[olc::BACKTICK] = 1;		ScanCodeMap[olc::K1] = 2;		ScanCodeMap[olc::K2] = 3;		ScanCodeMap[olc::K3] = 4; 		ScanCodeMap[olc::K4] = 5;		ScanCodeMap[olc::K5] = 6;		ScanCodeMap[olc::K6] = 7;		ScanCodeMap[olc::K7] = 8;		ScanCodeMap[olc::K8] = 9;		ScanCodeMap[olc::K9] = 10;		ScanCodeMap[olc::K0] = 11;		ScanCodeMap[olc::MINUS] = 12;		ScanCodeMap[olc::PLUS] = 13; 													ScanCodeMap[olc::BACK] = 15;				ScanCodeMap[olc::INS] = 75;				ScanCodeMap[olc::HOME] = 80;		ScanCodeMap[olc::PGUP] = 85;		
	ScanCodeMap[olc::TAB] = 16;							ScanCodeMap[olc::Q] = 17;		ScanCodeMap[olc::W] = 18; 		ScanCodeMap[olc::E] = 19; 		ScanCodeMap[olc::R] = 20; 		ScanCodeMap[olc::T] = 21;		ScanCodeMap[olc::Y] = 22; 		ScanCodeMap[olc::U] = 23; 		ScanCodeMap[olc::I] = 24; 		ScanCodeMap[olc::O] = 25; 		ScanCodeMap[olc::P] = 26; 
	ScanCodeMap[olc::CAPS] = 30; 							ScanCodeMap[olc::A] = 31;		ScanCodeMap[olc::S] = 32; 		ScanCodeMap[olc::D] = 33; 		ScanCodeMap[olc::F] = 34;		ScanCodeMap[olc::G] = 35; 		ScanCodeMap[olc::H] = 36; 		ScanCodeMap[olc::J] = 37; 		ScanCodeMap[olc::K] = 38;		ScanCodeMap[olc::L] = 39;																																ScanCodeMap[olc::ENTER] = 43;
	ScanCodeMap[olc::LSHIFT] = 44;								ScanCodeMap[olc::Z] = 46;		ScanCodeMap[olc::X] = 47; 		ScanCodeMap[olc::C] = 48;		ScanCodeMap[olc::V] = 49;		ScanCodeMap[olc::B] = 50;		ScanCodeMap[olc::N] = 51;		ScanCodeMap[olc::M] = 52;																																													ScanCodeMap[olc::RSHIFT] = 57;
	ScanCodeMap[olc::LCTRL] = 58;																				ScanCodeMap[olc::SPACE] = 61;

	MakeMap[1]  = {0x0E};		BreakMap[1]  = {0xF0, 0x0E};		MakeMap[2]  = {0x16};		BreakMap[2]  = {0xF0, 0x16};
	MakeMap[3]  = {0x1E};		BreakMap[3]  = {0xF0, 0x1E};		MakeMap[4]  = {0x26};		BreakMap[4]  = {0xF0, 0x26};
	MakeMap[5]  = {0x25};		BreakMap[5]  = {0xF0, 0x25};		MakeMap[6]  = {0x2E};		BreakMap[6]  = {0xF0, 0x2E};
	MakeMap[7]  = {0x36};		BreakMap[7]  = {0xF0, 0x36};		MakeMap[8]  = {0x3D};		BreakMap[8]  = {0xF0, 0x3D};
	MakeMap[9]  = {0x3E};		BreakMap[9]  = {0xF0, 0x3E};		MakeMap[10] = {0x46};		BreakMap[10] = {0xF0, 0x46};
	MakeMap[11] = {0x45};		BreakMap[11] = {0xF0, 0x45};		MakeMap[12] = {0x4E};		BreakMap[12] = {0xF0, 0x4E};
	MakeMap[13] = {0x55};		BreakMap[13] = {0xF0, 0x55};			
	MakeMap[15] = {0x66};		BreakMap[15] = {0xF0, 0x66};		MakeMap[16] = {0x0D};		BreakMap[16] = {0xF0, 0x0D};
	MakeMap[17] = {0x15};		BreakMap[17] = {0xF0, 0x15};		MakeMap[18] = {0x1D}; 		BreakMap[18] = {0xF0, 0x1D};
	MakeMap[19] = {0x24};		BreakMap[19] = {0xF0, 0x24};		MakeMap[20] = {0x2D};		BreakMap[20] = {0xF0, 0x2D};
	MakeMap[21] = {0x2C};		BreakMap[21] = {0xF0, 0x2C};		MakeMap[22] = {0x35};		BreakMap[22] = {0xF0, 0x35};
	MakeMap[23] = {0x3C};		BreakMap[23] = {0xF0, 0x3C};		MakeMap[24] = {0x43};		BreakMap[24] = {0xF0, 0x43};
	MakeMap[25] = {0x44};		BreakMap[25] = {0xF0, 0x44};		MakeMap[26] = {0x4D};		BreakMap[26] = {0xF0, 0x4D};
	MakeMap[27] = {0x54};		BreakMap[27] = {0xF0, 0x54};		MakeMap[28] = {0x5B};		BreakMap[28] = {0xF0, 0x5B};
	MakeMap[29] = {0x5D};		BreakMap[29] = {0xF0, 0x5D};		MakeMap[30] = {0x58};		BreakMap[30] = {0xF0, 0x58};
	MakeMap[31] = {0x1C};		BreakMap[31] = {0xF0, 0x1C};		MakeMap[32] = {0x1B};		BreakMap[32] = {0xF0, 0x1B};
	MakeMap[33] = {0x23};		BreakMap[33] = {0xF0, 0x23};		MakeMap[34] = {0x2B};		BreakMap[34] = {0xF0, 0x2B};
	MakeMap[35] = {0x34};		BreakMap[35] = {0xF0, 0x34};		MakeMap[36] = {0x33};		BreakMap[36] = {0xF0, 0x33};
	MakeMap[37] = {0x3B};		BreakMap[37] = {0xF0, 0x3B};		MakeMap[38] = {0x42};		BreakMap[38] = {0xF0, 0x42};
	MakeMap[39] = {0x4B};		BreakMap[39] = {0xF0, 0x4B};		MakeMap[40] = {0x4C};		BreakMap[40] = {0xF0, 0x4C};
	MakeMap[41] = {0x52};		BreakMap[41] = {0xF0, 0x52};			
	MakeMap[43] = {0x5A};		BreakMap[43] = {0xF0, 0x5A};		MakeMap[44] = {0x12};		BreakMap[44] = {0xF0, 0x12};
																	MakeMap[46] = {0x1A};		BreakMap[46] = {0xF0, 0x1A};
	MakeMap[47] = {0x22};		BreakMap[47] = {0xF0, 0x22};		MakeMap[48] = {0x21};		BreakMap[48] = {0xF0, 0x21};
	MakeMap[49] = {0x2A};		BreakMap[49] = {0xF0, 0x2A};		MakeMap[50] = {0x32};		BreakMap[50] = {0xF0, 0x32};
	MakeMap[51] = {0x31};		BreakMap[51] = {0xF0, 0x31};		MakeMap[52] = {0x3A};		BreakMap[52] = {0xF0, 0x3A};
	MakeMap[53] = {0x41};		BreakMap[53] = {0xF0, 0x41};		MakeMap[54] = {0x49};		BreakMap[54] = {0xF0, 0x49};
	MakeMap[55]	= {0x4A};		BreakMap[55] = {0xF0, 0x4A};	
	MakeMap[57] = {0x59};		BreakMap[57] = {0xF0, 0x59};		MakeMap[58] = {0x14};		BreakMap[58] = {0xF0, 0x14};
																	MakeMap[60] = {0x11};		BreakMap[60] = {0xF0, 0x11};
	MakeMap[61] = {0x29};		BreakMap[61] = {0xF0, 0x29};		MakeMap[62] = {0x0E, 0x11};	BreakMap[62] = {0xE0, 0xF0, 0x11};
																	MakeMap[64] = {0xE0, 0x14};	BreakMap[64] = {0xE0, 0xF0, 0x14};
	MakeMap[75] = {0xE0, 0x70};	BreakMap[75] = {0xE0, 0xF0, 0x70};	MakeMap[76] = {0xE0, 0x71};	BreakMap[76] = {0xE0, 0xF0, 0x71};
	MakeMap[79] = {0xE0, 0x6B};	BreakMap[79] = {0xE0, 0xF0, 0x6B};	MakeMap[80] = {0xE0, 0x6C};	BreakMap[80] = {0xE0, 0xF0, 0x6C};
	MakeMap[81] = {0xE0, 0x69};	BreakMap[81] = {0xE0, 0xF0, 0x69};	
	MakeMap[83] = {0xE0, 0x75}; BreakMap[83] = {0xE0, 0xF0, 0x75};	MakeMap[84] = {0xE0, 0x72}; BreakMap[84] = {0xE0, 0xF0, 0x72};
	MakeMap[85] = {0xE0, 0x7D}; BreakMap[85] = {0xE0, 0xF0, 0x7D};	MakeMap[86] = {0xE0, 0x7A}; BreakMap[86] = {0xE0, 0xF0, 0x7A};
	MakeMap[89] = {0xE0, 0x74}; BreakMap[89] = {0xE0, 0xF0, 0x74};



	for(auto &state : keyState){
		state = 0;
	}
	for(auto &state : prevKeyState){
		state = 0;
	}

}

atmKeyboard::~atmKeyboard(){
}

void atmKeyboard::ConnectVIA(atm6522* v){
	via = v;
}

void atmKeyboard::setState(bool onoff){
	kb_on = onoff;
}

void atmKeyboard::clock(){
	if(clock_state){
		via->setCA(false, false);
	}else if(typematic){
		typematic_timeout--;
		if(typematic_timeout == 0){
			//send most recent key press;
			typematic_timeout = 1.0 / (typematic_rate * 0.0000070f);
			com_message = std::list<uint8_t>(typematic_code);
			com_state = com_message.size()*11;
		}
	}
	//check all keys for change
	clock_state = !clock_state;
	if(kb_on)
		for(const auto key : ScanCodeMap){
			olc::HWButton _keystate = demo.GetKey((olc::Key)key.first);

			if(_keystate.bPressed)
				keyState[key.second] = true;
			else if(_keystate.bReleased)
				keyState[key.second] = false;
		}

	if(com_state == 0){
		//ready to send new key 
		for(const auto key : ScanCodeMap){
			if(keyState[key.second] ^ prevKeyState[key.second]){
				
				prevKeyState[key.second] = keyState[key.second];
				if(keyState[key.second]){
					//make
					com_message = std::list<uint8_t>(MakeMap[key.second]);	//clone list, not reference
					typematic_code = std::list<uint8_t>(MakeMap[key.second]);
					typematic_timeout = typematic_delay / (0.070f);
				}else{
					//break
					com_message = std::list<uint8_t>(BreakMap[key.second]);  //clone list, not reference
					typematic_code = std::list<uint8_t>({});
				}
				com_state = com_message.size()*11;	//1 start bit, 8 data bits, 1 parity, 1 stop.
				//only deal with one at a time!
				break;
			}
		}
	}
	if(com_state > 0){
		if(clock_state){
			via->setCA(false, false);
			return;
		}
		//data bit that will be sent to the via
		bool bit;
		//receiving/transmitting new command, deal with it
		uint8_t sub_com_state = (com_state)%11;
		if(sub_com_state == 0){
			//start bit
			bit = false;
			//send start bit

			//populate com_byte with data
			com_byte = com_message.front();
			com_message.pop_front();

			// printf("Character :%2x \n", com_byte);
			// fflush(stdout);
			parity = true;	//odd parity
		}else if(sub_com_state > 2){
			//data bit
			bit = (com_byte>>(10-sub_com_state))&0x01;
			//send data bit 
			parity ^= bit;

		}else if(sub_com_state == 2){
			//parity bit
			bit = parity;
		}else if(sub_com_state == 1){
			//stop bit
			bit = true;
		}

		if(bit){
			via->setB(1<<5);
		}else{
			via->clrB(1<<5);
		}
		via->setCA(true, false);
		// printf("keyboard triggered state :%2i - %2i\n", com_state-1, (com_state-1)%11);
		com_state--;
	}

}