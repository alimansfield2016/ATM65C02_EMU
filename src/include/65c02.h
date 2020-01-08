#pragma once
#include "olc6502.h"

class atm65c02 : public olc6502{
public:
	atm65c02();
	~atm65c02();

private:
	uint8_t BBR0(); uint8_t BBR1(); uint8_t BBR2(); uint8_t BBR3(); 
	uint8_t BBR4(); uint8_t BBR5(); uint8_t BBR6(); uint8_t BBR7(); 
	uint8_t BBS0(); uint8_t BBS1(); uint8_t BBS2(); uint8_t BBS3(); 
	uint8_t BBS4(); uint8_t BBS5(); uint8_t BBS6(); uint8_t BBS7(); 
	uint8_t BRA();
	uint8_t PHX(); uint8_t PHY(); uint8_t PLX(); uint8_t PLY();
	uint8_t RMB0(); uint8_t RMB1(); uint8_t RMB2(); uint8_t RMB3(); 
	uint8_t RMB4(); uint8_t RMB5(); uint8_t RMB6(); uint8_t RMB7(); 
	uint8_t SMB0(); uint8_t SMB1(); uint8_t SMB2(); uint8_t SMB3(); 
	uint8_t SMB4(); uint8_t SMB5(); uint8_t SMB6(); uint8_t SMB7(); 
	uint8_t STP(); uint8_t STZ(); uint8_t TRB(); uint8_t TSB();
	uint8_t WAI();

};