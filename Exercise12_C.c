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

int main (void) {
	
	Move m;
	char move_buffer[10];
	
  __asm("CPSID   I");
	
  Init_UART0_IRQ();
	
  __asm("CPSIE   I");
	
	new_board(&b);
	//b = from_fen("rnbqkbnr/8/8/1p6/pPp1p1p1/P1PpPpPp/RKRP1P1P/QBBN1N2 w q - 0 27");

  for (;;) {	
		print_board(&b);
		for_each_legal(&b, print_moves);
		
		get_input:
		GetStringSB(move_buffer, 10);
		m = parse_move(move_buffer);
		
		if (*move_buffer == 'u') {
			make_unmove(&b);
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
