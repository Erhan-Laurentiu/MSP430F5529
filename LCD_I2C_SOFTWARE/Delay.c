/*
 * Delay.c
 *
 *  Created on: Apr 21, 2020
 *      Author: E.Laurentiu
 */

#include"Delay.h"

#define TIMER_CLK_ACLK  0x100   //1<<8
#define TIMER_CLK_SMCLK 0x200   //1<<9

#define TIMER_DIV_1 0x00
#define TIMER_DIV_2 0x40        //01<<6
#define TIMER_DIV_4 0x80        //10<<6
#define TIMER_DIV_8 0xC0        //11<<6

#define TIMER_DIV_EX_1 0x00
#define TIMER_DIV_EX_2 0x01
#define TIMER_DIV_EX_3 0x02
#define TIMER_DIV_EX_4 0x03
#define TIMER_DIV_EX_5 0x04
#define TIMER_DIV_EX_6 0x05
#define TIMER_DIV_EX_7 0x06
#define TIMER_DIV_EX_8 0x07

#define SMCLK_FREQUENCY 1000000 //defaul smclk~1MHz
#define SMCLK_PERIOD 1/SMCLK_FREQUENCY

#define ACLK_FREQUENCY 32768 //defaul smclk~1MHz
#define ACLK_PERIOD 1/ACLK_FREQUENCY

//void DELAY_TIMER_A2(unsigned char clockSrc , unsigned char divisionFactor , unsigned char extraDivisionFactor ){
//    TA2CCTL0= CCIE;
//    TA2CTL = clockSrc + divisionFactor + MC_1; // smclk/8 =~125KHz  up mode
//    TA2EX0 = extraDivisionFactor;
//    unsigned char period ;
//    if(TA2CTL&TIMER_CLK_ACLK) period = ACLK_PERIOD * divisionFactor>>5;
//}



void TIMER_A2_START_SMCLK(){
    TA2CCTL0= CCIE;
    TA2CTL = TASSEL_2 + ID_3 + MC_1; // smclk/8 =~125KHz  up mode

}

void TIMER_A2_START_ACLK(){

    TA2CCTL0= CCIE;
    TA2CTL = TASSEL_2 + ID_3 + MC_1; // smclk/8 =~125KHz  up mode

}

void DELAY_TIMER_A2_SMCLK(unsigned int calculatedValue ){
    //calculatedValue = delayWanted/8    (delayWanted = [us])
    //max(calculatedValue) = 2^16 = 65536

    TA2CCR0 = calculatedValue-1;
    TIMER_A2_START_SMCLK();
    __bis_SR_register(LPM0_bits + GIE);       // Enter LPM3 w/ interrupt (CPU->OFF)
    TIMER_A2_STOP();


}
void DELAY_TIMER_A2_ACLK(unsigned int calculatedValue){
    //calculatedValue = delayWanted/30.517578125    (delayWanted = [us])
    //max(calculatedValue) = 2^16 = 65536

    TA2CCR0 = calculatedValue-1;
    TIMER_A2_START_ACLK();
    __bis_SR_register(LPM3_bits + GIE);       // Enter LPM3 w/ interrupt (CPU->OFF)
    TIMER_A2_STOP();

}

void TIMER_A2_STOP(){
    TA2CTL = MC_0;
}


// Timer2 A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER2_A0_VECTOR
__interrupt void Timer2_A0 (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER2_A0_VECTOR))) Timer2_A0 (void)
#else
#error Compiler not supported!
#endif
{
    __bic_SR_register_on_exit(LPM3_bits);   //Exit LPM3 (CPU->ON)

}
