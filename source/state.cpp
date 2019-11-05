#include "state.hpp"

#include <fstream>
#include <iostream>

computer_state::computer_state(const char * filename) {
	std::ifstream ifile(filename, std::ios::binary);
	ifile.read((char *)(void *)this->rom, 0x1FFFF);
}

uint8_t * computer_state::memory_access(uint16_t addr) {
	if ((addr & 0xC000) == 0x0000) {
		return &this->ram[addr];
	} else if ((addr & 0xC000) == 0xC000 && this->devices[(addr&0x3F00) >> 8] != NULL) {
		uint8_t * mem = &this->open_bus;
		if (this->devices[(addr&0x3F00) >> 8] != NULL) {
			mem = this->devices[(addr&0x3F00) >> 8]->memory((uint8_t)(addr & 0xFF));
			this->devices[(addr&0x3F00) >> 8]->trigger((uint8_t)(addr & 0xFF));
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
		std::cout << "Trigger #" << std::hex << (addr & 0x00FF) << " for device " << ((addr&0x3F00) >> 8) << "@0x" << std::hex << addr << std::endl;
		this->devices[(addr&0x3F00) >> 8]->trigger((uint8_t)(addr & 0xFF));
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
