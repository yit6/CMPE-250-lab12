#include "Exercise12_C.h"
#include "MKL05Z4.h"

/* Port B pin 7 symbols */
#define SET_TO_TPM (PORT_PCR_ISF_MASK | (2u << PORT_PCR_MUX_SHIFT))
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
  PORTB->PCR[POS_RED] = SET_TO_TPM;
	PORTB->PCR[POS_GREEN] = SET_TO_TPM;
	PORTB->PCR[POS_BLUE] = SET_TO_TPM;
  TPM0->CONF = TPM_CONF_DEFAULT;
  TPM0->CNT = TPM_CNT_INIT;
  TPM0->MOD = MAX_BRIGHTNESS;
  TPM0->CONTROLS[3].CnSC = TPM_CnSC_PWMH;
  TPM0->CONTROLS[3].CnV = MAX_BRIGHTNESS;
	TPM0->CONTROLS[2].CnSC = TPM_CnSC_PWMH;
  TPM0->CONTROLS[2].CnV = MAX_BRIGHTNESS;
	TPM0->CONTROLS[1].CnSC = TPM_CnSC_PWMH;
  TPM0->CONTROLS[1].CnV = MAX_BRIGHTNESS;
  TPM0->SC = TPM_SC_CLK_DIV16;
}

//haha yeah
void set_RGB(UInt32 rgb) {
	int r = 255 - ((rgb & RED_MASK) >> 16);
	int g = 255 - ((rgb & GREEN_MASK) >> 8);
	int b = 255 - (rgb & BLUE_MASK);
	TPM0->CONTROLS[3].CnV = r * MAX_BRIGHTNESS / 255;
	TPM0->CONTROLS[2].CnV = g * MAX_BRIGHTNESS / 255;
	TPM0->CONTROLS[1].CnV = b * MAX_BRIGHTNESS / 255;
}
