#include "Exercise12_C.h"

Move best;
short best_eval;
Board *engine_board;

short minimax(Board *b);

void _best_move(int i, Move m) {
	make_move(engine_board, &m);
	
	short eval = minimax(engine_board);
	
	if (engine_board->current_turn==White) { eval = 0-eval; }
	
	if (i == 0 || eval > best_eval) {
		best_eval = eval;
		best = m;
	}
	
	make_unmove(engine_board);
}

Move best_move(Board *board) {
	engine_board = board;
	for_each_legal(engine_board, _best_move);
	
	return best;
}

short m1;

void _m1(int i, Move m) {
	
	make_move(engine_board, &m);
	
	short e = evaluate(engine_board);
	if (i == 0) { m1 = e; }
	
	if (engine_board->current_turn == White && e < m1) { m1 = e; }
	if (engine_board->current_turn == Black && e > m1) { m1 = e; }
	
	make_unmove(engine_board);
}

short minimax(Board *b) {
	engine_board = b;
	for_each_legal(engine_board,_m1);
	return m1;
}