/*
 * adc.c
 *
 *  Created on: Jun 28, 2018
 *      Author: user
 */


#include "adc.h"


volatile uint16 g_adcResult = 0;


ISR(ADC_vect)
{
	/* Read ADC Data after conversion complete */
	g_adcResult = ADC;
}

void ADC_init(void)
{

	ADMUX = 0;

	ADCSRA = (1<<ADEN) | (1<<ADIE) | (1<<ADPS1) | (1<<ADPS0);
}

void ADC_readChannel(uint8 channel_num)
{
	channel_num &= 0x07; /* channel number must be from 0 --> 7 */
	ADMUX &= 0xE0; /* clear first 5 bits in the ADMUX (channel number MUX4:0 bits) before set the required channel */
	ADMUX = ADMUX | channel_num; /* choose the correct channel by setting the channel number in MUX4:0 bits */
	SET_BIT(ADCSRA,ADSC); /* start conversion write '1' to ADSC */
}

