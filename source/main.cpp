#include <iostream>
#include <fstream>

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

const int cycleLength = 100/60; // slowed down to 100000th the speed for testing
class Emu6502 : public olc::PixelGameEngine {
private:
	long long cycleOffset = 0;
public:
	Emu6502() {
		sAppName = "6502 Computer Emulator";
	}
	bool OnUserCreate() override {
		state->add_device(gpu, 0);
		return true;
	}
	bool OnUserUpdate(float delta) override {
		(void)delta;
		int cycleAmount = cycleLength-this->cycleOffset;
		unsigned long int cyclesCompleted = 0;
		cpu->Run(cycleAmount, cyclesCompleted);
		this->cycleOffset = cyclesCompleted-cycleAmount;

		return true;
	}
};

int main() {
	state = new computer_state("test/Dots.bin");
	cpu = new mos6502(singleton_hacks::memget, singleton_hacks::memset);
	Emu6502 demo;
	gpu = new gpu_device(&demo);
	if (demo.Construct(200, 300, 4, 2)) {
		demo.Start();
	}
	return 0;
}
