MEMORY {
	ROM0: start=$4000, size=$8000, type=ro, file = %O, fill = yes, fillval = $00;
	ROM1: start=$4000, size=$8000, type=ro, file = %O, fill = yes, fillval = $00;
	ROM2: start=$4000, size=$8000, type=ro, file = %O, fill = yes, fillval = $00;
	ROM3: start=$4000, size=$8000, type=ro, file = %O, fill = yes, fillval = $00;
	RAM: start=$0000, size=$4000, type=rw, file = "";
}

SEGMENTS {
	CODE0:   load = ROM0, type = ro;
	DATA:    load = ROM0, run = RAM, type = rw, define = yes;
	RODATA0: load = ROM0, type = ro;
	CODE1:   load = ROM1, type = ro;
	RODATA1: load = ROM1, type = ro;
	CODE2:   load = ROM2, type = ro;
	RODATA2: load = ROM2, type = ro;
	CODE3:   load = ROM3, type = ro;
	RODATA3: load = ROM3, type = ro;
	BSS:     load = RAM, type = bss, define = yes;
	LASTPG:  load = ROM3, start = $BF00, type = ro; # Last page of ROM is always mapped to the last page of memory.
	VECTORS: load = ROM3, start = $BFFA, type = ro;
}
