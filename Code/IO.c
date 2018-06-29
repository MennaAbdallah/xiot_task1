#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc.h"
#include <util/delay.h>
#include "uart.h"

#define NUMBER_OF_OVERFLOWS_PER_THREE_SECONDS 12

unsigned char g_tick = 0;

/* External INT0 Interrupt Service Routine */
ISR(INT0_vect)
{
	PORTA = PORTA ^ (1<<PA0); //toggle led state

	UART_sendString("Pressed ");

	if ( (PINA &0b00000001) == 0 )
		UART_sendString("ON ");
	else
		UART_sendString("OFF ");
}

/* External INT0 enable and configuration function */
void INT0_Init(void)
{
	SREG  &= ~(1<<7);                   // Disable interrupts by clearing I-bit
	DDRD  &= (~(1<<PD2));               // Configure INT0/PD2 as input pin
	GICR  |= (1<<INT0);                 // Enable external interrupt pin INT0
	MCUCR |= (1<<ISC00) | (1<<ISC01);   // Trigger INT0 with the raising edge
	SREG  |= (1<<7);                    // Enable interrupts by setting I-bit
}

ISR(TIMER0_OVF_vect)
{
	uint16 temp; //better to be uint32 but the simulation doesn't work .. why!
	uint8 Temp[20];
	g_tick++;
	if(g_tick == NUMBER_OF_OVERFLOWS_PER_THREE_SECONDS)
	{
		temp = ADC_readChannel(2);
		temp = (temp*150*5)/(1023*1.5);
		itoa (temp,Temp,10);
		UART_sendString(Temp );
		g_tick = 0; //clear the tick counter again to count a new 3 seconds
	}
}

/* Description :
 * For clock=1Mhz and prescale F_CPU/1024 every count will take 1ms
 * so put initial timer counter=0  0 --> 255 (256ms per overflow)
 * so we need timer to overflow 2 times to get a 0.5 second
 */


void timer0_init_normal_mode(void)
{
	TCNT0 = 0; //timer initial value
	TIMSK |= (1<<TOIE0); //enable overflow interrupt
	TCCR0 = (1<<FOC0) | (1<<CS02) | (1<<CS00);
}

int main(void)
{
	INT0_Init();              // Enable external INT0

	DDRA  = DDRA | (1<<PA0);  // Configure pin PA0 in PORTA as output pin
	PORTA = PORTA | (1<<PA0); // Initialization Led is off at the beginning(Negative Logic)

	SREG |= (1<<7);/* Enable Global Interrupts */

	ADC_init();

	timer0_init_normal_mode(); //start the timer.


	UART_init();

	while(1)
	{
	}

return 0;
}
