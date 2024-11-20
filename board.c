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
	
	if (b->castling_rights.white_kingside)  { PutChar('K'); }
	if (b->castling_rights.white_queenside) { PutChar('Q'); }
	if (b->castling_rights.black_kingside)  { PutChar('k'); }
	if (b->castling_rights.black_queenside) { PutChar('q'); }	
	
	PutStringSB(b->current_turn==White?"White":"Black",255);
	PutStringSB(" to move\r\n",255);
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
	
	// Can't move opponents piece
	if (piece.color != b->current_turn) { return 0; }
	
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
		if (piece.type == Pawn && m->destination_rank == (piece.color==White ? 7 : 0)) { return 0; }
	}
	
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
				if (sy != (piece.color==White?1:-1)) { return 0; }
				if (dy == 2) {
					// Can't jump over piece
					int t = (m->soure_rank+m->destination_rank)/2;
					return b->board[t][m->soure_rank].type == None;
				}
				return dy == 1;
			} else if (dx == 1) {
				if (target.type == None || target.color == piece.color) { return 0; }
				return m->destination_rank-m->soure_rank == ((piece.color==White)?1:-1);
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
			
			//	Castling
			if (m->soure_file==4 && (m->destination_file==2 || m->destination_file==6)) {
				
				// Make sure source and destination rank are correct
				if (m->destination_rank != m->soure_rank || m->destination_rank != (b->current_turn==White?0:7)) { return 0; }
				
				// Make sure the rights are still there
				if (m->destination_file == 6 && b->current_turn == White && !b->castling_rights.white_kingside)  { return 0; }
				if (m->destination_file == 2 && b->current_turn == White && !b->castling_rights.white_queenside) { return 0; }
				if (m->destination_file == 6 && b->current_turn == Black && !b->castling_rights.black_kingside)  { return 0; }
				if (m->destination_file == 2 && b->current_turn == Black && !b->castling_rights.black_queenside) { return 0; }
				
				// Further checks go in is_legal()
				if (m->destination_file==2 && b->board[m->destination_rank][1].type != None) { return 0; }
				return b->board[m->destination_rank][(m->soure_file+m->destination_file)/2].type == None;
			}
			
			return dx < 2 && dy < 2;
	}
	
	// Check sliding pieces
	
	for (x = m->soure_file+sx, y = m->soure_rank+sy; x != m->destination_file || y != m->destination_rank; x+=sx,y+=sy) {
		if (b->board[y][x].type != None) { return 0; }
	}
	
	// Already guarranteed to be attacking valid square, so no further checks needed
	return 1;
}

char is_attacked(Board *b, Color color, char rank, char file) {
	int i;
	int j;
	char checkState = 0;
	for(i = 0; i < 8; i++) {
		for(j = 0; j < 8; j++) {
			Piece p = b->board[i][j];
			if(p.color == color && (checkState & 2) == 0) {
				Move potential;
				potential.soure_rank = i;
				potential.soure_file = j;
				potential.destination_rank = rank;
				potential.destination_file = file;
				potential.promotion = None;
				if(is_pseudolegal(b, &potential)) {
					return 1;
				}
			}
		}
	}
	return 0;
}

//checks if either king into check, returns 0 for no check, 1 for white check, 2 for black check, 3 for both
char is_check(Board *b) {
	int i;
	int j;
	
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
	
	checkState |= is_attacked(b, Black, white_rank, white_file);
	checkState |= is_attacked(b, White, black_rank, black_file) << 1;
	
	return checkState;
}

// 0 for illegal, 1 for legal
char is_legal(Board *b, Move *m) {
	Piece p = b->board[m->soure_rank][m->soure_file];
	
	char check;
	char checkMask = 1 << p.color;
	char psuedo = is_pseudolegal(b, m);
	if(!psuedo) {
		return 0;
	}
	
	
	// Can't castle through check
	if (b->board[m->soure_rank][m->soure_file].type == King && m->soure_file == 4 && (m->destination_file == 2 || m->destination_file == 6)) {
		if (is_attacked(b, b->current_turn^1, m->soure_rank, (m->soure_file+m->destination_file)/2)) {
			return 0;
		}
	}
	
	make_move(b, m);
	check = is_check(b) & checkMask;
	make_unmove(b);
	
	return check == 0;
	
}
void make_move(Board *b, Move *m) {
	struct game_history hist;
	
	b->current_turn ^= 1;
	
	hist.captured = b->board[m->destination_rank][m->destination_file];
	hist.castling_rights = b->castling_rights;
	hist.move = *m;
	
	b->hist[b->ply++] = hist;
		// Castling
	
	if (b->board[m->soure_rank][m->soure_file].type == King && m->soure_file == 4 && (m->destination_file == 2 || m->destination_file == 6)) {
		if (m->destination_file == 6 && m->destination_rank == 0) {
			b->board[0][7].type = None;
			b->board[0][5].type = Rook;
			b->board[0][5].color = White;
		}
		if (m->destination_file == 2 && m->destination_rank == 0) {
			b->board[0][0].type = None;
			b->board[0][3].type = Rook;
			b->board[0][3].color = White;
		}
		if (m->destination_file == 6 && m->destination_rank == 7) {
			b->board[7][7].type = None;
			b->board[7][5].type = Rook;
			b->board[7][5].color = Black;
		}
		if (m->destination_file == 2 && m->destination_rank == 7) {
			b->board[7][0].type = None;
			b->board[7][3].type = Rook;
			b->board[7][3].color = Black;
		}
		b->castling_rights.white_kingside  = 0;
		b->castling_rights.white_queenside = 0;
		b->castling_rights.black_kingside  = 0;
		b->castling_rights.black_queenside = 0;
	}
	
	b->board[m->destination_rank][m->destination_file] = b->board[m->soure_rank][m->soure_file];
	b->board[m->soure_rank][m->soure_file].type = None;
}

void make_unmove(Board *b) {
	struct game_history hist;
	Move m;
	
	if (b->ply == 0) { return; }
	
	b->current_turn ^= 1;
	
	hist = b->hist[--(b->ply)];
	m = hist.move;
	b->castling_rights = hist.castling_rights;
	
	b->board[m.soure_rank][m.soure_file] = b->board[m.destination_rank][m.destination_file];
	b->board[m.destination_rank][m.destination_file] = hist.captured;
	
	// Castling
	if (b->board[m.soure_rank][m.soure_file].type == King && m.soure_file == 4 && (m.destination_file == 2 || m.destination_file == 6)) {
		if (m.destination_file == 6 && m.destination_rank == 0) {
			b->board[0][7].type = Rook;
			b->board[0][5].type = None;
			b->board[0][7].color = White;
		}
		if (m.destination_file == 2 && m.destination_rank == 0) {
			b->board[0][0].type = Rook;
			b->board[0][3].type = None;
			b->board[0][0].color = White;
		}
		if (m.destination_file == 6 && m.destination_rank == 7) {
			b->board[7][7].type = Rook;
			b->board[7][5].type = None;
			b->board[7][7].color = Black;
		}
		if (m.destination_file == 2 && m.destination_rank == 7) {
			b->board[7][0].type = Rook;
			b->board[7][3].type = None;
			b->board[7][0].color = Black;
		}
	}
}

void for_each_pseudolegal(Board *b, void f(int i, Move m)) {
	signed char sf,sr,df,dr,i=0;
	Move m;
	
	// try to move every piece
	for (sf = 0; sf < 8; sf++) {
		for (sr = 0; sr < 8; sr++) {
			
			// don't move nothing
			if (b->board[sr][sf].type == None) { continue; }
			
			// don't move opponent piece
			if (b->board[sr][sf].color != b->current_turn) { continue; }
			
			m.soure_file = sf;
			m.soure_rank = sr;
			
			// Special logic for pawns
			if (b->board[sr][sf].type == Pawn) {
				
				// Pawns can only move one rank
				dr = sr + (b->current_turn==White?1:-1);
				
				// Pawns can move at most one file from where they start
				for (df = sf-1; df <= sf+1; df++) {
					
					m.destination_file = df;
					m.destination_rank = dr;
					
					// Pawns must promote when they reach the end
					if (dr == (b->current_turn==White)?7:0) {
						for (m.promotion = Knight; m.promotion <= Queen; m.promotion++) {
							if (is_pseudolegal(b, &m)) { f(i++,m); }
						}
					} else {
						m.promotion = None;						
						if (is_pseudolegal(b, &m)) { f(i++,m); }
					}	
				}
				
				if (sr == (b->current_turn==White?1:6)) {
					m.destination_rank = sr + (b->current_turn==White?2:-2);
					m.destination_file = sf;
					m.promotion = None;
					if (is_pseudolegal(b, &m)) { f(i++, m); }
				}
			} else {
			
				// Pieces don't promote
				m.promotion = None;
			
				// Try every square
				// TODO: don't
				for (dr = 0; dr < 8; dr++) {
					for (df = 0; df < 8; df++) {
						m.destination_file = df;
						m.destination_rank = dr;
						if (is_pseudolegal(b, &m)) { f(i++,m); }
					}
				}
			}
		}
	}
}

Board *_fe_helper_board;
int _fe_count;
void (*_fe_helper_f)(int i, Move m);

void _fe_helper(int i, Move m) {
	if (is_legal(_fe_helper_board, &m)) {
		_fe_helper_f(_fe_count++, m);
	}
}

void for_each_legal(Board *b, void f(int i, Move m)) {
	_fe_helper_board = b;
	_fe_helper_f = f;
	_fe_count = 0;
	
	for_each_pseudolegal(b, _fe_helper);
}

Move _random_move;

void _random_move_helper(int i, Move m) {
	if (rand()%(i+1) == 0) {
		_random_move = m;
	}
}

Move random_move(Board *b) {
	for_each_legal(b, _random_move_helper);
	return _random_move;
}