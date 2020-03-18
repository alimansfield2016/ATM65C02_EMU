#ifndef CONFIG_H
#define CONFIG_H

#define CONFIG_STD
#ifdef CONFIG_STD

#define CPU_FREQ	1000000.0

#define LCDW		16
#define LCDH		2
#define LCD_NDISP	1
#define LCDX		100
#define LCDY		20
#define CURSOR_FULL_BLINK

#define SRAM_START 	0x0000
#define SRAM_END 	0x3FFF

#define VIA_START	0x6000
#define VIA_END		0x7FFF

#define ROM_START	0x8000
#define ROM_END	 	0xFFFF

// For use when LCD simulated connection to VIA, select between data on PORTA or PORTB
// #define LCD_PORTA
#define LCD_PORTB

#define LCD_SW_RS_RW

// #define LCD_NO_DELAY

#else


#define LCDW 		40
#define LCDH 		4
#define LCD_NDISP 	2
#define LCDX		2
#define LCDY		2
#define LCD_BUS

#define LCD_START 	0x6000
#define LCD_END		0x67FF

#define SRAM_START 	0x0000
#define SRAM_END 	0x5FFF

#define VIA_START	0x7000
#define VIA_END		0x77FF

#define KB

#define VIA2
#define VIA2_START	0x7800
#define VIA2_END	0x7FFF

#endif

//set the default clock frequency to 1MHz
#ifndef CPU_FREQ
#define CPU_FREQ 1000000.0		//1MHz
#endif

#define LCD_FREQ 270000.0	//lcd clock runs at 270KHz

#define TIMESTEP 1.0/1.8432000


//If any of the address ranges are left undefined, we default them to start > end so that they can never be addressed

#ifndef LCD_START
#define LCD_START 0x0001
#endif
#ifndef LCD_END
#define LCD_END   0x0000
#endif

#ifndef ACIA_START
#define ACIA_START 0x0001
#endif
#ifndef ACIA_END
#define ACIA_END   0x0000
#endif

#ifndef VIA2_START
#define VIA2_START 0x0001
#endif
#ifndef VIA2_END
#define VIA2_END   0x0000
#endif

#ifndef ROM_START
#define ROM_START	0x8000
#endif
#ifndef ROM_END
#define ROM_END	 	0xFFFF
#endif



#endif