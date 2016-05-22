#ifndef SENSORS_H_
#define SENSORS_H_

//*******************************  Библиотеки  ******************************//
#include "Type.h"
//*******************************  /Библиотеки  *****************************//

//*******************************  Все датчики  *****************************//
void InitSensors();
//*******************************  /Все датчики *****************************//

//***************************  Все датчики на ADC  **************************//
void AdcSensorsSendDataInStruct();
//***************************  /Все датчики на ADC **************************//

//***************************  Все датчики на TWI  **************************//

//*******************************  MS580330BA  ******************************//
void Ms580330BaInit(uint8_t check);

static void Ms580330BaReset();

static void Ms580330BaSendCommand(uint8_t command, uint8_t waitFinish);

static uint16_t Ms580330BaReadCalibrationCoefficients(uint8_t number);

static uint8_t Ms580330BaCalculateCrc(uint16_t* n_prom);

void Ms580330BaBegin(Ms580330Ba* sensor);

void Ms580330BaGetData(Ms580330Ba* sensor);

static void Ms580330BaGetRawData(uint8_t cmd, Ms580330Ba* sensor);

void Ms580330BaSendDataInStruct();
//*******************************  /MS580330BA ******************************//

//***************************  /Все датчики на TWI **************************//

#endif //SENSORS_H_
