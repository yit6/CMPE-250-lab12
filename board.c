#include <stdlib.h>
#include "Exercise12_C.h"

void new_board(Board *b) {
	int i;
	
	Piece p;
	p.color = White;
	p.type = Pawn;
	
	// White pawns
	for (i = 0; i < 8; b->board[1][i++] = p);
	
	p.type = Knight;
	b->board[0][1] = p;
	b->board[0][6] = p;
	
	p.type = Bishop;
	b->board[0][2] = p;
	b->board[0][5] = p;
	
	p.type = Rook;
	b->board[0][0] = p;
	b->board[0][7] = p;
	
	p.type = Queen;
	b->board[0][3] = p;
	
	p.type = King;
	b->board[0][4] = p;
	
	p.color = Black;
	p.type = Pawn;
	
	// Black pawns
	for (i = 0; i < 8; b->board[6][i++] = p);
	
	p.type = Knight;
	b->board[7][1] = p;
	b->board[7][6] = p;
	
	p.type = Bishop;
	b->board[7][2] = p;
	b->board[7][5] = p;
	
	p.type = Rook;
	b->board[7][0] = p;
	b->board[7][7] = p;
	
	p.type = Queen;
	b->board[7][3] = p;
	
	p.type = King;
	b->board[7][4] = p;
	
	b->castling_rights.white_kingside = 1;
	b->castling_rights.white_queenside = 1;
	b->castling_rights.black_kingside = 1;
	b->castling_rights.black_queenside = 1;
	
	b->ply = 0;
}

void from_fen(Board *b, char *fen) {
	
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
				b->board[rank][file++] = p;
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
		
		b->board[rank][file++] = p;
	}
	
	b->castling_rights.white_kingside = 1;
	b->castling_rights.white_queenside = 1;
	b->castling_rights.black_kingside = 1;
	b->castling_rights.black_queenside = 1;
	
	b->ply = 0;
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
			if (dx == 0) {
				if (target.type != None) { return 0; }
				return m->destination_rank-m->destination_file == (piece.color==White)?1:-1;
			} else if (dx == 1) {
				if (target.type == None || target.color == piece.color) { return 0; }
				return m->destination_rank-m->destination_file == (piece.color==White)?1:-1;
			} else {
				return 0;
			}
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

//checks if a move puts either king into check, returns 0 for no check, 1 for black check, 2 for white check, 3 for both
char is_check(Board *b) {
	int i;
	int j;
	int k;
	
	int white_rank = -1;
	int white_file = -1;
	int black_rank = -1;
	int black_file = -1;
	
	int checkState = 0;
	
	//scan for kings
	
	for(i = 0; i < 8; i++) {
		for(j = 0; j < 8; j++) {
			if(b->board[i][j].type == King) {
				if(b->board[i][j].color == White) {
					white_rank = i;
					white_file = j;
				} else {
					black_rank = i;
					black_file = j;
				}
			}
		}
	}

	// check if each piece can move to king
	
	for(i = 0; i < 8; i++) {
		for(j = 0; j < 8; j++) {
			Piece p = b->board[i][j];
			if(p.color == Black && (checkState & 2) == 0) {
				Move potential;
				potential.soure_rank = i;
				potential.soure_file = j;
				potential.destination_rank = white_rank;
				potential.destination_file = white_file;
				potential.promotion = None;
				//PutStringSB("king pow penis\r\n", 255);
				if(is_pseudolegal(b, &potential)) {
					checkState |= 2;
				}
			}
			if(p.color == White && (checkState & 1) == 0) {
				Move potential;
				potential.soure_rank = i;
				potential.soure_file = j;
				potential.destination_rank = black_rank;
				potential.destination_file = black_file;
				potential.promotion = None;
				if(is_pseudolegal(b, &potential)) {
					checkState |= 1;
				}
			}
		}
	}
	return checkState;
}

// 0 for illegal, 1 for legal
char is_legal(Board *b, Move *m) {
	Piece p = b->board[m->soure_rank][m->soure_file];
	
	char check;
	char checkMask = p.color << 1;
	char psuedo = is_pseudolegal(b, m);
	if(!psuedo) {
		return 0;
	}
	
	//make_move(b, m);
	check = is_check(b) & checkMask;
	//make_unmove(b);
	
	return check == 0;
	
}
void make_move(Board *b, Move *m) {
	struct game_history hist;
	
	hist.captured = b->board[m->destination_rank][m->destination_file];
	hist.castling_rights = b->castling_rights;
	hist.move = *m;
	
	b->hist[b->ply++] = hist;
	
	b->board[m->destination_rank][m->destination_file] = b->board[m->soure_rank][m->soure_file];
	b->board[m->soure_rank][m->soure_file].type = None;
}

void make_unmove(Board *b) {
	struct game_history hist;
	Move m;
	
	if (b->ply == 0) { return; }
	
	hist = b->hist[--(b->ply)];
	m = hist.move;
	
	b->board[m.soure_rank][m.soure_file] = b->board[m.destination_rank][m.destination_file];
}
