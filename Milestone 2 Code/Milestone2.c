/*
 * Written by: Kyle Limbaga, Sean Prendergast and Dawson Tocarchick
 * Assignment: Milestone 2
 * The purpose of this program is to have the MSP430G2553 capable
 * of changing the duty cycle of a fan by transmitting a PWM signal.
 * The duty cycle will change based on the value the user enters
 * for the desired temperature on Realterm as UART will be utilized.
 */

// Header file for MSP430
#include <msp430.h> 

// All of these variables are initially set to 0
volatile int temp_conversion = 0;
volatile int desired_temp = 0;
volatile int actual_temp = 0;
volatile int change_temp_higher = 0;
volatile int change_temp_lower = 0;
unsigned int ADC_value = 0;
volatile int want = 0;

// Function to turn fan on immediately when program is compiled and ran on MSP430
void fan_kick(void)
{
    // Fan will be on initially at a duty cycle of 100%, the maximum speed the fan can run at.
    TA0CCR1 = 255;                  // The value 255 is stored in TA0CCR1 to initialize 100% duty cycle for fan
    __delay_cycles(1000000);        // Delay
}

// Function to control the temperature based on desired temp. value entered via Realterm
void temperature_control(void)
{
    UCA0TXBUF = actual_temp;                // Send user current temperature on Realterm

    while(change_temp_lower == 1)           // If the user wants to make the current temperature lower
    {
        if (desired_temp <= 38)             // Check value entered by user for desired temperature from Realterm to see if its less than or equal to 38 degrees Celsius
        {
            TA0CCR1 = 255;                  // 100% duty cycle (Along with 90% and 80% to achieve lowest temp possible)
        }
        /* No else if statement for 90% and 80% duty cycles as the fan utilized
         * would cool the voltage regulator to the same temperature as a 100% duty cycle fan
         */
        else if (desired_temp == 39)     // Check value entered by user for desired temperature from Realterm to see if its equivalent to 39 degrees Celsius
            {

                TA0CCR1 = 178;              // 178 is stored in TA0CCR1 to achieve 70% duty cycle and temp of 39 degrees C
            }
        else if (desired_temp == 40)     // Check if desired temperature value is equal to 40 degrees Celsius
                    {

                        TA0CCR1 = 153;      // 153 is stored in TA0CCR1 to achieve 60% duty cycle and temp. of 40 degrees C
                    }
        else if (desired_temp >= 41 && desired_temp <43)        // Check if desired temperature value is greater than or equal to 41 degrees C and less than 43 degrees C
            {

                TA0CCR1 = 127;                      // 127 is stored in TA0CCR1 to achieve 50% duty cycle and temp. greater than or equal to 41 degrees C and less than 43 degrees C
            }
        else if (desired_temp >= 43 && desired_temp <= 45)       // Check if desired temp. value is greater than or equal to 43 C and less than or equal to 45 C
            {

                TA0CCR1 = 102;      // 102 is stored in TA0CCR1 to achieve 40% duty cycle and desired temp. value greater than or equal to 43 C and less than or equal to 45 C
            }
        else if (desired_temp >= 46 && desired_temp < 48)       // Check if desired temp. value is greater than or equal to 46 C and less than 48 C
            {

                TA0CCR1 = 76;      // 76 is stored in TA0CCR1 to achieve 30% duty cycle and temp. value greater than or equal to 46 C and less than 48 C
            }
        else if (desired_temp >= 48 && desired_temp < 51)       // Check if desired temp. value is greater than or equal to 48 C and less than 51 C
            {

                TA0CCR1 = 63;      // 63 is stored in TA0CCR1 to achieve 25% duty cycle and temp. value greater than or equal to 48 C and less than 51 C
            }
        else if (desired_temp >= 52 && desired_temp < 54)       // Check if desired temp. value is greater than or equal to 52 C and less than 54 C
            {

                TA0CCR1 = 51;      // 51 is stored in TA0CCR1 to achieve 20% duty cycle and desired temp. value inside else if statement
            }
        else if (desired_temp >= 55 && desired_temp < 57)       // Check if desired temp. value is greater than or equal to 55 C and less than 57 C
            {

                TA0CCR1 = 38;      // 38 is stored in TA0CCR1 to achieve 15% duty cycle and desired temp. value inside else if statement
            }
        else if (desired_temp >= 58 && desired_temp < 64)        // Check if desired temp. value is greater than or equal to 58 C and less than 64 C
            {

                TA0CCR1 = 26;      // 26 is stored in TA0CCR1 to achieve 10% duty cycle and desired temp. value inside else if statement
            }
        else if (desired_temp > 64) // Check if desired temp. value is greater than 64 C
        {
                TA0CCR1 =  26;     // 26 is stored in TA0CCR1 to achive temp value greater than 64 C
        }

        change_temp_lower = 0;      // End while loop


    }

    while(change_temp_higher == 1)      // If the user wants to make the current temp. higher
        {
            TA0CCR1 = 26;               // Fan is initially on at 10% duty cycle

        if (actual_temp >= desired_temp-2)             // Check if desired temperature is within 2 C of actual temperature of regulator

            if (desired_temp <= 38)             // Check if entered desired temperature value is less than or equal to 38 C
                    {
                        TA0CCR1 = 255;      // 100% duty cycle to achieve temp. value less than or equal to 38 C
                    }
                else if (desired_temp == 39)     // Check if entered desired temperature value is equal to 39 C
                    {

                        TA0CCR1 = 178;      // 70% duty cycle to achieve temp. of 39 C
                    }
                else if (desired_temp == 40)     // Check if entered desired temperature value is equal to 40 C
                            {

                                TA0CCR1 = 153;      // 60% duty cycle to achieve temp of 40 C
                            }
                else if (desired_temp >= 41 && desired_temp < 43)        // Check if entered desired temperature value is greater than or equal to 41 C and less than 43 C
                    {

                        TA0CCR1 = 127;      // 50% duty cycle to achieve desired temp inside else if statement
                    }
                else if (desired_temp >= 43 && desired_temp <= 45)       // Check if desired temp is greater than or equal to 43 C and less than or equal to 45 C
                    {

                        TA0CCR1 = 102;      // 40% duty cycle to achieve this desired temp
                    }
                else if (desired_temp >= 46 && desired_temp < 48)       // Check if desired temp is greater than or equal to 46 C and less than 48 C
                    {

                        TA0CCR1 = 76;      // 30% duty cycle to achieve this desired temp
                    }
                else if (desired_temp >= 48 && desired_temp < 51)       // Check if desired temp is greater than or equal to 48 C and less than 51 C
                    {

                        TA0CCR1 = 63;      // 25% duty cycle to achieve this desired temp
                    }
                else if (desired_temp >= 52 && desired_temp < 54)       // Check if desired temp is greater than or equal to 52 C and less than 54 C
                    {

                        TA0CCR1 = 51;      // 20% duty cycle to achieve this desired temp
                    }
                else if (desired_temp >= 55 && desired_temp < 57)       // Check if desired temp is greater than or equal to 55 C and less than 57 C
                    {

                        TA0CCR1 = 38;      // 15% duty cycle to achieve this desired temp
                    }
                else if (desired_temp >= 58 && desired_temp < 64)        // Check if desired temp is greater than or equal to 58 C and less than 64 C
                    {

                        TA0CCR1 = 26;      // 10% duty cycle to achieve this desired temp
                    }
                else if (desired_temp > 64) // Check if desired temp is greater than 64 C
                {
                        TA0CCR1 =  26;  // 10% duty cycle to achieve this desired temp
                }

                change_temp_lower = 0;      // End while loop

        }
}

// Declare function for UART (universal asynchronous receiver-transmitter)
void UART_setup(void)
{
      P1SEL |= BIT1 + BIT2 ;                    // P1.1 = RXD (Receive Data), P1.2=TXD (Transmit Data)
      P1SEL2 |= BIT1 + BIT2 ;                   // P1.1 = RXD (Receive Data), P1.2=TXD (Transmit Data)
      UCA0CTL1 |= UCSSEL_2;                     // smclk (sub-main clock) for 9600 baud rate
      UCA0BR0 = 104;                            // (1 MHz) / (9600 baud rate) = 104
      UCA0BR1 = 0;                              // 1 MHz 9600 baud rate
      UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1
      UCA0CTL1 &= ~UCSWRST;                     // USCI State Machine is initialized
      IE2 |= UCA0RXIE;                          // RX interrupt is enabled
}

void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCI0RX_ISR (void)
  {
    while (!(IFG2&UCA0TXIFG));        // Wait until a byte is ready, is USCI_A0 TX buffer ready?
    {
       desired_temp = UCA0RXBUF;     // Change desired temperature value to temp value received over UART

       if(actual_temp >= desired_temp)      // Compare current temperature value and desired temperature value to see if current temp is greater than or equal to desired temp
       {
        change_temp_higher = 1;     // If so, change temperature higher
       }
       else if (desired_temp > actual_temp)     // Compare current temperature value and desired temperature value to see if desired temp value is greater than current temp. value
       {
        change_temp_lower = 1;      // If so, change temperature lower
       }

       temperature_control();       // Run function to control temperature
       want = 1;                    // Enable transmitting current temperature value
    }
  }

// Function for setting up Timers being utilized
void timer_setup(void)
{
    TA0CTL |= TASSEL_2 + MC_1;      // TASSEL_2 selects smclk (sub-main clock) is set at 1 MHz
                                    // MC_1 selects up mode
    TA0CCTL1 |= OUTMOD_7;           // Enables reset/set mode
                                    // The output is reset when the timer counts to the TACCRx value
                                    // It is set when the timer counts to the TACCR0 value
    TA0CCR0 = 255;                  // Set PWM Period in the Timer A0 Capture/Compare 0 Register to 255 to achieve 100% duty cycle
    TA0CCR1 = 255;                  // Current duty cycle is 100%
    __delay_cycles(1000000);        // Delay cycle for fan kick to start
    TA0CCR1 = 26;                   // 10% duty cycle

    TA1CTL |= TASSEL_2 + MC_1;     // TASSEL_2 selects smclk (sub-main clock) is set at 1 MHz
                                   // MC_1 selects up mode
}

// Function for setting up ADC (Analog to Digital) features of MSP430
void ADC_setup(void)
{
    BCSCTL1 = CALBC1_1MHZ;                      // Set range   DCOCTL = CALDCO_1MHZ;
    BCSCTL2 &= ~(DIVS_3);                       // SMCLK = DCO = 1MHz
    P1SEL |= BIT3;                              // P1.3 is the input pin for ADC functionality on MSP430 so set P1.3 to input direction
    ADC10CTL1 = INCH_3 + ADC10DIV_3 ;           // Channel 3, ADC10CLK/3
    ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON + ADC10IE;  // Vcc & Vss as reference, Sample and hold for 64 Clock cycles, ADC on, ADC interrupt enable
    ADC10AE0 |= BIT3;                           // Enable ADC input for P1.3

    while(1)
    {
        __delay_cycles(1000);               // Wait for ADC Ref to settle
        ADC10CTL0 |= ENC + ADC10SC;         // Sampling and conversion start for ADC
        __bis_SR_register(CPUOFF + GIE);    // Low Power Mode 0 with interrupts enabled
        ADC_value = ADC10MEM;               // ADC_value is created to assign value stored in ADC101MEM to an integer ADC_value
        temp_conversion = (ADC_value-155)/3.1;  // Conversion factor to achieve temp of regulator using value stored in ADC_VALUE, digital offset value of 155 and constant value of 3.1
        actual_temp = temp_conversion;      // ADC value is converted to temp value in Celsius

        // If the current temp is within 2 C of desired temp, current temp value is sent and displayed on Realterm
        if(actual_temp <= desired_temp+2 && actual_temp >= desired_temp-2 && want == 1) //if temperature is done changing and send current temperature is enabled
        {
            UCA0TXBUF = actual_temp;    // Send current temp value of regulator
            want = 0;                   // Send current temperature is turned off as current temp value has already been sent to Realterm
        }
    }
}


#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
    __bic_SR_register_on_exit(CPUOFF);        // Return to active mode
}

// Function for main code block
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P1DIR |= BIT6;              // Set P1.6 to output direction for sending PWM signal
    P1SEL |= BIT6;              // Enable PWM for P1.6

    __enable_interrupt();       // Interrupts are enabled

    UART_setup();               // Call function for UART setup
    timer_setup();              // Call function for Timer setup
    ADC_setup();                // Call function for ADC setup

}
