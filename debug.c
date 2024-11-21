#include <stdio.h>
#include "Exercise12_C.h"

unsigned long long _perft_num;
unsigned long long _perft_subtotal;
int _perft_depth;
int _perft_subtotal_level;
Board *_perft_board;
char _perft_sb[20];

void _perft_helper(int i, Move m) {
	_perft_depth--;
	make_move(_perft_board, &m);
	
	if (_perft_depth == 0) {
		_perft_num++;
	} else {
		for_each_legal(_perft_board, _perft_helper);
	}	

	make_unmove(_perft_board);
		
	if (_perft_depth == _perft_subtotal_level) {
		print_move(&m);
		sprintf(_perft_sb, ": %lld\r\n", _perft_num-_perft_subtotal);
		PutStringSB(_perft_sb, 255);
		_perft_subtotal = _perft_num;
	}
	
	_perft_depth++;
}

unsigned long long perft(Board *b, int depth) {
	_perft_num = 0;
	_perft_subtotal = 0;
	_perft_depth = depth;
	_perft_board = b;
	_perft_subtotal_level = depth-1;
	
	for_each_legal(_perft_board,_perft_helper);
	
	return _perft_num;
}