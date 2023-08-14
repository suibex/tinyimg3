#!/bin/bash


#arm-linux-gnueabihf-as boot.s -o boot.o -march=armv6;

arm-none-eabi-gcc  boot.s main.c -march=armv6 -nostdlib -O0 -lgcc -Ttext=0 -o iboot;
