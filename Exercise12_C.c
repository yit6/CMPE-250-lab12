/*********************************************************************/
/* <Your program description here>                                   */
/* Name:  <Your name here>                                           */
/* Date:  <Date completed>                                           */
/* Class:  CMPE 250                                                  */
/* Section:  <Your section here>                                     */
/*-------------------------------------------------------------------*/
/* Template:  R. W. Melton                                           */
/*            November 11, 2024                                      */
/*********************************************************************/
#include "MKL05Z4.h"
#include "Exercise12_C.h"

#include <stdio.h>
#include <stdlib.h>

Board b;

void print_moves(int i, Move m) {
	PutNumHex(i);
	PutStringSB(":	",255);
	print_move(&m);
	PutStringSB("\r\n",255);
}

unsigned int seed = 1;

unsigned int random() {
	seed = seed * 33456789 + 2345600078;
	return seed;
}

int main (void) {
	
	Move m;
	char move_buffer[10];
	char perft_print[20];
	unsigned long long perft_num;
	
  __asm("CPSID   I");
	
  Init_UART0_IRQ();
	
  __asm("CPSIE   I");
	
	new_board(&b);

  for (;;) {	
		print_board(&b);
		//for_each_legal(&b, print_moves);
		
		get_input:
		GetStringSB(move_buffer, 10);
		m = parse_move(move_buffer);
		
		if (*move_buffer == 'u') {
			make_unmove(&b);
			continue;
		}
		if (*move_buffer == 'r') {
			m = random_move(&b);
			make_move(&b, &m);
			continue;
		}
		if (*move_buffer == 'p') {
			perft_num = perft(&b, move_buffer[1]-'0');
			sprintf(perft_print, "%lld nodes.\r\n", perft_num);
			PutStringSB(perft_print, 255);
			goto get_input;
		}
		
		print_move(&m);
		PutStringSB("\r\n",255);
		
		if (is_pseudolegal(&b, &m)) {
			PutStringSB("Ok!\r\n",255);
		} else {
			PutStringSB("Bad!\r\n",255);
		}
		
		if (is_legal(&b, &m)) {
			PutStringSB("Legal!\r\n",255);
		} else {
			PutStringSB("Not Legal!\r\n",255);
			goto get_input;
		}
		
		make_move(&b, &m);
	}
}
