#pragma once

#include "tools.h"

#ifndef _NOR_FLASH_
#define _NOR_FLASH_
#define NULL 0 

#define NOR0_FLASH_ADDRESS 0xb71b000
#define NOR0_SIZE 62
#define NOR_DEVICE_BUSY -2
#define NOR_OPERATION_SUCCESS 0 
#define NOR_OPERATION_FAILED -1
#define HEADER_SIZE 4 
#define HEADER "LUNA"


typedef struct{

    uint8_t header[4];
    uint32_t base_address;
    uint32_t size_in_mb;
    uint8_t busy;
    uint32_t written_offset;

} nor_flash_storage_t;

nor_flash_storage_t nor0;

uint8_t nor_init(){

    nor0=(nor_flash_storage_t){
        .base_address = NOR0_FLASH_ADDRESS,
        .written_offset = sizeof(nor_flash_storage_t),
        .size_in_mb = NOR0_SIZE,
        .busy = 0 
    };

    _memcpy(&nor0.header,HEADER,HEADER_SIZE);

    if(nor0.header[0] != HEADER[0] && nor0.header[1] != HEADER[1]  && nor0.header[2] != HEADER[2]  && nor0.header[3] != HEADER[3] ) {
        return NOR_OPERATION_FAILED;       
    }

    unsigned char *x = (unsigned char*)((void*)NOR0_FLASH_ADDRESS);

    _memcpy(x,&nor0,sizeof(nor_flash_storage_t));

    return NOR_OPERATION_SUCCESS;

}


uint8_t nor_write(nor_flash_storage_t *dev, unsigned char* data, uint32_t size){

    unsigned char* mem_fd = (unsigned char*)((void*)(dev->base_address+dev->written_offset));
    if(!dev->busy)
    {   

        dev->busy = 1;

        for(int i = 0 ;i < size;i++){
            mem_fd[i] = data[i];
        }
        
        dev->busy = 0;

        dev->written_offset = sizeof(nor_flash_storage_t)+size;

        return NOR_OPERATION_SUCCESS;

    }
    else{
        return NOR_DEVICE_BUSY;
    }

}

unsigned char* nor_read(nor_flash_storage_t *dev,  uint32_t size){
        
    

    unsigned char outb[size];
    _memset(outb,0x42,size);
    
    
    unsigned char* mem_fd = (unsigned char*)((void*)(dev->base_address+dev->written_offset));

    for(int i = 0 ; i < size;i++){

        outb[i] = mem_fd[i];
        
    }

    unsigned char* outz = outb;

    return outz;

}




#endif
