#ifndef GPU_H
#define GPU_H

#include "olcPixelGameEngine.hpp"

struct screen_buffer {
	unsigned char p[0x100*0x200];
};

struct colour_t {
	uint8_t r:5;
	uint8_t g:6;
	uint8_t b:5;
};

class gpu {
private:
	colour_t get_pixel(uint16_t, uint16_t);

	screen_buffer * back_buffer;
	screen_buffer * front_buffer;
	colour_t display_colours[0x100];
	olc::PixelGameEngine * screen_output;
public:
	gpu(olc::PixelGameEngine *);
};

#endif
