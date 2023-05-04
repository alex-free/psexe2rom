; Assemble with No$PSX
; Written by MottZilla
;
; Simple EXE Loader

.nocash
.psx
.mips
.auto_nop

 org 1F000000h

dd 1F000108h
db "Licensed by "
db "Sony "
db "Computer "
db "Entertainment Inc."
db "EXE2ROM Stub"
.align 10h
db "By MottZilla"

.align 80h

dd 1F000100h
db "Licensed by "
db "Sony "
db "Computer "
db "Entertainment Inc."
db "This program is NOT Licensed by Sony Computer Entertainment Inc."

.align 100h

	jmp PREBOOT
	ret
	jmp DO_IT
	ret
	ret
	ret
	ret
	ret
PREBOOT:
	subt sp,8h
	mov [sp],r8
	mov [sp+4h],r9
	nop

	movp	r8,1F000400h	; Ptr to ROM
	movp	r9,582D5350h	; 'PS-X' Check
	mov	r8,[r8]		; Load from ROM
	nop
	jne	r8,r9,PREBOOTCLEANUP
	nop

	movp    r9,80000040h	; Sets a pointer to $80000040, the COP0 Exception Vector
	movp    r8,3C1A1F00h	; Writes these 16 bytes to $80000040
	mov     [r9],r8	; Which are instructions:
	movp    r8,275A0110h	; movp k0,1F000110h
	mov     [r9+4h],r8	; jmp k0
	movp    r8,3400008h	; nop
	mov     [r9+8h],r8	; these instructions execute when the break happens to get us to $1F000110.
	mov     r8,0h
	mov     [r9+0Ch],r8

	; Check Version 4.1 = $312E3420
	movp 	r8,0BFC7FF44h	; Set Pointer to Version in BIOS ROM
	mov	r9,[r8]		; Grab 32bit Word
	nop
	movp r8,312E3420h	; Load Word to Compare to
	nop
	jne r8,r9,NOTV4		; Check if Version 4.1
	movp    r8,801C0DACh	; V4.1 Decompressed at $801C0DAC
	jmp	HOOKSET

NOTV4:
	; Check Version 4.0 = $302E3420
	movp 	r8,0BFC7FF44h	; Set Pointer to Version in BIOS ROM
	mov	r9,[r8]		; Grab 32bit Word
	nop
	movp r8,302E3420h	; Load Word to Compare to
	nop
	jne r8,r9,NOTV3		; Check if Version 4.0
	movp    r8,801C2114h	; V4.0 Decompressed at $801C2114
	jmp	HOOKSET	
NOTV3:
	; Default to Shell Execute Address
	movp    r8,80030000h	; For Not compressed Shells

HOOKSET:
	mov     cop0r3,r8	; Setting the Break Address

	mov     r8,-1h
	mov     cop0r11,r8	; Setting Break Address Mask to FFFFFFFF
	mov     r8,0E1800000h
	mov     cop0r7,r8	; Enabling things in the BP Control Register

PREBOOTCLEANUP:
	mov     r8,[sp]	; Pull r8 from Stack
	mov     r9,[sp+4h]	; Pull r9 from Stack
	nop
	addt    sp,8h		; Adjust Stack Pointer
	ret			; Return from Pre-Boot Function.

;-------------------------------------------------------------------

; r4 is Temporary Variable
; r5 is CD Reg Pointer
; r6 is Parameter Pointer
; r7 is Parameter Length
; r8 is CD Command Number

DO_IT:
	mov cop0r7,0		; Disable Breakpoint Exception
	subt sp,18h		; pushing 20 bytes onto stack
	mov [sp],r4		; Save r4
	mov [sp+4],r5		; Save r5
	mov [sp+8],r6		; Save r6
	mov [sp+0Ch],r7		; Save r7
	mov [sp+10h],r8		; Save r8
	mov [sp+14h],ra		; Save ra
	nop

	movp r4,1F000400h	; Set Pointer to EXE Header in ROM
	nop
	mov r8,[r4+1Ch]		; r8 load with Size Counter
	mov r7,[r4+18h]		; r7 load with Load Address
	mov r14,[r4+10h]	; r14 load with Entry Point
	movp r6,1F000C00h	; r6 load with EXE data section ROM Ptr
	nop
	
LOAD:
	mov r4,[r6]	; Load Byte from ROM
	add r6,4h	; Advance ROM Ptr
	mov [r7],r4	; Write byte to RAM at Load Address Ptr
	add r7,4h	; Advance Load Ptr
	subt r8,4h	; Subtract from Size Counter
	nop	
	jgtz r8,LOAD
	nop

	; Warm Up Delay for CD-ROM
	mov r5,400000h
	nop
DELAY:
	sub r5,1h
	nop
	jgtz r5,DELAY
	nop

	; Enable Interrupts
	mov r4,1h
	movp r5,1F801800h
	movb [r5],r4	; set page 1
	mov r4,7h
	mov r6,1fh
	movb [r5+3h],r4
	movb [r5+2h],r6



	; At End
	mov r4,[sp]		; Restore r4
	mov r5,[sp+4]		; Restore r5
	mov r6,[sp+8]		; Restore r6
	mov r7,[sp+0Ch]		; Restore r7
	mov r8,[sp+10h]		; Restore r8
	mov ra,[sp+14h]		; Restore ra
	nop
	addt sp,18h		; pull stack pointer.
	jmp r14			; r14 holds 80030000 at this point.
				; Jump back to Shell
	nop
.align 10h



 .align 10h
 db "MOTTZILLA"

 .align 400h   ;..7FFh    Zerofilled

end
