#include "Exercise12_C.h"

// Parse a move from a uci format
Move parse_move(char *uci) {
	Move m;
	
	m.soure_file = uci[0]-'a';
	m.soure_rank = uci[1]-'1';
	m.destination_file = uci[2]-'a';
	m.destination_rank = uci[3]-'1';
	
	m.promotion = None;
	if (uci[0] == 'n') { m.promotion = Knight; }
	if (uci[0] == 'b') { m.promotion = Bishop; }
	if (uci[0] == 'r') { m.promotion = Rook;   }
	if (uci[0] == 'q') { m.promotion = Queen;  }
	
	return m;
}

// Print a move in uci format
void print_move(Move *m) {
	PutChar(m->soure_file+'a');
	PutChar(m->soure_rank+'1');
	PutChar(m->destination_file+'a');
	PutChar(m->destination_rank+'1');
	if (m->promotion == Knight) { PutChar('n'); }
	if (m->promotion == Bishop) { PutChar('b'); }
	if (m->promotion ==   Rook) { PutChar('r'); }
	if (m->promotion ==  Queen) { PutChar('q'); }
}