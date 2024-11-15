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

int main (void) {
		
	Board b;
	Piece wp;
	wp.color = 1;
	wp.type = Pawn;
	b.board[0][0] = wp;

  __asm("CPSID   I");
	
  Init_UART0_IRQ();
	
  __asm("CPSIE   I");
	
  for (;;) {  }
}
