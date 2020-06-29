/******************************************************************************/
/*                                                                            */
/* Project: countdown                                                         */
/* Subject: INP                                                               */
/* Year: 2019/2020                                                            */
/* Author: Adam Sedlacek | xsedla1e                                           */
/*                                                                            */
/* Changes: ~80 % original was made by Michal Bidlo (INP demo FitKit 3.0)     */
/*                                                                            */
/*        I'm using buttons definition, MCUinit, and LPTMR function.          */
/*                                                                            */
/******************************************************************************/

/* Header file with all the essential definitions for a given type of MCU */
#include "MK60D10.h"

/* Mapping buttons to specific port pins: */
#define BTN_SW2 0x400     // Port E, bit 10
#define BTN_SW3 0x1000    // Port E, bit 12
#define BTN_SW4 0x8000000 // Port E, bit 27
#define BTN_SW5 0x4000000 // Port E, bit 26
#define BTN_SW6 0x800     // Port E, bit 11

#define SPK 0x10          // Speaker is on PTA4

/* array for each digit on display as "bitmask" */
unsigned int digit[4] = {
    0b00000000000000000000000100000000, // 1
    0b00000000000000000000000001000000, // 2
    0b00000000000000000000000010000000, // 3
    0b00000000000000000000001000000000  // 4
};

/* array for number which are display on display */
unsigned int number[11] = {
   0b00000000000000000000110000000000, // 0
   0b00111010000000000000110000000000, // 1
   0b00000011000000000000100000000000, // 2
   0b00010010000000000000100000000000, // 3
   0b00111000000000000000100000000000, // 4
   0b00010100000000000000100000000000, // 5
   0b00001100000000000000100000000000, // 6
   0b00110010000000000000110000000000, // 7
   0b00000000000000000000100000000000, // 8
   0b00110000000000000000100000000000, // 9
   0b11111111111111111111011111111111, // dot & with the rest
};

unsigned int compare = 0x400;
unsigned int cursor_digit = 500U;
unsigned int select_number = 500U;
int finish_flag = 0;
int confirm_flag = 0;
int pressed_up = 0;              // SW 5
int pressed_down = 0;            // SW 3
int pressed_left = 0;            // SW 4
int pressed_right = 0;           // SW 2
int pressed_confirm_flag = 0;    // SW 6
int fast_forward = 0;            // fast countdown

unsigned int time[4] = {0U, 2U, 1U, 0U}; // default state => two minutes and ten seconds


/* A delay function */
void delay(unsigned long long int bound) {
  for (unsigned long long int i=0 ; i < bound; i++);
}

/* Initialize the MCU - basic clock settings, turning the watchdog off */
void MCUInit(void)  {
    MCG_C4 |= ( MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(0x01) );
    SIM_CLKDIV1 |= SIM_CLKDIV1_OUTDIV1(0x00);
    WDOG_STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;
}

void PortsInit(void)
{
    /* Turn on all port clocks */
    SIM->SCGC5 = SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTE_MASK | SIM_SCGC5_PORTA_MASK;

    PORTE->PCR[10] = PORT_PCR_MUX(0x01); // SW2
    PORTE->PCR[12] = PORT_PCR_MUX(0x01); // SW3
    PORTE->PCR[27] = PORT_PCR_MUX(0x01); // SW4
    PORTE->PCR[26] = PORT_PCR_MUX(0x01); // SW5
    PORTE->PCR[11] = PORT_PCR_MUX(0x01); // SW6

    PORTA->PCR[4] = PORT_PCR_MUX(0x01);  // Speaker
    PORTA->PCR[6] = PORT_PCR_MUX(0x01);  // digit 2
    PORTA->PCR[7] = PORT_PCR_MUX(0x01);  // digit 3
    PORTA->PCR[8] = PORT_PCR_MUX(0x01);  // digit 1
    PORTA->PCR[9] = PORT_PCR_MUX(0x01);  // digit 4
    PORTA->PCR[10] = PORT_PCR_MUX(0x01); // segment G
    PORTA->PCR[11] = PORT_PCR_MUX(0x01); // segment P | dot
    PORTA->PCR[24] = PORT_PCR_MUX(0x01); // segment C
    PORTA->PCR[25] = PORT_PCR_MUX(0x01); // segment F
    PORTA->PCR[26] = PORT_PCR_MUX(0x01); // segment B
    PORTA->PCR[27] = PORT_PCR_MUX(0x01); // segment A
    PORTA->PCR[28] = PORT_PCR_MUX(0x01); // segment E
    PORTA->PCR[29] = PORT_PCR_MUX(0x01); // segment D

    /* Change corresponding PTA port pins as outputs */
    PTA->PDDR = GPIO_PDDR_PDD(0b00111111000000000000111111010000);
}


/* interrupt handler function, each second or 1/4 of second (fast forward) do the math */
void LPTMR0_IRQHandler(void) {
    unsigned int min;
    unsigned int sec;
    min = time[0] * 10 + time[1];
    sec = time[2] * 10 + time[3];

    // Set new compare value set by fast_forward value <- fast countdown mode
    LPTMR0_CMR = compare;                // !! the CMR reg. may only be changed while TCF == 1
    LPTMR0_CSR |=  LPTMR_CSR_TCF_MASK;   // writing 1 to TCF tclear the flag
    if (!finish_flag) { // if finish flag is still false -> do countdown
        if (0 == sec && min > 0) { //
            min--;
            sec = 59;
        } else {
            sec--;
        }
        time[0] = min / 10;
        time[1] = min % 10;
        time[2] = sec / 10;
        time[3] = sec % 10;
        if (0 >= min && 0 >= sec) finish_flag = 1;
    }
}

void LPTMR0Init(int count)
{
    SIM_SCGC5 |= SIM_SCGC5_LPTIMER_MASK; // Enable clock to LPTMR
    LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK;   // Turn OFF LPTMR to perform setup
    LPTMR0_PSR = ( LPTMR_PSR_PRESCALE(0) // 0000 is div 2
                 | LPTMR_PSR_PBYP_MASK   // LPO feeds directly to LPT
                 | LPTMR_PSR_PCS(1)) ;   // use the choice of clock
    LPTMR0_CMR = count;                  // Set compare value
    LPTMR0_CSR =(  LPTMR_CSR_TCF_MASK    // Clear any pending interrupt (now)
                 | LPTMR_CSR_TIE_MASK    // LPT interrupt enabled
                );
    NVIC_EnableIRQ(LPTMR0_IRQn);         // enable interrupts from LPTMR0
    LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;    // Turn ON LPTMR0 and start counting
}

/* a display function which provides infinite loop to show current time */
void displayTime() {
    while (1) {
        if (finish_flag) return;
        for (int i = 0; i < 4; i++) {
            if (1 == i) {
                i = 1; // without this assign it still has zero, mcu bug?
                PTA->PDOR = GPIO_PDOR_PDO((digit[i] | number[time[i]]) & number[10]);
            } else {
                PTA->PDOR = GPIO_PDOR_PDO(digit[i] | number[time[i]]);
            }
            delay(1000);
        }
    }
}

/* a beep function */
void beep(void) {
int q;
int m;
int cnt = 0;
    for (m = 0; m < 3; m++) {
        cnt += 10;
        for (q = 0; q < 1000; q++) {
            PTA->PDOR = GPIO_PDOR_PDO(SPK);
            delay(50*cnt);
            PTA->PDOR &= GPIO_PDOR_PDO(~SPK);
            delay(50*cnt);
        }
    }
}


void setDefaultValues() {
    LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK; // switch off LPTMR
    beep(); // notify that countdown has finished
    confirm_flag = 0;
    finish_flag = 0;
    fast_forward = 0;
    /* default value for timer -> 02:10 */
    time[0] = 0U;
    time[1] = 2U;
    time[2] = 1U;
    time[3] = 0U;
}


int main(void) {

    MCUInit();      // sets MCU
    PortsInit();    // sets pins PTA and PTE

    while (1) {

        if (!pressed_up && !(GPIOE_PDIR & BTN_SW5)) {
            pressed_up = 1;
            select_number++;
        } else if (GPIOE_PDIR & BTN_SW5)
            pressed_up = 0;

        if (!pressed_down && !(GPIOE_PDIR & BTN_SW3)) {
            select_number--;
            pressed_down = 1;
        } else if (GPIOE_PDIR & BTN_SW3)
            pressed_down = 0;

        if (!pressed_right && !(GPIOE_PDIR & BTN_SW2)) {
            cursor_digit++;
            pressed_right = 1;
        } else if (GPIOE_PDIR & BTN_SW2)
            pressed_right = 0;

        if (!pressed_left && !(GPIOE_PDIR & BTN_SW4)) {
            fast_forward = 1;
            pressed_left = 1;
        } else if (GPIOE_PDIR & BTN_SW4)
            pressed_left = 0;

        if (!pressed_confirm_flag && !(GPIOE_PDIR & BTN_SW6)) {
            pressed_confirm_flag = 1;
            confirm_flag = 1;
        } else if (GPIOE_PDIR & BTN_SW6)
            pressed_confirm_flag = 0;

        if (finish_flag) {
            setDefaultValues();
        } else if (confirm_flag) {
            compare = fast_forward ? 0x100 : 0x400;
            LPTMR0Init(compare);
            displayTime();
        } else {
            if (cursor_digit % 4 == 2) {
                PTA->PDOR = digit[cursor_digit % 4] | number[select_number % 6];
                time[cursor_digit % 4] = select_number % 6;
            } else {
                time[cursor_digit % 4] = select_number % 10;
                PTA->PDOR = digit[cursor_digit % 4] | number[select_number % 10];
            }
        }
    }
    return 0;
}
