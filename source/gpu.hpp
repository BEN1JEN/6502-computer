#ifndef GPU_HPP
#define GPU_HPP

#include "oldPixelGameEngine.hpp"

#include "state.hpp"

struct screen_buffer {
	uint8_t p[0x100*0x200];
};

enum gpu_task_type {
	render_line,
	render_rectangle,
	render_triangle,
	render_circle
};

struct gpu_plan {
	gpu_task_type taskType;
	uint16_t x1:9;
	uint8_t y1;
	uint16_t x2:9;
	uint8_t y2;
	uint8_t x3;
	uint8_t y3;
};

struct gpu_task {
	uint16_t progress;
	gpu_plan plan;
};

struct gpu_registers {
	uint16_t palettes;
	uint8_t attributes;
	struct {
		uint8_t x;
		uint8_t y;
	} positions[3];
};

struct colour_t {
	uint8_t r:5;
	uint8_t g:6;
	uint8_t b:5;
};

class gpu_device : public generic_device {
private:
	colour_t get_pixel(uint16_t, uint16_t);

	gpu_task currentTask;
	gpu_registers reg;

	screen_buffer * back_buffer;
	screen_buffer * front_buffer;
	colour_t display_colours[0x100];
	olc::PixelGameEngine * screen_output;
public:
	gpu_device(olc::PixelGameEngine *);

	clock(int);

	void trigger(uint8_t);
	uint8_t * memory(uint8_t);
};

#endif
