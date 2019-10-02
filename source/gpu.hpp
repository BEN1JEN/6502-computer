#ifndef GPU_H
#define GPU_H

struct screenBuffer {
	unsigned char p[0x100*0x200];
};

class gpu {
private:
	screenBuffer * backBuffer;
	screenBuffer * frontBuffer;
public:
	gpu();
};

#endif
