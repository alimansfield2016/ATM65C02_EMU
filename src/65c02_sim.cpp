/*
	olc::NES - Part #3 - Buses, Rams, Roms & Mappers
	"Thanks Dad for believing computers were gonna be a big deal..." - javidx9

	License (OLC-3)
	~~~~~~~~~~~~~~~

	Copyright 2018-2019 OneLoneCoder.com

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

	1. Redistributions or derivations of source code must retain the above
	copyright notice, this list of conditions and the following disclaimer.

	2. Redistributions or derivative works in binary form must reproduce
	the above copyright notice. This list of conditions and the following
	disclaimer must be reproduced in the documentation and/or other
	materials provided with the distribution.

	3. Neither the name of the copyright holder nor the names of its
	contributors may be used to endorse or promote products derived
	from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
	HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
	LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


	Relevant Video: https://youtu.be/xdzOvpYPmGE

	Links
	~~~~~
	YouTube:	https://www.youtube.com/javidx9
				https://www.youtube.com/javidx9extra
	Discord:	https://discord.gg/WhwHUMV
	Twitter:	https://www.twitter.com/javidx9
	Twitch:		https://www.twitch.tv/javidx9
	GitHub:		https://www.github.com/onelonecoder
	Patreon:	https://www.patreon.com/javidx9
	Homepage:	https://www.onelonecoder.com

	Author
	~~~~~~
	David Barr, aka javidx9, �OneLoneCoder 2019
*/

/*
	Modified from original by: Ali Mansfield
	Original author: as stated above
	Licence: as stated above

*/

#include <iostream>
#include <sstream>

#include "include/config.h"

#include "include/Bus.h"
#include "include/olc6502.h"
#ifdef KB
#include "include/atmKeyboard.h"
#endif

#define OLC_PGE_APPLICATION
#include "include/olcPixelGameEngine.h"

bool interrupt = false;
std::string rom_file = "65c02.rom";

class atm65c22_emu : public olc::PixelGameEngine
{
public:
	atm65c22_emu() { sAppName = "6502 emulator"; }

private: 
	Bus bus;
	#ifdef KB
	atmKeyboard kb;
	#endif
	bool bEmulationRun = false;
	float fResidualTime = 0.0f;
	float fTotalTime = 0.0f;
	bool wait_for_return = false;
	bool int_pause = false;
	bool bKeyboardInput = false;
private: 
	// Support Utilities
	std::map<uint16_t, std::string> mapAsm;

	std::string hex(uint32_t n, uint8_t d)
	{
		std::string s(d, '0');
		for (int i = d - 1; i >= 0; i--, n >>= 4)
			s[i] = "0123456789ABCDEF"[n & 0xF];
		return s;
	};

	void DrawRam(int x, int y, uint16_t nAddr, int nRows, int nColumns)
	{
		int nRamX = x, nRamY = y;
		for (int row = 0; row < nRows; row++)
		{
			std::string sOffset = "$" + hex(nAddr, 4) + ":";
			for (int col = 0; col < nColumns; col++)
			{
				sOffset += " " + hex(bus.cpuRead(nAddr, true), 2);
				nAddr += 1;
			}
			DrawString(nRamX, nRamY, sOffset);
			nRamY += 10;
		}
	}

	void DrawCpu(int x, int y)
	{
		std::string status = "STATUS: ";
		DrawString(x , y , "STATUS:", olc::WHITE);
		DrawString(x  + 64, y, "N", bus.cpu.status & olc6502::N ? olc::GREEN : olc::RED);
		DrawString(x  + 80, y , "V", bus.cpu.status & olc6502::V ? olc::GREEN : olc::RED);
		DrawString(x  + 96, y , "-", bus.cpu.status & olc6502::U ? olc::GREEN : olc::RED);
		DrawString(x  + 112, y , "B", bus.cpu.status & olc6502::B ? olc::GREEN : olc::RED);
		DrawString(x  + 128, y , "D", bus.cpu.status & olc6502::D ? olc::GREEN : olc::RED);
		DrawString(x  + 144, y , "I", bus.cpu.status & olc6502::I ? olc::GREEN : olc::RED);
		DrawString(x  + 160, y , "Z", bus.cpu.status & olc6502::Z ? olc::GREEN : olc::RED);
		DrawString(x  + 178, y , "C", bus.cpu.status & olc6502::C ? olc::GREEN : olc::RED);
		DrawString(x , y + 10, "PC: $" + hex(bus.cpu.pc, 4));
		DrawString(x , y + 20, "A: $" +  hex(bus.cpu.a, 2) + "  [" + std::to_string(bus.cpu.a) + "]");
		DrawString(x , y + 30, "X: $" +  hex(bus.cpu.x, 2) + "  [" + std::to_string(bus.cpu.x) + "]");
		DrawString(x , y + 40, "Y: $" +  hex(bus.cpu.y, 2) + "  [" + std::to_string(bus.cpu.y) + "]");
		DrawString(x , y + 50, "Stack P: $" + hex(bus.cpu.stkp+0x100, 4));
		DrawString(x , y + 60, "CPU Cycles: " + std::to_string(bus.cpu.GetClockCount()));
	}

	void DrawCode(int x, int y, int nLines)
	{
		//put the start of the disassembly way further than needed
		//so that any instruction misalignments fix themselves
		uint16_t addr_start = bus.cpu.pc-0x40;
		uint16_t addr_end   = bus.cpu.pc+0x40;
		
		std::map<uint16_t, std::string> new_mapAsm = bus.cpu.disassemble(addr_start, addr_end);
		auto it_a = new_mapAsm.find(bus.cpu.pc);
		int nLineY = (nLines >> 1) * 10 + y;
		if (it_a != new_mapAsm.end())
		{
			DrawString(x, nLineY, (*it_a).second, olc::CYAN);
			while (nLineY < (nLines * 10) + y)
			{
				nLineY += 10;
				if (++it_a != new_mapAsm.end())
				{
					DrawString(x, nLineY, (*it_a).second);
				}
			}
		}

		it_a = new_mapAsm.find(bus.cpu.pc);
		nLineY = (nLines >> 1) * 10 + y;
		if (it_a != new_mapAsm.end())
		{
			while (nLineY > y)
			{
				nLineY -= 10;
				if (--it_a != new_mapAsm.end())
				{
					DrawString(x, nLineY, (*it_a).second);
				}
			}
		}
	}

	void DrawStack(int x, int y, int n_bytes){

	}

	void DrawControls(int x, int y){
		std::string text = "KB ";
		uint8_t offset = 0;
		#ifdef KB
		DrawString(x, y, text, bKeyboardInput?olc::GREEN:olc::RED);
		offset += 3;
		text = "<INS>|";
		DrawString(x + offset*7, y, text);
		offset += 7;
		#endif
		text = bEmulationRun?" RUN  ":"PAUSE ";
		DrawString(x + offset*7, y, text, bEmulationRun?olc::GREEN:olc::RED);
		offset += 6;
		text = "<SPACE>| ";
		DrawString(x + offset*7, y, text);
		offset += 9;

		text = " BRK ";
		DrawString(x + offset*7, y, text, int_pause?olc::GREEN:olc::RED);
		offset += 5;
		text = "<I>| ";
		DrawString(x+offset*7, y, text);
	}


	bool OnUserCreate()
	{

		for(uint16_t i = 0; i < 65535; i++){
			bus.cpuWrite(i, 0);
			bus.lcd.unInit();
		}
		bus.via.write(0x000E, ~0x80);
		if(bus.loadROM(rom_file) != 0){
			printf("ROM Binary file \"%s\" not found!\n", rom_file);
			return false;
		}
		// Extract dissassembly
		mapAsm = bus.cpu.disassemble(0x0000, 0xFFFF);
		#ifdef KB
		kb.ConnectVIA(&bus.via);
		bus.via.ConnectKeyboard(&kb);
		#endif
		#ifndef LCD_BUS
		bus.lcd.ConnectVIA(&bus.via);
		#endif
		// Reset system
		bus.reset();
		return true;
	}

	bool OnUserUpdate(float fElapsedTime)
	{
		Clear(olc::DARK_BLUE);
		fTotalTime += fElapsedTime;
		static uint32_t key_hold = 0, kh_up = 0, kh_dn = 0, kh_l = 0, kh_r = 0;
		static uint8_t page = 2;
		if(bEmulationRun){
			for(float cycles = fElapsedTime*CPU_FREQ; cycles > 0 && !(interrupt && int_pause); cycles-= TIMESTEP){
				bus.acia.clock();
				#ifdef KB
				if(fmod(cycles, (CPU_FREQ/(KB_FREQ*2.0)) ) < TIMESTEP ){
					//clock the KB twice (H/L) at 10khz
					kb.clock();
				}
				#endif
				if(fmod(cycles, CPU_FREQ/LCD_FREQ) < TIMESTEP){
					bus.lcd.clock();
				}

				if(fmod(cycles, 1.0) < 1.0/1.8432000 ){
					bus.clock();
					if(wait_for_return){
						if(bus.cpu.returned()){
							bEmulationRun = false;
							wait_for_return = false;
							cycles = 0;
							break;
						}
					}
				}
			}
		}
		#ifdef KB
		if(GetKey(olc::Key::INS).bPressed){
			bKeyboardInput = !bKeyboardInput;
			kb.setState(bKeyboardInput);
		}
		#endif

		if(!bKeyboardInput){
			// Emulate code step-by-step
			if (GetKey(olc::Key::C).bPressed)
			{
				if(GetKey(olc::Key::LSHIFT).bHeld){
					bus.clock();
				}else{
					// Clock enough times to execute a whole CPU instruction
					do { bus.clock(); } while (!bus.cpu.complete() && !(interrupt && int_pause));
					// CPU clock runs slower than system clock, so it may be
					// complete for additional system clock cycles. Drain
					// those out
					do { bus.clock(); } while (bus.cpu.complete() && !(interrupt && int_pause));
				}
			}else if(GetKey(olc::Key::C).bHeld){
				key_hold++;
				if(key_hold > 30){
				if(GetKey(olc::Key::LSHIFT).bHeld)
					bus.clock();
				else
					do { bus.clock(); } while (!bus.cpu.complete() && !(interrupt && int_pause));
				}
			}else{
				key_hold = 0;
			}
			if(GetKey(olc::Key::E).bPressed){
				bus.cpu.wait_return();
				wait_for_return = true;
				bEmulationRun = true;
			}

			if (GetKey(olc::Key::SPACE).bPressed) bEmulationRun = !bEmulationRun;
			if (GetKey(olc::Key::R).bPressed){
				if(GetKey(olc::Key::LCTRL).bHeld){
					//hard reset of emulator
					OnUserCreate();
				}else
					bus.reset();		

			} 
			if(GetKey(olc::Key::LCTRL).bHeld && GetKey(olc::Key::C).bPressed)
				return false;

			if(GetKey(olc::Key::UP).bPressed)
				page++;
			else if(GetKey(olc::Key::UP).bHeld)
				kh_up++ > 30?page++:0;
			else 
				kh_up = 0;
			if(GetKey(olc::Key::DOWN).bPressed)
				page--;
			else if(GetKey(olc::Key::DOWN).bHeld)
				kh_dn++ > 30?page--:0;
			else
				kh_dn = 0;
			if(GetKey(olc::Key::RIGHT).bPressed)
				page+=16;
			else if(GetKey(olc::Key::RIGHT).bHeld)
				kh_l++ > 30?page+=16:0;
			else 
				kh_l = 0;
			if(GetKey(olc::Key::LEFT).bPressed)
				page-=16;
			else if(GetKey(olc::Key::LEFT).bHeld)
				kh_r++ > 30?page-=16:0;
			else
				kh_r = 0;
			if(GetKey(olc::Key::I).bPressed)
				int_pause = !int_pause;
		}

		if(interrupt && int_pause){
			bEmulationRun = false;
			interrupt = false;
		}
		DrawCpu(495, 2);
		DrawCode(495, 82, 20);
		DrawRam(4, 75, 0x0000, 16, 16);
		DrawRam(4, 240, page << 8, 16, 16);
		DrawStack(495, 300, 0x20-bus.cpu.stkp);
		bus.lcd.redraw(fTotalTime);
		bus.via.redraw();
		DrawSprite(440, 75, &bus.via.GetScreen(), 1);
		#ifdef VIA2
		bus.via2.redraw();
		DrawSprite(440, 220, &bus.via2.GetScreen(), 1);
		#endif
		DrawSprite(LCDX, LCDY, &bus.lcd.GetScreen(), 2);
		DrawControls(15, 525);
		return true;
	}
};




atm65c22_emu demo;

int main(int argc, char* argv[])
{
	if(argc == 2){
		rom_file = argv[1];
	}
	demo.Construct(960, 540, 2, 2);
	demo.Start();
	return 0;
}