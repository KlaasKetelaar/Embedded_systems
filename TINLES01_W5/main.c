// #include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdio.h>
#include "msp432.h"
#include "math.h"
#include <string.h>

#define LED2_BLUE 4
#define LED2_RED 1
#define LED2_GREEN 2
#define delay_ms(t) __delay_cycles(2400 * t)

uint8_t data;
uint8_t transfercomplete = 0;

void pin_setup()
{
    P1->SEL0 |= 0xC0;
    P1->SEL1 &= ~0xC0;
    //    P1->DIR = 96;
}
void twi_master_setup()
{
    EUSCI_B0->CTLW0 = 0x0001; // turn on byte 1
    EUSCI_B0->CTLW0 = 0x0051; // activate bit 12,11,10,7 =
    EUSCI_B0->CTLW1 = 0x0008;

    EUSCI_B0->TBCNT = 0x0001;
    EUSCI_B0->BRW = 0x0040;
    EUSCI_B0->CTLW0 &= ~0x0001;
    EUSCI_B0->IE = 0x0008;

    NVIC->IP[10] = 0x60;
    NVIC->ISER[0] |= 0x00100000;
    __enable_irq();
}
void i2c_B0_transmit()
{
    EUSCI_B0->I2CSA = 0x4A;
    EUSCI_B0->CTLW0 |= 0x0010;
    EUSCI_B0->CTLW0 |= 0x0002;
}

void i2c_B0_receive()
{
    EUSCI_B0->I2CSA = 0x4A;
    EUSCI_B0->CTLW0 &= ~0x0010;
    EUSCI_B0->CTLW0 |= 0x0002;
    transfercomplete = 0;
}
void EUSCIB0_IRQHandler()
{
    uint16_t temp;
    temp = EUSCI_B0->IFG;
    if (temp & 2)
    {
        EUSCI_B0->IFG &= ~0x02;
        if (!transfercomplete)
        {
            EUSCI_B0->TXBUF = data;
            transfercomplete = 1;
        }
        else
        {
            EUSCI_B0->CTLW0 |= 8;
            EUSCI_B0->IFG &= ~0x01;
        }
    }
    else if (temp & 1)
    {
        EUSCI_B0->IFG &= ~0x01;
        if (!transfercomplete)
        {
            data = EUSCI_B0->RXBUF;
            transfercomplete = 1;
        }
    }
}

// Change the main function as follows:
int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer

    pin_setup();
    twi_master_setup();
    i2c_B0_transmit();

    printf("%d \n", data);

    delay_ms(1000);

    printf("%d \n", data);

    while (1)
    {
        i2c_B0_receive();
        printf("%d \n", data);
        delay_ms(1000);
    }
}
