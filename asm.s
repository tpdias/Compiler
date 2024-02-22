;Thiago Parisotto Dias - Compilador
;BEGINFUN
	.globl	_p
	.p2align	2
_p:
	.cfi_startproc
	; %bb.0:
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	str	w0, [sp, #12]
;ADD
	adrp	x8, _b@PAGE
	ldr	w8, [x8, _b@PAGEOFF]
	adrp	x9, _4@PAGE
	ldr	w9, [x9, _4@PAGEOFF]
	add	w8, w8, w9
	adrp	x9, ___temp0@PAGE
	str	w8, [x9, ___temp0@PAGEOFF]
;Return
	adrp x8, ___temp0@PAGE
	ldr w0, [x8, ___temp0@PAGEOFF]
	add	sp, sp, #16
	ret
	.cfi_endproc

;BEGINFUN
	.globl	_j
	.p2align	2
_j:
	.cfi_startproc
	; %bb.0:
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	str	w0, [sp, #12]
;Return
	adrp x8, _a@PAGE
	ldr w0, [x8, _a@PAGEOFF]
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
	mov x9, sp
	adrp	x8, ___temp1@PAGE
	add	x8, x8, ___temp1@PAGEOFF
	str	x8, [x9]
	adrp	x0, l_input.str.int@PAGE
	add	x0, x0, l_input.str.int@PAGEOFF
	bl	_scanf
;MOVE
	adrp	x8, ___temp1@PAGE
	ldr	w8, [x8, ___temp1@PAGEOFF]
	adrp	x9, _c@PAGE
	str	w8, [x9, _c@PAGEOFF]
;LABEL
___label0:
;LESS
	adrp	x8, _i@PAGE
	ldr	w8, [x8, _i@PAGEOFF]
	adrp	x9, _20@PAGE
	ldr	w9, [x9, _20@PAGEOFF]
	subs	w8, w8, w9
	cset	w8, lt
	adrp	x9, ___temp2@PAGE
	str	w8, [x9, ___temp2@PAGEOFF]
;IFZ
	adrp	x8, ___temp2@PAGE
	ldr	w8, [x8, ___temp2@PAGEOFF]
	cbz	w8, ___label1
;ADD
	adrp	x8, _c@PAGE
	ldr	w8, [x8, _c@PAGEOFF]
	adrp	x9, _1@PAGE
	ldr	w9, [x9, _1@PAGEOFF]
	add	w8, w8, w9
	adrp	x9, ___temp3@PAGE
	str	w8, [x9, ___temp3@PAGEOFF]
;MOVE
	adrp	x8, ___temp3@PAGE
	ldr	w8, [x8, ___temp3@PAGEOFF]
	adrp	x9, _c@PAGE
	str	w8, [x9, _c@PAGEOFF]
;ADD
	adrp	x8, _i@PAGE
	ldr	w8, [x8, _i@PAGEOFF]
	adrp	x9, _1@PAGE
	ldr	w9, [x9, _1@PAGEOFF]
	add	w8, w8, w9
	adrp	x9, ___temp4@PAGE
	str	w8, [x9, ___temp4@PAGEOFF]
;MOVE
	adrp	x8, ___temp4@PAGE
	ldr	w8, [x8, ___temp4@PAGEOFF]
	adrp	x9, _i@PAGE
	str	w8, [x9, _i@PAGEOFF]
;JUMP
	b	___label0
;LABEL
___label1:
;PRINT
	adrp	x8, _c@PAGE
	str	w0, [x8, _a@PAGEOFF]
	ldr	w9, [x8, _c@PAGEOFF]
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

	.globl _1
		.p2align	2
_1:
	.long 1

	.globl _2
		.p2align	2
_2:
	.long 2

	.globl _4
		.p2align	2
_4:
	.long 4

	.globl ___temp1
		.p2align	2
___temp1:
	.long 0

	.globl ___temp4
		.p2align	2
___temp4:
	.long 0

	.globl _20
		.p2align	2
_20:
	.long 20

	.globl ___temp2
		.p2align	2
___temp2:
	.long 0

	.globl _16
		.p2align	2
_16:
	.long 16

	.globl ___temp0
		.p2align	2
___temp0:
	.long 0

	.globl ___temp3
		.p2align	2
___temp3:
	.long 0

	.globl _a
		.p2align	2
_a:
	.long 2

	.globl _c
		.p2align	2
_c:
	.long 1

	.globl _b
		.p2align	2
_b:
	.long 16

	.globl _i
		.p2align	2
_i:
	.long 0

	.section	__TEXT,__cstring,cstring_literals
	l_input.str.int:                                 ; @.str
.asciz	"%d"
	l_input.str.char:                                 ; @.str
.asciz	"%c"
	.section	__TEXT,__cstring,cstring_literals
	l_.str.int:                                 ; @.str
.asciz	"%d\n"
	l_.str.char:                                 ; @.str
.asciz	"%c\n"
