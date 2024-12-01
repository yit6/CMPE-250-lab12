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

char player_status;
char pgn_valid;

unsigned int random() {
	seed = seed * 33456789 + 2345600078l;
	return seed;
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

void play_game(void) {
	char mateState = 0;
	UInt32 rgb;
	
	while (!mateState) {
		print_board(&b);
		rgb = 0xFF << (8 & (-(b.current_turn ^ 1)));//blue if white's turn, green if black's turn
		set_RGB(rgb);
		
		if (!(player_status & (1<<b.current_turn))) {
			m = best_move(&b);
			puts("Engine plays: ");
			print_move_san(&b, &m);
			puts("\r\n");
			make_move(&b, &m);
			goto game_end_check;
		}
		
		gets(buffer);
		m = parse_move(buffer);
		
		if (*buffer == 'u') { //modify for takebacks
			make_unmove(&b);
			make_unmove(&b);
			continue;
		}
		if (*buffer == 'r') {
			m = random_move(&b);
			puts("Doing: ");
			print_move_san(&b, &m);
			puts("\r\n");
			make_move(&b, &m);
			continue;
		}		
		
		if (!is_legal(&b, &m)) {
			rgb = RED_MASK;
			set_RGB(rgb);
			continue;
		}
		
		print_move_san(&b, &m);
		puts("\r\n");

		make_move(&b, &m);
		
		game_end_check:
		mateState = get_mate_state(&b);
		if(mateState == 1) {
			puts("White wins!\r\n");
			rainbowCycle = 1;
		} else if(mateState == 2) {
			puts("Black wins!\r\n");
			rainbowCycle = 1;
		} else if(mateState == 3) {
			rgb = 0x33FF00;
			set_RGB(rgb);
			puts("Stalemate\r\n");
		}
	}
	print_board(&b);
	
	if (!pgn_valid) { return; }
	
	puts("Export fen? [y/N]: ");
	gets(buffer);
	if ((*buffer | 1<<5) == 'y') {
		print_pgn(&b);
	}
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
		
		rainbowCycle = 0;
		
		if(*buffer == 0) {
			new_board(&b);
			pgn_valid = 1;
		} else {
			from_fen(&b, buffer);
			pgn_valid = 0;
		}
		
		puts("Would you like to play as (W)hite, (B)lack, (T)wo players, or Two (E)ngines? ");
		
		gets(buffer);
		
		if ((*buffer | 1<<5) == 'd') {
			debug_game();
			continue;
		} else if ((*buffer | 1<<5) == 't') {
			player_status = 3;
		} else if ((*buffer | 1<<5) == 'w') {
			player_status = 1;
		} else if ((*buffer | 1<<5) == 'b') {
			player_status = 2;
		} else if ((*buffer | 1<<5) == 'e') {
			player_status = 0;
		}
		
		play_game();
	}
}
