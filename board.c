#include <stdlib.h>
#include "Exercise12_C.h"

char gameoverStatus;

void new_board(Board *b) {
	int i;
	
	Piece p;
	
	p.type = None;
	
	for (i = 0; i < 8; b->board[2][i++] = p);
	for (i = 0; i < 8; b->board[3][i++] = p);
	for (i = 0; i < 8; b->board[4][i++] = p);
	for (i = 0; i < 8; b->board[5][i++] = p);
	
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
	
	b->en_pas_file = -1;
	
	b->ply = 0;
	
	b->pst_eval = 0;
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
	
	b->en_pas_file = -1;
	
	b->ply = 0;
	
	b->pst_eval = evaluate(b);
}

#define PAWN_MASK 0x2
#define KNIGHT_MASK 0x4
#define BISHOP_MASK 0x8
#define ROOK_MASK 0x10
#define QUEEN_MASK 0x20
#define KING_MASK 0x40

// Return if the piece at rank,file is in the mask and the specified color
char is_attacked_helper(Board *b, Color color, char rank, char file, char mask) {
	if (b->board[rank][file].color != color) { return 0; }
	
	if (0xF8 & (rank|file)) { return 0; }
	
	return (1<<b->board[rank][file].type) & mask;
}

// Return true if the square rank,file is attacked by a peice
char is_attacked(Board *b, Color color, char rank, char file) {
	
	char r,f;
		
	// Orthogonal
	for (r = rank+1; !(r&0xF8); r++) {
		if (b->board[r][file].type == None) { continue; }
		if (is_attacked_helper(b, color, r, file, ROOK_MASK | QUEEN_MASK)) { return 1; }
		break;
	}
	
	for (r = rank-1; !(r&0xF8); r--) {
		if (b->board[r][file].type == None) { continue; }
		if (is_attacked_helper(b, color, r, file, ROOK_MASK | QUEEN_MASK)) { return 1; }
		break;
	}
	
	for (f = file+1; !(f&0xF8); f++) {
		if (b->board[rank][f].type == None) { continue; }
		if (is_attacked_helper(b, color, rank, f, ROOK_MASK | QUEEN_MASK)) { return 1; }
		break;
	}
	
	for (f = file-1; !(f&0xF8); f--) {
		if (b->board[rank][f].type == None) { continue; }
		if (is_attacked_helper(b, color, rank, f, ROOK_MASK | QUEEN_MASK)) { return 1; }
		break;
	}
	
	// Diagonal
	for (r = rank+1, f = file+1; !((r|f)&0xF8); r++, f++) {
		if (b->board[r][f].type == None) { continue; }
		if (is_attacked_helper(b, color, r, f, BISHOP_MASK | QUEEN_MASK)) { return 1; }
		break;
	}
	
	for (r = rank+1, f = file-1; !((r|f)&0xF8); r++, f--) {
		if (b->board[r][f].type == None) { continue; }
		if (is_attacked_helper(b, color, r, f, BISHOP_MASK | QUEEN_MASK)) { return 1; }
		break;
	}
	
	for (r = rank-1, f = file+1; !((r|f)&0xF8); r--, f++) {
		if (b->board[r][f].type == None) { continue; }
		if (is_attacked_helper(b, color, r, f, BISHOP_MASK | QUEEN_MASK)) { return 1; }
		break;
	}
	
	for (r = rank-1, f = file-1; !((r|f)&0xF8); r--, f--) {
		if (b->board[r][f].type == None) { continue; }
		if (is_attacked_helper(b, color, r, f, BISHOP_MASK | QUEEN_MASK)) { return 1; }
		break;
	}
	
	if (is_attacked_helper(b, color, rank-1, file-1, (color==White?KING_MASK|PAWN_MASK:KING_MASK))) { return 1; }
	if (is_attacked_helper(b, color, rank-1, file  , KING_MASK))                                    { return 1; }
	if (is_attacked_helper(b, color, rank-1, file+1, (color==White?KING_MASK|PAWN_MASK:KING_MASK))) { return 1; }
	if (is_attacked_helper(b, color, rank  , file-1, KING_MASK))                                    { return 1; }
	if (is_attacked_helper(b, color, rank  , file+1, KING_MASK))                                    { return 1; }
	if (is_attacked_helper(b, color, rank+1, file-1, (color==Black?KING_MASK|PAWN_MASK:KING_MASK))) { return 1; }
	if (is_attacked_helper(b, color, rank  , file  , KING_MASK))                                    { return 1; }
	if (is_attacked_helper(b, color, rank+1, file+1, (color==Black?KING_MASK|PAWN_MASK:KING_MASK))) { return 1; }
	
	if (is_attacked_helper(b, color, rank+2, file+1, KNIGHT_MASK)) { return 1; }
	if (is_attacked_helper(b, color, rank+2, file-1, KNIGHT_MASK)) { return 1; }
	if (is_attacked_helper(b, color, rank-2, file+1, KNIGHT_MASK)) { return 1; }
	if (is_attacked_helper(b, color, rank-2, file-1, KNIGHT_MASK)) { return 1; }
	if (is_attacked_helper(b, color, rank+1, file+2, KNIGHT_MASK)) { return 1; }
	if (is_attacked_helper(b, color, rank+1, file-2, KNIGHT_MASK)) { return 1; }
	if (is_attacked_helper(b, color, rank-1, file+2, KNIGHT_MASK)) { return 1; }
	if (is_attacked_helper(b, color, rank-1, file-2, KNIGHT_MASK)) { return 1; }
	
	return 0;
}

//checks if either king into check, returns 0 for no check, 1 for white check, 2 for black check, 3 for both
char is_check(Board *b, Color c) {
	char rank,file;
	
	//scan for kings
	
	for(rank = 0; rank < 8; rank++) {
		for(file = 0; file < 8; file++) {
			if(b->board[rank][file].type == King) {
				if(b->board[rank][file].color == c) {
					return is_attacked(b, c^1, rank, file);
				}
			}
		}
	}
	
	return 0;
}

// 0 for illegal, 1 for legal
char is_legal(Board *b, Move *m) {
	char check;
	char sliding = 1;
	signed char x, y, dx, dy, sx, sy;
	Piece piece, target;
	
	// Piece has to move
	if (m->soure_file == m->destination_file && m->soure_rank == m->destination_rank) { return 0; }
	
	piece = b->board[m->soure_rank][m->soure_file];
	
	// Can't move opponents piece
	if (piece.color != b->current_turn) { return 0; }
	
	target = b->board[m->destination_rank][m->destination_file];
	
	// Can't attack own piece
	if (target.type != None && piece.color == target.color) { return 0; }
	
	// Check promotion
	if (m->promotion != None) {
		
		// Make sure promotion type is allowed
		if (m->promotion == Pawn) { return 0; }
		if (m->promotion == King) { return 0; }
		
		// Only pawns can be promoted
		if (piece.type != Pawn) { return 0; }
		
		// Pawns can only promote when they reach the end
		if (m->destination_rank != (piece.color==White ? 7 : 0)) { return 0; }
	} else {
		
		// Pawns must promote when the reach the end
		if (piece.type == Pawn && m->destination_rank == (piece.color==White ? 7 : 0)) { return 0; }
	}
	
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
			sliding = 0;
			if (dx == 0) {
				if (target.type != None) { return 0; }
				if (sy != (piece.color==White?1:-1)) { return 0; }
				if (dy == 2) {
					// Has to leave from starting rank
					if (m->soure_rank != (piece.color==White?1:6)) { return 0; }
					
					// Can't jump over piece
					int t = (m->soure_rank+m->destination_rank)/2;
					if (b->board[t][m->soure_file].type != None) { return 0; }
					break;
				}
				if (dy != 1) { return 0; }
				break;
			} else if (dx == 1) {
				if (target.type == None || target.color == piece.color) {
					if (m->destination_file != b->en_pas_file || m->destination_rank != (b->current_turn==White?5:2)) { return 0; }
				}
				if (m->destination_rank-m->soure_rank != ((piece.color==White)?1:-1)) {return 0; }
				break;
			} else {
				return 0;
			}
			
		case Knight:
			sliding = 0;
			if (dx*dx + dy*dy != 5) { return 0;}
			break;
			
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
			sliding = 0;
			//	Castling
			if (m->soure_file==4 && (m->destination_file==2 || m->destination_file==6)) {
				
				// Make sure source and destination rank are correct
				if (m->destination_rank != m->soure_rank || m->destination_rank != (b->current_turn==White?0:7)) { return 0; }
				
				// Cannot capture with castle
				if (b->board[m->destination_rank][m->destination_file].type != None) { return 0; }
				
				// Make sure the rights are still there
				if (m->destination_file == 6 && b->current_turn == White && !b->castling_rights.white_kingside)  { return 0; }
				if (m->destination_file == 2 && b->current_turn == White && !b->castling_rights.white_queenside) { return 0; }
				if (m->destination_file == 6 && b->current_turn == Black && !b->castling_rights.black_kingside)  { return 0; }
				if (m->destination_file == 2 && b->current_turn == Black && !b->castling_rights.black_queenside) { return 0; }
				
				// Further checks go in is_legal()
				if (m->destination_file==2 && b->board[m->destination_rank][1].type != None) { return 0; }
				if (b->board[m->destination_rank][(m->soure_file+m->destination_file)/2].type != None) { return 0; }
				if (is_check(b, b->current_turn)) {
					return 0;
				}
				if (is_attacked(b, b->current_turn^1, m->soure_rank, (m->soure_file+m->destination_file)/2)) {
					return 0;
				}
			}
			
			if (dx > 1 || dy > 1) { return 0;}
			break;
	}
	
	// Check sliding pieces
	
	
	if(sliding) {
		for (x = m->soure_file+sx, y = m->soure_rank+sy; x != m->destination_file || y != m->destination_rank; x+=sx,y+=sy) {
			if (b->board[y][x].type != None) { return 0; }
		}	
	}
			
	make_move(b, m);
	check = is_check(b, b->current_turn^1);
	make_unmove(b);
	
	return check == 0;
}

void make_move(Board *b, Move *m) {
	struct game_history hist;
	
	b->current_turn ^= 1;
	
	hist.captured = b->board[m->destination_rank][m->destination_file];
	hist.castling_rights = b->castling_rights;
	hist.en_pas_file = b->en_pas_file;
	hist.move = *m;
	hist.pst_eval = b->pst_eval;
	
	b->hist[b->ply++] = hist;
	
	// En Passant
	if (b->board[m->soure_rank][m->soure_file].type == Pawn && m->destination_file == b->en_pas_file && m->destination_rank == (b->current_turn==White?2:5)) {
		//b->pst_eval -= pos_pst_value(b, m->soure_rank, m->soure_file);
		b->board[m->soure_rank][b->en_pas_file].type = None;
	}	
	
	b->en_pas_file = -1;
	
	if (b->board[m->soure_rank][m->soure_file].type == Pawn && abs(m->destination_rank-m->soure_rank) == 2) {
		b->en_pas_file = m->soure_file;
	}
	
	// Remove castling rights on rook move
	if (m->soure_file == 0 && m->soure_rank == 0) { b->castling_rights.white_queenside = 0; }
	if (m->soure_file == 7 && m->soure_rank == 0) { b->castling_rights.white_kingside  = 0; }
	if (m->soure_file == 0 && m->soure_rank == 7) { b->castling_rights.black_queenside = 0; }
	if (m->soure_file == 7 && m->soure_rank == 7) { b->castling_rights.black_kingside  = 0; }
	
	// Remove castling rights if rook captured
	if (m->destination_file == 0 && m->destination_rank == 0) { b->castling_rights.white_queenside = 0; }
	if (m->destination_file == 7 && m->destination_rank == 0) { b->castling_rights.white_kingside  = 0; }
	if (m->destination_file == 0 && m->destination_rank == 7) { b->castling_rights.black_queenside = 0; }
	if (m->destination_file == 7 && m->destination_rank == 7) { b->castling_rights.black_kingside  = 0; }
	
	// Remove castling rights on king move
	if (b->board[m->soure_rank][m->soure_file].type == King) {
		if (b->current_turn == Black) {
			b->castling_rights.white_queenside = 0;
			b->castling_rights.white_kingside = 0;
		} else {
			b->castling_rights.black_queenside = 0;
			b->castling_rights.black_kingside = 0;
		}
	}
	
	// Castling
	if (b->board[m->soure_rank][m->soure_file].type == King && m->soure_file == 4 && (m->destination_file == 2 || m->destination_file == 6)) {
		if (m->destination_file == 6 && m->destination_rank == 0) {
			//b->pst_eval -= pos_pst_value(b, 0, 7);
			
			b->board[0][7].type = None;
			b->board[0][5].type = Rook;
			b->board[0][5].color = White;
			
			//b->pst_eval += pos_pst_value(b, 0, 5);
		}
		if (m->destination_file == 2 && m->destination_rank == 0) {
			//b->pst_eval -= pos_pst_value(b, 0, 0);
			
			b->board[0][0].type = None;
			b->board[0][3].type = Rook;
			b->board[0][3].color = White;
			
			//b->pst_eval += pos_pst_value(b, 0, 3);
		}
		if (m->destination_file == 6 && m->destination_rank == 7) {
			//b->pst_eval -= pos_pst_value(b, 7, 7);
			
			b->board[7][7].type = None;
			b->board[7][5].type = Rook;
			b->board[7][5].color = Black;
			
			//b->pst_eval += pos_pst_value(b, 7, 5);
		}
		if (m->destination_file == 2 && m->destination_rank == 7) {
			//b->pst_eval -= pos_pst_value(b, 7, 0);
			
			b->board[7][0].type = None;
			b->board[7][3].type = Rook;
			b->board[7][3].color = Black;
			
			//b->pst_eval += pos_pst_value(b, 7, 3);
		}
		
		if (b->current_turn == Black) {
			b->castling_rights.white_kingside  = 0;
			b->castling_rights.white_queenside = 0;
		} else {
			b->castling_rights.black_kingside  = 0;
			b->castling_rights.black_queenside = 0;
		}
	}
	
	b->pst_eval -= pos_pst_value(b, m->destination_rank, m->destination_file);
	b->board[m->destination_rank][m->destination_file] = b->board[m->soure_rank][m->soure_file];
	b->pst_eval += pos_pst_value(b, m->destination_rank, m->destination_file);
	
	if (m->promotion != None) {
		b->pst_eval -= pos_pst_value(b, m->destination_rank, m->destination_file);
		b->board[m->destination_rank][m->destination_file].type = m->promotion;
		b->pst_eval += pos_pst_value(b, m->destination_rank, m->destination_file);
	}
	
	b->pst_eval -= pos_pst_value(b, m->soure_rank, m->soure_file);
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
	b->en_pas_file = hist.en_pas_file;
	b->pst_eval = hist.pst_eval;
	
	b->board[m.soure_rank][m.soure_file] = b->board[m.destination_rank][m.destination_file];
	b->board[m.destination_rank][m.destination_file] = hist.captured;
	
	if (m.promotion != None) {
		b->board[m.soure_rank][m.soure_file].type = Pawn;
	}
	
	// En passant
	if (b->board[m.soure_rank][m.soure_file].type == Pawn && m.destination_file == hist.en_pas_file && m.destination_rank == (b->current_turn==White?5:2)) {
		b->board[m.soure_rank][m.destination_file].type = Pawn;
		b->board[m.soure_rank][m.destination_file].color = b->current_turn^1;
	}
	
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

void for_each_legal(Board *b, void f(int i, Move m)) {
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
			m.promotion = None;
			
			switch (b->board[sr][sf].type) {
				case None:
					break;
				case Pawn:
				
					// Pawns can only move one rank
					dr = sr + (b->current_turn==White?1:-1);
				
					// Pawns can move at most one file from where they start
					for (df = sf-1; df <= sf+1; df++) {
					
						m.destination_file = df;
						m.destination_rank = dr;
					
						// Pawns must promote when they reach the end
						if (dr == (b->current_turn==White?7:0)) {
							for (m.promotion = Knight; m.promotion <= Queen; m.promotion++) {
								if (is_legal(b, &m)) { f(i++,m); }
							}
						} else {
							m.promotion = None;						
							if (is_legal(b, &m)) { f(i++,m); }
						}	
					}
				
					if (sr == (b->current_turn==White?1:6)) {
						m.destination_rank = sr + (b->current_turn==White?2:-2);
						m.destination_file = sf;
						m.promotion = None;
						if (is_legal(b, &m)) { f(i++, m); }
					}
					break;
				case Knight:
					m.destination_rank = sr+2;
					m.destination_file = sf+1;
					if (is_legal(b, &m)) { f(i++, m); }
					m.destination_rank = sr-2;
					m.destination_file = sf+1;
					if (is_legal(b, &m)) { f(i++, m); }
					m.destination_rank = sr+2;
					m.destination_file = sf-1;
					if (is_legal(b, &m)) { f(i++, m); }
					m.destination_rank = sr-2;
					m.destination_file = sf-1;
					if (is_legal(b, &m)) { f(i++, m); }
					m.destination_rank = sr+1;
					m.destination_file = sf+2;
					if (is_legal(b, &m)) { f(i++, m); }
					m.destination_rank = sr-1;
					m.destination_file = sf+2;
					if (is_legal(b, &m)) { f(i++, m); }
					m.destination_rank = sr+1;
					m.destination_file = sf-2;
					if (is_legal(b, &m)) { f(i++, m); }
					m.destination_rank = sr-1;
					m.destination_file = sf-2;
					if (is_legal(b, &m)) { f(i++, m); }
					break;
				case King:
					m.destination_rank = sr-1;
					m.destination_file = sf-1;
					if (is_legal(b, &m)) { f(i++, m); }
					m.destination_file++;
					if (is_legal(b, &m)) { f(i++, m); }
					m.destination_file++;
					if (is_legal(b, &m)) { f(i++, m); }
					
					m.destination_rank++;
					if (is_legal(b, &m)) { f(i++, m); }
					m.destination_file -= 2;
					if (is_legal(b, &m)) { f(i++, m); }
					
					m.destination_rank++;
					if (is_legal(b, &m)) { f(i++, m); }
					m.destination_file++;
					if (is_legal(b, &m)) { f(i++, m); }
					m.destination_file++;
					if (is_legal(b, &m)) { f(i++, m); }
					
					// Test this one, maybe more could cause this
					if (sf != 4) { break; }
					m.destination_rank = sr;
					m.destination_file = sf+2;
					if (is_legal(b, &m)) { f(i++, m); }
					m.destination_file -= 4;
					if (is_legal(b, &m)) { f(i++, m); }
					
					break;
				case Queen:
					m.destination_rank = sr;
					for (df = sf; df < 8; df++) {
						m.destination_file = df;
						if (is_legal(b, &m)) { f(i++, m); }
					}
					m.destination_rank = sr;
					for (df = sf; df >= 0; df--) {
						m.destination_file = df;
						if (is_legal(b, &m)) { f(i++, m); }
					}
					m.destination_file = sf;
					for (dr = sr; dr < 8; dr++) {
						m.destination_rank = dr;
						if (is_legal(b, &m)) { f(i++, m); }
					}
					m.destination_file = sf;
					for (dr = sr; dr >= 0; dr--) {
						m.destination_rank = dr;
						if (is_legal(b, &m)) { f(i++, m); }
					}
					// Reuse bishop for queen's diagonals
					
				case Bishop:
					for (df = sf+1, dr = sr+1; !((df & 0xF8)|(dr & 0xF8)); df++, dr++) {
						m.destination_file = df;
						m.destination_rank = dr;
						if (is_legal(b, &m)) { f(i++, m); continue; }
						if (b->board[m.destination_rank][m.destination_file].type != None) { break; }
					}
					for (df = sf+1, dr = sr-1; !((df & 0xF8)|(dr & 0xF8)); df++, dr--) {
						m.destination_file = df;
						m.destination_rank = dr;
						if (is_legal(b, &m)) { f(i++, m); continue; }
						if (b->board[m.destination_rank][m.destination_file].type != None) { break; }
					}
					for (df = sf-1, dr = sr+1; !((df & 0xF8)|(dr & 0xF8)); df--, dr++) {
						m.destination_file = df;
						m.destination_rank = dr;
						if (is_legal(b, &m)) { f(i++, m); continue; }
						if (b->board[m.destination_rank][m.destination_file].type != None) { break; }
					}
					for (df = sf-1, dr = sr-1; !((df & 0xF8)|(dr & 0xF8)); df--, dr--) {
						m.destination_file = df;
						m.destination_rank = dr;
						if (is_legal(b, &m)) { f(i++, m); continue; }
						if (b->board[m.destination_rank][m.destination_file].type != None) { break; }
					}
					break;
				case Rook:
					m.destination_rank = sr;
					for (df = sf; df < 8; df++) {
						m.destination_file = df;
						if (is_legal(b, &m)) { f(i++, m); }
					}
					m.destination_rank = sr;
					for (df = sf; df >= 0; df--) {
						m.destination_file = df;
						if (is_legal(b, &m)) { f(i++, m); }
					}
					m.destination_file = sf;
					for (dr = sr; dr < 8; dr++) {
						m.destination_rank = dr;
						if (is_legal(b, &m)) { f(i++, m); }
					}
					m.destination_file = sf;
					for (dr = sr; dr >= 0; dr--) {
						m.destination_rank = dr;
						if (is_legal(b, &m)) { f(i++, m); }
					}
					break;
			}
		}
	}
}

Move _random_move;

void _random_move_helper(int i, Move m) {
	if (random()%(i+1) == 0) {
		_random_move = m;
	}
}

Move random_move(Board *b) {
	for_each_legal(b, _random_move_helper);
	return _random_move;
}

void _gameover_helper(int i, Move m) {
	gameoverStatus = 0;
}

//returns 0 if not in checkmate, 1 if yes in checkmate
char is_gameover(Board *b) {
	gameoverStatus = 1;
	for_each_legal(b, _gameover_helper);
	return gameoverStatus;
}

//returns 0 for normal play, 1 for white checkmate, 2 for black checkmate, 3 for stalemate
char get_mate_state(Board *b) {
	if(!is_gameover(b)) {
		return 0;
	} else {
		char ret = 3;
		ret ^= (is_check(b, White));
		ret ^= (is_check(b, Black) << 1);
		return ret;
	}
}