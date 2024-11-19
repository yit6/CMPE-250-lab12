#include <stdlib.h>
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

Board from_fen(char *fen) {
	Board b;
	
	char rank=7, file=0;
	char c;
	
	while ((c = *(fen++)) != ' ') {
		Piece p;
		
		if (c == '/') {
			rank--;
			file=0;
			continue;
		}
		
		if ('1' <= c && c <= '8') {
			p.type = None;
			while (c-- != '0') {
				b.board[rank][file++] = p;
			}
			continue;
		}
		
		p.color = (c&(1<<5)) ? Black : White;
		
		c |= 1<<5;
		
		if (c == 'p') { p.type = Pawn; }
		if (c == 'n') { p.type = Knight; }
		if (c == 'b') { p.type = Bishop; }
		if (c == 'r') { p.type = Rook; }
		if (c == 'q') { p.type = Queen; }
		if (c == 'k') { p.type = King; }
		
		b.board[rank][file++] = p;
	}
	
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

/***
*
* This function checks if a move is psuedolegal, meaning that the piece movement is legal, but does not check
* if the move violates the king being in check.
*
* Param b: Board object
* Param m: Move object
*
* Returns: Code representing psuedolegality of move, 0 for illegal, 1 for psuedolegal
*/
char is_pseudolegal(Board *b, Move *m) {
	signed char x, y, dx, dy, sx, sy;
	
	Piece piece = b->board[m->soure_rank][m->soure_file];
	Piece target = b->board[m->destination_rank][m->destination_file];
	
	// Can't move nothing
	if (piece.type == None) { return 0; }
	
	// Piece has to move
	if (m->soure_file == m->destination_file && m->soure_rank == m->destination_rank) { return 0; }
	
	// Check promotion
	if (m->promotion != None) {
		
		// Make sure promotion type is allowed
		if (m->promotion == None) { return 0; }
		if (m->promotion == Pawn) { return 0; }
		if (m->promotion == King) { return 0; }
		
		// Only pawns can be promoted
		if (piece.type != Pawn) { return 0; }
		
		// Pawns can only promote when they reach the end
		if (m->destination_rank != piece.color==White ? 7 : 0) { return 0; }
	} else {
		
		// Pawns must promote when the reach the end
		if (piece.type == Pawn && m->destination_rank == piece.color==White ? 7 : 0) { return 0; }
	}
	
	// Castling goes here
	
	// Can't attack own piece
	if (target.type != None && piece.color == target.color) { return 0; }
	
	dx = m->destination_file-m->soure_file;
	dy = m->destination_rank-m->soure_rank;
		
	sx = (dx > 0) - (dx < 0);
	sy = (dy > 0) - (dy < 0);
	
	dx = abs(dx);
	dy = abs(dy);
	
	// Check piece movement, return false if invalid
	// and true for valid moves for non-sliding pieces
	switch (piece.type) {
		case None:
			return 0;
		case Pawn:
			return 0;
		case Knight:
			return dx*dx + dy*dy == 5;
		case Bishop:
			if (dx != dy && dx != -dy) { return 0; }
			break;
		case Rook:
			if (dx != 0 && dy != 0) { return 0; }
			break;
		case Queen:
			if (dx != 0 && dy != 0 && dx != dy && dx != -dy) { return 0; }
			break;
		case King:
			return dx < 2 && dy < 2;
	}
	
	// Check sliding pieces
	
	for (x = m->soure_file+sx, y = m->soure_rank+sy; x != m->destination_file || y != m->destination_rank; x+=sx,y+=sy) {
		if (b->board[y][x].type != None) { return 0; }
	}
	
	// Already guarranteed to be attacking valid square, so no further checks needed
	return 1;
}

//checks if a move puts either king into check, returns 0 for no check, 1 for black check, 2 for white check
//does not check if move was legal
char is_check(Board *b, Move *m) {
	int i = 0;
	int j = 0;
	int k = 0;
	
	int rank = m->destination_rank;
	int file = m->destination_file;
	Piece mover = b->board[m->soure_rank][m->soure_file];
	
	int offset = mover.color == White ? 1 : -1;
	if(m->promotion != 0) {
		mover.type = m->promotion;
	}
	switch(mover.type) {
		case None:
			return 0; //can't exactly put something into check with nothing
		case Pawn:
			if((b->board[rank + offset][file + 1].type == King && b->board[rank + offset][file + 1].color != mover.color) ||
						(b->board[rank + offset][file - 1].type == King && b->board[rank + offset][file - 1].color != mover.color)) {
				return 2 - mover.color;
			}
			break;
		case Knight: //how tf do i do this clean
			for(i = m->destination_rank - 2; i < m->destination_rank + 2; i++) {
				for(j = m->destination_file - 2; j < m->destination_file + 2; j++) {
					int dx = i - m->destination_rank;
					int dy = j - m->destination_file;
					Piece p = b->board[i][j];
					if(dx * dx + dy * dy == 5 && p.type == King && p.color != mover.color) { //checking opposing king
						return 1 + p.color;
					}
				}
			}
			return 0;
		case Bishop:
			for(i = -1; i <= 1; i += 2) {
				for(j = -1; j <= 1; j += 2) {
					for(k = 0; k < 8; k++) { //who doesn't love a triple for
						Piece p;
						if(rank + i * k < 0 || rank + i * k >= 8 ||
									file + j * k < 0 || file + j * k >= 8) {
							break;
						}
						p = b->board[rank + i * k][file + j * k];
						if(p.type != None) {
							if(p.type == King && p.color != mover.color) { // hit opposing king
								return 1 + p.color;
							} else { // (ray?)cast has hit a non king piece
								break;
							}
						}
					}
				}
			}
			return 0;
		case Rook: //change this so it isnt a bishop
			for(i = 1; i < 8; i += 2) {
				int dx = i % 3 - 1;
				int dy = i / 3 - 1;
				for(k = 0; k < 8; k++) {
					Piece p;
					if(rank + dx * k < 0 || rank + dx * k >= 8 ||
								file + dy * k < 0 || file + dy * k >= 8) {
						break;
					}
					p = b->board[rank + dx * k][file + dy * k];
					if(p.type != None) {
						if(p.type == King && p.color != mover.color) { // hit opposing king
							return 1 + p.color;
						} else { // (ray?)cast has hit a non king piece
							break;
						}
					}
				}
			}
			return 0;
		case Queen:
			for(i = -1; i <= 1; i++) {
				for(j = -1; j <= 1; j++) {
					if(i*i + j*j == 0) { //wow only one branch operation! wahoo! (this was unnecessary)
						continue;
					}
					for(k = 0; k < 8; k++) { //who doesn't love a triple for
						Piece p;
						if(rank + i * k < 0 || rank + i * k >= 8 ||
									file + j * k < 0 || file + j * k >= 8) {
							break;
						}
						p = b->board[rank + i * k][file + j * k];
						if(p.type != None) {
							if(p.type == King && p.color != mover.color) { // hit opposing king
								return 1 + p.color;
							} else { // (ray?)cast has hit a non king piece
								break;
							}
						}
					}
				}
			}
			return 0;
		case King: //we figuring this shit out in diffeq fr
			break;
	}
	return 0;
}
