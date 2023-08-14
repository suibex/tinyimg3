#include <stdint.h>

#ifndef _PL110_COMP_H
#define _PL110_COMP_H


typedef struct{

   uint8_t red;
   uint8_t green;
   uint8_t blue;

}pixel_data;

#define PL110_FB_ADDR 0x200000

uint16_t rgb565_from_triplet(uint8_t red, uint8_t green, uint8_t blue)
{
  red   >>= 3;
  green >>= 2;
  blue  >>= 3;
  return (red << 11) | (green << 5) | blue;

}

void pl110_write_data(pixel_data val , uint32_t x,uint32_t y){



   uint32_t base_address = PL110_FB_ADDR;

   uint16_t * dat = (uint16_t *)((void*)base_address);
   dat[x*640+y] = rgb565_from_triplet(val.red,val.green,val.blue);


}



#endif