#include "state.hpp"

#include <fstream>
#include <iostream>

computer_state::computer_state() {}

bool computer_state::load_rom(const char * filename) {
	std::cout << "Loading rom '" << filename << "'..." << std::endl;
	std::ifstream ifile(filename, std::ios::binary);
	if (!ifile.good()) {
		return false;
	}
	ifile.read((char *)(void *)this->rom, 0x1FFFF);
	return true;
}

void computer_state::clock(int times) {
	for (int i = 0; i < 64; i++) {
		if (this->devices[i] != NULL) {
			this->devices[i]->clock(times);
		}
	}
}

uint8_t * computer_state::memory_access(uint16_t addr) {
	if ((addr & 0xC000) == 0x0000) {
		return &this->ram[addr];
	} else if ((addr & 0xC000) == 0xC000 && this->devices[(addr&0x3F00) >> 8] != NULL) {
		uint8_t * mem;
		if (this->devices[(addr&0x3F00) >> 8] != NULL) {
			mem = this->devices[(addr&0x3F00) >> 8]->memory((uint8_t)(addr & 0xFF));
		}
		if (mem == NULL) {
			mem = &this->open_bus;
		}
		return mem;
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
	if ((addr & 0xC000) == 0xC000 && this->devices[(addr&0x3F00) >> 8] != NULL) {
		this->devices[(addr&0x3F00) >> 8]->trigger((uint8_t)(addr & 0xFF));
		//std::cout << "Trigger #" << (int)(addr&0xFF) << " for dev." << (int)((addr&0x3F00) >> 8) << std::endl;
	}
}
uint8_t computer_state::get_memory(uint16_t addr) {
	return *this->memory_access(addr);
}

bool computer_state::add_device(generic_device * deviceptr, uint8_t addr) {
	if (addr >= 64) {
		return false;
	}
	if (this->devices[addr] != NULL) {
		delete this->devices[addr];
	}
	this->devices[addr] = deviceptr;
	return true;
}

bool computer_state::remove_device(uint8_t addr) {
	if (addr >= 64) {
		return false;
	}
	if (this->devices[addr] == NULL) {
		return false;
	}
	delete this->devices[addr];
	this->devices[addr] = NULL;
	return true;
}
