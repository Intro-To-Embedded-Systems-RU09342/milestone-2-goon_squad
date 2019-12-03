/*
 * Written by: Kyle Limbaga, Sean Prendergast and Dawson Tocarchick
 * Assignment: Milestone 2
 * The purpose of this program is to have the MSP430G2553 capable
 * of changing the duty cycle of a fan by transmitting a PWM signal.
 * The duty cycle will change based on the value the user enters
 * for the desired temperature on Realterm as UART will be utilized.
 */

#include <msp430.h>
 unsigned int value=0;
 unsigned int desired_temp=40;
 float PTAT_value=0;
 float error=0;
 float temperature=0;

void UART(void)             //UART function
{

    UCA0CTL1 |= UCSSEL_2;   //smclk
    UCA0BR0 = 104;          //set baud rate 9600
    UCA0BR1 = 0;            //set baud rate 9600
    UCA0MCTL = UCBRS0;      //modulation UBRSx = 1
    UCA0CTL1 &= ~UCSWRST;   //initialize usci state machine
    IE2 |= UCA0RXIE;        //enable RX interrupt

    P1SEL |= BIT1 + BIT2;   //P1.1 = RXD P1.2 = TXD
    P1SEL2 |= BIT1 + BIT2;  //P1.1 = RXD P1.2 = TXD
}

void Timer_Setup(void)           //Timers function
{
    //timer A1
    TA1CTL |= TASSEL_2 + MC_1;     //smclk, up mode
    TA1CCTL1 |= OUTMOD_7;   //output mode for PWM
    TA1CCTL2 |= OUTMOD_7;   //output mode for PWM
    TA1CCR0 = 1025;         //pwm period
    TA1CCR1 = 0;            //PWM signal for fan
}

void ADC_Setup(void)
{
    ADC10CTL1 = INCH_3 + ADC10DIV_3 ;         //P1.3, ADC10CLK/3
    ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON + ADC10IE;  //ADC Control enabling interrupts
    ADC10AE0 |= BIT3;                         //input enable P1.3
}

#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
    __bic_SR_register_on_exit(CPUOFF);
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void UART_ISR(void)
{
  while (!(IFG2&UCA0TXIFG));
  desired_temp = UCA0RXBUF;      //Receives value over UART transmission
}


void main(void)
{
    {
        WDTCTL = WDTPW + WDTHOLD;       //Stop WDT
        BCSCTL1 = CALBC1_1MHZ;          //Set range
        BCSCTL2 &= ~(DIVS_3);           //SMCLK 1MHz
        P1SEL |= BIT3;                  //ADC input pin P1.3
        P1DIR |= BIT6;                  //Set p1.6 Direction Register to output
        P1SEL |= BIT6;                  //Enable pwm for p1.6
        P2SEL |= BIT1;
        P2DIR |= BIT1;

        ADC_Setup();                 //ADC setup call
        Timer_Setup();               //Timer setup call
        UART();                      //UART setup call

        __enable_interrupt();        // Enable interrupts.

        while(1)
        {
            __delay_cycles(1000);               //Wait
            ADC10CTL0 |= ENC + ADC10SC;         //Sample start ADC
            __bis_SR_register(CPUOFF + GIE);    // Low Power Mode 0

            value = ADC10MEM;   //Assigns value in ADC10MEM to int value
            PTAT_value = 3.3 * value / 1024; //Voltage Reading from the PTAT
            temperature = (( PTAT_value - 0.75) / 0.01) + 22; //Temperature conversion
            error=temperature-desired_temp;

            if(error>3)
                TA1CCR1 = 1025;
            else if(error>1)
                TA1CCR1 = 800;
            else if(error == 0)
                TA1CCR1 = 600;
            else if(error < 0 )
                TA1CCR1 = 200;

        }
    }
}


