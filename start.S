.syntax unified
.thumb

.section .vectors, "a"
.word _estack   /* Stack top, processor loads it in for us! */
.word oh_god    /* This is the reset vector, it'll just hang for now */
.word oh_god    /* Other exception handlers... */
.word oh_god    /* Don't worry about them for now, if they occur then we done goofed */
.word oh_god
.word oh_god
.word oh_god
.word oh_god
.word oh_god
.word oh_god
.word oh_god
.word oh_god
.word oh_god
.word oh_god
.word oh_god
.word oh_god

.section .kernel_text
oh_god:
    b oh_god

