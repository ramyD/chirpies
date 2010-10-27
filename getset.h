//////////////////////////////////////////////////////
//
// GET/SET ABSTRACTION LAYER FOR ATTINY13
//
//////////////////////////////////////////////////////
//
// USAGE
//         get(gPin) to get input state from pin
//             The gPin variable can be anything from 
//             PB0 to PB4 to get digital input and 
//             ADC1 to ADC3 to get analog input.
//             The return value can be HIGH (1) or
//             LOW (0) for digital and 0..255 for analog.
//
//         set(sPin, value) to set state of pin
//             The sPin variable can be anything from
//             PB0 to PB4 to set the output (0-5V) of
//             the output pin.
//             The value variable can be HIGH (1) or
//             LOW (0).
//
//
// (CC) Vincent Leclerc 10/2006
//
//////////////////////////////////////////////////////

#include <avr/interrupt.h>

#define PB0  0
#define PB1  1
#define PB2  2
#define PB3  3
#define PB4  4

#define ADC1 11
#define ADC2 12
#define ADC3 13

#define HIGH 1
#define LOW  0

void set(uint8_t sPin, uint8_t value){
	if (value) PORTB |= 1 << sPin; // set pin high
	else PORTB &= ~(1 << sPin); // set pin low
	DDRB |= 1 << sPin; // set DDRB pin to output
}

uint8_t get(uint8_t gPin){
	if (gPin > 9) { // analog input
		ADCSRA |= 0x88;	// enable ADC (turn on ADC power) & enable ADC interrupts
		ADMUX |= 0x20; // set to left-adjusted result
		sei(); // turn on interrupts (if not already on)	
		ADMUX = (ADMUX & 0xFC) | (gPin-10); // set channel
		ADCSRA |= 0x50;	// clear hardware "conversion complete" flag & start conversion
		while(ADCSRA & (1 << ADSC));	// wait until conversion complete
		ADCSRA &= 0x77; // disable ADC interrupts
		return (ADCH);	// read ADC (8 M.S.B.);
	
	} else { // digital input
		DDRB  &= ~(1 << gPin); // set DDRB pin to input
		PORTB |= 1 << gPin; // pullup
		return PINB & (1 << gPin); // return high or low
	}
}

SIGNAL(SIG_ADC)
{
	// need this for the ADC to work properly	
}
