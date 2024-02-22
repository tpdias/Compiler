	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 14, 0	sdk_version 14, 2
	.globl	_p                              ; -- Begin function p
	.p2align	2
_p:                                     ; @p
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	str	w0, [sp, #12]
	adrp	x8, _b@PAGE
	ldr	w0, [x8, _b@PAGEOFF]
	add	sp, sp, #16
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	str	wzr, [sp, #12]
	b	LBB1_1
LBB1_1:                                 ; =>This Inner Loop Header: Depth=1
	adrp	x8, _i@PAGE
	ldr	w8, [x8, _i@PAGEOFF]
	adrp	x9, __3@PAGE
	ldr	w9, [x9, __3@PAGEOFF]
	subs	w8, w8, w9
	cset	w8, ge
	tbnz	w8, #0, LBB1_3
	b	LBB1_2
LBB1_2:                                 ;   in Loop: Header=BB1_1 Depth=1
	adrp	x9, _a@PAGE
	ldr	w8, [x9, _a@PAGEOFF]
	add	w8, w8, #1
	str	w8, [x9, _a@PAGEOFF]
	b	LBB1_1
LBB1_3:
	mov	w0, #0
	add	sp, sp, #16
	ret
	.cfi_endproc
                                        ; -- End function
	.section	__DATA,__data
	.globl	_a                              ; @a
	.p2align	2, 0x0
_a:
	.long	5                               ; 0x5

	.globl	_b                              ; @b
	.p2align	2, 0x0
_b:
	.long	2                               ; 0x2

	.globl	__2                             ; @_2
	.p2align	2, 0x0
__2:
	.long	2                               ; 0x2

	.globl	_i                              ; @i
.zerofill __DATA,__common,_i,4,2
	.globl	__3                             ; @_3
	.p2align	2, 0x0
__3:
	.long	3                               ; 0x3

.subsections_via_symbols
