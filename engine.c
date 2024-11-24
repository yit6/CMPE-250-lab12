#include "Exercise12_C.h"

Move best;
short best_eval;
Board *engine_board;

void _best_move(int i, Move m) {
	make_move(engine_board, &m);
	
	short eval = evaluate(engine_board);
	
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