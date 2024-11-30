#include <stdio.h>
#include "Exercise12_C.h"

#define P + 100
#define N + 300
#define B + 310
#define R + 500
#define Q + 900
#define K + 100

const short piece_square[7][8][8] = 
{
	0,		0,		0,		0,		0,		0,		0,		0,
	0,		0,		0,		0,		0,		0,		0,		0,
	0,		0,		0,		0,		0,		0,		0,		0,
	0,		0,		0,		0,		0,		0,		0,		0,
	0,		0,		0,		0,		0,		0,		0,		0,
	0,		0,		0,		0,		0,		0,		0,		0,
	0,		0,		0,		0,		0,		0,		0,		0,
	0,		0,		0,		0,		0,		0,		0,		0,
	
	0 P,  0 P,  0 P,  0 P,  0 P,  0 P,  0 P,  0 P,
	50 P, 50 P, 50 P, 50 P, 50 P, 50 P, 50 P, 50 P,
	10 P, 10 P, 20 P, 30 P, 30 P, 20 P, 10 P, 10 P,
	5 P,  5 P, 10 P, 25 P, 25 P, 10 P,  5 P,  5 P,
	0 P,  0 P,  0 P, 20 P, 20 P,  0 P,  0 P,  0 P,	
	5 P, -5 P,-10 P,  0 P,  0 P,-10 P, -5 P,  5 P,
	5 P, 10 P, 10 P,-20 P,-20 P, 10 P, 10 P,  5 P,
	0 P,  0 P,  0 P,  0 P,  0 P,  0 P,  0 P,  0 P,
	
	-50 N,-40 N,-30 N,-30 N,-30 N,-30 N,-40 N,-50 N,
	-40 N,-20 N,  0 N,  0 N,  0 N,  0 N,-20 N,-40 N,
	-30 N,  0 N, 10 N, 15 N, 15 N, 10 N,  0 N,-30 N,
	-30 N,  5 N, 15 N, 20 N, 20 N, 15 N,  5 N,-30 N,
	-30 N,  0 N, 15 N, 20 N, 20 N, 15 N,  0 N,-30 N,
	-30 N,  5 N, 10 N, 15 N, 15 N, 10 N,  5 N,-30 N,
	-40 N,-20 N,  0 N,  5 N,  5 N,  0 N,-20 N,-40 N,
	-50 N,-40 N,-30 N,-30 N,-30 N,-30 N,-40 N,-50 N,
	
	-20 B,-10 B,-10 B,-10 B,-10 B,-10 B,-10 B,-20 B,
	-10 B,  0 B,  0 B,  0 B,  0 B,  0 B,  0 B,-10 B,
	-10 B,  0 B,  5 B, 10 B, 10 B,  5 B,  0 B,-10 B,
	-10 B,  5 B,  5 B, 10 B, 10 B,  5 B,  5 B,-10 B,
	-10 B,  0 B, 10 B, 10 B, 10 B, 10 B,  0 B,-10 B,
	-10 B, 10 B, 10 B, 10 B, 10 B, 10 B, 10 B,-10 B,
	-10 B,  5 B,  0 B,  0 B,  0 B,  0 B,  5 B,-10 B,
	-20 B,-10 B,-10 B,-10 B,-10 B,-10 B,-10 B,-20 B,
	
	0 R,  0 R,  0 R,  0 R,  0 R,  0 R,  0 R,  0 R,
	5 R, 10 R, 10 R, 10 R, 10 R, 10 R, 10 R,  5 R,
	-5 R,  0 R,  0 R,  0 R,  0 R,  0 R,  0 R, -5 R,
	-5 R,  0 R,  0 R,  0 R,  0 R,  0 R,  0 R, -5 R,
	-5 R,  0 R,  0 R,  0 R,  0 R,  0 R,  0 R, -5 R,
	-5 R,  0 R,  0 R,  0 R,  0 R,  0 R,  0 R, -5 R,
	-5 R,  0 R,  0 R,  0 R,  0 R,  0 R,  0 R, -5 R,
	0 R,  0 R,  0 R,  5 R,  5 R,  0 R,  0 R,  0 R,
	
	-20 Q,-10 Q,-10 Q, -5 Q, -5 Q,-10 Q,-10 Q,-20 Q,
	-10 Q,  0 Q,  0 Q,  0 Q,  0 Q,  0 Q,  0 Q,-10 Q,
	-10 Q,  0 Q,  5 Q,  5 Q,  5 Q,  5 Q,  0 Q,-10 Q,
	-5 Q,  0 Q,  5 Q,  5 Q,  5 Q,  5 Q,  0 Q, -5 Q,
	0 Q,  0 Q,  5 Q,  5 Q,  5 Q,  5 Q,  0 Q, -5 Q,
	-10 Q,  5 Q,  5 Q,  5 Q,  5 Q,  5 Q,  0 Q,-10 Q,
	-10 Q,  0 Q,  5 Q,  0 Q,  0 Q,  0 Q,  0 Q,-10 Q,
	-20 Q,-10 Q,-10 Q, -5 Q, -5 Q,-10 Q,-10 Q,-20 Q,
	
	-30 K,-40 K,-40 K,-50 K,-50 K,-40 K,-40 K,-30 K,
	-30 K,-40 K,-40 K,-50 K,-50 K,-40 K,-40 K,-30 K,
	-30 K,-40 K,-40 K,-50 K,-50 K,-40 K,-40 K,-30 K,
	-30 K,-40 K,-40 K,-50 K,-50 K,-40 K,-40 K,-30 K,
	-20 K,-30 K,-30 K,-40 K,-40 K,-30 K,-30 K,-20 K,
	-10 K,-20 K,-20 K,-20 K,-20 K,-20 K,-20 K,-10 K,
	20 K, 20 K,  0 K,  0 K,  0 K,  0 K, 20 K, 20 K,
	20 K, 30 K, 10 K,  0 K,  0 K, 10 K, 30 K, 20 K
};

short evaluate(Board *b) {
	int i;
	int j;
	short total = 0;
	for(i = 0; i < 8; i++) {
		for(j = 0; j < 8; j++) {
			total += pos_pst_value(b,i,j);
		}
	}
	return total;
}

Move best;
short best_eval;
Board *engine_board;

char num_moves;
extern char *buffer;

void _minimax(void);

void _best_move(int i, Move m) {
	short eval;
	
	make_move(engine_board, &m);
	
	eval = minimax(engine_board, 3);
	
	if (i == 0 || (eval < best_eval && engine_board->current_turn==White) || (eval > best_eval && engine_board->current_turn==Black)) {
		best_eval = eval;
		best = m;
	}
	
	make_unmove(engine_board);
	
	eval = num_moves - i;
	
	PutChar('\r');
	if (i < 10) { PutChar(' '); }
	PutNumU(i);
	PutChar('/');
	PutNumU(num_moves);
	
	PutChar(' ');
	
	PutChar('[');
	while (i--) { PutChar('#'); }
	while (eval--) { PutChar(' '); }
	PutChar(']');
}

void _move_counter(int i, Move m) {
	num_moves = i+1;
}

Move best_move(Board *board) {
	engine_board = board;
	num_moves = 0;
	for_each_legal(engine_board, _move_counter);
	
	for_each_legal(engine_board, _best_move);
	
	PutChar('\r');
	num_moves += 10;
	while (num_moves--) { PutChar(' '); }
	PutChar('\r');
	
	return best;
}

// I have no idea why this need to be initialized
// with a nonzero value, these should be assigned
// before they get read by the minimax code below
short minimax_evals[5] = { -5 };
short minimax_alpha[5] = { -5 };
short minimax_beta [5] = { -5 };

char minimax_flag;
char minimax_depth;

void _minimax_helper(int i, Move m) {
	if (minimax_flag) { return; }
	make_move(engine_board, &m);
	
	_minimax();
	
	if (i == 0 || (engine_board->current_turn == White && minimax_evals[minimax_depth+1] > minimax_evals[minimax_depth]) || (engine_board->current_turn == Black && minimax_evals[minimax_depth+1] < minimax_evals[minimax_depth])) {		
		minimax_evals[minimax_depth+1] = minimax_evals[minimax_depth];
	}
	
	// Maximizing player (white has just tested move, black next)
	if (engine_board->current_turn == Black) {
		if (minimax_evals[minimax_depth] > minimax_alpha[minimax_depth]) {
			minimax_alpha[minimax_depth] = minimax_evals[minimax_depth];
		}
		
		// Beta cutoff
		if (minimax_evals[minimax_depth] >= minimax_beta[minimax_depth]) {
			minimax_flag = 1;
		}
	}
	
	// Minimizing player (black has just tested a move, white next)
	else if (engine_board->current_turn == White) {
		if (minimax_evals[minimax_depth] < minimax_beta[minimax_depth]) {
			minimax_beta[minimax_depth] = minimax_evals[minimax_depth];
		}
		
		// Alpha cutoff
		if (minimax_evals[minimax_depth] <= minimax_alpha[minimax_depth]) {
			minimax_flag = 1;
		}
	}
	
	make_unmove(engine_board);
}

void _minimax(void) {
	
	if (minimax_depth == 0) {
		minimax_evals[0] = engine_board->pst_eval;
		return;
	}
	
	// This value will not be overwritten is there are no moves
	minimax_evals[minimax_depth] = (engine_board->current_turn==White)?0x8000:0x7FFF;
	//if (!is_check(engine_board, engine_board->current_turn)) { minimax_evals[minimax_depth] = 0; }
	
	// Pass alpha and beta to next ply
	minimax_alpha[minimax_depth-1] = minimax_alpha[minimax_depth];
	minimax_beta [minimax_depth-1] = minimax_beta [minimax_depth];
	
	minimax_depth--;
	
	for_each_legal(engine_board, _minimax_helper);
	minimax_flag = 0;
	
	minimax_depth++;
}

short minimax(Board *b, char depth) {
	engine_board = b;
	minimax_depth = depth;
	minimax_alpha[minimax_depth] = 0x8000;
	minimax_beta [minimax_depth] = 0x7FFF;
	_minimax();
	return minimax_evals[depth];
}
