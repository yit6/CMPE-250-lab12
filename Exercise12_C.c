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
char buffer[40];
Move m;
unsigned long long perft_num;

unsigned int seed = 1;
extern char rainbowCycle;

unsigned int random() {
	seed = seed * 33456789 + 2345600078;
	return seed;
}

int main (void) {
	
  __asm("CPSID   I");
	
  Init_UART0_IRQ();
	init_TPM();
	init_PIT();
	
  __asm("CPSIE   I");
		
	new_board(&b);
	//from_fen(&b, "NKbqkbnr/PPRp1pPP/PN1R4/P3p1BQ/2B1P3/3P4/8/8 w k - 0 1");

  for (;;) {
		print_board(&b);
		
		GetStringSB(buffer, 40);
		m = parse_move(buffer);
		
		if (*buffer == 'u') {
			make_unmove(&b);
			continue;
		}
		if (*buffer == 'r') {
			m = random_move(&b);
			puts("Doing: ");
			print_move(&m);
			puts("\r\n");
			make_move(&b, &m);
			continue;
		}
		if (*buffer == 'p') {
			perft_num = perft(&b, buffer[1]-'0');
			sprintf(buffer, "\r\n%lld nodes.\r\n", perft_num);
			puts(buffer);
			continue;
		}
		if (*buffer == 'm') {
			Move engine_move = best_move(&b);
			print_move(&engine_move);
			make_move(&b,&engine_move);
			PutStringSB("\r\n",255);
			continue;
		}

		if(*buffer == 'X') { //spingbob
			UInt32 rgb = 0;
			int succ = sscanf(buffer + 1, "%X", &rgb);
			set_RGB(rgb);
			PutNumHex(succ);
			puts(buffer);
			puts("\r\n");
			continue;
		}
		
		if(*buffer == 'R') {
			rainbowCycle = !rainbowCycle;
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
	}
}
