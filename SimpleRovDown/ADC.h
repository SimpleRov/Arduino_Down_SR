#ifndef ADC_H_
#define ADC_H_

//*******************************  Библиотеки  ******************************//
#include "Def.h"
//*******************************  /Библиотеки  *****************************//

#if defined(ARDUINO_PRO_MINI)
    #define AVR_ADC_TEMPERATURE_SENSOR    8 
#endif

#if defined(ARDUINO_PRO_MICRO)
    #define AVR_ADC_TEMPERATURE_SENSOR    255
#endif

void AdcInit();

uint8_t AdcReadyToRead();

void SetAdcPin(uint8_t adcInput);

uint16_t GetAdcValue();

#endif /* ADC_H_ */


