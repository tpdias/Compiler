	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 14, 0	sdk_version 14, 2
	.globl	_j                              ; -- Begin function j
	.p2align	2
_j:                                     ; @j
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	str	w0, [sp, #12]
	str	w1, [sp, #8]
	str	w2, [sp, #4]
	str	w3, [sp]
	ldr	w0, [sp, #12]
	add	sp, sp, #16
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_x                              ; -- Begin function x
	.p2align	2
_x:                                     ; @x
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	str	w0, [sp, #12]
	adrp	x8, _c@PAGE
	ldr	w8, [x8, _c@PAGEOFF]
	adrp	x9, _a@PAGE
	str	w8, [x9, _a@PAGEOFF]
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
	adrp	x8, _a@PAGE
	str	x8, [sp]                        ; 8-byte Folded Spill
	ldr	w0, [x8, _a@PAGEOFF]
	adrp	x9, _b@PAGE
	ldr	w1, [x9, _b@PAGEOFF]
	adrp	x9, _c@PAGE
	ldr	w2, [x9, _c@PAGEOFF]
	ldr	w3, [x8, _a@PAGEOFF]
	bl	_j
	ldr	x8, [sp]                        ; 8-byte Folded Reload
	ldr	w0, [x8, _a@PAGEOFF]
	bl	_x
	ldr	w0, [sp, #8]                    ; 4-byte Folded Reload
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	add	sp, sp, #32
	ret
	.cfi_endproc
                                        ; -- End function
	.section	__DATA,__data
	.globl	_a                              ; @a
	.p2align	2, 0x0
_a:
	.long	5                               ; 0x5

	.globl	_b                              ; @b
.zerofill __DATA,__common,_b,4,2
	.globl	_c                              ; @c
	.p2align	2, 0x0
_c:
	.long	5                               ; 0x5

.subsections_via_symbols
