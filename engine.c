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

void _minimax(void);

void _best_move(int i, Move m) {
	make_move(engine_board, &m);
	
	short eval = minimax(engine_board, 3);
	
	if (engine_board->current_turn==White) { eval = 0-eval; }
	
	if (i == 0 || eval > best_eval) {
		best_eval = eval;
		best = m;
	}
	
	make_unmove(engine_board);
	PutChar('.');
}

Move best_move(Board *board) {
	engine_board = board;
	for_each_legal(engine_board, _best_move);
	PutChar('\r');
	
	return best;
}

short minimax_evals[5] = { -5 };
char minimax_depth;

void _minimax_helper(int i, Move m) {
	make_move(engine_board, &m);
	
	_minimax();
	
	if (i == 0 || (engine_board->current_turn == White && minimax_evals[minimax_depth+1] > minimax_evals[minimax_depth]) || (engine_board->current_turn == Black && minimax_evals[minimax_depth+1] < minimax_evals[minimax_depth])) {		
		minimax_evals[minimax_depth+1] = minimax_evals[minimax_depth];
	}
	
	make_unmove(engine_board);
}

void _minimax(void) {
	
	if (minimax_depth == 0) {
		minimax_evals[0] = evaluate(engine_board);
		return;
	}
	
	minimax_depth--;
	
	for_each_legal(engine_board, _minimax_helper);
	
	minimax_depth++;
}

short minimax(Board *b, char depth) {
	engine_board = b;
	minimax_depth = depth;
	_minimax();
	return minimax_evals[depth];
}
