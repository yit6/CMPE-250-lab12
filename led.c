#include "Exercise12_C.h"
#include "MKL05Z4.h"

#define POS_RED 8
#define POS_GREEN 9
#define POS_BLUE 10

#define PORTB_LED_RED_MASK (1 << POS_RED)
#define PORTB_LED_GREEN_MASK (1 << POS_GREEN)
#define PORTB_LED_BLUE_MASK (1 << POS_BLUE)
#define PORTB_LEDS_MASK (PORTB_LED_RED_MASK | PORTB_LED_GREEN_MASK | PORTB_LED_BLUE_MASK)

#define PORT_PCR_MUX_SELECT_1_MASK (1 << PORT_PCR_MUX_SHIFT)
#define PORT_PCR_SET_GPIO (PORT_PCR_ISF_MASK | PORT_PCR_MUX_SELECT_1_MASK)

void init_LED() {
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB->PCR[POS_RED] = PORT_PCR_SET_GPIO;
	PORTB->PCR[POS_GREEN] = PORT_PCR_SET_GPIO;
	PORTB->PCR[POS_BLUE] = PORT_PCR_SET_GPIO;
	FPTB->PDDR = PORTB_LEDS_MASK;
	
	//turn off LEDs
	FPTB->PSOR = PORTB_LED_RED_MASK;
	FPTB->PSOR = PORTB_LED_GREEN_MASK;
	FPTB->PSOR = PORTB_LED_BLUE_MASK;
}

int (led_offsets)[] = {
	(&(FPTB->PSOR))-(uint32_t *)FPTB, // 0b000
	(&(FPTB->PCOR))-(uint32_t *)FPTB, // 0b001
	(&(FPTB->PCOR))-(uint32_t *)FPTB, // 0b010
	0,                                // 0b011
	(&(FPTB->PCOR))-(uint32_t *)FPTB, // 0b100
};

/**
Hello TAs/Dr. Melton, to explain what is happening here, basically
we have a LUT that maps the color masks to the pointers to either
PCOR or PSOR so that we don't have to branch in this function and can
do it as three one liners
*/
void set_LED(char rgb) {
	*((int *) FPTB+led_offsets[rgb & RED_MASK]) = PORTB_LED_RED_MASK;
	*((int *) FPTB+led_offsets[rgb & GREEN_MASK]) = PORTB_LED_GREEN_MASK;
	*((int *) FPTB+led_offsets[rgb & BLUE_MASK]) = PORTB_LED_BLUE_MASK;
}