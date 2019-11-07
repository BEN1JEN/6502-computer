.segment "CODE0"

.org $0000
LDA #$2A
STA $0000

.segment "VECTORS"
.word $4000
.word $4000
.word $4000
