//*******************************  Библиотеки  ******************************//
// Класс общие типы данных.
#include "Type.h"

#include "Def.h"

#if defined(ARDUINO_PRO_MINI)
  #include <SoftwareSerial.h>
#endif
//*******************************  /Библиотеки  *****************************//

#ifndef DEBUG_PRINT
  #define DEBUG_PRINT(x) DebugSerial->print(x)
#endif

#ifndef DEBUG_PRINTF
  #define DEBUG_PRINTF(x, y) DebugSerial->print(x, y)
#endif

#ifndef DEBUG_PRINTLN
  #define DEBUG_PRINTLN(x) DebugSerial->println(x)
#endif

#ifndef DEBUG_PRINTLNF
  #define DEBUG_PRINTLNF(x, y) DebugSerial->println(x, y)
#endif

//********************  Объявление переменных, констант  ********************//
// Структура хранящая текущие значения нажатых кнопок и положения стиков джойстика от PS2.
extern ps2Struct ps2S;

// Структура хранящая предыдущие значения нажатых кнопок и положения стиков джойстика от PS2.
extern rovDataStruct rovDataS;
//********************  /Объявление переменных, констант  *******************//

//*******************  Переменные и функции для отладки  ********************//
#if defined(ARDUINO_PRO_MICRO)
  extern Serial_* DebugSerial;
  
  void SetSSerial(Serial_* HWSerial);
#endif

#if defined(ARDUINO_PRO_MINI)
  extern SoftwareSerial* DebugSerial;
  
  void SetSSerial(SoftwareSerial* HWSerial);
#endif
//*******************  /Переменные и функции для отладки  *******************//
