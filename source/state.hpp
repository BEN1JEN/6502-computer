#ifndef STATE_H
#define STATE_H

#include <cstdint>

class computer_state {
private:
	uint8_t * memory_access(uint16_t);
	uint8_t ram[0x80000]; // A HM628512LP-55 chip
	uint8_t rom[0x10000]; // 2 AT28C256-15 chips
	uint8_t open_bus;
public:
	computer_state(const char *);

	void set_memory(uint16_t, uint8_t);
	uint8_t get_memory(uint16_t);
};

#endif
