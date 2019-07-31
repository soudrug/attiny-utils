#ifndef ATTTINY_DEBUG_H
#define ATTTINY_DEBUG_H

#define DEBUG

#ifdef DEBUG
  #include <SoftwareSerial.h>
  #define DEBUG_DECLARE(x,y) SoftwareSerial dbgSerial(x, y); 
  #define DEBUG_CONNECT(x)  dbgSerial.begin(x)
  #define DEBUG_PRINTLN(x)  dbgSerial.println(x)
  #define DEBUG_PRINT(x)  dbgSerial.print(x)
  #define DEBUG_FLUSH()     dbgSerial.flush()
#else
  #define DEBUG_DECLARE(x,y)  
  #define DEBUG_CONNECT(x)  
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINT(x)
  #define DEBUG_FLUSH()
#endif

#endif //ATTTINY_DEBUG_H
