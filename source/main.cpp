#include <iostream>
#include <fstream>
#include <cmath>

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

const int cycleLength = 5000000/30; // running at 50% speed
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
		int cycleAmount = cycleLength-this->cycleOffset;
		unsigned long int cyclesCompleted = 0;
		cpu->Run(cycleAmount, cyclesCompleted);
		this->cycleOffset = cyclesCompleted-cycleAmount;

		state->clock(cycleAmount);
		gpu->frame_update();

		std::cout << "FPS: " << floor(1.0/delta) << "/30"
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
	const char * rom_path;
	if (argc > 1) {
		rom_path = argv[1];
	} else {
		rom_path = "test/dots.bin";
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
