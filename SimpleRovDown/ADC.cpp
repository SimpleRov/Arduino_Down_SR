//*******************************  Библиотеки  ******************************//
#include "Arduino.h"

#include "ADC.h"

#include "Timer.h"
//*******************************  /Библиотеки  *****************************//

// Переменная для хранения полученных значений с ADC.
volatile uint16_t adcValue;

// Переменная для хранения шага работы с ADC.
// adcStep = 0, ADC не запускался или обработка полученных значений завершена.
// adcStep = 1, установлен новый пин на чтение.
// adcStep = 2, установлен пин на чтение.
// adcStep = 3, запущено преобразование.
// adcStep = 4, данные от ADC получены.
volatile uint8_t adcStep;

// Переменная для хранения последнего пина.
uint8_t oldAdcPin;

// Время когда пин был переключен.
uint32_t adcChangePinPreviousT = 0;

// Интервал между переключением пина и началом преобразования ADC.
uint32_t adcChangePinT = 10;

// Макрос для регистра входа мультиплексора.
#define ADC_ADMUX ((0<<REFS1) | (1<<REFS0) | (0<<ADLAR))

void AdcInit()
{
  // ADCSRA - Регистр управления АЦП A.
  // ADEN - Чтобы АЦП заработал его надо включить, то есть установить бит ADEN – 1.
  // ADSC - Запуск преобразования осуществляется установкой бита ADSC - 1. Во время инициализации мы не будем запускать АЦП, поэтому ADSC – 0.
  // ADATE - Одиночное преобразование (ADATE = 0), непрерывное (ADATE = 1).
  // ADIE - Разрешение прерываний.
  // ADPS - Предделитель.
  ADCSRA |= (1 << ADEN)|(0 << ADSC)|(0 << ADATE)|(1 << ADIE)|(1 << ADPS2)|(1 << ADPS1)|(0 << ADPS0);

  // ADMUX - Регистр выбора входа мультиплексора.
  // REFS - Выбор опорного напряжения.
  // ADLAR - Выравнивание результата преобразования.
  // MUXn - Управление мультиплексором.
  #if defined(ARDUINO_PRO_MINI)
    ADMUX |= (0 << REFS1) | (1 << REFS0) | (0 << ADLAR) | (0 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0);
  #endif
  
  #if defined(ARDUINO_PRO_MICRO)
    ADMUX |= (0 << REFS1) | (1 << REFS0) | (0 << ADLAR) | (0 << MUX4) | (0 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0);
  #endif
  
  #if defined(ARDUINO_MEGA_2560)
    ADMUX |= (0 << REFS1) | (1 << REFS0) | (0 << ADLAR) | (0 << MUX4) | (0 << MUX3) | (0 << MUX2) | (0 << MUX1) | (0 << MUX0);
  #endif
  
  // ADCSRB - Регистр управления АЦП B.
  // MUXn - Управление мультиплексором.
  #if defined(ARDUINO_PRO_MICRO) || defined(ARDUINO_MEGA_2560)
    ADCSRB |= (0 << MUX5);
  #endif
  
  // DIDR0 - Регистр 0 отключения входов мультиплексора.
  // DIDR1 - Регистр 1 отключения входов мультиплексора.
  // ADC - Запрещают использование входов ADC0-ADC1 и ADC4-ADC7, ADC8-ADC13 (DIDR1), если в них установить 1.
  //DIDR0 = (1<<ADC0D)|(1<<ADC1D);
}

/// <summary>
/// Включаем преобразование ADC.
/// </summary>
static void StartConversionAdc()
{
  // Включаем преобразование ADC.
  ADCSRA |= (1<<ADSC);

  // Запущено преобразование.
  adcStep = 3;
}

uint8_t AdcReadyToRead()
{
  return adcStep == 4;
}

/// <summary>
/// Устанавливаем выход ADC.
/// </summary>
void SetAdcPin(uint8_t adcInput)
{
  // ADC не запускался или обработка полученных значений завершена.
  if (adcStep == 0)
  {
    // Поверяем установлен ли новый порт.
    if (oldAdcPin != adcInput)
    {
      // Настраиваем регистр ADMUX (выбор входа мультиплексора).
      #if defined(ARDUINO_PRO_MINI)
        if (adcInput > 7)
        {
          ADMUX |= (1 << REFS1) | (1 << REFS0) | (1 << MUX3) | (0 << MUX2) | (0 << MUX1) | (0 << MUX0);
        }
        else 
        {
          ADMUX  = adcInput | ADC_ADMUX;  
        }
      #endif
      #if defined(ARDUINO_PRO_MICRO)
        if (adcInput > 5)
        {
          ADCSRB = (1 << MUX5);
        }
        else 
        {
          ADCSRB = (0 << MUX5);
        }

        if (adcInput < 4)
        {
          ADMUX  =  (7 - adcInput) | ADC_ADMUX;
        }
        else if (adcInput < 6)
        {
          ADMUX  =  (5 - adcInput) | ADC_ADMUX;
        }
        else if (adcInput == 6)
        {
          ADMUX  =  0 | ADC_ADMUX;
        }
        else if (adcInput < 11)
        {
          ADMUX  =  (adcInput - 5) | ADC_ADMUX;
        }
        else if (adcInput == 11)
        {
          ADMUX  =  1 | ADC_ADMUX;
        }
        else if (adcInput == 255)
        {
          ADMUX |= (1 << REFS1) | (1 << REFS0) | (0 << MUX4) | (0 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0);
        }
      #endif
      #if defined(ARDUINO_MEGA_2560)
        if (adcInput > 7)
        {
          ADCSRB = (1<< MUX5);
          adcInput = adcInput - 8;
        }
      #endif
      
      // Сохраняем время установки нового порта.
      adcChangePinPreviousT = micros();
  
      // Установлен новый пин на чтение.
      adcStep = 1;
    }
    else
    {
      // Установлен пин на чтение.
      adcStep = 2;
    }
  
    // Сохраняем новый порт в переменную oldAdcPin;
    oldAdcPin = adcInput;
  }

  // Установлен новый пин на чтение.
  if (adcStep == 1)
  {    
    // Выжидаем время, после переключения пина.
    if (adcChangePinPreviousT && GetDifferenceULong(adcChangePinPreviousT, micros()) > adcChangePinT)
    {
      // Установлен пин на чтение.
      adcStep = 2;

      // Обнуляем переменную adcChangePinPreviousT.
      adcChangePinPreviousT = 0;
    } 
  }

  // Запуск преобразования.
  if (adcStep == 2)
  {
    // Запуск преобразования.
    StartConversionAdc();
  }
}

uint16_t GetAdcValue()
{
  // Обрабатываем полученные данные.
  if (adcStep == 4)
  {
    MC_CRITICAL_SECTION_START
    
    // Переносим  значение в локальную переменную.
    uint16_t adcPrintValue = adcValue;
    
    MC_CRITICAL_SECTION_END

    // Обнуляем шаг.
    adcStep = 0;

    return adcPrintValue;
  }
  else
  {
    return NULL;
  }
}

/// <summary>
/// Прерывание по завершению преобразования.
/// </summary>
ISR(ADC_vect)
{ 
  // Получаем значение.
  adcValue = ADCW;

  // Данные от ADC получены.
  adcStep = 4;
}

