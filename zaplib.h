// written by Doug Fraker 2019-2020
// NES zapper gun code

// pass it 0 or 1, which port
// returns 0 or 1 if trigger pulled
unsigned char __fastcall__ zap_shoot(unsigned char pad);


// pass it 0 or 1, which port
// returns 0 or 1 if detects light this frame
unsigned char __fastcall__ zap_read(unsigned char pad);
	
	
	
	