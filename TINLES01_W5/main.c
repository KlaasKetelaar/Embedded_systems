#include <stdio.h>
#include "msp.h"
#include "math.h"

uint8_t data;
uint8_t transfer_complete = 0;

void pin_setup()
{
    P1->SEL0 &= 0x01;
    P1->SEL1 &= 0x00;
}

void twi_master_setup()
{
    __disable_irq();
    EUSCI_B0->CTLW0 = 0x01;     //0b0000000000000001
    EUSCI_B0->CTLW0 = 0x0F41;   //0b 0000 1111 0100 0001
    EUSCI_B0->CTLW1 = 0x08;     //0b 0000 0000 0000 1000
    EUSCI_B0->TBCNT = 0x01;
    EUSCI_B0->BRW = 0x40;
    EUSCI_B0->CTLW0 &=~0x01;    //0b0000000000000001
    EUSCI_B0->IE |= 0x8;
    NVIC_SetPriority(EUSCIB0_IRQn, 3);
    NVIC_EnableIRQ(EUSCIB0_IRQn);
    __enable_irq();

}
void i2c_B0_transmit()
{

}
void i2c_B0_recieve()
{

}
void EUSCIB0_IRQHandler()
{

}
/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

}


