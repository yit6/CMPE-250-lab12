#include "Exercise12_C.h"

Move best;
short best_eval;
Board *engine_board;

void _minimax(void);

void _best_move(int i, Move m) {
	make_move(engine_board, &m);
	
	short eval = minimax(engine_board, 2);
	
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