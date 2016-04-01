/******************************************************************************
     * Based on the file Protocol.cpp project Multiwii. 
     * Author project Multiwii - Alexandre Dubus.
     * Version on commit e957a70 on 24 Feb 2016.
     * Project description Multiwii - http://www.multiwii.com. 
     * You can find the code of the Multiwii project here - https://github.com/multiwii/multiwii-firmware.
     * License GNU GPL v3
******************************************************************************/

/******************************************************************************
 * TODO: Продумать работу с ошибками.
 * TODO: Подумать какие глобальные переменные можно перенести в локальные.
******************************************************************************/

//*******************************  Библиотеки  ******************************//
#include "Arduino.h"

#include "Protocol.h"

#include "Config.h"

#include "Def.h"

#include "Type.h"

#include "GlobalVar.h"

#include "UART.h"
//*******************************  /Библиотеки  *****************************//
static uint8_t CurrentUARTPortSend = UART_PORT;
static uint8_t CurrentUARTPortRead = 0;

#define InBufferSize 64

static uint8_t InBuffer[InBufferSize][UARTNumber];
static uint8_t IndexBuffer[UARTNumber];

static uint8_t CmdDataSize[UARTNumber];
static uint8_t CmdCheckSum[UARTNumber];
static uint8_t Cmd[UARTNumber];

void InitUart(void)
{  
  // Инициализируем и открываем UART порт связи с Rov
  UARTOpen(UART_PORT, UART_PORT_SPEED);
}

static void Serialize8(uint8_t a) 
{
  UARTSerialize(CurrentUARTPortSend,a);
  CmdCheckSum[CurrentUARTPortSend] ^= a;
}

static void Serialize16(int16_t a) 
{
  Serialize8((a   ) & 0xFF);
  Serialize8((a>>8) & 0xFF);
}

static void Serialize32(uint32_t a) 
{
  Serialize8((a    ) & 0xFF);
  Serialize8((a>> 8) & 0xFF);
  Serialize8((a>>16) & 0xFF);
  Serialize8((a>>24) & 0xFF);
}

static void SerializeStruct(uint8_t *cb,uint8_t siz) 
{
  while(siz--) 
  {
    Serialize8(*cb++);
  }
}

static void HeadCmdSend(uint8_t s, uint8_t c) 
{
  Serialize8('?');
  Serialize8('C');
  
  CmdCheckSum[CurrentUARTPortSend] = 0;
  
  Serialize8(s);
  Serialize8(c);
}

static void TailCmdSend(void) 
{
  Serialize8(CmdCheckSum[CurrentUARTPortSend]);
  UARTSendData(CurrentUARTPortSend);
}

static uint8_t Read8(void)  
{
  return InBuffer[IndexBuffer[CurrentUARTPortRead]++][CurrentUARTPortRead]&0xff;
}

static uint16_t Read16(void) 
{
  uint16_t t = Read8();
  t+= (uint16_t)Read8()<<8;
  return t;
}

static uint32_t Read32(void) 
{
  uint32_t t = Read16();
  t+= (uint32_t)Read16()<<16;
  return t;
}

static void ReadStruct(uint8_t *cb,uint8_t siz) 
{
  while(siz--) 
  {
    *cb++ = Read8();
  }
}

static void EvaluateCommand(void) 
{   
  switch(Cmd[CurrentUARTPortRead])
  {
    case 1:
      ReadStruct((uint8_t*)&ps2S,12);
      break;
    case 2:
      ReadStruct((uint8_t*)&rovDataS, 1);
      break;
  }    
}

void SendStruct(uint8_t *st, uint8_t siz, uint8_t c)
{
  HeadCmdSend(siz, c);
                        
  SerializeStruct(st, siz);

  TailCmdSend(); 
}

void CheckUart(void)
{
  uint8_t c, n;  
  static uint8_t InBufferOffset[UARTNumber];
  static enum _CmdState 
  {
    IDLE,
    HEADER_START,
    HEADER_SIZE,
    HEADER_CMD,
    HEADER_DATA
  } CmdState[UARTNumber];
  
  for(n=0; n < UARTNumber; n++) 
  {
    #if defined(ARDUINO_PRO_MICRO)
      CurrentUARTPortRead = 0;
    #else
      CurrentUARTPortRead = n;
    #endif
    
    uint8_t cc = UARTAvailable(CurrentUARTPortRead);
    while (cc--) 
    {
      c = UARTRead(CurrentUARTPortRead);
      
      if (CmdState[CurrentUARTPortRead] == IDLE) 
      {
        CmdState[CurrentUARTPortRead] = (c=='?') ? HEADER_START : IDLE;    
      }
      else if (CmdState[CurrentUARTPortRead] == HEADER_START) 
      { 
        CmdState[CurrentUARTPortRead] = (c=='C') ? HEADER_SIZE : IDLE;
      }
      else if (CmdState[CurrentUARTPortRead] == HEADER_SIZE) 
      {
        if (c > InBufferSize) 
        {  
          CmdState[CurrentUARTPortRead] = IDLE;
          continue;
        }
        CmdDataSize[CurrentUARTPortRead] = c;
        
        CmdCheckSum[CurrentUARTPortRead] = 0;
        IndexBuffer[CurrentUARTPortRead] = 0;
        InBufferOffset[CurrentUARTPortRead] = 0;
        
        CmdCheckSum[CurrentUARTPortRead] ^= c;
        CmdState[CurrentUARTPortRead] = HEADER_CMD;
      }
      else if (CmdState[CurrentUARTPortRead] == HEADER_CMD)
      {   
        Cmd[CurrentUARTPortRead] = c;        
        CmdCheckSum[CurrentUARTPortRead] ^= c;
        CmdState[CurrentUARTPortRead] = HEADER_DATA;
      }
      else if (CmdState[CurrentUARTPortRead] == HEADER_DATA && InBufferOffset[CurrentUARTPortRead] < CmdDataSize[CurrentUARTPortRead])
      {             
        CmdCheckSum[CurrentUARTPortRead] ^= c;
        InBuffer[InBufferOffset[CurrentUARTPortRead]++][CurrentUARTPortRead] = c;
      }
      else if (CmdState[CurrentUARTPortRead] == HEADER_DATA && InBufferOffset[CurrentUARTPortRead] >= CmdDataSize[CurrentUARTPortRead])
      {   
        if (CmdCheckSum[CurrentUARTPortRead] == c) 
        { 
          EvaluateCommand();
        }
                        
        CmdState[CurrentUARTPortRead] = IDLE;
        cc = 0;
      }
    }
  }
}

