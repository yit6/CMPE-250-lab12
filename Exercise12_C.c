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
	
	from_fen(&b,"rnb1kbnr/pppp3Q/1pp5/1pp5/2P1q3/2P5/PPP1KPPP/RNB2BNR w kq - 0 1");
	//b = from_fen("rnbqkbnr/8/8/1p6/pPp1p1p1/P1PpPpPp/RKRP1P1P/QBBN1N2 w q - 0 27");
	
  for (;;) {	
		print_board(&b);
		
		get_input:
		GetStringSB(move_buffer, 10);
		m = parse_move(move_buffer);
		
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
		for_each_pseudolegal(&b, print_moves);
	}
}
