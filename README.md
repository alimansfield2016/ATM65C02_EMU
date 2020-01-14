# Ali Mansfield's 65C02 Breadboard computer emulator


This was a little project I thought could be useful to people for developing assembly ROMs for Ben Eater's 6502 computer kit. It was originally based on [JavidX9's NES emulator](https://github.com/OneLoneCoder/olcNES) for the core 6502 emulation. It was modified to support the full 65C02 instruction set, and 32K ROM/SRAM. By default, it loads the ROM from a file called `65c02.rom`. This can of course be modified in the source code if you so wish.

I also added other functionality such as the LCD, VIA, and the ACIA is a work in progress

## Configuration of the emulator

Configuration of the ATM65C02 Emulator is fairly simple. The source contains a file [config.h](src/include/config.h). This has both a default configuration, useable by defining `CONFIG_STD`, which is representative of the Ben Eater schematics, and an advanced configuration, with a 40x4 LCD connected directly to the bus, 2 VIAs, etc.

This file contains almost all of the options for the emulator that can be configured at complile/build time. These include:
```c
#define LCDW		// Character width of the LCD 		(Default=16)
#define LCDH		// Character height of the LCD		(Default=2)
#define LCD_NDISP	// Number of emulated display controllers. This is necessary for the larger LCDs such as 40x4, which require a separate controller for the top two and bottom two lines
#define LCDX		// X position to draw the LCD at on the GUI
#define LCDY		// Y position to draw the LCD at on the GUI
#define LCD_BUS		// If defined, the LCD will be connected directly to the system bus, rather than being connected through the VIA
#define LCD_PORTA	// If defined, the LCD will use PORTA of the VIA as the data word, and the lower bits of PORTB for the control.
#define LCD_PORTB	// If defined, the LCD will use PORTB of the VIA as the data word, and the upper bits of PORTA for the control. 
#define LCD_SW_RS_RW	// If defined, the RS and RWB control lines from the VIA will be RS, RWB, EN. rather than RWB, RS, EN. 
```

Each element on the bus also has definitions of it's mapped memory space:
```c
#define SRAM_START	0x0000
#define SRAM_END	0x3FFF

//There are other definitions for the:
//	LCD
//	VIA
//	VIA2 (If used)
// 	ACIA

//If any of these are left undefined, They will be mapped a negative memory region, and will not be accessed by the bus.
```

## 65C02

The emulated 65C02 core is a timing accurate, but not cycle accurate emulation (i think that's the right way to put it?). Basically, every instruction lasts the correct number of clock cycles, but micro-operations such as reading each byte of the instruction all happen in the first clock cycle, and idles for the remaining cycles of the instruction. Interupts via the IRQ and NMI are fully supported.

The CPU registers, flags, stack pointer and PC are all displayed on the screen for debugging purposes. A disassembler is also provided for the memory region around the PC.

Two memory views are also available. The first contains the ZP, the second is selectable using the arrow keys to navigate the address. LR - +-$0100, UD - += $1000.

## LCD

The LCD is an emulation of the Hitachi HD44780 LCD controller. It supports nearly any size of LCD, except 20x4 ( it might but i havent tested it soo...). It supports up to the 40x4, which requires two enable lines. 

The LCD is connected directly to the system bus (contrary to the original design which used the output ports on the VIA), but this can be configured fairly easily with a different assembly 'driver' for it.

Functionally the LCD is almost 100% complete, but is otherwise completely useable. 

By default, the LCD is connected to the VIA on PORTB, with the upper 3 bits of PORTA as the control lines, RS, RWB, EN.
Alternatively in the advanced configuration, the LCD is addressable in the region $6000 - $67FF. The register select is connected to A0, and \[A1:A2\] are the enable lines when used in 40x4 (default for advanced) 

## 65C22 VIA

So far, I have also implemented the 65C22 VIA to the system. It has both PORTs, which can be input or output, full interrupt support on CA and CB. The two timers are also fully functional. The only thing that is not 100% complete is the shift register at the moment.

With the standard configuration, the 65C22 is addressable in the region $6000 - $7FFF.
The address mapping range of the VIA can be changed using the configuration file.
A second VIA can be added to the system by defining `VIA2` in the `config.h` file.

## PS/2 Keyboard

I thought it would be fun, so I implemented an emulated version of the PS/2 keyboard. All of the alphabetic keys work, but many of the non-alphabet keys and modifier keys are not yet implemented. By default the KB is connected to PB5 and CA1 for interrupts on clock edges. It is also currently uni-directional, just sending scancodes for make and break.

## conclusion

Overall, I think this is fairly complete. There are things that need work, but everything works to an acceptable standard IMO. nearly all of the addresses/settings are configurable in code.