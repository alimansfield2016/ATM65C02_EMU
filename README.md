# Ali Mansfield's 65C02 Breadboard computer emulator


This was a little project I thought could be useful to people for developing assembly ROMs for Ben Eater's 6502 computer kit. It was originally based on [JavidX9's NES emulator](https://github.com/OneLoneCoder/olcNES) for the core 6502 emulation. It was modified to support the full 65C02 instruction set, and 32K ROM/SRAM. By default, it loads the ROM from a file called `65c02.rom`. This can of course be modified in the source code if you so wish.

I also added other functionality such as the LCD, VIA, and the ACIA is a work in progress

## 65C02

The emulated 65C02 core is a timing accurate, but not cycle accurate emulation (i think that's the right way to put it?). Basically, every instruction lasts the correct number of clock cycles, but micro-operations such as reading each byte of the instruction all happen in the first clock cycle, and idles for the remaining cycles of the instruction. Interupts via the IRQ and NMI are fully supported.

The CPU registers, flags, stack pointer and PC are all displayed on the screen for debugging purposes. A disassembler is also provided for the memory region around the PC.

Two memory views are also available. The first contains the ZP, the second is selectable using the arrow keys to navigate the address. LR - +-$0100, UD - += $1000.

## LCD

The LCD is an emulation of the Hitachi HD44780 LCD controller. It supports nearly any size of LCD, except 20x4 ( it might but i havent tested it soo...). It supports up to the 40x4, which requires two enable lines. 

The LCD is connected directly to the system bus (contrary to the original design which used the output ports on the VIA), but this can be configured fairly easily with a different assembly 'driver' for it.

Functionally the LCD is almost 100% complete, but is otherwise completely useable. 

The LCD is addressable in the region $6000 - $67FF. The register select is connected to A0, and \[A1:A2\] are the enable lines when used in 40x4 (default) 

## 65C22 VIA

So far, I have also implemented the 65C22 VIA to the system. It has both PORTs, which can be input or output, full interrupt support on CA and CB. The two timers are also fully functional. The only thing that is not 100% complete is the shift register at the moment.

The 65C22 is addressable in the region $7000 - $77FF

## PS/2 Keyboard

I thought it would be fun, so I implemented an emulated version of the PS/2 keyboard. All of the alphabetic keys work, but many of the non-alphabet keys and modifier keys are not yet implemented. By default the KB is connected to PB5 and CA1 for interrupts on clock edges. It is also currently uni-directional, just sending scancodes for make and break.

## conclusion

Overall, I think this is fairly complete. There are things that need work, but everything works to an acceptable standard IMO. nearly all of the addresses/settings are configurable in code.