
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define LED2_BLUE 4
#define LED2_RED 1
#define LED2_GREEN 2
#define delay_ms(t) __delay_cycles(2400*t)

//Change the main function as follows:
int main(void){
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;
    __disable_irq();
    P2->SEL0 &= ~(LED2_BLUE & LED2_RED & LED2_GREEN);
    P2->SEL1 &= ~(LED2_BLUE & LED2_RED & LED2_GREEN);
    P2->DIR = LED2_BLUE | LED2_RED | LED2_GREEN;
//    P2->OUT ^= LED2_BLUE;

    TIMER_A1->CTL = 0x2D1;
    TIMER_A1->EX0 = 0x07;
    TIMER_A1->CCR[0] = 46875 - 1; //time for the timer to interrupt
    TIMER_A1->CCTL[0] |= 1 << 4; //toggle CCIFG

    TIMER_A2->CTL = 0x2D1;
    TIMER_A2->EX0 = 0x07;
    TIMER_A2->CCR[0] = 46875 - 1; //time for the timer to interrupt
    TIMER_A2->CCTL[0] |= 1 << 4; //toggle CCIFG

    NVIC->IP[10] = 0x60;
    NVIC->ISER[0] = 1<<10 | 1<<12;

    // RED PRIORITY
    NVIC_SetPriority(TA1_0_IRQn, 1);
    // BLUE PRIORITY
    NVIC_SetPriority(TA2_0_IRQn, 3);

    NVIC_EnableIRQ(TA1_0_IRQn);
    NVIC_EnableIRQ(TA2_0_IRQn);

    __enable_irq();

    while(1){

    };
};

void TA1_0_IRQHandler() {
//    TIMER_A1->CCTL[0] &=~1; //toggle CCIFG
    TIMER_A1->CTL = ~TIMER_A_CTL_IFG;
    P2->OUT &= ~ LED2_GREEN;
    P2->OUT ^= LED2_BLUE;
    delay_ms(1000);
}
void TA2_0_IRQHandler() {
//    TIMER_A2->CCTL[0] &=~1; //toggle CCIFG
    TIMER_A2->CTL = ~TIMER_A_CTL_IFG;
    P2->OUT &= ~ LED2_BLUE;
    P2->OUT ^= LED2_GREEN;
    delay_ms(1000);
}
