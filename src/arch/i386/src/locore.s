
; Assembly routines for use in C

bits 32
section .text

; Exports
global outb
global inb
global halt
global load_idt
global enable_interrupts
global disable_interrupts

; outb - write byte to I/O port address
; void outb(uint16_t addr, uint8_t value)
outb:
	; Get arguments
	mov al, [esp + 8] ; value
	mov dx, [esp + 4] ; addr

	; Write out
	out dx, al
	ret

; inb - read byte from I/O port address
; uint8_t inb(uint16_t addr)
inb:
	; Get argument
	mov dx, [esp + 4]
	
	; Read in
	in al, dx
	ret

; halt - halt the system and processor
; void halt(void)
halt:
	; Clear interrupts and halt
	cli
	hlt

; load_idt - Load the idtr
; void load_idt(void* ptr)
load_idt:
	mov bx, [esp + 4]
	lidt [bx]
	ret

; enable_interrupts - Enable interrupts
; void enable_interrupts(void)
enable_interrupts:
	sti
	ret

; disable_interrupts - Disable interrupts
; void disable_interrupts(void)
disable_interrupts:
	cli
	ret
