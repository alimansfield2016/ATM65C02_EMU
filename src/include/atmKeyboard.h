#pragma once

#include <map>
#include <vector>
#include <list>

class atm6522;


class atmKeyboard
{
public:
	atmKeyboard(/* args */);
	~atmKeyboard();

	void clock();
	void ConnectVIA(atm6522* v);
	void setState(bool onoff);

private:
	/* data */
	atm6522* via;

	bool kb_on = false;
	bool typematic = true;
	uint16_t typematic_delay = 500;		// 500ms
	uint16_t typematic_rate = 109;		// 10.9cps - 10.9*100ms
	uint16_t typematic_timeout;
	std::list<uint8_t> typematic_code;

	bool keyState[256];
	bool prevKeyState[256];
	bool clock_state = false;
	uint8_t com_state;
	uint8_t com_byte;
	bool parity;
	std::list<uint8_t> com_message;
	std::map<size_t, uint8_t> ScanCodeMap;
	std::map<uint8_t, std::list<uint8_t>> MakeMap;
	std::map<uint8_t, std::list<uint8_t>> BreakMap;
};
