#include "Exercise12_C.h"

Board new_board() {
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
	for (i = 0; i < 8; b.board[1][i++] = p);
	
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
}