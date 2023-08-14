#pragma once
#include <stdint.h>

#ifndef _TOOLS_H
#define _TOOLS_H


//low lvl functions 


void halt_core(){
    __asm("hlt");
}

static inline uint32_t io_read32(uint32_t addr){

    volatile uint32_t *ad =(volatile uint32_t*)addr;
    return *ad;

}

static inline uint8_t io_read8(uint32_t addr){

    volatile uint8_t *ad =(volatile uint8_t*)((void*)addr);
    return *ad;

}


static inline uint16_t io_read16(uint32_t addr){

    volatile uint16_t *ad =(volatile uint16_t*)((void*)addr);
    return *ad;

}


static inline void io_write32(uint32_t addr,uint32_t val){

    volatile uint32_t *ad =( volatile uint32_t*)((void*)addr);
    *ad = val;

}

static inline void io_write16(uint32_t addr,uint16_t val){

   volatile uint16_t *ad =(volatile uint16_t*)((void*)addr);
    *ad = val;

}

static inline void io_write8(uint32_t addr,uint8_t val){

    volatile uint8_t *ad =(volatile uint8_t*)((void*)addr);
    *ad = val;
    
}

void _memset(void *addr, uint8_t val, int len ){

    unsigned char* x = (unsigned char*)((void*)addr);
    for(int i = 0 ;i < len;i++){
        x[i] = val;
    }

}

void* _memcpy(void *restrict dst, const void *restrict src, int len ){

    unsigned char* d = (unsigned char*)((void*)dst);
    unsigned char* sr = (unsigned char*)((void*)src);

    for(int i = 0 ;i < len;i++){
        d[i] = sr[i];
    }

    return d;

}

static inline uint32_t stream2int(const uint8_t *stream) {
 
    return (((uint32_t) stream[0]) << 24 |
            ((uint32_t) stream[1]) << 16 |
            ((uint32_t) stream[2]) <<  8 |
            ((uint32_t) stream[3]) <<  0);

}



void critical_assert(uint8_t expression){
    if(!expression){
        halt_core();
    }
}


#endif