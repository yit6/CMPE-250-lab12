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
char perft_print[40];

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
	unsigned long long perft_num;
	
  __asm("CPSID   I");
	
  Init_UART0_IRQ();
	init_TPM();
	
  __asm("CPSIE   I");
		
	new_board(&b);
	//from_fen(&b, "NKbqkbnr/PPRp1pPP/PN1R4/P3p1BQ/2B1P3/3P4/8/8 w k - 0 1");

  for (;;) {
		print_board(&b);
		
		//sprintf(perft_print, "Eval: %d, internal board: %d\r\n", evaluate(&b), b.pst_eval);
		//puts(perft_print);
		
		//for_each_legal(&b, print_moves);
		
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
			continue;
		}
		if (*move_buffer == 'm') {
			Move engine_move = best_move(&b);
			print_move(&engine_move);
			make_move(&b,&engine_move);
			PutStringSB("\r\n",255);
			continue;
		}
		if(*move_buffer == 'X') { //spingbob
			UInt32 rgb = 0;
			int succ = sscanf(move_buffer + 1, "%X", &rgb);
			set_RGB(rgb);
			PutNumHex(succ);
			puts(move_buffer);
			puts("\r\n");
			continue;
		}
		
		print_move(&m);
		puts("\r\n");
		
		if (is_legal(&b, &m)) {
			UInt32 turn = b.current_turn;
			//UInt32 ledMask = ((-(b.current_turn ^ 0x1)) ^ GREEN_MASK) | ((-b.current_turn) & BLUE_MASK); // only blue mask if users turn, only green mask if engines turn
			UInt32 rizzy = 0xFF << (8 & (-(turn ^ 1)));
			PutNumHex(rizzy);
			puts("Legal!\r\n");
			set_RGB(rizzy);
		} else {
			puts("Not Legal!\r\n");
			set_RGB(RED_MASK);
			continue;
		}
		
		make_move(&b, &m);
		/*if(is_gameover(&b)) {
			puts("Game Over\r\n");
		} else {
			puts("Game NOT Over\r\n");
		}*/
	}
}
