//*******************************  Библиотеки  ******************************//
#include "Arduino.h"

// Класс глобальные переменные.
#include "GlobalVar.h"

#include "Def.h"

#if defined(ARDUINO_PRO_MINI)
  #include <SoftwareSerial.h>
#endif
//*******************************  /Библиотеки  *****************************//

//********************  Объявление переменных, констант  ********************//
// Структура хранящая текущие значения нажатых кнопок и положения стиков джойстика от PS2.
ps2Struct ps2S;

// Структура хранящая предыдущие значения нажатых кнопок и положения стиков джойстика от PS2.
rovDataStruct rovDataS;
//********************  /Объявление переменных, констант  *******************//

//*******************  Переменные и функции для отладки  ********************//
#if defined(ARDUINO_PRO_MICRO)
  Serial_* DebugSerial;
  
  void SetSSerial(Serial_* HWSerial)
  {
    DebugSerial = HWSerial;
  }
#endif

#if defined(ARDUINO_PRO_MINI)
  SoftwareSerial* DebugSerial;
  
  void SetSSerial(SoftwareSerial* HWSerial)
  {
    DebugSerial = HWSerial;
  }
#endif
//*******************  /Переменные и функции для отладки  *******************//
