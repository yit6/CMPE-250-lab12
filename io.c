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

Move _pms_move;
Board *_pms_board;
char _pms_type_dest_match;
char _pms_type_rank_match;
char _pms_type_file_match;
PieceType _pms_moved;

void _pms_helper(int i, Move m) {
	if (_pms_board->board[m.soure_rank][m.soure_file].type != _pms_moved) { return; }
	if (m.destination_file != _pms_move.destination_file) { return; }
	if (m.destination_rank != _pms_move.destination_rank) { return; }
	if (m.promotion != _pms_move.promotion) { return; }
	
	_pms_type_dest_match++;
	
	if (m.soure_file == _pms_move.soure_file) {
		_pms_type_file_match++;
	}
	
	if (m.soure_rank == _pms_move.soure_rank) {
		_pms_type_rank_match++;
	}
}

// Print a move in standard algebraic notation
void print_move_san(Board *b, Move *m) {
	char check_checkmate = 0;
	
	_pms_board = b;
	_pms_moved = b->board[m->soure_rank][m->soure_file].type;
	_pms_move = *m;
	
	_pms_type_dest_match=-1;
	_pms_type_rank_match=-1;
	_pms_type_file_match=-1;
	
	if (_pms_moved == King && m->soure_file == 4 && m->destination_file == 6) {
		puts("O-O");
		check_checkmate = 1;
	}
	if (_pms_moved == King && m->soure_file == 4 && m->destination_file == 2) {
		puts("O-O-O");
		check_checkmate = 1;
	}
	
	if(!check_checkmate) {
		PutChar("\0\0NBRQK"[_pms_moved]);
	
		for_each_legal(b,_pms_helper);
		
		if (_pms_type_rank_match && _pms_type_file_match) {
			PutChar(_pms_move.soure_file+'a');
			PutChar(_pms_move.soure_rank+'1');
		} else if (_pms_type_file_match) {
			PutChar(_pms_move.soure_rank+'1');
		} else if (_pms_type_dest_match || (_pms_moved == Pawn && b->board[m->destination_rank][m->destination_file].type != None)) {
			PutChar(_pms_move.soure_file+'a');
		}
		
		if (b->board[m->destination_rank][m->destination_file].type != None) {
			PutChar('x');
		}
		
		PutChar(m->destination_file+'a');
		PutChar(m->destination_rank+'1');
		
		if (m->promotion != None) {
			PutChar('=');
			PutChar(" PNBRQK"[m->promotion]);
		}
	}
	
	make_move(b,m);
	
	if (is_check(b, b->current_turn)) {
		PutChar(is_gameover(b)?'#':'+');
	}
	
	make_unmove(b);
}

// Pretty print the board
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
	
	puts(b->current_turn==White?"White":"Black");
	puts(" to move\r\n");
}

// Print the fen string of the board
void print_fen(Board *b) {
	signed char rank, file, empty_count = 0;
	for (rank = 7; rank >= 0; rank--) {
		for (file = 0; file < 8; file++) {
			if (b->board[rank][file].type != None) {
				if (empty_count) {
					PutChar(empty_count+'0');
					empty_count = 0;
				}
				PutChar(" PNBRQK"[b->board[rank][file].type] | (b->board[rank][file].color << 5));
			} else {
				empty_count += 1;
			}
		}
		if (empty_count) {
			PutChar(empty_count+'0');
			empty_count = 0;
		}
		PutChar('/');
	}
	puts("\b ");
	
	PutChar(b->current_turn==White?'w':'b');

	PutChar(' ');
	
	if (b->castling_rights.white_kingside)  { PutChar('K'); }
	if (b->castling_rights.white_queenside) { PutChar('Q'); }
	if (b->castling_rights.black_kingside)  { PutChar('k'); }
	if (b->castling_rights.black_queenside) { PutChar('q'); }	
	
	PutChar(' ');
	
	if (b->en_pas_file != 255) {
		PutChar(b->en_pas_file+'a');
		PutChar((b->current_turn==White)?'6':'3');
	} else {
		PutChar('-');
	}
	
	// We don't currently keep track of 50 move draw
	puts(" 0 ");
	
	PutNumU(b->ply>>2);
}

// Print a pgn file of the game history stored in the board
void print_pgn(Board *b) {
	int i, j;
	Move moves[HIST_AMT];
	
	for (i = 0; i < b->ply; i++) {
		moves[i] = b->hist[i].move;
	}
	
	new_board(b);
	
	for (j = 0; j < i; j++) {
		puts((j&1)?"\t\t":"\r\n");
		if (!(j&1)) {
			if (((j>>1)+1)>9) { PutChar(' '); }
			PutNumU((j>>1)+1);
			puts(".\t");
		}
		print_move_san(b, &moves[j]);
		make_move(b, &moves[j]);
	}
	puts("\r\n\r\n");
}