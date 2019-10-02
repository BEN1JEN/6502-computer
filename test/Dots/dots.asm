; First two bits are 1, 1, then the device ID, then 4 bits that if one act as a trigger for an event, then the device address
drawBufferSwap   = $C0F0
drawPixelTrigger = $C0F1
drawPaletteLow   = $C000
drawPaletteHigh  = $C001
drawAttributes   = $C002 ; high bit is last x1 position, then last x2 position, then 6 colour bits
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

; load pallettes 4x16 colours out of 256 colours
lda #$45
sta drawPaletteLow
lda #$67
sta drawPaletteHigh

mainLoop:
jsr prng ; write position
sta drawPositionX1
jsr prng
sta drawPositionY1
jsr prng
and $BF
sta drawAttributes
sta drawPixelTrigger
nop
nop ; give time for the gpu to render
sta drawBufferSwap
jmp mainLoop ; main loop for now

prng:
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
