#pragma once
#ifndef __CLCDC_H
#define __CLCDC_H

#include "tools.h"
#include <stdint.h>


#define PIXEL_WIDTH 3   // width in bytes of a pixel
#define CHAR_WIDTH  8
#define CHAR_HEIGHT 8


#define PL110_CR_EN		0x001
#define PL110_CR_PWR		0x800
#define PL110_IOBASE		0x10120000
#define PL110_PALBASE		(PL110_IOBASE + 0x200)

enum PL110Consts
{
    NUM_TIMINGS 		= 4,
    NUM_PALETTE_WORDS 	= 0x378,
    LCD_16_BPP			= 4,			// Register constant for 16 bits per pixel
};

struct PL110
{
    uint32_t  	timing[NUM_TIMINGS];
    uintptr_t 	upperFrameBufferPtr;
    uintptr_t 	lowerFrameBufferPtr;
    // TODO: QEMU veratilePB mofifies the PL110 so the next two registers are
    // the wrong way around.  
    uint32_t  	control;
    uint32_t  	interruptMaskSetClear;
    uint32_t	rawInterruptState;
    uint32_t	maskedInterruptState;
    uint32_t	interruptClear;
    uintptr_t	upperCurrent;
    uintptr_t	lowerCurrent;
    uint32_t	reserved[0x73];
	uint32_t	palette[NUM_PALETTE_WORDS];
    // TODO: Some other stuff
};
typedef struct PL110 PL110;

#define KLIB_MAKE_MASK(POS,SIZE)	((~(~0 << (SIZE))) << (POS))

#define MASKDEFINE(N,P,S)	\
	N ## _POS = (P),			\
	N ## _SIZE = (S),		\
	N ## _MASK = KLIB_MAKE_MASK((P),(S))

enum PL110RegisterMasks
{
   MASKDEFINE(PIXELS_PER_LINE,  2,  6),
   MASKDEFINE(LINES_PER_PANEL,  0, 10),
    
	MASKDEFINE(LCD_POWER	  , 11,  1),
	MASKDEFINE(LCD_ENDIAN	  ,  9,  2),
	MASKDEFINE(LCD_BGR	 	  ,  8,  1),
	MASKDEFINE(LCD_DUAL		  ,  7,  1),
	MASKDEFINE(LCD_TFT		  ,  5,  1),
	MASKDEFINE(LCD_BW		  ,  4,  1),
	MASKDEFINE(LCD_BPP		  ,  1,  3),
   MASKDEFINE(LCD_EN		  ,  0,  1),
};

typedef struct _PL110MMIO 
{
	uint32_t		volatile tim0;		//0
	uint32_t		volatile tim1;		//4
	uint32_t		volatile tim2;		//8
	uint32_t		volatile d;		//c
	uint32_t		volatile upbase;	//10
	uint32_t		volatile f;		//14
	uint32_t		volatile g;		//18
	uint32_t		volatile control;	//1c
} PL110MMIO;



typedef struct{
   
   uint32_t address; // everything you need.


}Framebuffer;

typedef struct{

   Framebuffer *fb;
   PL110 *registers;

}pl110_device;

pl110_device one_and_only_dev = {0};

typedef struct{

   uint8_t red;
   uint8_t green;
   uint8_t blue;

}pixel_data;

#define PL110_FB_ADDR 0x0c000000

uint16_t rgb565_from_triplet(uint8_t red, uint8_t green, uint8_t blue)
{
  red   >>= 3;
  green >>= 2;
  blue  >>= 3;
  return (red << 11) | (green << 5) | blue;

}

void pl110_forceUpdate(pl110_device *fb)
{
    /*
     *  In QEMU we can force an update simply by reading and writing a register 
     */
    volatile uint32_t control = ((pl110_device*)fb)->registers->control;
    ((pl110_device*)fb)->registers->control = control;
}




void pl110_write_data(pixel_data val , uint32_t x,uint32_t y){



   uint32_t base_address = PL110_FB_ADDR;

   uint16_t * dat = (uint16_t *)((void*)base_address);

   dat[x*640+y] = rgb565_from_triplet(val.red,val.green,val.blue);

   pl110_forceUpdate(&one_and_only_dev);


}



pl110_device* pl110_init(uint32_t w , uint32_t h ){
   _memset(&one_and_only_dev,0x00,sizeof(pl110_device));

   
   PL110* regs = (PL110*)PL110_IOBASE;
   one_and_only_dev.registers = regs;
   uint32_t controlRegister;

   regs->upperFrameBufferPtr = (uintptr_t )PL110_FB_ADDR;

   uint32_t ppl = w/ 16-1;

   uint32_t timing_reg= regs->timing[0];
   timing_reg &= ~PIXELS_PER_LINE_MASK;
   timing_reg |= (ppl << PIXELS_PER_LINE_POS) & PIXELS_PER_LINE_MASK;
   regs->timing[0] = timing_reg;
 

   timing_reg = regs->timing[1];
      timing_reg &= ~LINES_PER_PANEL_MASK;
      timing_reg |= ((h - 1) << LINES_PER_PANEL_POS)
                        & LINES_PER_PANEL_MASK;
      regs->timing[1] = timing_reg;
      
      
   
  controlRegister = regs->control;

   controlRegister &=    ~LCD_POWER_MASK
//        					| ~LCD_ENDIAN_MASK
               | ~LCD_BGR_MASK
//        					| ~LCD_DUAL_MASK
//        					| ~LCD_TFT_MASK
//        					| ~LCD_BW_MASK
               | ~LCD_BPP_MASK
               | ~LCD_EN_MASK;
   controlRegister |=	  LCD_POWER_MASK		// Switch it on
//        											// endian 0 = little endian
             // | LCD_BGR_MASK			// BGR 0 = RGB,** WE WANT BGR!! **
//        											// Dual 0 = one panel
               | LCD_TFT_MASK			// TFT panel
//        											// BW 0 - in colour
               | (LCD_16_BPP << LCD_BPP_POS)	// 16 bits per pixel
               | LCD_EN_MASK;			// Enable the PL110

   regs->control = controlRegister;
   
   one_and_only_dev.fb->address = PL110_FB_ADDR;

   _memcpy(&one_and_only_dev.registers,regs,sizeof(PL110));


   return (pl110_device*)&one_and_only_dev;

}



#endif


