; First two bits are 1, 1, then the 6-bit device ID, then the 8-bit device address
drawBufferSwap   = $C0FF
drawPaletteLow   = $C000
drawPaletteHigh  = $C001
; Attributes: High bit is last x1 position, then last x2 position, then 6
; colour. Also triggers the drawing operation.
drawAttributes   = $C002
drawPositionX1   = $C003
drawPositionY1   = $C004

.segment "BSS"
seed: .res 2

.segment "CODE0"
; seed is 0x51C8 little-endian
lda #$C8
sta seed
lda #$51
sta seed+1

; load pallettes 4x16 colours out of 256 colours (0, 5, a, f)
lda #$50
sta drawPaletteLow
lda #$FA
sta drawPaletteHigh

mainLoop:
	lda #$FF
	tax
	tay
	drawLoop:
		jsr prng ; write position
		sta drawPositionX1
		jsr prng
		asl a
		sta drawPositionY1
		jsr prng
		and #$3f
		sta drawAttributes ; render
		inc drawPositionY1 ; make a 1x2 block, pixels are streached 2x1
		sta drawAttributes
		dey
		beq :+; if y = 0
			lda #$FF
			tay
			dex
		:
		cpx #00
	bne drawLoop ; loop until x=0
	sta drawBufferSwap ; swap buffers
jmp mainLoop ; main loop for now

prng: ; random generator from NESdev
	ldy #8     ; iteration count (generates 8 bits)
	lda seed+0
:
	asl        ; shift the register
	rol seed+1
	bcc :+
	eor #$39   ; apply XOR feedback whenever a 1 bit is shifted out
:
	dey
	bne :--
	sta seed+0
	cmp #0     ; reload flags
	rts

.segment "VECTORS" ; jump to first rom bank
.word $4000
.word $4000
.word mainLoop
