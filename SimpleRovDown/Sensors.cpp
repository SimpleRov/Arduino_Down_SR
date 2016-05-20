 //*******************************  Библиотеки  ******************************//
#include "Arduino.h"

#include "Sensors.h"

#include "Config.h"

#include "Def.h"

#include "GlobalVar.h"

#include "TWI.h"

#include <util/delay.h>
//*******************************  /Библиотеки  *****************************//

//*******************************  MS580330BA  ******************************//
// Начальный адрес MS5803_30BA.
#define MS5803_30BA_ADDRESS    0x77

#define CMD_RESET 0x1E    // ADC reset command
#define CMD_ADC_READ 0x00 // ADC read command
#define CMD_ADC_CONV 0x40 // ADC conversion command
#define CMD_ADC_D1 0x00   // ADC D1 conversion
#define CMD_ADC_D2 0x10   // ADC D2 conversion
#define CMD_ADC_256 0x00  // ADC OSR=256
#define CMD_ADC_512 0x02  // ADC OSR=512
#define CMD_ADC_1024 0x04 // ADC OSR=1024
#define CMD_ADC_2048 0x06 // ADC OSR=2048
#define CMD_ADC_4096 0x08 // ADC OSR=4096
#define CMD_PROM_RD 0xA0  // Prom read command 

uint16_t calibrationCoefficients[8]; // calibration coefficients
uint8_t crcValue;                    // crc value of the prom

static uint32_t intervalConvert = 1000;
static uint32_t beginConvert = 0; 

void Ms580330BaInit(uint8_t check) 
{ 
  // Устанавливаем размер регистра адреса в байтах.
  TwiSetToLengthAddressReg(0);

  // Устанавливаем скорость передачи.
  TwiChangeSpeed(100000UL);
  
  Ms580330BaReset();
  for (uint8_t i=0; i<8; i++)
  {
    calibrationCoefficients[i] = Ms580330BaReadCalibrationCoefficients(i);
  }
  crcValue=Ms580330BaCalculateCrc(calibrationCoefficients); // calculate the CRC
}

static void Ms580330BaReset()
{
  Ms580330BaSendCommand(CMD_RESET, 1);

  _delay_ms(3);
}

static void Ms580330BaSendCommand(uint8_t command, uint8_t waitFinish)
{
  if (waitFinish)
  {
    TwiSetBeginRegOnInit(MS5803_30BA_ADDRESS, command);
  }
  else
  {
    TwiSetBeginReg(MS5803_30BA_ADDRESS, command);
  }
}

static uint16_t Ms580330BaReadCalibrationCoefficients(uint8_t number)
{
  uint16_t ret;
  uint16_t rC=0;

  Ms580330BaSendCommand(CMD_PROM_RD+(number*2), 1);

  uint8_t buffer[3];
  TwiReadBytesOnInit(MS5803_30BA_ADDRESS, 3);
  TwiGetData(buffer, 2);

  rC=(((uint16_t)buffer[0]) << 8) | buffer[1];

  return rC;
}

static uint8_t Ms580330BaCalculateCrc(uint16_t* n_prom)
{
  uint16_t n_rem; // crc reminder
  uint16_t crc_read; // original value of the crc
  uint8_t n_bit;
  n_rem = 0x00;
  crc_read=n_prom[7]; //save read CRC
  n_prom[7]=(0xFF00 & (n_prom[7])); //CRC byte is replaced by 0
  for (int8_t cnt = 0; cnt < 16; cnt++) // operation is performed on bytes
  { 
    // choose LSB or MSB
    if (cnt%2==1) n_rem ^= (uint16_t) ((n_prom[cnt>>1]) & 0x00FF);
    else n_rem ^= (uint16_t) (n_prom[cnt>>1]>>8);
    for (n_bit = 8; n_bit > 0; n_bit--)
    {
      if (n_rem & (0x8000))
      {
        n_rem = (n_rem << 1) ^ 0x3000; 
      }
      else
      {
        n_rem = (n_rem << 1);
      }
    }
  }
  n_rem= (0x000F & (n_rem >> 12)); // final 4-bit reminder is CRC code
  n_prom[7]=crc_read; // restore the crc_read to its original place
  return (n_rem ^ 0x0);
} 

static void Ms580330BaGetRawData(uint8_t cmd, Ms580330Ba* sensor)
{
  if (sensor->twimStep == 1)
  {
    Ms580330BaSendCommand(CMD_ADC_CONV+cmd, 0);

    sensor->twimStep++;
  }

  if (sensor->twimStep == 2 && TwiGetFinishStep())
  {
    switch (cmd & 0x0f) // wait necessary conversion time
    {
    case CMD_ADC_256 : 
      intervalConvert = 900; 
      break;
    case CMD_ADC_512 : 
      intervalConvert = 3000; 
      break;
    case CMD_ADC_1024: 
      intervalConvert = 4000; 
      break;
    case CMD_ADC_2048: 
      intervalConvert = 6000;
      break;
    case CMD_ADC_4096: 
      intervalConvert = 10000; 
      break;
    }
    beginConvert = micros();

    sensor->twimStep++;
  }

  if (sensor->twimStep == 3 && TwiGetFinishStep())
  {
    uint32_t currentMillis = micros();
    if(currentMillis - beginConvert > intervalConvert)     
    {
      beginConvert = currentMillis;
      
      Ms580330BaSendCommand(CMD_ADC_READ, 0);
  
      sensor->twimStep++;
    }
  }
  
  if (sensor->twimStep == 4 && TwiGetFinishStep())
  {
    TwiReadBytes(MS5803_30BA_ADDRESS, 4);

    sensor->twimStep++;
  }
  
  if (sensor->twimStep == 5 && TwiGetFinishStep())
  {
    uint8_t buffer[3];
    TwiGetData(buffer, 3);

    if (cmd >> 4)
    {
      sensor->temperatureRaw = ((uint32_t)buffer[0] << 16 |(uint32_t)buffer[1] << 8) | buffer[2];
      sensor->valueGet = 1;
    }
    else 
    {
      sensor->pressureRaw = ((uint32_t)buffer[0] << 16 |(uint32_t)buffer[1] << 8) | buffer[2];
      sensor->valueGet = 2;
    }

    sensor->twimStep = 1;
  }
}

void Ms580330BaBegin(Ms580330Ba* sensor)
{
  sensor->twimStep = 1;
  sensor->temperatureRaw = sensor->valueGet = 0;
  sensor->pressureRaw = sensor->pressureValue = sensor->temperatureValue = 0;
}

void Ms580330BaGetData(Ms580330Ba* sensor)
{  
  if (sensor->twimStep && !sensor->valueGet)
  {
    Ms580330BaGetRawData(CMD_ADC_D2+CMD_ADC_4096, sensor);
  }
  
  if (sensor->twimStep && sensor->valueGet == 1)
  {
    Ms580330BaGetRawData(CMD_ADC_D1+CMD_ADC_4096, sensor);
  }
  
  if (sensor->twimStep && sensor->valueGet == 2)
  {
    double dT=sensor->temperatureRaw-calibrationCoefficients[5]*pow(2,8);
    double OFF=calibrationCoefficients[2]*pow(2,17)+dT*calibrationCoefficients[4]/pow(2,6);
    double SENS=calibrationCoefficients[1]*pow(2,16)+dT*calibrationCoefficients[3]/pow(2,7);
    
    sensor->temperatureValue = (2000+(dT*calibrationCoefficients[6])/pow(2,23))/100;
    sensor->pressureValue = (((sensor->pressureRaw*SENS)/pow(2,21)-OFF)/pow(2,15))/100;
    
    sensor->valueGet = 3;
  }
}
//******************************  /MS580330BA  ******************************//
