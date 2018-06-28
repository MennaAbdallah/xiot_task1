/*
 * task1.c
 *
 *  Created on: Jun 28, 2018
 *      Author: user
 */

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
	uint32 temp;
	int i;
	g_tick++;
	if(g_tick == NUMBER_OF_OVERFLOWS_PER_THREE_SECONDS)
	{
		ADC_readChannel(2);
		temp = ((uint32)g_adcResult*150*5)/(1023*1.5);
		//for(i=0; i<4; i++)
			//UART_sendByte((temp>>(i*8)) & 0xff);
		UART_sendByte(temp);
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
	timer0_init_normal_mode(); //start the timer.

	int i;
	uint8 Str[20];
	uint8 data ;
	UART_init();



	while(1)
	{
/*
		uint32 temp;
		//uint8 temp =  ADC_readChannel(2);
		//data = UART_recieveByte(); //Receive Byte from Temperature Sensor
		//UART_sendString(ADC_readChannel(2));
		ADC_readChannel(2);
		temp = ((uint32)g_adcResult*150*5)/(1023*1.5);
		for(i=0; i<4; i++)
			UART_sendByte((temp>>(i*8)) & 0xff);
		UART_sendByte(temp);
		//UART_sendByte(temp); //Resend the received byte to Terminal2
		//UART_receiveString("M"); //Receive String from Temperature Sensor
		//Str = itoa((g_adcResult*150*5)/(1023*1.5));
		UART_sendString(Str); //Resend the string to Terminal2

		_delay_ms(3000);
*/
		}
}

