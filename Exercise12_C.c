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
	seed = seed * 33456789 + 2345600078l;
	return seed;
}

int main (void) {
	
	char mateState = 0;
	UInt32 rgb;
	
  __asm("CPSID   I");
	
  Init_UART0_IRQ();
	init_TPM();
	init_PIT();
	
  __asm("CPSIE   I");
		
	new_board(&b);
	//from_fen(&b, "2Q2bnr/4p1pq/5pkr/7p/7P/4P3/PPPP1PP1/RNB1KBNR w KQ - 0 1");

  while (!mateState) {
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
			int succ = sscanf(buffer + 1, "%X", &rgb);
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
			UInt32 rizzy = 0xFF << (8 & (-(turn ^ 1)));//blue if white's turn, green if black's turn
			puts("Legal!\r\n");
			rgb = rizzy;
		} else {
			puts("Not Legal!\r\n");
			rgb = RED_MASK;
			continue;
		}
		
		make_move(&b, &m);
		
		mateState = get_mate_state(&b);
		if(mateState == 1) {
			puts("Player wins!\r\n");
			rainbowCycle = 1;
		} else if(mateState == 2) {
			puts("Engine wins!\r\n");
			rgb = 0x000000;
		} else if(mateState == 3) {
			rgb = 0x33FF00;
			puts("Stalemate\r\n");
		}
		set_RGB(rgb);
	}
	for(;;); //without this the program can and will stop accepting interrupts and prints will not finish
}
