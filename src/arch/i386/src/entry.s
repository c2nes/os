
; Entry point for kernel, this module is linked into the very start of the
; kernel binary

bits 32
section .text

extern main
global _start

_start:
	; Perform a jump into the kernels main routine
	jmp main
