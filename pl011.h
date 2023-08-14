#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "tools.h"
#ifndef _PL011_
#define _PL011_

#define UART0 0x101F1000
#define UART1 0x101F2000
#define UART2 0x101F3000


#define UART_DR		0x00 /* data register */
#define UART_RSR_ECR	0x04 /* receive status or error clear */
#define UART_DMAWM	0x08 /* DMA watermark configure */
#define UART_TIMEOUT	0x0C /* Timeout period */
/* reserved space */
#define UART_FR		0x18 /* flag register */
#define UART_ILPR	0x20 /* IrDA low-poer */
#define UART_IBRD	0x24 /* integer baud register */
#define UART_FBRD	0x28 /* fractional baud register */
#define UART_LCR_H	0x2C /* line control register */
#define UART_CR		0x30 /* control register */
#define UART_IFLS	0x34 /* interrupt FIFO level select */
#define UART_IMSC	0x38 /* interrupt mask set/clear */
#define UART_RIS	0x3C /* raw interrupt register */
#define UART_MIS	0x40 /* masked interrupt register */
#define UART_ICR	0x44 /* interrupt clear register */
#define UART_DMACR	0x48 /* DMA control register */


/* flag register bits */
#define UART_FR_RTXDIS	(1 << 13)
#define UART_FR_TERI	(1 << 12)
#define UART_FR_DDCD	(1 << 11)
#define UART_FR_DDSR	(1 << 10)
#define UART_FR_DCTS	(1 << 9)
#define UART_FR_RI	(1 << 8)
#define UART_FR_TXFE	(1 << 7)
#define UART_FR_RXFF	(1 << 6)
#define UART_FR_TXFF	(1 << 5)
#define UART_FR_RXFE	(1 << 4)
#define UART_FR_BUSY	(1 << 3)
#define UART_FR_DCD	(1 << 2)
#define UART_FR_DSR	(1 << 1)
#define UART_FR_CTS	(1 << 0)

/* transmit/receive line register bits */
#define UART_LCRH_SPS		(1 << 7)
#define UART_LCRH_WLEN_8	(3 << 5)
#define UART_LCRH_WLEN_7	(2 << 5)
#define UART_LCRH_WLEN_6	(1 << 5)
#define UART_LCRH_WLEN_5	(0 << 5)
#define UART_LCRH_FEN		(1 << 4)
#define UART_LCRH_STP2		(1 << 3)
#define UART_LCRH_EPS		(1 << 2)
#define UART_LCRH_PEN		(1 << 1)
#define UART_LCRH_BRK		(1 << 0)

/* control register bits */
#define UART_CR_CTSEN		(1 << 15)
#define UART_CR_RTSEN		(1 << 14)
#define UART_CR_OUT2		(1 << 13)
#define UART_CR_OUT1		(1 << 12)
#define UART_CR_RTS		(1 << 11)
#define UART_CR_DTR		(1 << 10)
#define UART_CR_RXE		(1 << 9)
#define UART_CR_TXE		(1 << 8)
#define UART_CR_LPE		(1 << 7)
#define UART_CR_OVSFACT		(1 << 3)
#define UART_CR_UARTEN		(1 << 0)

#define UART_IMSC_RTIM		(1 << 6)
#define UART_IMSC_RXIM		(1 << 4)

#define UART_CLOCK_HZ  1846153

typedef struct {
 uint32_t DR;
 uint32_t RSR_ECR;
 uint8_t reserved1[0x10];
 const uint32_t FR;
 uint8_t reserved2[0x4];
 uint32_t LPR;
 uint32_t IBRD;
 uint32_t FBRD;
 uint32_t LCR_H;
 uint32_t CR;
 uint32_t IFLS;
 uint32_t IMSC;
 const uint32_t RIS;
 const uint32_t MIS;
 uint32_t ICR;
 uint32_t DMACR;
} pl011_t;


static const unsigned long BAUD_RATES[7] = {
	9600,
	19200,
	28800,
	38400,
	57600,
	76800,
	115200
};



static void pl011_flush(uint32_t base)
{

	while ((io_read32(base + UART_CR) & UART_CR_UARTEN) &&
	       !(io_read32(base + UART_FR) & UART_FR_TXFE))
		;
        
}

static bool pl011_have_rx_data(pl011_t *dev,uint32_t base_address){

    return !(io_read32((uint32_t)(base_address+UART_FR)) & UART_FR_RXFE);

}


static uint8_t pl011_getchar(pl011_t *dev,uint32_t base_address)
{

	while (!pl011_have_rx_data(dev,base_address));

	return io_read8((uint32_t)(base_address+UART_DR)) & 0xff;
}


static void pl011_putc(pl011_t* dev,uint32_t base_address, uint8_t ch)
{

	/* Wait until there is space in the FIFO */
	while (io_read32((base_address + UART_FR)) & UART_FR_TXFF)
		;

    io_write8(((base_address+UART_DR)),ch);
    

}

pl011_t *pl011_init(uint32_t base_addr, uint32_t bd_option){
    
  

	io_write32(base_addr + UART_RSR_ECR, 0);

	io_write32(base_addr + UART_CR, 0);

	if (bd_option) {
		
		int divisor = (UART_CLOCK_HZ*4) / bd_option;

		io_write32(base_addr + UART_IBRD, divisor >> 6);
		io_write32(base_addr + UART_FBRD, divisor & 0x3f);

	}


	io_write32(base_addr + UART_LCR_H, UART_LCRH_WLEN_8);
	io_write32(base_addr + UART_IMSC, UART_IMSC_RXIM | UART_IMSC_RTIM);

	io_write32(base_addr + UART_CR, UART_CR_UARTEN | UART_CR_TXE | UART_CR_RXE);

	pl011_flush(base_addr);
    

    pl011_t* dev = (pl011_t*)((void*)base_addr);

    return dev;

}
#endif