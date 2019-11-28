#include <msp430.h> 

volatile int temp_conversion = 0;
volatile int desired_temp = 0;
volatile int actual_temp = 0;
volatile int change_temp_higher = 0;
volatile int change_temp_lower = 0;
unsigned int ADC_value = 0;
volatile int want = 0;

void fan_kick(void)
{
    TA0CCR1 = 255;                  //100% current duty cycle
    __delay_cycles(1000000);        //delay
}

void temperature_control(void)
{
    UCA0TXBUF = actual_temp;                //send user current temperature

    while(change_temp_lower == 1)           //if user wants to change the temperature lower
    {
        if (desired_temp <= 38)             //check desired temperature
        {
            TA0CCR1 = 255;      //100% duty cycle with 90% and 80% lowest temp possible
        }
        else if (desired_temp == 39)     //check desired temperature
            {

                TA0CCR1 = 178;      //70% duty cycle
            }
        else if (desired_temp == 40)     //check desired temperature
                    {

                        TA0CCR1 = 153;      //60% duty cycle
                    }
        else if (desired_temp >= 41 && desired_temp <43)        //check desired temperature
            {

                TA0CCR1 = 127;      //50% duty cycle
            }
        else if (desired_temp >= 43 && desired_temp <= 45)       //check desired temperature
            {

                TA0CCR1 = 102;      //40% duty cycle
            }
        else if (desired_temp >= 46 && desired_temp < 48)       //check desired temperature
            {

                TA0CCR1 = 76;      //30% duty cycle
            }
        else if (desired_temp >= 48 && desired_temp < 51)       //check desired temperature
            {

                TA0CCR1 = 63;      //25% duty cycle
            }
        else if (desired_temp >= 52 && desired_temp < 54)       //check desired temperature
            {

                TA0CCR1 = 51;      //20% duty cycle
            }
        else if (desired_temp >= 55 && desired_temp < 57)       //check desired temperature
            {

                TA0CCR1 = 38;      //15% duty cycle
            }
        else if (desired_temp >= 58 && desired_temp < 64)        //check desired temperature
            {

                TA0CCR1 = 26;      //10% duty cycle
            }
        else if (desired_temp > 64)
        {
                TA0CCR1 =  26;
        }

        change_temp_lower = 0;      //end loop


    }

    while(change_temp_higher == 1)      //if user wants to change the temperature higher
        {
            TA0CCR1 = 26;
        if (actual_temp >= desired_temp-2)             //check desired temperature

            if (desired_temp <= 38)             //check desired temperature
                    {
                        TA0CCR1 = 255;      //100% duty cycle with 90% and 80% lowest temp possible
                    }
                else if (desired_temp == 39)     //check desired temperature
                    {

                        TA0CCR1 = 178;      //70% duty cycle
                    }
                else if (desired_temp == 40)     //check desired temperature
                            {

                                TA0CCR1 = 153;      //60% duty cycle
                            }
                else if (desired_temp >= 41 && desired_temp < 43)        //check desired temperature
                    {

                        TA0CCR1 = 127;      //50% duty cycle
                    }
                else if (desired_temp >= 43 && desired_temp <= 45)       //check desired temperature
                    {

                        TA0CCR1 = 102;      //40% duty cycle
                    }
                else if (desired_temp >= 46 && desired_temp < 48)       //check desired temperature
                    {

                        TA0CCR1 = 76;      //30% duty cycle
                    }
                else if (desired_temp >= 48 && desired_temp < 51)       //check desired temperature
                    {

                        TA0CCR1 = 63;      //25% duty cycle
                    }
                else if (desired_temp >= 52 && desired_temp < 54)       //check desired temperature
                    {

                        TA0CCR1 = 51;      //20% duty cycle
                    }
                else if (desired_temp >= 55 && desired_temp < 57)       //check desired temperature
                    {

                        TA0CCR1 = 38;      //15% duty cycle
                    }
                else if (desired_temp >= 58 && desired_temp < 64)        //check desired temperature
                    {

                        TA0CCR1 = 26;      //10% duty cycle
                    }
                else if (desired_temp > 64)
                {
                        TA0CCR1 =  26;
                }

                change_temp_lower = 0;      //end loop

        }
}

void UART_setup(void)
{
      P1SEL |= BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
      P1SEL2 |= BIT1 + BIT2 ;                    // P1.1 = RXD, P1.2=TXD
      UCA0CTL1 |= UCSSEL_2;                     // SMCLK
      UCA0BR0 = 104;                            // 1MHz 9600
      UCA0BR1 = 0;                              // 1MHz 9600
      UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1
      UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
      IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
}

void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCI0RX_ISR (void)
  {
    while (!(IFG2&UCA0TXIFG));        //Wait until a byte is ready, is USCI_A0 TX buffer ready?
    {
       desired_temp = UCA0RXBUF;     //change desired temperature to the temperature received over UART

       if(actual_temp >= desired_temp)      //compare desired temperature to current temperature
       {
        change_temp_higher = 1;     //change temperature higher
       }
       else if (desired_temp > actual_temp)     //compare desired temperature to current temperature
       {
        change_temp_lower = 1;      //change temperature lower
       }

       temperature_control();       //run temperature control function
       want = 1;                    //enable sending current temperature
    }
  }

void timer_setup(void)
{
    TA0CTL |= TASSEL_2 + MC_1;    //set smclk, up mode
    TA0CCTL1 |= OUTMOD_7;         //set/reset output
    TA0CCR0 = 255;                //100% duty cycle
    TA0CCR1 = 255;                //100% current duty cycle
    __delay_cycles(1000000);       // delay for fan kick to start
    TA0CCR1 = 26;                 //10% duty cycle

    TA1CTL |= TASSEL_2 + MC_1;    //set smclk, up mode
    //TA1CCR0 = 5000;
}

void ADC_setup(void)
{
    BCSCTL1 = CALBC1_1MHZ;          // Set range   DCOCTL = CALDCO_1MHZ;
    BCSCTL2 &= ~(DIVS_3);           // SMCLK = DCO = 1MHz
    P1SEL |= BIT3;                  // ADC input pin P1.3
    ADC10CTL1 = INCH_3 + ADC10DIV_3 ;         // Channel 3, ADC10CLK/3
    ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON + ADC10IE;  // Vcc & Vss as reference, Sample and hold for 64 Clock cycles, ADC on, ADC interrupt enable
    ADC10AE0 |= BIT3;                         // ADC input enable P1.3

    while(1)
    {
        __delay_cycles(1000);               // Wait for ADC Ref to settle
        ADC10CTL0 |= ENC + ADC10SC;         // Sampling and conversion start
        __bis_SR_register(CPUOFF + GIE);    // Low Power Mode 0 with interrupts enabled
        ADC_value = ADC10MEM;               // Assigns the value held in ADC10MEM to the integer called ADC_value
        temp_conversion = (ADC_value-182)/3.1;  //convert ADC value to Celsius 182 possibly
        actual_temp = temp_conversion;      //convert ADC value to Celsius

        if(actual_temp <= desired_temp+2 && actual_temp >= desired_temp-2 && want == 1) //if temperature is done changing and send current temperature is enabled
        {
            UCA0TXBUF = actual_temp;    //send current temperature
            want = 0;                   //disable send current temperature
        }
    }
}


#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
    __bic_SR_register_on_exit(CPUOFF);        // Return to active mode }
}




int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P1DIR |= BIT6;          //set P1.6 to output
    P1SEL |= BIT6;          //enable pwm for P1.6

    __enable_interrupt();   // Enable interrupts.

    UART_setup();       //run function
    timer_setup();      //run function
    ADC_setup();        //run function

}
