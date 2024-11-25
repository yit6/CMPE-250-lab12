#include "Exercise12_C.h"
#include "MKL05Z4.h"

/* Port B pin 7 symbols */
#define PTB8_MUX_TPM0_CH3_OUT (2u << PORT_PCR_MUX_SHIFT)
#define SET_PTB8_TPM0_CH3_OUT (PORT_PCR_ISF_MASK | PTB8_MUX_TPM0_CH3_OUT)
/* SIM_SOPT2 symbols */
#define SIM_SOPT2_TPMSRC_MCGFLLCLK (1u << SIM_SOPT2_TPMSRC_SHIFT)
/* TPM0_CONF symbol */
#define TPM_CONF_DEFAULT (0)
/* TPM0_CNT symbol */
#define TPM_CNT_INIT (0)
/* TPM0_MOD symbol */
#define TPM_MOD_PWM_PERIOD_10ms (30000u - 1)
/* TPM0_SC symbols */
#define TPM_SC_CMOD_CLK (1u)
#define TPM_SC_PS_DIV16 (0x4u)
#define TPM_SC_CLK_DIV16 ((TPM_SC_CMOD_CLK << \
TPM_SC_CMOD_SHIFT) | \
TPM_SC_PS_DIV16)
/* TPM0_C2SC symbol */
#define TPM_CnSC_PWMH (TPM_CnSC_MSB_MASK | \
TPM_CnSC_ELSB_MASK)

//LED things

#define MAX_BRIGHTNESS TPM_MOD_PWM_PERIOD_10ms

#define POS_RED 8
#define POS_GREEN 9
#define POS_BLUE 10

#define PORTB_LED_RED_MASK (1 << POS_RED)
#define PORTB_LED_GREEN_MASK (1 << POS_GREEN)
#define PORTB_LED_BLUE_MASK (1 << POS_BLUE)
#define PORTB_LEDS_MASK (PORTB_LED_RED_MASK | PORTB_LED_GREEN_MASK | PORTB_LED_BLUE_MASK)

#define PORT_PCR_MUX_SELECT_1_MASK (1 << PORT_PCR_MUX_SHIFT)
#define PORT_PCR_SET_GPIO (PORT_PCR_ISF_MASK | PORT_PCR_MUX_SELECT_1_MASK)

void init_TPM(void) {
  SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
  SIM->SOPT2 |= SIM_SOPT2_TPMSRC_MCGFLLCLK;
  SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;
  SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
  PORTB->PCR[8] = SET_PTB8_TPM0_CH3_OUT;
  TPM0->CONF = TPM_CONF_DEFAULT;
  TPM0->CNT = TPM_CNT_INIT;
  TPM0->MOD = MAX_BRIGHTNESS;
  TPM0->CONTROLS[3].CnSC = TPM_CnSC_PWMH;
  TPM0->CONTROLS[3].CnV = MAX_BRIGHTNESS / 2;
  TPM0->SC = TPM_SC_CLK_DIV16;
}

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