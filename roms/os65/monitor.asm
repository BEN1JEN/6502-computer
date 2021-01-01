.ifndef MONITOR_INC
MONITOR_INC = 1

.include "font.h"

.scope monitor_scope

.segment "CODE3"
monitor:
	jsr font::print

.export monitor

.endscope
.endif
