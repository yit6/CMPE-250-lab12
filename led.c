#include "Exercise12_C.h"
#include "MKL05Z4.h"

#include <math.h>

/* Port B pin 7 symbols */
#define SET_TO_TPM (PORT_PCR_ISF_MASK | (2u << PORT_PCR_MUX_SHIFT))
/* SIM_SOPT2 symbols */
#define SIM_SOPT2_TPMSRC_MCGFLLCLK (1u << SIM_SOPT2_TPMSRC_SHIFT)
/* TPM0_CONF symbol */
#define TPM_CONF_DEFAULT (0)
/* TPM0_CNT symbol */
#define TPM_CNT_INIT (0)
/* TPM0_MOD symbol */
#define TPM_MOD_PWM_PERIOD_6ms (2 << 14) //16384
/* TPM0_SC symbols */
#define TPM_SC_CMOD_CLK (1u)
#define TPM_SC_PS_DIV16 (0x4u)
#define TPM_SC_CLK_DIV16 ((TPM_SC_CMOD_CLK << TPM_SC_CMOD_SHIFT) | TPM_SC_PS_DIV16)
/* TPM0_C2SC symbol */
#define EDGE_ALIGN_LOW_TRUE (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK)

//PIT things

#define PIT_IRQ_NUMBER (22)
/*------------------------------------------------------------*/
/* NVIC_ICPR                                                  */
/* 31-00:CLRPEND=pending status for HW IRQ sources;           */
/*              read:   0 = not pending;  1 = pending         */
/*              write:  0 = no effect;                        */
/*                      1 = change status to not pending      */
/* 22:PIT IRQ pending status                                  */
/*------------------------------------------------------------*/
#define NVIC_ICPR_PIT_MASK (1 << PIT_IRQ_NUMBER)
/*------------------------------------------------------------*/
/* NVIC_IPR0-NVIC_IPR7                                        */
/* 2-bit priority:  0 = highest; 3 = lowest                   */
/*------------------------------------------------------------*/
#define PIT_IRQ_PRIORITY (0)
#define PIT_IPR_REGISTER (PIT_IRQ_NUMBER >> 2)
#define NVIC_IPR_PIT_MASK \
                      (3 << (((PIT_IRQ_NUMBER & 3) << 3) + 6))
/*------------------------------------------------------------*/
/* NVIC_ISER                                                  */
/* 31-00:SETENA=masks for HW IRQ sources;                     */
/*              read:   0 = masked;     1 = unmasked          */
/*              write:  0 = no effect;  1 = unmask            */
/* 22:PIT IRQ mask                                            */
/*------------------------------------------------------------*/
#define NVIC_ISER_PIT_MASK (1 << PIT_IRQ_NUMBER)
/*------------------------------------------------------------*/
/* PIT_LDVALn                                                 */
/* Clock ticks for 0.01 s = 10 ms at 24 MHz PIT clock rate    */
/* 0.01 s * 24,000,000 Hz = 240,000                           */
/* TSV = 240,000 - 1 = 239,999                                */
/* Clock ticks for 0.01 s at 23,986,176 Hz count rate         */
/* 0.01 s * 23,986,176 Hz = 239,862                           */
/* TSV = 239,862 - 1 = 239,861                                */
/*------------------------------------------------------------*/
#define PIT_LDVAL_10ms  (239861u)
/*------------------------------------------------------------*/
/* PIT_MCR:  PIT module control register                      */
/* 1-->    0:FRZ=freeze (continue'/stop in debug mode)        */
/* 0-->    1:MDIS=module disable (PIT section)                */
/*                RTI timer not affected                      */
/*                must be enabled before any other PIT setup  */
/*------------------------------------------------------------*/
#define PIT_MCR_EN_FRZ  (PIT_MCR_FRZ_MASK)
/*------------------------------------------------------------*/
/* PIT_TCTRL:  timer control register                         */
/* 0-->   2:CHN=chain mode (enable)                           */
/* 1-->   1:TIE=timer interrupt enable                        */
/* 1-->   0:TEN=timer enable                                  */
/*------------------------------------------------------------*/
#define PIT_TCTRL_CH_IE  (PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK)

//LED things

#define MAX_BRIGHTNESS TPM_MOD_PWM_PERIOD_6ms
#define MAX_BRIGHT_SHIFT 14

#define POS_RED 8
#define POS_GREEN 9
#define POS_BLUE 10

#define PORTB_LED_RED_MASK (1 << POS_RED)
#define PORTB_LED_GREEN_MASK (1 << POS_GREEN)
#define PORTB_LED_BLUE_MASK (1 << POS_BLUE)
#define PORTB_LEDS_MASK (PORTB_LED_RED_MASK | PORTB_LED_GREEN_MASK | PORTB_LED_BLUE_MASK)

#define PORT_PCR_MUX_SELECT_1_MASK (1 << PORT_PCR_MUX_SHIFT)
#define PORT_PCR_SET_GPIO (PORT_PCR_ISF_MASK | PORT_PCR_MUX_SELECT_1_MASK)

UInt32 rainbowCounter = 0;
char rainbowCycle = 0;

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
  TPM0->CONTROLS[3].CnSC = EDGE_ALIGN_LOW_TRUE;
  TPM0->CONTROLS[3].CnV = 0;
	TPM0->CONTROLS[2].CnSC = EDGE_ALIGN_LOW_TRUE;
  TPM0->CONTROLS[2].CnV = 0;
	TPM0->CONTROLS[1].CnSC = EDGE_ALIGN_LOW_TRUE;
  TPM0->CONTROLS[1].CnV = 0;
  TPM0->SC = TPM_SC_CLK_DIV16;
}

//Mostly taken from Lab10 materials (the equivalent C project)
void init_PIT(void) {
  /* Enable PIT module clock */
  SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
  /* Disable PIT Timer 0 */
  PIT->CHANNEL[0].TCTRL &= ~PIT_TCTRL_TEN_MASK;
  /* Set PIT interrupt priority to 0 (highest) */
  NVIC->IP[PIT_IPR_REGISTER] &= NVIC_IPR_PIT_MASK;
  /* Clear any pending PIT interrupts */
  NVIC->ICPR[0] = NVIC_ICPR_PIT_MASK;
  /* Unmask UART0 interrupts */
  NVIC->ISER[0] = NVIC_ISER_PIT_MASK;
  /* Enable PIT timer module */
  /* and set to stop in debug mode */
  PIT->MCR = PIT_MCR_EN_FRZ;
  /* Set PIT Timer 0 period for 0.01 s */
  PIT->CHANNEL[0].LDVAL = PIT_LDVAL_10ms;
  /* Enable PIT Timer 0 and interrupt */
  PIT->CHANNEL[0].TCTRL = PIT_TCTRL_CH_IE;
}

UInt32 hueToRGB(float hue) {
	float kr = fmodf(5 + hue * 6, 6);
	float kg = fmodf(3 + hue * 6, 6);
	float kb = fmodf(1 + hue * 6, 6);
	
	float r = 1 - fmax(fmin(fmin(kr, 4 - kr), 1), 0);
	float g = 1 - fmax(fmin(fmin(kg, 4 - kg), 1), 0);
	float b = 1 - fmax(fmin(fmin(kb, 4 - kb), 1), 0);
	
	UInt32 ir = (r * 255.f * 0.2f);
	UInt32 ig = (g * 255.f);
	UInt32 ib = (b * 255.f);
	
	return (ir << 16) | (ig << 8) | (ib);
}

void PIT_IRQHandler (void) {
  __asm("CPSID   I");  /* mask interrupts */
	if(rainbowCycle) {
		rainbowCounter++;
		set_RGB(hueToRGB((float) rainbowCounter * 0.004f));	
	}
	/* clear PIT timer 0 interrupt flag */
  PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;
  __asm("CPSIE   I");  /* unmask interrupts */
}

//haha yeah
void set_RGB(UInt32 rgb) {
	UInt32 r = (rgb & RED_MASK) >> (16 - MAX_BRIGHT_SHIFT + 8);
	UInt32 g = (rgb & GREEN_MASK) >> (8 - MAX_BRIGHT_SHIFT + 8);
	UInt32 b = (rgb & BLUE_MASK) << (MAX_BRIGHT_SHIFT - 8);
	TPM0->CONTROLS[3].CnV = r;
	TPM0->CONTROLS[2].CnV = g;
	TPM0->CONTROLS[1].CnV = b;
	return;
}
