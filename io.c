#include "Exercise12_C.h"

// Parse a move from a uci format
Move parse_move(char *uci) {
	Move m;
	
	m.soure_file = uci[0]-'a';
	m.soure_rank = uci[1]-'1';
	m.destination_file = uci[2]-'a';
	m.destination_rank = uci[3]-'1';
	
	m.promotion = None;
	if (uci[4] == 'n') { m.promotion = Knight; }
	if (uci[4] == 'b') { m.promotion = Bishop; }
	if (uci[4] == 'r') { m.promotion = Rook;   }
	if (uci[4] == 'q') { m.promotion = Queen;  }
	
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

void print_board(Board *b) {
	
	int rank,file;
	char divider[] = "|\r\n  +---+---+---+---+---+---+---+---+\r\n8 ";
	char files[] = "    a   b   c   d   e   f   g   h\r\n";
	
	puts(files);
	puts(divider+3);
	
	for (rank = 7; rank >= 0; rank--) {
		for (file = 0; file < 8; file++) {
			char c[4] = "|   ";
			
			c[2] = " PNBRQK"[b->board[rank][file].type] | (b->board[rank][file].color << 5);
						
			PutStringSB(c,4);
		}
		divider[40] = " 1234567"[rank];
		puts(divider);
	}
	puts(files+2);
	
	//if (b->castling_rights.white_kingside)  { PutChar('K'); }
	//if (b->castling_rights.white_queenside) { PutChar('Q'); }
	//if (b->castling_rights.black_kingside)  { PutChar('k'); }
	//if (b->castling_rights.black_queenside) { PutChar('q'); }	
	
	puts(b->current_turn==White?"White":"Black");
	puts(" to move\r\n");
}
