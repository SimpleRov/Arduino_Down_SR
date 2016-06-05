//*******************************  Библиотеки  ******************************//
#include "Arduino.h"

#include "UARTDebug.h"
//*******************************  /Библиотеки  *****************************//

#if defined DEBUGGING_THROUGH_UART && defined(ARDUINO_MEGA_2560)
  void WriteDebug(uint8_t c)
  {
    while (UARTUsedTXBuff(UART_DEBUG_PORT)>(TXBufferSize-2));
    UARTSerialize(UART_DEBUG_PORT,c);
  }
  
  void WriteDebug(const uint8_t* buffer, size_t size)
  {
    while (size--) 
    {
      WriteDebug(*buffer++);
    }
  }
  
  inline void WriteDebug(unsigned long n) 
  { 
    WriteDebug((uint8_t)n); 
  }
  
  inline void WriteDebug(long n) 
  { 
    WriteDebug((uint8_t)n); 
  }
  
  inline void WriteDebug(unsigned int n) 
  { 
    WriteDebug((uint8_t)n); 
  }
  
  inline void WriteDebug(int n) 
  { 
    WriteDebug((uint8_t)n); 
  }
  
  void WriteDebug(const char *buffer, size_t size) 
  {
    WriteDebug((const uint8_t *)buffer, size);
  }
  
  void WriteDebug(const char *str) 
  {
        if (str != NULL) 
        {
          WriteDebug((const uint8_t *)str, strlen(str));
        }
  }
  
  void PrintDebugNumber(unsigned long n, uint8_t base) 
  {
    char buf[8 * sizeof(long) + 1]; 
    char *str = &buf[sizeof(buf) - 1];
  
    *str = '\0';
  
    if (base < 2) 
    {
      base = 10;
    }
  
    do 
    {
      unsigned long m = n;
      n /= base;
      char c = m - base * n;
      *--str = c < 10 ? c + '0' : c + 'A' - 10;
    } 
    while(n);
  
    WriteDebug(str);
  }
  
  void PrintDebugFloat(double number, uint8_t digits) 
  { 
    if (isnan(number)) 
    {
      PrintDebug("nan");
      return;
    }
    if (isinf(number)) 
    {
      PrintDebug("inf");
      return;
    }
    if (number > 4294967040.0) 
    {
      PrintDebug ("ovf"); 
      return;
    } 
    if (number <-4294967040.0) 
    {
      PrintDebug ("ovf");  
      return;
    }
    
    if (number < 0.0)
    {
       PrintDebug('-');
       number = -number;
    }
  
    double rounding = 0.5;
    for (uint8_t i=0; i<digits; ++i)
    {
      rounding /= 10.0;
    }
    
    number += rounding;
  
    unsigned long int_part = (unsigned long)number;
    double remainder = number - (double)int_part;
    PrintDebug(int_part);
  
    if (digits > 0) 
    {
      PrintDebug("."); 
    }
  
    while (digits-- > 0)
    {
      remainder *= 10.0;
      int toPrint = int(remainder);
      PrintDebug(toPrint);
      remainder -= toPrint; 
    } 
  }
  
  void PrintDebug(const char str[])
  {
    WriteDebug(str);
  }
  
  void PrintDebug(char c)
  {
    WriteDebug(c);
  }
  
  void PrintDebug(unsigned char b, uint8_t base)
  {
    PrintDebug((unsigned long) b, base);
  }
  
  void PrintDebug(int n, uint8_t base)
  {
    PrintDebug((long) n, base);
  }
  
  void PrintDebug(unsigned int n, uint8_t base)
  {
    PrintDebug((unsigned long) n, base);
  }
  
  void PrintDebug(long n, uint8_t base)
  {
    if (base == 0) 
    {
      WriteDebug(n);
    } 
    else if (base == 10) 
    {
      if (n < 0) 
      {
        PrintDebug('-');
        n = -n;
        PrintDebugNumber(n, 10);
      }
      PrintDebugNumber(n, 10);
    } 
    else 
    {
      PrintDebugNumber(n, base);
    }
  }
  
  void PrintDebug(unsigned long n, uint8_t base)
  {
    if (base == 0) 
    {
      WriteDebug(n);
    }
    else 
    {
      PrintDebugNumber(n, base);
    }
  }
  
  void PrintDebug(double n, uint8_t digits)
  {
    PrintDebugFloat(n, digits);
  }
  
  void PrintDebug(const __FlashStringHelper *ifsh)
  {
    PGM_P p = reinterpret_cast<PGM_P>(ifsh);
    while (1) 
    {
      unsigned char c = pgm_read_byte(p++);
      if (c == 0) 
      {
        break;
      }
      WriteDebug(c);
    }
  }
  
  void PrintDebug(const String &s)
  {
    for (uint16_t i = 0; i < s.length(); i++) 
    {
      WriteDebug(s[i]);
    }
  }
  
  void PrintDebug(unsigned char n)
  {
    PrintDebug(n, DEC);
  }
  
  void PrintDebug(int n)
  {
    PrintDebug(n, DEC);
  }
  
  void PrintDebug(unsigned int n)
  {
    PrintDebug(n, DEC);
  }
  
  void PrintDebug(long n)
  {
    PrintDebug(n, DEC);
  }
  
  void PrintDebug(unsigned long n)
  {
    PrintDebug(n, DEC);
  }

  void PrintDebug(double n)
  {
    PrintDebug(n, 2);
  }
#endif

