

.global _start

.extern _boot_main


/*
 [VE_NORFLASH0]: 0x40000000,            //First 64MB parallel NOR flash
    /* CS1: 0x44000000 .. 0x48000000 */

.org 0x000

_start: 

    b _reset
    b . 
    b _SVC_handler
    b .
    b _data_handler
    nop 
    b .
    b .

   
.org 0x100
/* stack setup, etc... calls main (C code ) function , unzips IMG3 */

_reset:

    ldr r0,=stack_base
    mov sp,r0   

    MSR     CPSR_c, 2 // disable interrupts IRQ

    sub r0,r0,#256
    mov sp,r0

    MSR     CPSR_c, 1 // disable interrupts FIQ

    sub r0,r0,#256
    mov sp,r0

    MSR CPSR_c,0 // disable interrupts SVC

    mov r0,#0x0 

    //enable interrupts

    mov r4,#0x8

    

.enable_regs:

    ldr r2,=NVIC_ISER0
    mov r1,#0x1
    str r1,[r2]

    sub r4,r4,#0x1 
    cmp r0,r4
   
    bne .enable_regs

.after_enable: 
    CPS #19 // enter svc mode


    b _boot_main


//while(1)

.loop:

    b .loop





.equ stack_base, 0x10000
.equ len_irq_fiq_stack, 256
.equ NVIC_ISER0,0xE000E100





.org 0x200

_SVC_handler:

    mov r0,#32


.org 0x300

_data_handler:

    mrc p15 ,0,r1,c1,c0,0
    cmp r1,#0x1 

    bne .no_mmu_err

    /* handle virt memory error here */
.no_mmu_err:
    hlt



    