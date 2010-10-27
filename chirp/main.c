#include <avr/io.h>
#include <avr/signal.h>
#include <avr/delay.h>
#include "../getset.h"

int main(void){
  //initialize GPIO pins
  set(PB0, 0);//set pb0 low
  
  //declare and initialize variables
  uint16_t i, ain, pause;
  //change this number to change the frequency
  
  //main loop
  while(1){
    set(PB0, 1); //pull PB0 high
    ain = get(ADC1);
    ain *= 2;
    for (i=0; i<ain; i++) asm volatile("nop\n\t"::); //pause
    set(PB0, 0); // pull PB0 low
    for (i=0; i<ain; i++) asm volatile("nop\n\t"::); //pause
  }
}
