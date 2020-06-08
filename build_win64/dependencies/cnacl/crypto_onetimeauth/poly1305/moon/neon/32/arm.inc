.macro SECTION_TEXT
	.text
.endm

.macro SECTION_RODATA
	.text
.endm

.macro FN name
	.align 2
	\name:
	_\name:
.endm

.macro FN_END name
	.size \name, .-\name
	.type \name STT_FUNC
	.size _\name, .-_\name
	.type _\name STT_FUNC
.endm

.macro HIDDEN name
.endm

.macro GLOBAL name
	.globl \name
	.globl _\name
.endm

.macro GLOBAL_HIDDEN_FN name
	GLOBAL \name
	HIDDEN \name
	FN \name
.endm
