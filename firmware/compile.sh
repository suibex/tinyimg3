#!/bin/sh

arm-none-eabi-gcc firm.c -o firm.o -O0 -march=armv6 -nostdlib -lgcc -Ttext=0xb71b014
