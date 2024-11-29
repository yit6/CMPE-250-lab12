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

#define gets(buf) GetStringSB(buf, 72)

Board b;
char buffer[72];
Move m;
unsigned long long perft_num;

unsigned int seed = 1;
extern char rainbowCycle;

Color playerColor;

unsigned int random() {
	seed = seed * 33456789 + 2345600078l;
	return seed;
}

void one_player_game(void) {
	char mateState = 0;
	UInt32 rgb;
	
	while (!mateState) {
		print_board(&b);
		
		if(b.current_turn == playerColor) {
			gets(buffer);
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
			
			puts("Your Move: ");
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
		} else {
			Move engine_move = best_move(&b);
			puts("Engine Move: ");
			print_move(&engine_move);
			m = engine_move;
			puts("\r\n");
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
	print_board(&b);
}

void two_player_game(void) {
	char mateState = 0;
	UInt32 rgb;
	
	while (!mateState) {
		print_board(&b);
		
		gets(buffer);
		m = parse_move(buffer);
		
		if (*buffer == 'u') { //modify for takebacks
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
			puts("White wins!\r\n");
			rainbowCycle = 1;
		} else if(mateState == 2) {
			puts("Black wins!\r\n");
			rainbowCycle = 1;
		} else if(mateState == 3) {
			rgb = 0x33FF00;
			puts("Stalemate\r\n");
		}
		set_RGB(rgb);
	}
	print_board(&b);
}

void debug_game(void) {
	char mateState = 0;
	UInt32 rgb;
	
	while (!mateState) {
		print_board(&b);
		
		gets(buffer);
		
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
			puts("\r\n");
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
			puts("Legal!\r\n");
		} else {
			puts("Not Legal!\r\n");
			continue;
		}
		
		make_move(&b, &m);
	
		mateState = get_mate_state(&b);
		if(mateState == 1) {
			puts("Player wins!\r\n");
		} else if(mateState == 2) {
			puts("Engine wins!\r\n");
		} else if(mateState == 3) {
			puts("Stalemate\r\n");
		}
		set_RGB(rgb);
	}
	print_board(&b);
}

int main (void) {
	
  __asm("CPSID   I");
	
  Init_UART0_IRQ();
	init_TPM();
	init_PIT();
	
  __asm("CPSIE   I");
	
	for(;;) {
		puts("Welcome to the Frankly Unnecessary Chess Kernel\r\n");
		puts("Hit enter to play on a new board or enter FEN notation:\r\n");
		
		gets(buffer);
		
		if(*buffer == 0) {
			new_board(&b);
		} else {
			from_fen(&b, buffer);
		}
		
		puts("Would you like to play as (W)hite, (B)lack, or (T)wo players? ");
		
		gets(buffer);
		if(*buffer == 'D' || *buffer == 'd') {
			debug_game();
		} else if(*buffer == 'T' || *buffer == 't') {
			two_player_game();
		} else {
			playerColor = (*buffer == 'B' || *buffer == 'b'); //defaults to white if nothing is input
			one_player_game();
		}
	}
}
