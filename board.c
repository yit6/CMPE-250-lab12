#include "Exercise12_C.h"

Board new_board(void) {
	Board b;
	int i;
	
	Piece p;
	p.color = White;
	p.type = Pawn;
	
	// White pawns
	for (i = 0; i < 8; b.board[1][i++] = p);
	
	p.type = Knight;
	b.board[0][1] = p;
	b.board[0][6] = p;
	
	p.type = Bishop;
	b.board[0][2] = p;
	b.board[0][5] = p;
	
	p.type = Rook;
	b.board[0][0] = p;
	b.board[0][7] = p;
	
	p.type = Queen;
	b.board[0][3] = p;
	
	p.type = King;
	b.board[0][4] = p;
	
	p.color = Black;
	p.type = Pawn;
	
	// Black pawns
	for (i = 0; i < 8; b.board[6][i++] = p);
	
	p.type = Knight;
	b.board[7][1] = p;
	b.board[7][6] = p;
	
	p.type = Bishop;
	b.board[7][2] = p;
	b.board[7][5] = p;
	
	p.type = Rook;
	b.board[7][0] = p;
	b.board[7][7] = p;
	
	p.type = Queen;
	b.board[7][3] = p;
	
	p.type = King;
	b.board[7][4] = p;
	
	return b;
}

void print_board(Board *b) {
	
	int rank,file;
	char divider[] = "|\r\n  +---+---+---+---+---+---+---+---+\r\n8 ";
	char files[] = "    a   b   c   d   e   f   g   h\r\n";
	
	PutStringSB(files,255);
	PutStringSB(divider+3,255);
	
	for (rank = 7; rank >= 0; rank--) {
		for (file = 0; file < 8; file++) {
			char c[4] = "|   ";
			if (b->board[rank][file].type == None) {
				PutStringSB(c,4);
				continue;
			}
			
			c[2] = "PNBRQK"[b->board[rank][file].type-1];
			
			if (b->board[rank][file].color == Black) {
				c[2] |= 1<<5;
			}
			
			PutStringSB(c,4);
		}
		divider[40] = " 1234567"[rank];
		PutStringSB(divider,255);
	}
	PutStringSB(files+2,255);
}