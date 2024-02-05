	.section	__DATA,__data
	.globl _1
	.p2align	2
_1:
	.long 1

	.globl _5
	.p2align	2
_5:
	.long 5

	.globl ___temp1
	.p2align	2
	.data
___temp1:
	.long 0

	.globl _120
	.p2align	2
_120:
	.long 120

	.globl ___temp0
	.p2align	2
	.data
___temp0:
	.long 0

	.globl _115
	.p2align	2
_115:
	.long 115

.LC0:
	.string	"%d"
.LC1:
	.string	"%f"
	.globl _c
	.p2align	2
_c:
	.long 5

	.globl _a
	.p2align	2
_a:
	.long 115

	.globl _x
	.p2align	2
_x:
	.long 120

	.globl _main
	.p2align	2
_main:
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
;TAC_ADD
	adrp	x8, _1@PAGE
	ldr	w8, [x8, _1@PAGEOFF]
	adrp	x9, _5@PAGE
	ldr	w9, [x9, _5@PAGEOFF]
	add	w8, w8, w9
	adrp	x9, ___temp0@PAGE
	str	w8, [x9, ___temp0@PAGEOFF]
	add	sp, sp, #16
;TAC_MOVE
	adrp	x8, ___temp0@PAGE
	ldr	w8, [x8, ___temp0@PAGEOFF]
	adrp	x9, _c@PAGE
	str	w8, [x9, _c@PAGEOFF]
;TAC_ADD
	adrp	x8, _a@PAGE
	ldr	w8, [x8, _a@PAGEOFF]
	adrp	x9, _c@PAGE
	ldr	w9, [x9, _c@PAGEOFF]
	add	w8, w8, w9
	adrp	x9, ___temp1@PAGE
	str	w8, [x9, ___temp1@PAGEOFF]
	add	sp, sp, #16
;TAC_MOVE
	adrp	x8, ___temp1@PAGE
	ldr	w8, [x8, ___temp1@PAGEOFF]
	adrp	x9, _c@PAGE
	str	w8, [x9, _c@PAGEOFF]
;TAC_MOVE
	adrp	x8, _5@PAGE
	ldr	w8, [x8, _5@PAGEOFF]
	adrp	x9, _c@PAGE
	str	w8, [x9, _c@PAGEOFF]
	add	sp, sp, #16
	ret
	.cfi_endproc
