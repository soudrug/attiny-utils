#ifndef ATTTINY_UTILS_H
#define ATTTINY_UTILS_H

#include <avr/wdt.h>
#include <avr/sleep.h>

//Disabling ADC saves ~230uAF. Needs to be re-enable for the internal voltage check
#if !defined(adc_disable)
# define adc_disable() (ADCSRA &= ~(1<<ADEN)) // disable ADC
#endif
#if !defined(adc_enable)
# define adc_enable()  (ADCSRA |=  (1<<ADEN)) // re-enable ADC
#endif

int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}


int readVcc(bool leaveAdcOn = false) {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference

  //Re-enable ADC 
  adc_enable();

#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
  ADMUX = _BV(MUX5) | _BV(MUX0);
#elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
  ADMUX = _BV(MUX3) | _BV(MUX2);
#else
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#endif 

  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring

  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH 
  uint8_t high = ADCH; // unlocks both

  long result = (high<<8) | low;

  result = 1126400L / result; // Calculate Vcc (in mV);
  //result = 1074835L / result;
  
  //Disable ADC
  if(!leaveAdcOn) {
    adc_disable();
  }

  return (int)result; // Vcc in millivolts
}

//-----------------------------------------------------------------------------
//Sleep code from Narcoleptic library -- https://code.google.com/p/narcoleptic
//Tweaked for Attiny85 and used straight for small code size
//-----------------------------------------------------------------------------

SIGNAL(WDT_vect) {
  wdt_disable();
  wdt_reset();
#if defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny25__)
  WDTCR &= ~_BV(WDIE);
#else
  WDTCSR &= ~_BV(WDIE);
#endif
}


void powerdown(int wdt_period, uint8_t sleepMode) {
  wdt_enable(wdt_period);
  wdt_reset();

#if defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny25__)
  WDTCR |= _BV(WDIE);
#else
  WDTCSR |= _BV(WDIE);
#endif

  set_sleep_mode(sleepMode);

  // Set sleep enable (SE) bit:
  sleep_enable();
  // Disable BOD during sleep
  sleep_bod_disable();
  //Going to sleep
  sleep_mode();

  //Woke up
  sleep_disable();
  
  wdt_disable();

#if defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny25__)
  WDTCR &= ~_BV(WDIE);
#else
  WDTCSR &= ~_BV(WDIE);
#endif
}

void powerdownDelay(unsigned long milliseconds, uint8_t sleepMode = SLEEP_MODE_PWR_DOWN) {
  while (milliseconds >= 8000) { powerdown(WDTO_8S, sleepMode); milliseconds -= 8000; }
  if (milliseconds >= 4000)    { powerdown(WDTO_4S, sleepMode); milliseconds -= 4000; }
  if (milliseconds >= 2000)    { powerdown(WDTO_2S, sleepMode); milliseconds -= 2000; }
  if (milliseconds >= 1000)    { powerdown(WDTO_1S, sleepMode); milliseconds -= 1000; }
  if (milliseconds >= 500)     { powerdown(WDTO_500MS, sleepMode); milliseconds -= 500; }
  if (milliseconds >= 250)     { powerdown(WDTO_250MS, sleepMode); milliseconds -= 250; }
  if (milliseconds >= 125)     { powerdown(WDTO_120MS, sleepMode); milliseconds -= 120; }
  if (milliseconds >= 64)      { powerdown(WDTO_60MS, sleepMode); milliseconds -= 60; }
  if (milliseconds >= 32)      { powerdown(WDTO_30MS, sleepMode); milliseconds -= 30; }
  if (milliseconds >= 16)      { powerdown(WDTO_15MS, sleepMode); milliseconds -= 15; }
}

#endif //ATTTINY_UTILS_H
