;BEGINFUN
	.globl	_p
	.p2align	2
_p:
	.cfi_startproc
	; %bb.0:
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	str	w0, [sp, #12]
;Return
	adrp x8, _b@PAGE
	ldr w0, [x8, _b@PAGEOFF]
	add	sp, sp, #16
	ret
	.cfi_endproc

;BEGINFUN
	.globl	_main
	.p2align	2
_main:
	.cfi_startproc
	; %bb.0:
	sub	sp, sp, #32
	.cfi_def_cfa_offset 32
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w8, #0
	str	w8, [sp, #8]                    ; 4-byte Folded Spill
	stur	wzr, [x29, #-4]
;INPUT
	mov x9,
	adrp	x8, ___temp0@PAGE
	add	x8, x8, ___temp0@PAGEOFF
	str	x8, [x9]
	adrp	x0, l_.str.int@PAGE
	add	x0, x0, l_.str.int@PAGEOFF
	bl	_scanf
;MOVE
	adrp	x8, ___temp0@PAGE
	ldr	w8, [x8, ___temp0@PAGEOFF]
	adrp	x9, _b@PAGE
	str	w8, [x9, _b@PAGEOFF]
;ARG
	adrp	x8, _2@PAGE
	ldr	w0, [x8, _2@PAGEOFF]
;CALL
	bl	_p
	ldr	x9, [sp]
;MOVE
	adrp	x8, ___temp1@PAGE
	ldr	w8, [x8, ___temp1@PAGEOFF]
	adrp	x9, _a@PAGE
	str	w8, [x9, _a@PAGEOFF]
;PRINT
	adrp	x8, _a@PAGE
	str	w0, [x8, _a@PAGEOFF]
	ldr	w9, [x8, _a@PAGEOFF]
		; implicit-def: $x8
	mov	x8, x9
	mov	x9, sp
	str	x8, [x9]
	adrp	x0, l_.str.int@PAGE
	add	x0, x0, l_.str.int@PAGEOFF
	bl	_printf
;Return
	ldr	w0, [sp, #8]
	ldp	x29, x30, [sp, #16]
	add	sp, sp, #32
	ret
	.cfi_endproc

	.section	__DATA,__data
	.globl _0
		.p2align	2
_0:
	.long 0

	.globl _2
		.p2align	2
_2:
	.long 2

	.globl _5
		.p2align	2
_5:
	.long 5

	.globl ___temp1
		.p2align	2
___temp1:
	.long 0

	.globl _16
		.p2align	2
_16:
	.long 16

	.globl ___temp0
		.p2align	2
___temp0:
	.long 0

	.globl _a
		.p2align	2
_a:
	.long 5

	.globl _b
		.p2align	2
_b:
	.long 16

	.section	__TEXT,__cstring,cstring_literals
	l_.str.int:                                 ; @.str
.asciz	"%d\n"
	l_.str.char:                                 ; @.str
.asciz	"%c\n"
