#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <cstring>

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.hpp"

#include "mos6502.hpp"

#include "state.hpp"
#include "gpu.hpp"

computer_state * state;
gpu_device * gpu;
uint64_t cycles;
mos6502 * cpu;
namespace singleton_hacks {
	uint8_t memget(uint16_t addr) {
		return state->get_memory(addr);
	}
	void memset(uint16_t addr, uint8_t data) {
		state->set_memory(addr, data);
	}
}

int cycleLength = 10000000; // running at 100% speed
class Emu6502 : public olc::PixelGameEngine {
private:
	long long cycleOffset = 0;
public:
	Emu6502() {
		sAppName = "6502 Computer Emulator";
	}
	bool OnUserCreate() override {
		gpu = new gpu_device(this);
		state->add_device(gpu, 0);
		return true;
	}
	bool OnUserUpdate(float delta) override {
		int cycleAmount = (int)((float)cycleLength*delta)-this->cycleOffset;
		unsigned long int cyclesCompleted = 0;
		cpu->Run(cycleAmount, cyclesCompleted);
		this->cycleOffset = cyclesCompleted-cycleAmount;

		state->clock(cycleAmount);
		gpu->frame_update();

		std::cout << "FPS: " << floor(1.0/delta) << "/60"
			<< " A: $" << std::hex << (int)cpu->A
			<< " X: $" << std::hex << (int)cpu->X
			<< " Y: $" << std::hex << (int)cpu->Y
			<< " PC: $" << std::hex << (int)cpu->pc
			<< " SP: $" << std::hex << (int)cpu->sp
			<< std::endl;

		return true;
	}
};

int main(int argc, char ** argv) {
	Emu6502 demo;
	state = new computer_state();
	const char * rom_path = "../roms/os65.bin";
	char option = 0;
	for (int i = 1; i < argc; i++) {
		if (option == 0) {
			if (strlen(argv[i]) == 2 && argv[i][0] == '-') {
				option = argv[i][1];
			} else {
				rom_path = argv[i];
			}
		} else {
			switch(option) {
				case 'c':
					cycleLength = strtol(argv[i], NULL, 0);
					if (cycleLength == 0) {
						std::cerr << "Invalid cycle length, must be a valid number above 0.";
					}
					break;
				default:
					std::cerr << "Invalid command line option: `-" << option << "'." << std::endl;
					return -1;
			}
			option = 0;
		}
	}
	if (!state->load_rom(rom_path)) {
		std::cerr << "Error: Could not open rom image '" << rom_path << "'." << std::endl;
		return -1;
	}
	
	cpu = new mos6502(singleton_hacks::memget, singleton_hacks::memset);
	if (demo.Construct(200, 300, 4, 2)) {
		demo.Start();
	}
	return 0;
}
