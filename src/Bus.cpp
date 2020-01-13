/*
	olc::NES - System Bus
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

#include "include/Bus.h"

Bus::Bus()
{
	// Connect CPU to communication bus
	cpu.ConnectBus(this);
	via.ConnectBus(this);
}


Bus::~Bus()
{
}

void Bus::cpuWrite(uint16_t addr, uint8_t data)
{	
	if (addr >= 0x0000 && addr <= 0x5FFF)
	{
		cpuRam[addr] = data;

	}else if(addr >= 0x6000 && addr < 0x67FF){
		//LCD
		lcd.write(addr, data);
	}else if(addr >= 0x7000 && addr <= 0x77FF){
		via.write(addr, data);	
	}else if(addr >= 0x7800 && addr <= 0x7FFF){
		acia.write(addr, data);	
	}else if (addr >= 0x8000 && addr <= 0xFFFF)
	{
		cpuRom[addr-0x8000] =  data;
	}		
}

uint8_t Bus::cpuRead(uint16_t addr, bool bReadOnly)
{
	uint8_t data = 0x00;	
	if (addr >= 0x0000 && addr <= 0x5FFF){
		data = cpuRam[addr];
	}else if(addr >= 0x6000 && addr <= 0x67FF){
		lcd.read(addr, data);
	}else if(addr >= 0x7000 && addr <= 0x77FF){
		via.read(addr, data);	
	}else if(addr >= 0x7800 && addr <= 0x7FFF){
		acia.read(addr, data);
	}else if (addr >= 0x8000 && addr <= 0xFFFF){
		data = cpuRom[addr-0x8000];
	}

	return data;
}

// void Bus::insertCartridge(const std::shared_ptr<Cartridge>& cartridge)
// {
// 	// Connects cartridge to both Main Bus and CPU Bus
// 	this->cart = cartridge;
// 	ppu.ConnectCartridge(cartridge);
// }

void Bus::reset()
{
	cpu.reset();
	nSystemClockCounter = 0;
	via.reset();
}

void Bus::clock()
{
	// Clocking. The heart and soul of an emulator. The running
	// frequency is controlled by whatever calls this function.
	// So here we "divide" the clock as necessary and call
	// the peripheral devices clock() function at the correct
	// times.

	// The fastest clock frequency the digital system cares
	// about is equivalent to the PPU clock. So the PPU is clocked
	// each time this function is called.
	// ppu.clock();

	// The CPU runs 3 times slower than the PPU so we only call its
	// clock() function every 3 times this function is called. We
	// have a global counter to keep track of this.
	// if (nSystemClockCounter % 3 == 0)
	// {
	cpu.clock();
	via.clock();
	// }

	nSystemClockCounter++;
}

uint8_t Bus::loadROM(const std::string filename){
	FILE* f_rom = fopen(filename.c_str(), "rb");
	if(f_rom == nullptr)
		return 1;
	for(uint16_t i = 0; i < 32768; i++){
		uint8_t byte = fgetc(f_rom);
		cpuRom[i] = byte;
	}
	return 0;
}

void Bus::IRQ(){
	irq = true;
}
void Bus::NMI(){
	nmi = true;
}
bool Bus::getIRQ(){
	bool out = irq;
	irq = false;
	return out;
}
bool Bus::getNMI(){
	bool out = nmi;
	nmi = false;
	return out;
}