#ifndef ATTTINY_DEBUG_H
#define ATTTINY_DEBUG_H
#ifdef DEBUG_SOFT
#include <SoftwareSerial.h>
#endif

#if defined(DEBUG) || defined(DEBUG_SOFT)
 #ifdef DEBUG_SOFT
  #define DEBUG_DECLARE(x,y) SoftwareSerial dbgSerial(x, y);
  #define DEBUG_CONNECT(x)  dbgSerial.begin(x);
  #define DEBUG_PRINTLN(x)  dbgSerial.println(x)
  #define DEBUG_PRINT(x)  dbgSerial.print(x)
  #define DEBUG_PRINT2(x,y)  dbgSerial.print(x,y)  
 #else
  #define DEBUG_DECLARE(x,y) 
  #define DEBUG_CONNECT(x)  Serial.begin(x);ACSR &=~(1<<ACIE);ACSR |=~(1<<ACD)
  #define DEBUG_PRINTLN(x)  Serial.println(x)
  #define DEBUG_PRINT(x)  Serial.print(x)
  #define DEBUG_PRINT2(x,y)  Serial.print(x,y)
 #endif
#else
  #define DEBUG_DECLARE(x,y)  
  #define DEBUG_CONNECT(x)  
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINT2(x,y)
#endif

#endif //ATTTINY_DEBUG_H
