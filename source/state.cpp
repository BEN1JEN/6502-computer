#include "state.hpp"

#include <fstream>

computer_state::computer_state(const char * filename) {
	std::ifstream ifile(filename, std::ios::binary);
	ifile.read((char *)&this->rom[0], 0x20000);
}

uint8_t * computer_state::memory_access(uint16_t addr) {
	// TODO: fully implement memory
	if ((addr & 0xC000) == 0x0000) {
		return &this->ram[addr];
	} else if ((addr & 0xC000) != 0xC000) {
		return &this->rom[addr & 0x3FFF];
	} else if ((addr & 0xFF00) == 0xFF00) {
		return &this->rom[(addr & 0xFF) | 0x1FF00];
	} else {
		return &this->open_bus;
	}
}

void computer_state::set_memory(uint16_t addr, uint8_t data) {
	*this->memory_access(addr) = data;
}
uint8_t computer_state::get_memory(uint16_t addr) {
	return *this->memory_access(addr);
}

colour_t computer_state::get_pixel(uint16_t x, uint16_t y) {
	uint8_t pc = this->display_buffer[(y<<8)|x];
	return this->display_colours[pc];
}


