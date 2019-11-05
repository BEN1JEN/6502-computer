#ifndef STATE_H
#define STATE_H

#include <cstdint>

struct colour_t {
	uint8_t r:5;
	uint8_t g:6;
	uint8_t b:5;
};

class generic_device {
public:
	virtual void clock(int);
	virtual void trigger(uint8_t);
	virtual uint8_t * memory(uint8_t);
}

class computer_state {
private:
	uint8_t * memory_access(uint16_t);
	uint8_t ram[0x80000]; // A HM628512LP-55 chip
	uint8_t rom[0x10000]; // 2 AT28C256-15 chips
	uint8_t display_buffer[0x20000]; // A HM628128LP-55 chip
	colour_t display_colours[0x100];
	generic_device * devices[];
	uint8_t open_bus;
public:
	computer_state(const char *);

	void set_memory(uint16_t, uint8_t);
	uint8_t get_memory(uint16_t);
	colour_t get_pixel(uint16_t, uint16_t);
};

#endif
