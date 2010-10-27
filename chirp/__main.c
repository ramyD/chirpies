// ATtiny13
// ADC Module
#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>

int ReadADC(void) {
	ADMUX |= (0 << REFS0); //VCC as Reference
	ADMUX = 2;//ADC2 PB.4
	ADCSRA |= (1 << ADSC); //Start Converstion

	while((ADCSRA & 0x40) !=0){}; //wait for conv complete

	return ADC;
} 

int val=0;
int main (void) {
	DDRB |= (1 << 1);
	DDRB &= ~(1 << 4);
	PORTB &= ~(1 << 4);

	ADMUX |= (0 << REFS0); //VCC as Reference
	ADMUX |= (1 << MUX1) | (0 << MUX0); //ADC2 PB.4
	ADCSRA |= (1 << ADEN); //Enable ADC

while (1){
	val = ReadADC();
	if (val > 625) {
		PORTB &= ~(1 << 1);
	} else {
		PORTB |= (1 << 1);
	}
	return -1;
}

}

