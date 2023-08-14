#include <stdint.h>
#include "tools.h"
#include "pl011.h"
#include "pl110.h"
#include "nor_flash.h"

typedef struct
{
  uint32_t magic;        // ASCII_LE("Img3")
  uint32_t fullSize;     // full size of fw image
  uint32_t sizeNoPack;   // size of fw image without header
  uint32_t sigCheckArea; // although that is just my name for it, this is the
                         // size of the start of the data section (the code) up to
                         // the start of the RSA signature (SHSH section)
  uint32_t ident;        // identifier of image, used when bootrom is parsing images
                         // list to find LLB (illb), LLB parsing it to find iBoot (ibot),
} img3File;

unsigned char *download_firmware(uint32_t base)
{

  pl011_t *uart0 = pl011_init(base, 9600);

  uint8_t firm_size[4];

  for (int i = 0; i < 4; i++)
  {

    firm_size[i] = pl011_getchar(uart0, base);
  }

  uint32_t firmware_size = stream2int(firm_size);

  unsigned char firmware[firmware_size];

  if (firmware_size != 0x00)
  {
    pl011_putc(uart0, UART0, 0x34);
  }
  else
  {
    pl011_putc(uart0, UART0, 0xf3);
  }

  for (int i = 0; i < firmware_size; i++)
  {

    firmware[i] = pl011_getchar(uart0, base);
  }

  unsigned char *out = firmware;

  return out;
}

void unpack_and_load_firmware(unsigned char *frwr)
{

  // TODO: check SHA1 checksum.

  img3File *file = (img3File *)frwr;
  uint32_t fw_size = file->sizeNoPack - 20;
  uint32_t magic = file->magic;

  uint32_t main_fn_addr = file->ident;

  if (magic != 0x33676d49)
  {
    halt_core();
  }
  else
  {

    unsigned char *fwz = (unsigned char *)((void *)(((uint32_t)frwr) + sizeof(img3File)));

    nor_write(&nor0, fwz, fw_size);
  }

  // execute
  void (*ret)() = (void (*)())(main_fn_addr);
  ret();

  while(1){

  }
}

void _boot_main()
{

  pl110_init(640, 480);

  nor_init();


  unsigned char* raw_data = download_firmware(UART0);
  
  unpack_and_load_firmware(raw_data);


  while (1)
  {
  }

}