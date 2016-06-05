#ifndef UARTDEBUG_H_
#define UARTDEBUG_H_

//*******************************  Библиотеки  ******************************//
#include "Def.h"

#include "Config.h"

#include "UART.h"
//*******************************  /Библиотеки  *****************************//

#ifndef DEBUG_PRINT
  #define DEBUG_PRINT(x)
#endif

#ifndef DEBUG_PRINTF
  #define DEBUG_PRINTF(x, y)
#endif

#ifndef DEBUG_PRINTLN
  #define DEBUG_PRINTLN(x)
#endif

#ifndef DEBUG_PRINTLNF
  #define DEBUG_PRINTLNF(x, y)
#endif

#if defined DEBUGGING_THROUGH_UART && defined(ARDUINO_MEGA_2560)
  #ifndef DEBUG_PRINT
    #undef DEBUG_PRINT
  #endif

  #define DEBUG_PRINT(x)        PrintDebug(x);\
                                UARTSendData(UART_DEBUG_PORT); 

  #ifndef DEBUG_PRINTF
    #undef DEBUG_PRINTF
  #endif

  #define DEBUG_PRINTF(x, y)    PrintDebug(x, y);\
                                UARTSendData(UART_DEBUG_PORT);

  #ifndef DEBUG_PRINTLN
    #undef DEBUG_PRINTLN
  #endif

  #define DEBUG_PRINTLN(x)      PrintDebug(x);\
                                PrintDebug('\n');\
                                UARTSendData(UART_DEBUG_PORT);

  #ifndef DEBUG_PRINTLNF
    #undef DEBUG_PRINTLNF
  #endif

  #define DEBUG_PRINTLNF(x, y)  PrintDebug(x, y);\
                                PrintDebug('\n');\
                                UARTSendData(UART_DEBUG_PORT);

  void WriteDebug(uint8_t c);

  void WriteDebug(const uint8_t *buffer, size_t size);
  
  inline void WriteDebug(unsigned long n);
  
  inline void WriteDebug(long n);
  
  inline void WriteDebug(unsigned int n);
  
  inline void WriteDebug(int n);
  
  void WriteDebug(const char *buffer, size_t size);
  
  void WriteDebug(const char *str);
  
  void PrintDebugNumber(unsigned long n, uint8_t base);
  
  void PrintDebugFloat(double number, uint8_t digits);
  
  void PrintDebug(const char str[]);
  
  void PrintDebug(char c);
  
  void PrintDebug(unsigned char b, uint8_t base);
  
  void PrintDebug(int n, uint8_t base);
  
  void PrintDebug(unsigned int n, uint8_t base);
  
  void PrintDebug(long n, uint8_t base);
  
  void PrintDebug(unsigned long n, uint8_t base);
  
  void PrintDebug(double n, uint8_t digits);
  
  void PrintDebug(const String &s);
  
  void PrintDebug(unsigned char n);
  
  void PrintDebug(int n);
  
  void PrintDebug(unsigned int n);
  
  void PrintDebug(long n);
  
  void PrintDebug(unsigned long n);
  
  void PrintDebug(double n);
#endif

#endif /* UARTDEBUG_H_ */
