#include "gpu.hpp"

gpu::gpu(olc::PixelGameEngine * screenOutput) {
	 this->backBuffer = new screenBuffer();
	 this->frontBuffer = new screenBuffer();
	 this->screenOutput = screenOutput;
}

void gpu::frameUpdate() {
	for (int x = 0; x < self->screenOutput->ScreenWidth(); x++) {
		for (int y = 0; y < self->screenOutput->ScreenHeight(); y++) {
			colour_t c = self->get_pixel(x&0xff, y&0x1ff);
			Draw(x, y, olc::Pixel(c.r<<3, c.g<<2, c.b<<3));
		}
	}
}

colour_t computer_state::get_pixel(uint16_t x, uint16_t y) {
	uint8_t pc = this->display_buffer[(y<<8)|x];
	return this->display_colours[pc];
}

void gpu::clock(int times) {

}
