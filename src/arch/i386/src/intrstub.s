
; Interrupt handlers

; Exception trap function - handles all processors exceptions
extern exception_trap

; Generate an exception/interrupt handler stub
%macro EXCEPTION_STUB 1
	global intr_handler_stub_%1
	intr_handler_stub_%1:
		push %1 ; Push vector onto stack
		call exception_trap
		add esp, 4
		iret
%endmacro

; Create stubs for 32 reserved exceptions
%assign v 0
%rep 32
	EXCEPTION_STUB v
%assign v (v+1)
%endrep
