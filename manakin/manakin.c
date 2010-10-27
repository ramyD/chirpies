/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    * 
    * Ramy Daghstani
    * & 
    * Alexis Morin
    * Copyright 2009

*/

#include <avr/io.h>
#include <avr/signal.h>
#include <util/delay.h>
#include "../getset.h"

/* the MANAKIN reacts to light.
 * when it chirps, the tone of the chirp is directly related to the
 * light it receives. If it get less light, the chirp's pitch gets
 * higher. It if gets less light, the pich gets lower.
 */

// TONES  ==========================================
// Start by defining the relationship between 
//       note, period, &  frequency. 
#define  lb    4065    // 246 Hz
#define  c     3830    // 261 Hz
#define  d     3400    // 294 Hz
#define  ds    3215    // 311 Hz
#define  e     3038    // 329 Hz
#define  f     2864    // 349 Hz
#define  fs    2702    // 370 Hz
#define  g     2550    // 392 Hz
#define  a     2272    // 440 Hz
#define  as    2145    // 446 Hz
#define  b     2028    // 493 Hz
#define  C     1912    // 523 Hz
#define  CS    1805    // 554 Hz
#define  D     1702    // 587 Hz
#define  DS    1608    // 622 Hz
#define  E     1516    // 659 Hz
#define  F     1432    // 698 Hz
#define  G     1277    // 783 Hz
#define  A     3    // 880 Hz
// Define a special note, 'R', to represent a rest

// MELODY and TIMING  =======================================
//  melody[] is an array of notes, accompanied by beats[], 
//  which sets each note's relative length (higher #, longer note) 
uint16_t melody[] = {A, A, A, A, A, A, A, A, A};
uint8_t beats[]  = {20,  15,  25, 22, 7, 7, 86, 2, 45};

uint16_t max_count = sizeof(melody); // Melody length, for looping.

// Set overall tempo
//uint16_t tempo = 1000;
uint16_t tempo = 315;

// Initialize core variables
uint32_t tone = 0;
uint8_t beat = 0;
uint32_t duration  = 0;

uint16_t i;

// PLAY TONE  ==============================================
// Pulse the speaker to play a tone for a particular duration
void playTone(void) {
  uint32_t elapsed_time = 0;
  if (tone > 0) { // if this isn't a Rest beat, while the tone has 
    //  played less long than 'duration', pulse speaker HIGH and LOW
    while (elapsed_time < duration) {
      
      set(PB3, 1); //pull PB3 high
      for (i=0; i<tone; i++) asm volatile("nop\n\t"::); //pause
      set(PB3, 0); // pull PB3 low
      for (i=0; i<tone; i++) asm volatile("nop\n\t"::); //pause

      // Keep track of how long we pulsed
      elapsed_time += (tone);
    } 
  }
}

uint32_t map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main(void){
  //initialize GPIO pins
  set(PB3, 0);//set pb3 low
  
  //declare and initialize variables
  int32_t pause = 200000;
  //change this number to change the frequency
  //main loop
  while(1){
    
    for (int i=0; i<max_count; i++) {
    tone = melody[i] * (get(ADC1)/5);
    beat = beats[i];
    
    duration = beat * tempo; // Set up timing
    
    playTone();
    
    // A pause between chirps
    }
    
    set(PB1, LOW);
    for(uint32_t j = 0; j < pause; j++) asm volatile("nop\n\t"::); //pause
    set(PB1, HIGH);
  }
}
