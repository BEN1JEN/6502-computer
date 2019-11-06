#include "gpu.hpp"

gpu_device::gpu_device(olc::PixelGameEngine * screenOutput) {
	this->back_buffer = new screen_buffer();
	this->front_buffer = new screen_buffer();
	this->screen_output = screen_output;

	for (int i = 0; i < 0x100; i++) {
		this->display_colours[i].r = ((uint8_t)i & 7) << 2;
		this->display_colours[i].g = ((uint8_t)i >> 3 & 7) << 3;
		this->display_colours[i].b = ((uint8_t)i >> 6 & 3) << 3;
	}
}

colour_t gpu_device::get_pixel(uint16_t x, uint16_t y) {
	uint8_t pc = this->front_buffer->p[(y<<8)|x];
	return this->display_colours[pc];
}

void gpu_device::frame_update() {
	for (int x = 0; x < this->screen_output->ScreenWidth(); x++) {
		for (int y = 0; y < this->screen_output->ScreenHeight(); y++) {
			colour_t c = this->get_pixel(x&0xff, y&0x1ff);
			this->screen_output->Draw(x, y, olc::Pixel(c.r<<3, c.g<<2, c.b<<3));
		}
	}
}

void gpu_device::clock(int times) {
	for (int i = 0; i < times; i++) {
	
	}
}

#include <iostream>
void gpu_device::trigger(uint8_t trigger) {
	if (trigger == 2) {
		gpu_plan * plan = &this->tasks[this->last_task++];
		plan->taskType = render_line;
		plan->colour = this->reg.attributes & 0x3F;
		plan->y1 = (this->reg.attributes & 0x80) << 1 | this->reg.positions[0].y;
		plan->y2 = (this->reg.attributes & 0x40) << 2 | this->reg.positions[1].y;
		plan->y3 = this->reg.positions[2].y; plan->x3 = this->reg.positions[2].x;
		plan->x1 = this->reg.positions[0].x; plan->x2 = this->reg.positions[1].x;
	} else if (trigger == 0xFF) {
		screen_buffer * tmp = this->back_buffer;
		this->back_buffer = this->front_buffer;
		this->front_buffer = tmp;
	}
}

uint8_t * gpu_device::memory(uint8_t addr) {
	if (addr < 8) {
		return (uint8_t *)&this->reg + addr;
	}
	return 0;
}
