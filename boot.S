.syntax unified
.thumb
.cpu cortex-m4
.arch armv7e-m
.section .vectors, "a"

.word _estack       /* Stack top */
.word reset         /* Reset */
.word oh_god        /* NMI */
.word oh_god        /* Hard Fault */
.word oh_god        /* Mem Manage Fault */
.word oh_god        /* Bus Fault */
.word oh_god        /* Usage Fault */
.word oh_god        /* Reserved */
.word oh_god        /* Reserved */
.word oh_god        /* Reserved */
.word oh_god        /* Reserved */
.word svc_handler   /* SVC */
.word oh_god        /* Debug Monitor */
.word oh_god        /* Reserved */
.word oh_god        /* PendSV */
.word os_tick       /* Systick */

.section .text
.thumb
.thumb_func
.type oh_god, %function
oh_god:
    b oh_god

.thumb_func
.type reset, %function
reset:
    bl cstart
    bl clocks
    bl os_start
    b oh_god

.thumb_func
.type cstart, %function
cstart:
    ldr r0, =_srodata
    ldr r1, =_erodata
    ldr r2, =_sdata
loop_copyidata:
    ldr r4, [r0], #4
    str r4, [r2], #4
    cmp r0, r1
    bcc loop_copyidata
    ldr r0, =_sbss
    ldr r1, =_ebss
    ldr r2, =#0
loop_zerobss:
    str r2, [r0], #4
    cmp r0, r1
    bcc loop_zerobss
    bx lr
