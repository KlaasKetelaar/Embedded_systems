#include <stdio.h>
#include "msp.h"
#include "math.h"

#define delay_ms(t) __delay_cycles(2400*t)

uint8_t ADDRESS = 0x4A;
uint8_t data;
uint8_t transfer_complete = 0;

void pin_setup()
{
    //P1->SEL0 &= 0x01;
    //P1->SEL1 &= 0x00;
    P1->SEL0 |= 0xC0;
    P1->SEL1 &= ~0xC0;
}

void twi_master_setup()
{
    __disable_irq();
    EUSCI_B0->CTLW0 = 0x01;     //0b 0000 0000 0000 0001
    //EUSCI_B0->CTLW0 = 0x0F41;   //0b 0000 1111 0100 0001
    EUSCI_B0->CTLW0 = 0x0F51;   //0b 0000 1111 0100 0001
    EUSCI_B0->CTLW1 = 0x08;     //0b 0000 0000 0000 1000
    EUSCI_B0->TBCNT = 0x01;     //0b 0000 0000 0000 0001
    EUSCI_B0->BRW = 0x40;       //0b 0000 0000 0100 0000
    EUSCI_B0->CTLW0 &=~0x01;    //0b 0000 0000 0000 0001
    //EUSCI_B0->IE |= 0x8;        //0b 0000 0000 0000 1000
    EUSCI_B0->IE = 0x0003;
    //NVIC_SetPriority(EUSCIB0_IRQn, 3);
    //NVIC_EnableIRQ(EUSCIB0_IRQn);
    NVIC->IP[20] = 0x60;            // priority
    NVIC->ISER[0] |= 0x00100000;    // enable
    __enable_irq();

}
void i2c_B0_transmit()
{
    EUSCI_B0->I2CSA = ADDRESS;
    //EUSCI_B0->CTLW0 |= 0xF;     //0b 0000 0000 0001 0000
    EUSCI_B0->CTLW0 |= 0x0010;
    EUSCI_B0->CTLW0 |= 0x2;     //0b 0000 0000 0000 0010
    transfer_complete = 0;  //new, check your workshop manual
}
void i2c_B0_receive()
{
    EUSCI_B0->I2CSA = ADDRESS;
    //EUSCI_B0->CTLW0 &=~0xF;     //0b 0000 0000 0001 0000
    EUSCI_B0->CTLW0 &=~0x0010;
    EUSCI_B0->CTLW0 |= 0x2;     //0b 0000 0000 0000 0010
    transfer_complete = 0;       //0b 0000 0000 0000 0000
}
void EUSCIB0_IRQHandler()
{
    uint16_t temp = EUSCI_B0->IFG;
    // TX interrupt
    // Check for TX flag, check if TX buffer is empty (prior transmit complete)
    if (temp & BIT1) {
        EUSCI_B0->IFG &= ~BIT1; // clear flag
        if (!transfer_complete) {
            EUSCI_B0->TXBUF = data; // load data
            //transfercomplete = 1;
            transfer_complete = 1;
        } else {
            EUSCI_B0->CTLW0 |= UCTXSTP; // issue stop
            EUSCI_B0->IFG &= ~BIT1; // clear flag
        }
    }
    // RX interrupt
    // Check for RX flag (receive complete)
    else if (temp & BIT0) {
        EUSCI_B0->IFG &= ~BIT0; // clear flag
        if (!transfer_complete) {
            data = EUSCI_B0->RXBUF; // read data
            //transfercomplete = 1; // stop reading
            transfer_complete = 1; // stop reading
        }
    }
}
/**
 * main.c
 */
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    pin_setup();
    twi_master_setup();
    data = 0x00;    //new
    i2c_B0_transmit();
    delay_ms(1000);
    while(1){
        i2c_B0_receive();
        delay_ms(1000); //new
        int8_t temp = (int8_t)data; // new
        printf("Temperature is: %d\n",temp);
    }
}


