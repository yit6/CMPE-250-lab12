            TTL Program Title for Listing Header Goes Here
;****************************************************************
;Descriptive comment header goes here.
;(What does the program do?)
;Name:  <Your name here>
;Date:  <Date completed here>
;Class:  CMPE-250
;Section:  <Your lab section, day, and time here>
;---------------------------------------------------------------
;Keil Template for KL05 Assembly with Keil C startup
;R. W. Melton
;November 3, 2020
;****************************************************************
;Assembler directives
            THUMB
            GBLL  MIXED_ASM_C
MIXED_ASM_C SETL  {TRUE}
            OPT   64  ;Turn on listing macro expansions
;****************************************************************
;Include files
            GET  MKL05Z4.s
            OPT  1          ;Turn on listing
;****************************************************************
;EQUates
;****************************************************************
;EQUates
;---------------------------------------------------------------
;Characters
BS          EQU  0x08
CR          EQU  0x0D
DEL         EQU  0x7F
ESC         EQU  0x1B
LF          EQU  0x0A
NULL        EQU  0x00
;---------------------------------------------------------------
;DAC0
DAC0_BITS   EQU   12
DAC0_STEPS  EQU   4096
DAC0_0V     EQU   0x00
;---------------------------------------------------------------
;Servo
SERVO_POSITIONS  EQU  5
;---------------------------------------------------------------
PWM_FREQ          EQU  50
;TPM_SOURCE_FREQ  EQU  48000000
TPM_SOURCE_FREQ   EQU  47972352
TPM_SC_PS_VAL     EQU  4
;PWM_PERIOD       EQU  ((TPM_SOURCE_FREQ / (1 << TPM_SC_PS_VAL)) / \
;                       PWM_FREQ)
;PWM_DUTY_5       EQU  (PWM_PERIOD / 20)  ;  5% duty cycle
;PWM_DUTY_10      EQU  (PWM_PERIOD / 10)  ; 10% duty cycle
PWM_PERIOD        EQU  60000
PWM_DUTY_10       EQU  6000
PWM_DUTY_5        EQU  3000
;---------------------------------------------------------------
;Number output characteristics
MAX_WORD_DECIMAL_DIGITS  EQU  10
;---------------------------------------------------------------
; Queue management record field offsets
IN_PTR      EQU   0
OUT_PTR     EQU   4
BUF_STRT    EQU   8
BUF_PAST    EQU   12
BUF_SIZE    EQU   16
NUM_ENQD    EQU   17
; Queue structure sizes
XQ_BUF_SZ   EQU   80  ;Xmit queue contents
Q_REC_SZ    EQU   18  ;Queue management record
;---------------------------------------------------------------
;NVIC_ICER
;31-00:CLRENA=masks for HW IRQ sources;
;             read:   0 = unmasked;   1 = masked
;             write:  0 = no effect;  1 = mask
;22:PIT IRQ mask
;12:UART0 IRQ mask
NVIC_ICER_PIT_MASK    EQU  PIT_IRQ_MASK
NVIC_ICER_UART0_MASK  EQU  UART0_IRQ_MASK
;---------------------------------------------------------------
;NVIC_ICPR
;31-00:CLRPEND=pending status for HW IRQ sources;
;             read:   0 = not pending;  1 = pending
;             write:  0 = no effect;
;                     1 = change status to not pending
;22:PIT IRQ pending status
;12:UART0 IRQ pending status
NVIC_ICPR_PIT_MASK    EQU  PIT_IRQ_MASK
NVIC_ICPR_UART0_MASK  EQU  UART0_IRQ_MASK
;---------------------------------------------------------------
;NVIC_IPR0-NVIC_IPR7
;2-bit priority:  00 = highest; 11 = lowest
;--PIT--------------------
PIT_IRQ_PRIORITY    EQU  0
NVIC_IPR_PIT_MASK   EQU  (3 << PIT_PRI_POS)
NVIC_IPR_PIT_PRI_0  EQU  (PIT_IRQ_PRIORITY << PIT_PRI_POS)
;--UART0--------------------
UART0_IRQ_PRIORITY    EQU  3
NVIC_IPR_UART0_MASK   EQU (3 << UART0_PRI_POS)
NVIC_IPR_UART0_PRI_3  EQU (UART0_IRQ_PRIORITY << UART0_PRI_POS)
;---------------------------------------------------------------
;NVIC_ISER
;31-00:SETENA=masks for HW IRQ sources;
;             read:   0 = masked;     1 = unmasked
;             write:  0 = no effect;  1 = unmask
;22:PIT IRQ mask
;12:UART0 IRQ mask
NVIC_ISER_PIT_MASK    EQU  PIT_IRQ_MASK
NVIC_ISER_UART0_MASK  EQU  UART0_IRQ_MASK
;---------------------------------------------------------------
;PIT_LDVALn:  PIT load value register n
;31-00:TSV=timer start value (period in clock cycles - 1)
;Clock ticks for 0.01 s at ~24 MHz count rate
;0.01 s * ~24,000,000 Hz = ~240,000
;TSV = ~240,000 - 1
;Clock ticks for 0.01 s at 23,986,176 Hz count rate
;0.01 s * 23,986,176 Hz = 239,862
;TSV = 239,862 - 1
PIT_LDVAL_10ms  EQU  239861
;---------------------------------------------------------------
;PIT_MCR:  PIT module control register
;1-->    0:FRZ=freeze (continue'/stop in debug mode)
;0-->    1:MDIS=module disable (PIT section)
;               RTI timer not affected
;               must be enabled before any other PIT setup
PIT_MCR_EN_FRZ  EQU  PIT_MCR_FRZ_MASK
;---------------------------------------------------------------
;PIT_TCTRL:  timer control register
;0-->   2:CHN=chain mode (enable)
;1-->   1:TIE=timer interrupt enable
;1-->   0:TEN=timer enable
PIT_TCTRL_CH_IE  EQU  (PIT_TCTRL_TEN_MASK :OR: PIT_TCTRL_TIE_MASK)
;---------------------------------------------------------------
;PORTx_PCRn (Port x pin control register n [for pin n])
;___->10-08:Pin mux control (select 0 to 8)
;Use provided PORT_PCR_MUX_SELECT_2_MASK
;---------------------------------------------------------------
;Port B
PORT_PCR_SET_PTB2_UART0_RX  EQU  (PORT_PCR_ISF_MASK :OR: \
                                  PORT_PCR_MUX_SELECT_2_MASK)
PORT_PCR_SET_PTB1_UART0_TX  EQU  (PORT_PCR_ISF_MASK :OR: \
                                  PORT_PCR_MUX_SELECT_2_MASK)
;---------------------------------------------------------------
;SIM_SCGC4
;1->10:UART0 clock gate control (enabled)
;Use provided SIM_SCGC4_UART0_MASK
;---------------------------------------------------------------
;SIM_SCGC5
;1->09:Port B clock gate control (enabled)
;Use provided SIM_SCGC5_PORTB_MASK
;---------------------------------------------------------------
;SIM_SCGC6
;1->23:PIT clock gate control (enabled)
;Use provided SIM_SCGC6_PIT_MASK
;---------------------------------------------------------------
;SIM_SOPT2
;01=27-26:UART0SRC=UART0 clock source select (MCGFLLCLK)
;---------------------------------------------------------------
SIM_SOPT2_UART0SRC_MCGFLLCLK  EQU  \
                                 (1 << SIM_SOPT2_UART0SRC_SHIFT)
;---------------------------------------------------------------
;SIM_SOPT5
; 0->   16:UART0 open drain enable (disabled)
; 0->   02:UART0 receive data select (UART0_RX)
;00->01-00:UART0 transmit data select source (UART0_TX)
SIM_SOPT5_UART0_EXTERN_MASK_CLEAR  EQU  \
                               (SIM_SOPT5_UART0ODE_MASK :OR: \
                                SIM_SOPT5_UART0RXSRC_MASK :OR: \
                                SIM_SOPT5_UART0TXSRC_MASK)
;---------------------------------------------------------------
;UART0_BDH
;    0->  7:LIN break detect IE (disabled)
;    0->  6:RxD input active edge IE (disabled)
;    0->  5:Stop bit number select (1)
;00001->4-0:SBR[12:0] (UART0CLK / [9600 * (OSR + 1)]) 
;UART0CLK is MCGPLLCLK/2
;MCGPLLCLK is 96 MHz
;MCGPLLCLK/2 is 48 MHz
;SBR = 48 MHz / (9600 * 16) = 312.5 --> 312 = 0x138
UART0_BDH_9600  EQU  0x01
;---------------------------------------------------------------
;UART0_BDL
;26->7-0:SBR[7:0] (UART0CLK / [9600 * (OSR + 1)])
;UART0CLK is MCGPLLCLK/2
;MCGPLLCLK is 96 MHz
;MCGPLLCLK/2 is 48 MHz
;SBR = 48 MHz / (9600 * 16) = 312.5 --> 312 = 0x138
UART0_BDL_9600  EQU  0x38
;---------------------------------------------------------------
;UART0_C1
;0-->7:LOOPS=loops select (normal)
;0-->6:DOZEEN=doze enable (disabled)
;0-->5:RSRC=receiver source select (internal--no effect LOOPS=0)
;0-->4:M=9- or 8-bit mode select 
;        (1 start, 8 data [lsb first], 1 stop)
;0-->3:WAKE=receiver wakeup method select (idle)
;0-->2:IDLE=idle line type select (idle begins after start bit)
;0-->1:PE=parity enable (disabled)
;0-->0:PT=parity type (even parity--no effect PE=0)
UART0_C1_8N1  EQU  0x00
;---------------------------------------------------------------
;UART0_C2
;0-->7:TIE=transmit IE for TDRE (disabled)
;0-->6:TCIE=transmission complete IE for TC (disabled)
;0-->5:RIE=receiver IE for RDRF (disabled)
;0-->4:ILIE=idle line IE for IDLE (disabled)
;1-->3:TE=transmitter enable (enabled)
;1-->2:RE=receiver enable (enabled)
;0-->1:RWU=receiver wakeup control (normal)
;0-->0:SBK=send break (disabled, normal)
UART0_C2_T_R    EQU  (UART0_C2_TE_MASK :OR: UART0_C2_RE_MASK)
UART0_C2_T_RI   EQU  (UART0_C2_RIE_MASK :OR: UART0_C2_T_R)
UART0_C2_TI_RI  EQU  (UART0_C2_TIE_MASK :OR: UART0_C2_T_RI)
;---------------------------------------------------------------
;UART0_C3
;0-->7:R8T9=9th data bit for receiver (not used M=0)
;           10th data bit for transmitter (not used M10=0)
;0-->6:R9T8=9th data bit for transmitter (not used M=0)
;           10th data bit for receiver (not used M10=0)
;0-->5:TXDIR=UART_TX pin direction in single-wire mode
;            (no effect LOOPS=0)
;0-->4:TXINV=transmit data inversion (not inverted)
;0-->3:ORIE=overrun IE for OR (disabled)
;0-->2:NEIE=noise error IE for NF (disabled)
;0-->1:FEIE=framing error IE for FE (disabled)
;0-->0:PEIE=parity error IE for PF (disabled)
UART0_C3_NO_TXINV  EQU  0x00
;---------------------------------------------------------------
;UART0_C4
;    0-->  7:MAEN1=match address mode enable 1 (disabled)
;    0-->  6:MAEN2=match address mode enable 2 (disabled)
;    0-->  5:M10=10-bit mode select (not selected)
;01111-->4-0:OSR=over sampling ratio (16)
;               = 1 + OSR for 3 <= OSR <= 31
;               = 16 for 0 <= OSR <= 2 (invalid values)
UART0_C4_OSR_16           EQU  0x0F
UART0_C4_NO_MATCH_OSR_16  EQU  UART0_C4_OSR_16
;---------------------------------------------------------------
;UART0_C5
;  0-->  7:TDMAE=transmitter DMA enable (disabled)
;  0-->  6:Reserved; read-only; always 0
;  0-->  5:RDMAE=receiver full DMA enable (disabled)
;000-->4-2:Reserved; read-only; always 0
;  0-->  1:BOTHEDGE=both edge sampling (rising edge only)
;  0-->  0:RESYNCDIS=resynchronization disable (enabled)
UART0_C5_NO_DMA_SSR_SYNC  EQU  0x00
;---------------------------------------------------------------
;UART0_S1
;0-->7:TDRE=transmit data register empty flag; read-only
;0-->6:TC=transmission complete flag; read-only
;0-->5:RDRF=receive data register full flag; read-only
;1-->4:IDLE=idle line flag; write 1 to clear (clear)
;1-->3:OR=receiver overrun flag; write 1 to clear (clear)
;1-->2:NF=noise flag; write 1 to clear (clear)
;1-->1:FE=framing error flag; write 1 to clear (clear)
;1-->0:PF=parity error flag; write 1 to clear (clear)
UART0_S1_CLEAR_FLAGS  EQU  (UART0_S1_IDLE_MASK :OR: \
                            UART0_S1_OR_MASK :OR: \
                            UART0_S1_NF_MASK :OR: \
                            UART0_S1_FE_MASK :OR: \
                            UART0_S1_PF_MASK)
;---------------------------------------------------------------
;UART0_S2
;1-->7:LBKDIF=LIN break detect interrupt flag (clear)
;             write 1 to clear
;1-->6:RXEDGIF=RxD pin active edge interrupt flag (clear)
;              write 1 to clear
;0-->5:(reserved); read-only; always 0
;0-->4:RXINV=receive data inversion (disabled)
;0-->3:RWUID=receive wake-up idle detect
;0-->2:BRK13=break character generation length (10)
;0-->1:LBKDE=LIN break detect enable (disabled)
;0-->0:RAF=receiver active flag; read-only
UART0_S2_NO_RXINV_BRK10_NO_LBKDETECT_CLEAR_FLAGS  EQU  \
        (UART0_S2_LBKDIF_MASK :OR: UART0_S2_RXEDGIF_MASK)
;---------------------------------------------------------------

;****************************************************************
;MACROs
			MACRO
			NEWL
			MOVS	R0,#'\r'
			BL		PutChar
			MOVS	R0,#'\n'
			BL		PutChar
			MEND
;****************************************************************
;Program
;C source will contain main ()
;Only subroutines and ISRs in this assembly source
            AREA    MyCode,CODE,READONLY
			EXPORT	Init_UART0_IRQ
			EXPORT UART0_IRQHandler
;>>>>> begin subroutine code <<<<<
Init_UART0_IRQ	PROC	{R0-R14}
			PUSH	{R0-R3,LR}
			
			;Initialize input and output buffers
			MOVS	R2,#XQ_BUF_SZ
			LDR		R0,=RxQBuffer
			LDR		R1,=RxQRecord
			BL		InitQueue
			LDR		R0,=TxQBuffer
			LDR		R1,=TxQRecord
			BL		InitQueue
			
			;SIM_SPOT2 UART0SRC = 01
			LDR 	R0,=SIM_SOPT2
			LDR		R1,=SIM_SOPT2_UART0SRC_MASK
			LDR		R2,[R0,#0]							;Load copy of SOPT2
			BICS	R2,R2,R1							;Clear clock source select
			LDR		R1,=SIM_SOPT2_UART0SRC_MCGFLLCLK	;Load selection to select for this clock
			ORRS	R2,R2,R1							;Set selection bits in R2
			STR		R2,[R0,#0]							;Save clock selection back to device
			;SIM_SOPT5 UART0ODE = 0, UART0RXSRC = 0, UART0TXSRC = 0
			LDR		R0,=SIM_SOPT5
			LDR		R1,=SIM_SOPT5_UART0_EXTERN_MASK_CLEAR
			LDR		R2,[R0,#0]
			BICS	R2,R2,R1							;Clear bits to set external UART connection
			STR		R2,[R0,#0]
			;SIM_SCGC4 UART0 = 1
			LDR		R0,=SIM_SCGC4
			LDR		R1,=SIM_SCGC4_UART0_MASK
			LDR		R2,[R0,#0]
			ORRS	R2,R2,R1
			STR		R2,[R0,#0]
			;SIM_SCGC5 PORTB = 1
			LDR		R0,=SIM_SCGC5
			LDR		R1,=SIM_SCGC5_PORTB_MASK
			LDR		R2,[R0,#0]
			ORRS	R2,R2,R1
			STR		R2,[R0,#0]
			;MUX = 010
			LDR		R0,=PORTB_PCR2
			LDR		R1,=PORT_PCR_SET_PTB2_UART0_RX
			STR		R1,[R0,#0]
			LDR		R0,=PORTB_PCR1
			LDR		R1,=PORT_PCR_SET_PTB1_UART0_TX
			STR		R1,[R0,#0]
			
			LDR		R0,=UART0_BASE						;Load base address
			;Disable UART0
			MOVS	R1,#UART0_C2_T_R
			LDRB	R2,[R0,#UART0_C2_OFFSET]
			BICS	R2,R2,R1
			STRB	R2,[R0,#UART0_C2_OFFSET]
			;Initialize NVIC for interrupts on UART0
			;Set UART0 IRQ priority
			LDR		R0,=UART0_IPR
			;LDR	R1,=NVIC_IPR_UART0_MASK
			LDR		R2,=NVIC_IPR_UART0_PRI_3
			LDR		R3,[R0,#0]
			;BICS	R3,R3,R1
			ORRS	R3,R3,R2
			STR		R3,[R0,#0]
			;Clear any pending UART0 interrupts
			LDR		R0,=NVIC_ICPR
			LDR		R1,=NVIC_ICPR_UART0_MASK
			STR		R1,[R0,#0]
			;Unmask UART0 interrupts
			LDR		R0,=NVIC_ISER
			LDR		R1,=NVIC_ISER_UART0_MASK
			STR		R1,[R0,#0]
			;Set baud rate
			LDR		R0,=UART0_BASE
			MOVS	R1,#UART0_BDH_9600
			STRB	R1,[R0,#UART0_BDH_OFFSET]
			MOVS	R1,#UART0_BDL_9600
			STRB	R1,[R0,#UART0_BDL_OFFSET]
			;Set format
			MOVS	R1,#UART0_C1_8N1
			STRB	R1,[R0,#UART0_C1_OFFSET]
			MOVS	R1,#UART0_C3_NO_TXINV
			STRB	R1,[R0,#UART0_C3_OFFSET]
			MOVS	R1,#UART0_C4_NO_MATCH_OSR_16
			STRB	R1,[R0,#UART0_C4_OFFSET]
			MOVS	R1,#UART0_C5_NO_DMA_SSR_SYNC
			STRB	R1,[R0,#UART0_C5_OFFSET]
			MOVS	R1,#UART0_S1_CLEAR_FLAGS
			STRB	R1,[R0,#UART0_S1_OFFSET]
			MOVS	R1,#UART0_S2_NO_RXINV_BRK10_NO_LBKDETECT_CLEAR_FLAGS
			STRB	R1,[R0,#UART0_S2_OFFSET]
			;Enable UART0 again
			MOVS	R1,#UART0_C2_T_RI
			STRB	R1,[R0,#UART0_C2_OFFSET]
			
			POP		{R0-R3,PC}
			ENDP
;---------------------------------------------------------------
; Initialize a Queue
; R0 is start of buffer, R1 is start of record, R2 is size
InitQueue     PROC    {R0-R14}
              PUSH    {R0,R1,R2,LR}
              STR     R0,[R1,#IN_PTR]         ; Start in pointer at start
              STR     R0,[R1,#OUT_PTR]        ; Start out pointer at start
              STR     R0,[R1,#BUF_STRT]       ; Store start
              ADDS    R0,R0,R2                ; Calculate end address
              STR     R0,[R1,#BUF_PAST]       ; Store end address
              STRB    R2,[R1,#BUF_SIZE]       ; Store size of buffer
              MOVS    R0,#0
              STRB    R0,[R1,#NUM_ENQD]       ; Put 0 in size
              POP     {R0,R1,R2,PC}
              ENDP
				  ;---------------------------------------------------------------------------
; Put R0 into the UART0 output
				EXPORT PutChar
PutChar         PROC    {R0-R14}
                PUSH    {R0,R1,R2,LR}
                LDR             R1,=TxQRecord
PCLoop          CPSID   I
                BL              Enqueue
                CPSIE   I
                                BCS             PCLoop
                                LDR             R0,=UART0_BASE
                                LDRB    R1,[R0,#UART0_C2_OFFSET]
                                MOVS    R2,#UART0_C2_TI_RI
                                ORRS    R1,R1,R2
                                STRB    R1,[R0,#UART0_C2_OFFSET]
                POP     {R0,R1,R2,PC}
                ENDP
;-----------------------------------------------------------------------------
; Get a character from UART0
				EXPORT GetChar
GetChar                 PROC    {R1-R14}
                PUSH    {R1,LR}
                                LDR             R1,=RxQRecord
GCLoop                  CPSID   I
                                BL              Dequeue
                                CPSIE   I
                                BCS             GCLoop
                POP     {R1,PC}
                ENDP
;---------------------------------------------------------------
; Dequeue a character
; R1 has record structure
Dequeue                 PROC    {R1-R14}
                                PUSH    {R1,R2,R3,LR}
                                LDRB    R0,[R1,#NUM_ENQD]                       ; Get number in queue
                                CMP             R0,#0
                                BEQ             D_fail                                          ; Handle failure if 0 in queue

                                LDR             R2,[R1,#OUT_PTR]                        ; Load out pointer
                                LDRB    R0,[R2,#0]                                      ; Load queue output

                                ADDS    R2,R2,#1                                        ; Increment out pointer

                                LDR             R3,[R1,#BUF_PAST]                       ; Load end of buffer
                                CMP             R2,R3
                                BNE             D_ok                                            ; If the are not equal, it is ok

                                LDR             R2,[R1,#BUF_STRT]                       ; If past the end go to the start
D_ok                    STR             R2,[R1,#OUT_PTR]                        ; Update the out pointer

                                LDRB    R2,[R1,#NUM_ENQD]                       ; Load the number enqueued
                                SUBS    R2,R2,#1                                        ; Subtract 1
                                STRB    R2,[R1,#NUM_ENQD]                       ; Update it

                                MOVS    R2,#0
                                LSRS    R2,R2,#1                                        ; Clear carry
                                B               D_end
D_fail                  MOVS    R2,#1
                                LSRS    R2,R2,#1                                        ; Set carry
D_end                   POP             {R1,R2,R3,PC}
                                ENDP
;---------------------------------------------------------------
; Enqueue a character
; R0 has character, R1 has record structure
Enqueue                 PROC    {R1-R14}
                                PUSH    {R0,R1,R2,R3,LR}

                                LDRB    R2,[R1,#NUM_ENQD]                       ; Get the number enqueued
                                LDRB    R3,[R1,#BUF_SIZE]                       ; Get the size of the buffer

                                CMP             R2,R3
                                BEQ             E_fail                                          ; Handle failure if queue full

                                LDR             R2,[R1,#IN_PTR]                         ; Load the in pointer
                                STRB    R0,[R2,#0]                                      ; Store

                                ADDS    R2,R2,#1                                        ; Increment in pointer
                                LDR             R3,[R1,#BUF_PAST]                       ; Check if in pointer is past the end
                                CMP             R2,R3
                                BLT             E_ok

                                LDR             R2,[R1,#BUF_STRT]                       ; If past, set in pointer to buffer start

E_ok                    STR             R2,[R1,#IN_PTR]                         ; store the in pointer
                                LDRB    R0,[R1,#NUM_ENQD]
                                ADDS    R0,R0,#1
                                STRB    R0,[R1,#NUM_ENQD]                       ; increment number enqueued
                                MOVS    R2,#0
                                LSRS    R2,R2,#1                                        ; clear carry
                                B               E_end

E_fail                  MOVS    R2,#1
                                LSRS    R2,R2,#1                                        ; set carry
E_end                   POP             {R0,R1,R2,R3,PC}
                                ENDP

;---------------------------------------------------------------
; Interrupt subroutine
UART0_IRQHandler
			PUSH	{LR}
			CPSID	I
			LDR		R1,=UART0_BASE
			MOVS	R2,#UART0_C2_TIE_MASK
			LDRB	R3,[R1,#UART0_C2_OFFSET]
			ANDS	R3,R3,R2
			BEQ		UART0_ISR_Receive			;TxInterruptEnabled false
			MOVS	R2,#UART0_S1_TDRE_MASK
			LDRB	R3,[R1,#UART0_S1_OFFSET]
			ANDS	R3,R3,R2
			BEQ		UART0_ISR_Receive			;TxInterrupt false
			LDR		R1,=TxQRecord
			BL		Dequeue
			LDR		R1,=UART0_BASE				;Shouldn't set carry i think
			BCS		UART0_ISR_DisableTx			;Dequeue unsuccessful, disable txinterrupt
			STRB	R0,[R1,#UART0_D_OFFSET]
			B		UART0_ISR_End
UART0_ISR_DisableTx
			MOVS	R2,#UART0_C2_T_RI			;Disable transmit interrupts if there is not a character to transmit
			STRB	R2,[R1,#UART0_C2_OFFSET]
			B		UART0_ISR_End
UART0_ISR_Receive
			MOVS	R2,#UART0_S1_RDRF_MASK
			LDRB	R3,[R1,#UART0_S1_OFFSET]
			ANDS	R3,R3,R2
			BEQ		UART0_ISR_End
			LDRB	R0,[R1,#UART0_D_OFFSET]
			LDR		R1,=RxQRecord
			BL		Enqueue						;No check because can't do anything about a failure
UART0_ISR_End
			CPSIE	I
			POP		{PC}

;---------------------------------------------------------------
; Print string in R0 with max len R1
						EXPORT	PutStringSB
PutStringSB             PROC    {R0-R14}
                                PUSH    {R2,LR}
                                MOVS    R2,R0           ; Move address out of R0, needed for PutChar
                                ADDS    R1,R0,R1

PS_PutChLoop    CMP             R1,R2
                                BEQ             PS_LoopDone     ; Stop if at end of buffer
                                LDRB    R0,[R2,#0]      ; Load next character
                                CMP             R0,#0
                                BEQ             PS_LoopDone ; Stop if null
                                BL              PutChar         ; Print character
                                ADDS    R2,R2,#1        ; Increase address to load next time
                                B               PS_PutChLoop

PS_LoopDone             POP             {R2,PC}         ; Restore registers
                                ENDP
;---------------------------------------------------------------
; Print a number in hex format
; Number in R0
						EXPORT PutNumHex
PutNumHex               PROC    {R0-R14}
                                PUSH    {R1,R2,R3,LR}
                                MOVS    R1,R0
                                MOVS    R3,#NIBBLE_MASK
                                MOVS    R2,#28
PNH_loop                MOVS    R0,R1
                                LSRS    R0,R0,R2
                                ANDS    R0,R0,R3
                                CMP             R0,#9
                                BLS             PNH_alpha
                                ADDS    R0,R0,#('A'-'0'-10)
PNH_alpha               ADDS    R0,R0,#'0'
                                BL              PutChar
                                SUBS    R2,R2,#4
                                BGE             PNH_loop
PNH_done                POP             {R1,R2,R3,PC}
                                ENDP
;---------------------------------------------------------------
; Get a string from UART, store in R0 with max length R1
; Outputs length to R7
								EXPORT GetStringSB
GetStringSB             PROC    {R0-R5,R7-R14}
                                PUSH    {R0,R2,R3,LR}
                                MOVS    R2,#1                   ; Count characters
                                SUBS    R3,R0,#1                ; Free R0 for GetChar

GS_ReadChLoop   BL              GetChar                 ; Get the character
                                CMP             R0,#'\b'
                                BEQ             GS_back
                                CMP             R0,#0x7F
                                BEQ             GS_back                 ; Handle backspace if necessary
                                CMP             R0,#'\r'
                                BEQ             GS_LoopDone             ; End loop if newline
                                CMP             R0,#0x20
                                BLO             GS_ReadChLoop   ; Ignore control characters not already handled
                                STRB    R0,[R3,R2]              ; Store character typed
                                ADDS    R2,R2,#1                ; increment character count
                                BL              PutChar                 ; echo character
                                CMP             R2,R1
                                BEQ             GS_BufFull
                                B               GS_ReadChLoop
GS_BufFull              SUBS    R2,R2,#1
                                MOVS    R0,#'\b'
                                BL              PutChar
                                MOVS    R0,#' '
                                BL              PutChar
                                MOVS    R0,#'\b'
                                BL              PutChar
                                B               GS_ReadChLoop

GS_back                 CMP             R2,#1
                                BEQ             GS_ReadChLoop   ; ignore if string empty
                                SUBS    R2,R2,#1                ; decrease string length
                                MOVS    R0,#'\b';
                                BL              PutChar                 ; move cursor back
                                MOVS    R0,#' '
                                BL              PutChar                 ; clear character already there
                                MOVS    R0,#'\b';
                                BL              PutChar                 ; move cursor back again
                                B               GS_ReadChLoop   ; read another character

GS_LoopDone             MOVS    R0,#0
                                STRB    R0,[R3,R2]              ; Null terminate
                                NEWL                                    ; print newline
                                MOVS    R6,R2                   ; move length to R7
                                POP             {R0,R2,R3,PC}   ; restore registers
                                ENDP

;>>>>>   end subroutine code <<<<<
            ALIGN
;**********************************************************************
;Constants
            AREA    MyConst,DATA,READONLY
;>>>>> begin constants here <<<<<
			EXPORT	DAC0_table_0
DAC0_table_0
DAC0_table
			DCW			(((DAC0_STEPS-1)*1)/(SERVO_POSITIONS*2))
			DCW			(((DAC0_STEPS-1)*3)/(SERVO_POSITIONS*2))
			DCW			(((DAC0_STEPS-1)*5)/(SERVO_POSITIONS*2))
			DCW			(((DAC0_STEPS-1)*7)/(SERVO_POSITIONS*2))
			DCW			(((DAC0_STEPS-1)*9)/(SERVO_POSITIONS*2))
			
			EXPORT PWM_duty_table_0
PWM_duty_table
PWM_duty_table_0
			DCW			(PWM_DUTY_10-1)
			DCW			(((3*(PWM_DUTY_10-PWM_DUTY_5)/4)+PWM_DUTY_5)-1)
			DCW			(((1*(PWM_DUTY_10-PWM_DUTY_5)/2)+PWM_DUTY_5)-1)
			DCW			(((1*(PWM_DUTY_10-PWM_DUTY_5)/4)+PWM_DUTY_5)-1)
			DCW			(PWM_DUTY_5-1)
;>>>>>   end constants here <<<<<
;**********************************************************************
;Variables
            AREA    MyData,DATA,READWRITE
;>>>>> begin variables here <<<<<
TxQBuffer       SPACE   80
                        ALIGN
TxQRecord       SPACE   Q_REC_SZ
                        ALIGN
RxQBuffer       SPACE   80
                        ALIGN
RxQRecord       SPACE   Q_REC_SZ
                        ALIGN

;>>>>>   end variables here <<<<<
            END
