#include <stdio.h>
#include "Exercise12_C.h"

unsigned int _perft_num;
unsigned int _perft_subtotal;
int _perft_depth;
int _perft_subtotal_level;
Board *_perft_board;
char _perft_sb[20];

void _perft_helper(int i, Move m) {
	_perft_depth--;
	make_move(_perft_board, &m);
	
	if (evaluate(_perft_board) != _perft_board->pst_eval) {
		print_board(_perft_board);
		make_unmove(_perft_board);
		print_board(_perft_board);
		while (1);
	}
	
	if (_perft_depth == 0) {
		_perft_num++;
	} else {
		for_each_legal(_perft_board, _perft_helper);
	}	

	make_unmove(_perft_board);
		
	if (_perft_depth == _perft_subtotal_level) {
		print_move(&m);
		sprintf(_perft_sb, ": %u\r\n", _perft_num - _perft_subtotal);
		_perft_sb[0] = ':';
		_perft_sb[1] = ' ';
		puts(_perft_sb);
		_perft_subtotal = _perft_num;
	}
	
	_perft_depth++;
}

// Print how many positions are reached on a gametree search of a certain depth on board b
unsigned long long perft(Board *b, int depth) {
	_perft_num = 0;
	_perft_subtotal = 0;
	_perft_depth = depth;
	_perft_board = b;
	_perft_subtotal_level = depth-1;
	
	for_each_legal(_perft_board,_perft_helper);
	
	return _perft_num;
}
