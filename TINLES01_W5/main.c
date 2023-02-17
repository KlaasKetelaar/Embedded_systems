#include "msp.h"
#define LED2_RED BIT0
#define LED2_BLUE BIT4
#define S1 BIT1

int count = 0;
void delay_ms(uint16_t ms);

//for assignment 3
//void TA1_0_IRQHandler(){
//    TIMER_A1->CCTL[0] &=~1;
////    P5->OUT ^=1;
//    count++;
//    if(count == 4){
//        count = 0;
//        P5->OUT ^=1;
//    }
//}

//void W2_IntroFunc(void){
//    __disable_irq();
//    P5->SEL0 &=~0x01;
//    P5->SEL1 &=~0x01;
//    P5->DIR |= 0x01;
//    TIMER_A1->CTL =  0x2D2; // 0b001011010010 this config setsup the clock config
//    TIMER_A1->EX0 =  0x07; // 0b0111 sets up the divider
//    TIMER_A1->EX0 = 0x07;
//    TIMER_A1->CCR[0] = 46975 - 1;
//    TIMER_A1->CCTL[0] |= 1 << 4; //Enables the CCIFG flag by enabling the CCIE bit on the CCTL register of TIMER_A1
//    NVIC->IP[10] = 0x60;
//    NVIC->ISER[0] = 1<<10;
//    __enable_irq();
//    while(1){
//
//    }
//}

//void W2_Assignment3(void){
//    P2->DIR = LED2_RED;
//    P2->OUT = 0x00;
//    P1->REN = S1; //page 517
//    P1->OUT = S1;
//    P1->IE = S1; //Port interrupt is enabled for P1.1, page 519, Figure 10-11, Table 10-14
//    P1->IES = 0x00; // interrupt edge is select from low to high, page 518
//    P1->IFG = 0x00; // interrupt flag is cleared, page 519
//
//    NVIC->IP[35] = 0; // set the interrupt priority to 0 for port P1 interrupt
//    NVIC->ISER[1] = 0x00000008; // port P1 interrupt is enabled in NVIC
//    _enable_interrupts();
//    while (1);
//}
//
//void PORT1_IRQHandler(void){
//    P2->OUT ^=LED2_RED; // Toggle RED LED
//    delay_ms(2000);
//    P1->IFG &=~S1; //interrupt flag is cleared for P1.1
//}

void delay_ms(uint16_t ms){
    uint16_t delay;
    volatile uint32_t i;
    for (delay = ms; delay >0 ; delay--);
}

void W2_Assignment5(){
    __disable_irq();



    P2->DIR = LED2_RED | LED2_BLUE;

    TIMER_A0->CTL =  0x2D1; // 0b001011010010 this config setsup the clock config
    TIMER_A0->EX0 =  0x07; // 0b0111 sets up the divider
    TIMER_A0->CCR[0] = 46975 - 1;
    TIMER_A0->CCTL[0] |= 1 << 4; //Enables the CCIFG flag by enabling the CCIE bit on the CCTL register of TIMER_A0


    TIMER_A1->CTL =  0x2D1; // 0b001011010010 this config setsup the clock config
    TIMER_A1->EX0 =  0x07; // 0b0111 sets up the divider
    TIMER_A1->CCR[0] = 46975 - 1;
    TIMER_A1->CCTL[0] |= 1 << 4; //Enables the CCIFG flag by enabling the CCIE bit on the CCTL register of TIMER_A1



    NVIC_SetPriority(TA0_0_IRQn, 1);
    NVIC_SetPriority(TA1_0_IRQn, 1);

    NVIC_EnableIRQ(TA0_0_IRQn);
    NVIC_EnableIRQ(TA1_0_IRQn);

    __enable_irq();
}

void TA0_0_IRQHandler(){
//    TIMER_A0->CTL =~TIMER_A_CTL_IFG; //interrupt flag is cleared for P1.1
    TIMER_A0->CCTL[0] &=~1;
    P2->OUT ^=LED2_BLUE; // Toggle RED LED
    delay_ms(2000);
}
//for assignment 5
void TA1_0_IRQHandler(){
//    TIMER_A1->CTL =~TIMER_A_CTL_IFG;//interrupt flag is cleared for P1.1
    TIMER_A1->CCTL[0] &=~1;
    P2->OUT ^=LED2_RED; // Toggle RED LED
    delay_ms(2000);
}





/**
 * main.c
 */
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

//    W2_Assignment3();

//    W2_IntroFunc();
    W2_Assignment5();

    while(1){

    }
}
