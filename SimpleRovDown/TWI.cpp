//*******************************  Библиотеки  ******************************//
#include "Arduino.h"

#include "TWI.h"

#include "Config.h"

#include "Def.h"

#include "GlobalVar.h"
//*******************************  /Библиотеки  *****************************//

//*********************** Переменные, константы TWI *************************//
// Если не определена тактовая частота, определяем ее здесь.
#ifndef F_CPU
   #define F_CPU  16000000UL
#endif

// Размер буфера TWI модуля.
#define TWI_BUFFER_SIZE  32 

// Позиция R/W бита в адресном пакете.
#define TWI_READ_BIT     0      

// Буфер для TWI.
volatile static uint8_t twiBuf[TWI_BUFFER_SIZE];

// Размер получаемого/отправляемого пакета данных.
volatile static uint8_t twiMsgSize;    

// Текущий статус TWI.
volatile static uint8_t twiState = TWI_NO_STATE;

// Размер регистра адреса в байтах.
static uint8_t twiLengthAddressReg = 1;

#ifdef TWI_DEBUG
  // Текущий индекс буфера twiBuf.
  volatile static uint8_t twiBufIndex;
  
  // Кол-во ошибок TWI.
  volatile static uint16_t twiErrorsCount = 0;
  
  // Последний статус ошибки TWI.
  volatile static uint8_t twiLastStateError = 0;
  
  // Последний установленный регистр для чтения.
  static uint16_t twiLastSetRegForRead = 0;
  
  // Последний установленный адрес для чтения.
  static uint8_t twiLastAddressForRead = 0;
#endif
//*********************** /Переменные, константы TWI *************************//

//***************************  Приватные функции  ****************************//
/// <summary>
/// Ждем, пока модуль занят.
/// </summary>
static void TwiTransceiverBusy(void)
{
  // Ждем пока прерывание разрешено.  

  // TWCR (TWI Control Register) - Регистр управления TWCR.
  // TWIE (TWI Interrupt Enable) - разрешение прерывания TWI модуля. Когда бит TWIE и бит I регистра SREG установлены в 1 - прерывания модуля TWI разрешены. Прерывания будут вызываться при установке бита TWINT.
  while (TWCR & (1<<TWIE)){}                 
}
//***************************  /Приватные функции  ***************************//

//***************************  Публичные функции  ****************************//
/// <summary>
/// Инициализация и установка частоты SCL сигнала.
/// </summary>
void TwiMasterInit(void)
{
  // Проверка объявлен ли TWI_SPEED.
  #ifndef TWI_SPEED
    #error TWI_SPEED not declared!
  #endif

  #if defined(TWI_INTERNAL_PULLUPS)
    TWI_PULLUPS_ENABLE
  #else
    TWI_PULLUPS_DISABLE
  #endif

  // Обнуляем статусный регистр.
  // TWSR (TWI Status Register) - Статусный регистр TWSR отражает состояние TWI модуля и двухпроводной шины, а также содержит разряды, задающие коэффициент деления частоты SCL сигнала.
  TWSR = 0;       

  // Устанавливаем скорость передачи данных.
  TwiChangeSpeed(TWI_SPEED);

  // Разрешаем работу TWI модуля.
  // TWCR (TWI Control Register) - Регистр управления TWCR.
  // TWEN (TWI Enable Bit) - бит разрешения работы TWI модуля. Когда бит TWEN устанавливается в 1, TWI модуль включается и берет на себя управление выводами SCL и SDA. Когда бит TWEN сбрасывается, TWI модуль выключается.
  TWCR = 1<<TWEN;
}

/// <summary>
/// Изменение скорости TWI.
/// </summary>
/// <param name="twiSpeed">Скорость TWI.</param>
void TwiChangeSpeed(uint32_t twiSpeed)
{ 
  // Устанавливаем скорость передачи данных.
  // TWBR (TWI Bit Rate Register) - Регистр скорости передачи.
  TWBR = ((F_CPU / twiSpeed) - 16) / 2;       
}

/// <summary>
/// Получить статус TWI модуля.
/// </summary>
/// <returns>Статус TWI модуля.</returns>
uint8_t TwiGetState(void)
{  
  // Ждем, пока модуль занят.
  TwiTransceiverBusy();

  // Возвращаем статус TWI модуля.
  return twiState;                       
}

/// <summary>
/// Получить статус, работа с TWI завершена.
/// </summary>
/// <returns>Работа с TWI завершена.</returns>
uint8_t TwiGetFinishStep(void)
{
  // TWCR (TWI Control Register) - Регистр управления TWCR.
  // TWIE (TWI Interrupt Enable) - разрешение прерывания TWI модуля. Когда бит TWIE и бит I регистра SREG установлены в 1 - прерывания модуля TWI разрешены. Прерывания будут вызываться при установке бита TWINT.
  return (!(TWCR & (1<<TWIE)));
}

/// <summary>
/// Передать данные.
/// </summary>
/// <param name="msg">Массив данных для отправки.</param>
/// <param name="msgSize">Размер массива msg.</param>
void TwiSendData(uint8_t *msg, uint8_t msgSize)
{
  // Ждем, когда TWI модуль освободится. 
  TwiTransceiverBusy();

  // Сохряняем кол. байт для передачи.
  twiMsgSize = msgSize;   

  // Сохряняем первый байт сообщения.  
  twiBuf[0]  = msg[0];             

  // Если первый байт типа SLA+W.
  if (!(msg[0] & (1<<TWI_READ_BIT)))
  {   
    // Отладка
    #ifdef TWI_DEBUG
      if (msgSize > (twiLengthAddressReg + 1))
      {
        DEBUG_PRINT(F("I2C (0x"));
        twiLastAddressForRead = msg[0]>>1;
        DEBUG_PRINTF(twiLastAddressForRead, HEX);
        DEBUG_PRINT(F(") writing "));
        
        if (!twiLengthAddressReg)
        {
          DEBUG_PRINT(1);
        }
        else if (twiLengthAddressReg == 2)
        {
          DEBUG_PRINT(msgSize-3);
        }
        else 
        {
          DEBUG_PRINT(msgSize-2);
        }
        
        DEBUG_PRINT(F(" bytes to 0x"));
        
        if (!twiLengthAddressReg)
        {
          DEBUG_PRINT(0);
        }
        else if (twiLengthAddressReg == 2)
        {
          DEBUG_PRINTF((msg[1]<<8) | msg[2], HEX);
        }
        else 
        {
          DEBUG_PRINTF(msg[1], HEX);
        }
        
        #ifdef TWI_DEBUG_STEP_BY_STEP
          DEBUG_PRINT(F(" data - "));
        #endif
        
        #ifdef TWI_DEBUG_LIKE_I2CDEV
          DEBUG_PRINT(F("..."));
        #endif
      }
      else 
      {
        #ifdef TWI_DEBUG_STEP_BY_STEP
          DEBUG_PRINT(F("I2C (0x"));
          DEBUG_PRINTF(msg[0]>>1, HEX);
          DEBUG_PRINT(F(") set begin reg for read to 0x"));
        #endif  
        if (twiLengthAddressReg == 2)
        {
          twiLastSetRegForRead = (msg[1]<<8) | msg[2];
          
          #ifdef TWI_DEBUG_STEP_BY_STEP
            DEBUG_PRINTF(twiLastSetRegForRead, HEX);
          #endif
        }
        else 
        {
          twiLastSetRegForRead = (uint16_t)msg[1];
          
          #ifdef TWI_DEBUG_STEP_BY_STEP
            DEBUG_PRINTF(twiLastSetRegForRead, HEX);
          #endif
        }
        
        #ifdef TWI_DEBUG_STEP_BY_STEP
          DEBUG_PRINTLN(F(" Done."));  
        #endif    
      }
    #endif
    
    // Сохряняем остальную часть сообщения.  
    for (uint8_t i = (msgSize-1); i > 0; i--)
    {      
      twiBuf[i] = msg[i];
    }
    
    // Отладка
    #ifdef TWI_DEBUG
      if (msgSize > (twiLengthAddressReg + 1))
      {
        for (uint8_t i = 1; i < msgSize; i++)
        { 
          if (i >= (twiLengthAddressReg+1))
          {
            DEBUG_PRINT(F("0x"));
            DEBUG_PRINTF(msg[i], HEX);
            if ((i+1) < msgSize)
            {
              DEBUG_PRINT(F(" "));
            }
          }
        }  
        
        DEBUG_PRINTLN(F(". Done."));
      }

    #endif
  }

  // Обнуляем переменную, которая хранит статус операции.                     
  twiState = TWI_NO_STATE;

  // Разрешаем прерывание и формируем состояние старт.

  // TWCR (TWI Control Register) - Регистр управления TWCR.
  // TWEN (TWI Enable Bit) - бит разрешения работы TWI модуля. Когда бит TWEN устанавливается в 1, TWI модуль включается и берет на себя управление выводами SCL и SDA. Когда бит TWEN сбрасывается, TWI модуль выключается.
  // TWIE (TWI Interrupt Enable) - разрешение прерывания TWI модуля. Когда бит TWIE и бит I регистра SREG установлены в 1 - прерывания модуля TWI разрешены. Прерывания будут вызываться при установке бита TWINT.
  // TWINT(TWI Interrupt Flag) - флаг прерывания TWI модуля. Этот бит устанавливается аппаратно, когда TWI модуль завершает текущую операцию (формирование состояния СТАРТ, передачи адресного пакета и так далее).
  // Бит TWINT очищается программно, записью единицы. При выполнении обработчика прерывания этот бит не сбрасывается аппаратно, как в других модулях.
  // TWSTA (TWI START Condition Bit) - флаг состояния СТАРТ. Когда этот бит устанавливается в 1, TWI модуль проверяет не занята ли шина и формирует состояние СТАРТ. Если шина занята, он будет ожидать появления на ней состояния СТОП и после этого выдаст состояние СТАРТ. Бит TWSTA должен быть очищен программно, когда состояние СТАРТ передано. 
  TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWSTA);                             
}

/// <summary>
/// Передать данные. С дополнительным ожиданием завершения операции.
/// </summary>
/// <param name="msg">Массив данных для отправки.</param>
/// <param name="msgSize">Размер массива msg.</param>
void TwiSendDataOnInit(uint8_t* msg, uint8_t msgSize)
{
  // Передаем данные.
  TwiSendData(msg, msgSize);
  
  // Ждем, когда TWI модуль освободится. 
  TwiTransceiverBusy();
}

/// <summary>
/// Устанавливаем начальный регистр.
/// </summary>
/// <param name="address">Адрес устройства.</param>
/// <param name="beginReg">Номер начального регистра.</param>
void TwiSetBeginReg(uint8_t address, uint8_t beginReg)
{
  // Объявляем массив для отправки.
  uint8_t msgBuf[2];

  // Адресный пакет.
  msgBuf[0] = TWI_GET_WRITE_BYTE(address);
  
  // Адрес регистра.
  msgBuf[1] = beginReg;

  // Передаем данные.
  TwiSendData(msgBuf, 2);
}

/// <summary>
/// Устанавливаем начальный регистр. С дополнительным ожиданием завершения операции.
/// </summary>
/// <param name="address">Адрес устройства.</param>
/// <param name="beginReg">Номер начального регистра.</param>
void TwiSetBeginRegOnInit(uint8_t address, uint8_t beginReg)
{
  // Устанавливаем начальный регистр.
  TwiSetBeginReg(address, beginReg);
  
  // Ждем, когда TWI модуль освободится.
  TwiTransceiverBusy();   
}

/// <summary>
/// Отправляем запрос на чтение.
/// </summary>
/// <param name="address">Адрес устройства.</param>
/// <param name="msgSize">Размер массива msg.</param>
void TwiReadBytes(uint8_t address, uint8_t msgSize)
{
  // Объявляем массив для отправки.
  uint8_t msgBuf[2];

  // Адресный пакет.
  msgBuf[0] = TWI_GET_READ_BYTE(address);

  // Передаем данные.
  TwiSendData(msgBuf, msgSize);
}

/// <summary>
/// Отправляем запрос на чтение. С дополнительным ожиданием завершения операции.
/// </summary>
/// <param name="address">Адрес устройства.</param>
/// <param name="msgSize">Размер массива msg.</param>
void TwiReadBytesOnInit(uint8_t address, uint8_t msgSize)
{
  // Отправляем запрос на чтение.
  TwiReadBytes(address, msgSize);
  
  // Ждем, когда TWI модуль освободится.
  TwiTransceiverBusy();   
}

/// <summary>
/// Читаем значение регистра по адресу и номеру регистра. С дополнительным ожиданием завершения операции.
/// </summary>
/// <param name="address">Адрес устройства.</param>
/// <param name="numberReg">Номер регистра.</param>
uint8_t TwiGetByteOnInit(uint8_t address, uint8_t numberReg)
{
  // Устанавливаем начальный регистр.
  TwiSetBeginRegOnInit(address, numberReg);
  
  // Отправляем запрос на чтение.
  TwiReadBytesOnInit(address, 2);
  
  // Объявляем массив для получения данных.
  uint8_t msgBuf[1];
  
  // Переписываем данные буфера драйвера в свой буфер.
  TwiGetData(msgBuf, 1);
  
  return msgBuf[0];
}

/// <summary>
/// Получить принятые данные. Переписываем данные буфера драйвера в свой буфер.
/// </summary>
/// <param name="msg">Массив в который необходимо переписать данные.</param>
/// <param name="msgSize">Размер массива msg.</param>
/// <returns>Статус TWI модуля.</returns>
uint8_t TwiGetData(uint8_t *msg, uint8_t msgSize)
{
  // Ждем, когда TWI модуль освободится.
  TwiTransceiverBusy();     

  // Сохряняем кол. байт для передачи.
  msgSize = msgSize+1;

  // Если сообщение успешно принято.    
  if(twiState == TWI_SUCCESS)
  { 
    for(uint8_t i = (msgSize-1); i > 0; i--)
    {  
      msg[i-1] = twiBuf[i];
    }
    
    // Отладка
    #ifdef TWI_DEBUG
      DEBUG_PRINT(F("I2C (0x"));
      DEBUG_PRINTF(twiLastAddressForRead, HEX);
      DEBUG_PRINT(F(") reading "));
      DEBUG_PRINT(msgSize-1);
      DEBUG_PRINT(F(" bytes from 0x"));
      DEBUG_PRINTF(twiLastSetRegForRead, HEX);
      
      #ifdef TWI_DEBUG_STEP_BY_STEP
        DEBUG_PRINT(F(" data - "));
      #endif
      
      #ifdef TWI_DEBUG_LIKE_I2CDEV
        DEBUG_PRINT(F("..."));
      #endif
    #endif
    
    // Отладка
    #ifdef TWI_DEBUG
      for(uint8_t i = 0; i < (msgSize - 1); i++)
      { 
        DEBUG_PRINT(F("0x"));
        DEBUG_PRINTF(msg[i], HEX);
        if ((i+1) < (msgSize - 1))
        {
          DEBUG_PRINT(F(" "));
        }
      }
      
      #ifdef TWI_DEBUG_STEP_BY_STEP
        DEBUG_PRINTLN(F(". Done."));
      #endif
      
      #ifdef TWI_DEBUG_LIKE_I2CDEV
        DEBUG_PRINT(F(". Done ("));
        DEBUG_PRINT(msgSize-1);
        DEBUG_PRINTLN(F(" read)."));
      #endif
    #endif
  }

  return twiState;                                   
}

/// <summary>
/// Устанавляем размер регистра адреса в байтах.
/// </summary> 
/// <param name="length">Размер регистра адреса в байтах.</param>
void TwiSetToLengthAddressReg(uint8_t length)
{
  if (length > 2)
  {
    return;
  }
  twiLengthAddressReg = length;
}
//***************************  /Публичные функции  ***************************//

//*******************************  Прерывания  *******************************//
/// <summary>
/// Обработчик прерывания TWI модуля.
/// </summary>
ISR(TWI_vect)
{
  #ifndef TWI_DEBUG
    static uint8_t twiBufIndex;
  #endif

  switch (TWSR)
  {
    // Состояние START сформировано. 
  case TWI_START:     
    // Состояние повторный START сформировано.    
  case TWI_REP_START:                       
    twiBufIndex = 0; 
    // Был передан пакет SLA+W и получено подтверждение.   
  case TWI_MTX_ADR_ACK: 
    // Был передан байт данных и получено подтверждение.     
  case TWI_MTX_DATA_ACK:             
    if (twiBufIndex < twiMsgSize)
    {
      // Загружаем в регистр данных следующий байт.
      // TWDR (TWI Data Register) - Регистр данных. 
      TWDR = twiBuf[twiBufIndex];                  

      // Сбрасываем флаг TWINT.   
      // TWCR (TWI Control Register) - Регистр управления TWCR.
      // TWEN (TWI Enable Bit) - бит разрешения работы TWI модуля. Когда бит TWEN устанавливается в 1, TWI модуль включается и берет на себя управление выводами SCL и SDA. Когда бит TWEN сбрасывается, TWI модуль выключается.
      // TWIE (TWI Interrupt Enable) - разрешение прерывания TWI модуля. Когда бит TWIE и бит I регистра SREG установлены в 1 - прерывания модуля TWI разрешены. Прерывания будут вызываться при установке бита TWINT.
      // TWINT(TWI Interrupt Flag) - флаг прерывания TWI модуля. Этот бит устанавливается аппаратно, когда TWI модуль завершает текущую операцию (формирование состояния СТАРТ, передачи адресного пакета и так далее).
      // Бит TWINT очищается программно, записью единицы. При выполнении обработчика прерывания этот бит не сбрасывается аппаратно, как в других модулях.
      TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT);   
      twiBufIndex++;
    }
    else
    {
      twiState = TWI_SUCCESS;  

      // Формируем состояние СТОП, сбрасываем флаг, запрещаем прерывания.
      // TWCR (TWI Control Register) - Регистр управления TWCR.
      // TWEN (TWI Enable Bit) - бит разрешения работы TWI модуля. Когда бит TWEN устанавливается в 1, TWI модуль включается и берет на себя управление выводами SCL и SDA. Когда бит TWEN сбрасывается, TWI модуль выключается.
      // TWIE (TWI Interrupt Enable) - разрешение прерывания TWI модуля. Когда бит TWIE и бит I регистра SREG установлены в 1 - прерывания модуля TWI разрешены. Прерывания будут вызываться при установке бита TWINT.
      // TWINT(TWI Interrupt Flag) - флаг прерывания TWI модуля. Этот бит устанавливается аппаратно, когда TWI модуль завершает текущую операцию (формирование состояния СТАРТ, передачи адресного пакета и так далее).
      // Бит TWINT очищается программно, записью единицы. При выполнении обработчика прерывания этот бит не сбрасывается аппаратно, как в других модулях.
      // TWSTO (TWI STOP Condition Bit) - флаг состояния СТОП. Когда этот бит устанавливается в 1 в режиме ведущего, TWI модуль выдает на шину состояние СТОП и сбрасывает этот бит. В режиме ведомого установка этого бита может использоваться для восстановления после ошибки. При этом состояние СТОП не формируется, но TWI модуль возвращается к начальному не адресованному состоянию. 
      TWCR = (1<<TWEN)|(1<<TWINT)|(1<<TWSTO)|(0<<TWIE); 
    }
    break;
    // Байт данных принят и передано подтверждение. 
  case TWI_MRX_DATA_ACK:           
    twiBuf[twiBufIndex] = TWDR;
    twiBufIndex++;
    // Был передан пакет SLA+R и получено подтвеждение. 
  case TWI_MRX_ADR_ACK:            
    if (twiBufIndex < (twiMsgSize-1))
    {
      // Если это не предпоследний принятый байт, формируем подтверждение.
      // TWCR (TWI Control Register) - Регистр управления TWCR.
      // TWEN (TWI Enable Bit) - бит разрешения работы TWI модуля. Когда бит TWEN устанавливается в 1, TWI модуль включается и берет на себя управление выводами SCL и SDA. Когда бит TWEN сбрасывается, TWI модуль выключается.
      // TWIE (TWI Interrupt Enable) - разрешение прерывания TWI модуля. Когда бит TWIE и бит I регистра SREG установлены в 1 - прерывания модуля TWI разрешены. Прерывания будут вызываться при установке бита TWINT.
      // TWINT(TWI Interrupt Flag) - флаг прерывания TWI модуля. Этот бит устанавливается аппаратно, когда TWI модуль завершает текущую операцию (формирование состояния СТАРТ, передачи адресного пакета и так далее).
      // Бит TWINT очищается программно, записью единицы. При выполнении обработчика прерывания этот бит не сбрасывается аппаратно, как в других модулях.
      // TWEA (TWI Enable Acknowledge Bit) - разрешение бита подтверждения. Если бит TWEA установлен в 1, TWI модуль формирует сигнал подтверждения (ACK), когда это требуется. А требуется это в трех случаях: ведущее или ведомое устройство получило байт данных, ведомое устройство получило общий вызов, ведомое устройство получило свой адрес. 
      TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA);                               
    }
    else 
    {
      // Если приняли предпоследний байт, подтверждение не формируем.
      // TWCR (TWI Control Register) - Регистр управления TWCR.
      // TWEN (TWI Enable Bit) - бит разрешения работы TWI модуля. Когда бит TWEN устанавливается в 1, TWI модуль включается и берет на себя управление выводами SCL и SDA. Когда бит TWEN сбрасывается, TWI модуль выключается.
      // TWIE (TWI Interrupt Enable) - разрешение прерывания TWI модуля. Когда бит TWIE и бит I регистра SREG установлены в 1 - прерывания модуля TWI разрешены. Прерывания будут вызываться при установке бита TWINT.
      // TWINT(TWI Interrupt Flag) - флаг прерывания TWI модуля. Этот бит устанавливается аппаратно, когда TWI модуль завершает текущую операцию (формирование состояния СТАРТ, передачи адресного пакета и так далее).
      // Бит TWINT очищается программно, записью единицы. При выполнении обработчика прерывания этот бит не сбрасывается аппаратно, как в других модулях.
      TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT);            
    }    
    break; 
    // Был принят байт данных без подтверждения. 
  case TWI_MRX_DATA_NACK:            
    twiBuf[twiBufIndex] = TWDR;
    twiState = TWI_SUCCESS;  
    // Формируем состояние стоп.

    // TWCR (TWI Control Register) - Регистр управления TWCR.
    // TWEN (TWI Enable Bit) - бит разрешения работы TWI модуля. Когда бит TWEN устанавливается в 1, TWI модуль включается и берет на себя управление выводами SCL и SDA. Когда бит TWEN сбрасывается, TWI модуль выключается.
    // TWINT(TWI Interrupt Flag) - флаг прерывания TWI модуля. Этот бит устанавливается аппаратно, когда TWI модуль завершает текущую операцию (формирование состояния СТАРТ, передачи адресного пакета и так далее).
    // Бит TWINT очищается программно, записью единицы. При выполнении обработчика прерывания этот бит не сбрасывается аппаратно, как в других модулях.  
    // TWSTO (TWI STOP Condition Bit) - флаг состояния СТОП. Когда этот бит устанавливается в 1 в режиме ведущего, TWI модуль выдает на шину состояние СТОП и сбрасывает этот бит. В режиме ведомого установка этого бита может использоваться для восстановления после ошибки. При этом состояние СТОП не формируется, но TWI модуль возвращается к начальному не адресованному состоянию. 
    TWCR = (1<<TWEN)|(1<<TWINT)|(1<<TWSTO); 
    break; 
    // Был потерян приоритет.
  case TWI_ARB_LOST:        
    // Сбрасываем флаг TWINT, формируем повторный СТАРТ. 
    // TWCR (TWI Control Register) - Регистр управления TWCR.
    // TWIE (TWI Interrupt Enable) - разрешение прерывания TWI модуля. Когда бит TWIE и бит I регистра SREG установлены в 1 - прерывания модуля TWI разрешены. Прерывания будут вызываться при установке бита TWINT.
    // TWEN (TWI Enable Bit) - бит разрешения работы TWI модуля. Когда бит TWEN устанавливается в 1, TWI модуль включается и берет на себя управление выводами SCL и SDA. Когда бит TWEN сбрасывается, TWI модуль выключается.
    // TWINT(TWI Interrupt Flag) - флаг прерывания TWI модуля. Этот бит устанавливается аппаратно, когда TWI модуль завершает текущую операцию (формирование состояния СТАРТ, передачи адресного пакета и так далее).
    // Бит TWINT очищается программно, записью единицы. При выполнении обработчика прерывания этот бит не сбрасывается аппаратно, как в других модулях.    
    // TWSTA (TWI START Condition Bit) - флаг состояния СТАРТ. Когда этот бит устанавливается в 1, TWI модуль проверяет не занята ли шина и формирует состояние СТАРТ. Если шина занята, он будет ожидать появления на ней состояния СТОП и после этого выдаст состояние СТАРТ. Бит TWSTA должен быть очищен программно, когда состояние СТАРТ передано. 
    TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWSTA); 
    break;
    // Был передан пает SLA+W и не получено подтверждение. 
  case TWI_MTX_ADR_NACK:
    // Был передан пакет SLA+R и не получено подтверждение.     
  case TWI_MRX_ADR_NACK:  
    // Был передан байт данных и не получено подтверждение.    
  case TWI_MTX_DATA_NACK:   
    // Ошибка на шине из-за некоректных состояний СТАРТ или СТОП.  
  case TWI_BUS_ERROR:         
  default:     
    #ifdef TWI_DEBUG  
      twiErrorsCount++;
      twiLastStateError = twiState = TWSR; 
    #else
      twiState = TWSR;
    #endif 

    // Запретить прерывание.      
    // TWCR (TWI Control Register) - Регистр управления TWCR.
    // TWIE (TWI Interrupt Enable) - разрешение прерывания TWI модуля. Когда бит TWIE и бит I регистра SREG установлены в 1 - прерывания модуля TWI разрешены. Прерывания будут вызываться при установке бита TWINT.
    // TWEN (TWI Enable Bit) - бит разрешения работы TWI модуля. Когда бит TWEN устанавливается в 1, TWI модуль включается и берет на себя управление выводами SCL и SDA. Когда бит TWEN сбрасывается, TWI модуль выключается.
    // TWINT(TWI Interrupt Flag) - флаг прерывания TWI модуля. Этот бит устанавливается аппаратно, когда TWI модуль завершает текущую операцию (формирование состояния СТАРТ, передачи адресного пакета и так далее).
    // Бит TWINT очищается программно, записью единицы. При выполнении обработчика прерывания этот бит не сбрасывается аппаратно, как в других модулях.    
    // TWSTA (TWI START Condition Bit) - флаг состояния СТАРТ. Когда этот бит устанавливается в 1, TWI модуль проверяет не занята ли шина и формирует состояние СТАРТ. Если шина занята, он будет ожидать появления на ней состояния СТОП и после этого выдаст состояние СТАРТ. Бит TWSTA должен быть очищен программно, когда состояние СТАРТ передано. 
    // TWEA (TWI Enable Acknowledge Bit) - разрешение бита подтверждения. Если бит TWEA установлен в 1, TWI модуль формирует сигнал подтверждения (ACK), когда это требуется. А требуется это в трех случаях: ведущее или ведомое устройство получило байт данных, ведомое устройство получило общий вызов, ведомое устройство получило свой адрес. 
    // TWSTO (TWI STOP Condition Bit) - флаг состояния СТОП. Когда этот бит устанавливается в 1 в режиме ведущего, TWI модуль выдает на шину состояние СТОП и сбрасывает этот бит. В режиме ведомого установка этого бита может использоваться для восстановления после ошибки. При этом состояние СТОП не формируется, но TWI модуль возвращается к начальному не адресованному состоянию. 
    // TWWC (TWI Write Collision Flag) - флаг конфликта записи. Этот флаг устанавливается аппаратно, когда выполняется запись в регистр данных (TWDR) при низком значении бита TWINT. То есть когда TWI модуль уже выполняет какие-то операции. 
    // Флаг TWWC сбрасывается аппаратно, когда запись в регистр данных выполняется при установленном флаге прерывания TWINT.
    TWCR = (1<<TWEN)|(0<<TWIE)|(0<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC); 
    break;
  }
}
//*******************************  /Прерывания  ******************************//

#ifdef TWI_DEBUG 
//*****************************  Функции отладки  ****************************//
/// <summary>
/// Возращает кол-во ошибок со старта приложения.
/// </summary>
uint16_t TwiGetCountError()
{
  return twiErrorsCount;
}

/// <summary>
/// Возращает последнюю ошибку.
/// </summary>
uint8_t TwiGetLastError()
{
  return twiLastStateError;
}

/// <summary>
/// Возращает последний обработанный индекс массива в прерывании.
/// </summary>
uint8_t TwiGetTwiBufIndex()
{
  return twiBufIndex;
}
//****************************  /Функции отладки  ****************************//
#endif
