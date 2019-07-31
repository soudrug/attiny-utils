#ifndef ATTTINY_DEBUG_H
#define ATTTINY_DEBUG_H

#define DEBUG

#ifdef DEBUG
  #define DEBUG_DECLARE(x,y) 
  #define DEBUG_CONNECT(x)  Serial.begin(x);ACSR &=~(1<<ACIE);ACSR |=~(1<<ACD)
  #define DEBUG_PRINTLN(x)  Serial.println(x)
  #define DEBUG_PRINT(x)  Serial.print(x)
#else
  #define DEBUG_DECLARE(x,y)  
  #define DEBUG_CONNECT(x)  
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINT(x)
#endif

#endif //ATTTINY_DEBUG_H
