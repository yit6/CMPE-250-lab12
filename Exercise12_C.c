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

#include <stdio.h>
#include <stdlib.h>

#include "Exercise12_C.h"

Board b;

void print_moves(int i, Move m) {
	PutNumHex(i);
	puts(":	");
	print_move(&m);
	puts("\r\n");
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
	//init_LED();
	init_TPM();
	
  __asm("CPSIE   I");
		
	//new_board(&b);
	from_fen(&b, "NKbqkbnr/PPRp1pPP/PN1R4/P3p1BQ/2B1P3/3P4/8/8 w k - 0 1");

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
			puts("Doing: ");
			print_move(&m);
			puts("\r\n");
			make_move(&b, &m);
			continue;
		}
		if (*move_buffer == 'p') {
			perft_num = perft(&b, move_buffer[1]-'0');
			sprintf(perft_print, "\r\n%lld nodes.\r\n", perft_num);
			puts(perft_print);
			print_board(&b);
			goto get_input;
		}
		if (*move_buffer == 'm') {
			Move engine_move = best_move(&b);
			print_move(&engine_move);
			make_move(&b,&engine_move);
			PutStringSB("\r\n",255);
			continue;
		}
		
		print_move(&m);
		puts("\r\n");
		
		if (is_legal(&b, &m)) {
			UInt8 ledMask = ((b.current_turn << 1) ^ GREEN_MASK) | ((b.current_turn) & BLUE_MASK); // only blue mask if users turn, only green mask if engines turn
			puts("Legal!\r\n");
			set_LED(ledMask);
		} else {
			puts("Not Legal!\r\n");
			set_LED(RED_MASK);
			goto get_input;
		}
		
		make_move(&b, &m);
		if(is_gameover(&b)) {
			puts("Game Over\r\n");
		} else {
			puts("Game NOT Over\r\n");
		}
	}
}
