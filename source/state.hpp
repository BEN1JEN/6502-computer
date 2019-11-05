#ifndef STATE_H
#define STATE_H

#include <cstdint>

class generic_device {
public:
	virtual void clock(int) = 0;
	virtual void trigger(uint8_t) = 0;
	virtual uint8_t * memory(uint8_t) = 0;
	virtual ~generic_device() {};
};

class computer_state {
private:
	uint8_t * memory_access(uint16_t);
	uint8_t ram[0x80000]; // A HM628512LP-55 chip
	uint8_t rom[0x20000]; // 2 AT28C256-15 chips

	generic_device * devices[64];
	uint8_t open_bus;
public:
	computer_state(const char *);

	void set_memory(uint16_t, uint8_t);
	uint8_t get_memory(uint16_t);
	bool add_device(generic_device *, uint8_t);
	bool remove_device(uint8_t);
};

#endif
