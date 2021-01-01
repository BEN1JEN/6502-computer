.include "monitor.h"

.segment "BSS"

.segment "CODE0"

.org $0000
;jsr monitor

.segment "VECTORS"
.word $4000
.word $4000
.word $4000
