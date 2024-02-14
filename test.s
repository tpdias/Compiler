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
	sub	sp, sp, #48
	.cfi_def_cfa_offset 48
	stp	x29, x30, [sp, #32]             ; 16-byte Folded Spill
	add	x29, sp, #32
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w8, #0
	stur	w8, [x29, #-8]                  ; 4-byte Folded Spill
	stur	wzr, [x29, #-4]
	
	mov	x9, sp
	adrp	x8, _b@PAGE
	add	x8, x8, _b@PAGEOFF
	str	x8, [x9]
	adrp	x0, l_.str@PAGE
	add	x0, x0, l_.str@PAGEOFF
	str	x0, [sp, #16]                   ; 8-byte Folded Spill
	bl	_scanf


	adrp	x8, __2@PAGE
	ldr	w0, [x8, __2@PAGEOFF]
	bl	_p
	mov	x9, x0
	ldr	x0, [sp, #16]                   ; 8-byte Folded Reload
	adrp	x8, _a@PAGE
	str	w9, [x8, _a@PAGEOFF]
	ldr	w9, [x8, _a@PAGEOFF]
                                        ; implicit-def: $x8
	mov	x8, x9
	mov	x9, sp
	str	x8, [x9]
	bl	_printf
	ldur	w0, [x29, #-8]                  ; 4-byte Folded Reload
	ldp	x29, x30, [sp, #32]             ; 16-byte Folded Reload
	add	sp, sp, #48
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

	.section	__TEXT,__cstring,cstring_literals
l_.str:                                 ; @.str
	.asciz	"%d"

.subsections_via_symbols
