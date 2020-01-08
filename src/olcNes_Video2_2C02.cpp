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

#include <iostream>
#include <sstream>

#include "include/Bus.h"
#include "include/olc6502.h"

#define OLC_PGE_APPLICATION
#include <personal/olcPixelGameEngine.h>



class Demo_olc2C02 : public olc::PixelGameEngine
{
public:
	Demo_olc2C02() { sAppName = "6502 emulator"; }

private: 
	// The NES
	Bus nes;
	// std::vector<uint8_t> ROM;
	bool bEmulationRun = false;
	float fResidualTime = 0.0f;
	float fTotalTime = 0.0f;
	bool wait_for_return = false;

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
				sOffset += " " + hex(nes.cpuRead(nAddr, true), 2);
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
		DrawString(x  + 64, y, "N", nes.cpu.status & olc6502::N ? olc::GREEN : olc::RED);
		DrawString(x  + 80, y , "V", nes.cpu.status & olc6502::V ? olc::GREEN : olc::RED);
		DrawString(x  + 96, y , "-", nes.cpu.status & olc6502::U ? olc::GREEN : olc::RED);
		DrawString(x  + 112, y , "B", nes.cpu.status & olc6502::B ? olc::GREEN : olc::RED);
		DrawString(x  + 128, y , "D", nes.cpu.status & olc6502::D ? olc::GREEN : olc::RED);
		DrawString(x  + 144, y , "I", nes.cpu.status & olc6502::I ? olc::GREEN : olc::RED);
		DrawString(x  + 160, y , "Z", nes.cpu.status & olc6502::Z ? olc::GREEN : olc::RED);
		DrawString(x  + 178, y , "C", nes.cpu.status & olc6502::C ? olc::GREEN : olc::RED);
		DrawString(x , y + 10, "PC: $" + hex(nes.cpu.pc, 4));
		DrawString(x , y + 20, "A: $" +  hex(nes.cpu.a, 2) + "  [" + std::to_string(nes.cpu.a) + "]");
		DrawString(x , y + 30, "X: $" +  hex(nes.cpu.x, 2) + "  [" + std::to_string(nes.cpu.x) + "]");
		DrawString(x , y + 40, "Y: $" +  hex(nes.cpu.y, 2) + "  [" + std::to_string(nes.cpu.y) + "]");
		DrawString(x , y + 50, "Stack P: $" + hex(nes.cpu.stkp, 4));
		DrawString(x , y + 60, "CPU Cycles: " + std::to_string(nes.cpu.GetClockCount()));
	}

	void DrawCode(int x, int y, int nLines)
	{
		//put the start of the disassembly way further than needed
		//so that any instruction misalignments fix themselves
		uint16_t addr_start = nes.cpu.pc-0x40;
		uint16_t addr_end   = nes.cpu.pc+0x40;
		
		std::map<uint16_t, std::string> new_mapAsm = nes.cpu.disassemble(addr_start, addr_end);
		auto it_a = new_mapAsm.find(nes.cpu.pc);
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

		it_a = new_mapAsm.find(nes.cpu.pc);
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


	bool OnUserCreate()
	{
		// Load the cartridge
		// cart = std::make_shared<Cartridge>("nestest.nes");
		// if (!cart->ImageValid())
		// 	return false;

		// Insert into NES
		// nes.insertCartridge(cart);

		if(nes.loadROM("65c02.rom") != 0){
			return false;
		}
		// Extract dissassembly
		mapAsm = nes.cpu.disassemble(0x0000, 0xFFFF);

		// Reset NES
		nes.reset();
		return true;
	}

	bool OnUserUpdate(float fElapsedTime)
	{
		Clear(olc::DARK_BLUE);
		fTotalTime += fElapsedTime;
		static uint32_t key_hold = 0, kh_up = 0, kh_dn = 0;
		static uint8_t page = 2;
		if(bEmulationRun)
			for(uint32_t cycles = fElapsedTime*1000000*3 /*cpu clocked for every 3 cycles*/; cycles > 0; cycles --){
				nes.clock();
				if(wait_for_return){
					if(nes.cpu.returned()){
						bEmulationRun = false;
						wait_for_return = false;
						cycles = 0;
						break;
					}
				}

			}
			

		// Emulate code step-by-step
		if (GetKey(olc::Key::C).bPressed)
		{
			// Clock enough times to execute a whole CPU instruction
			do { nes.clock(); } while (!nes.cpu.complete());
			// CPU clock runs slower than system clock, so it may be
			// complete for additional system clock cycles. Drain
			// those out
			do { nes.clock(); } while (nes.cpu.complete());
		}else if(GetKey(olc::Key::C).bHeld){
			key_hold++;
			if(key_hold > 100){
				do { nes.clock(); } while (!nes.cpu.complete());
			}
		}else{
			key_hold = 0;
		}
		if(GetKey(olc::Key::E).bPressed){
			nes.cpu.wait_return();
			wait_for_return = true;
			bEmulationRun = true;
		}

		if (GetKey(olc::Key::SPACE).bPressed) bEmulationRun = !bEmulationRun;
		if (GetKey(olc::Key::R).bPressed) nes.reset();		
		if(GetKey(olc::Key::CTRL).bHeld && GetKey(olc::Key::C).bPressed)
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

		DrawCpu(495, 2);
		DrawCode(495, 82, 20);
		DrawRam(4, 75, 0x0000, 16, 16);
		DrawRam(4, 240, page << 8, 16, 16);
		DrawStack(495, 300, 0x20-nes.cpu.stkp);
		nes.lcd.redraw(fTotalTime);
		nes.via.redraw();
		DrawSprite(447, 75, &nes.via.GetScreen(), 2);
		DrawSprite(1, 1, &nes.lcd.GetScreen(), 2);
		return true;
	}
};




Demo_olc2C02 demo;

int main()
{
	demo.Construct(700, 400, 2, 2);
	demo.Start();
	return 0;
}