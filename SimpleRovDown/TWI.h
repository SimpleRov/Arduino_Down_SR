#ifndef TWI_H_
#define TWI_H_

//********************************  Отладка  *********************************//
// Отладка
//#define TWI_DEBUG

#ifdef TWI_DEBUG
  // Отладка в стиле I2CDEV
  #define TWI_DEBUG_LIKE_I2CDEV

  // Пошаговая отладка
  //#define TWI_DEBUG_STEP_BY_STEP
  
  #ifdef TWI_DEBUG_STEP_BY_STEP
    #undef TWI_DEBUG_LIKE_I2CDEV
  #endif
  
  #ifdef TWI_DEBUG_LIKE_I2CDEV
    #undef TWI_DEBUG_STEP_BY_STEP
  #endif
  
  #define TWI_DEBUG_USE_SERIAL
  
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
#endif
//********************************  /Отладка  ********************************//

//********************** Статусные коды TWI модуля ***************************//
// Общие статусные коды.

// Состояние START сформировано.
#define TWI_START                  0x08 
// Состояние повторный START сформировано.
#define TWI_REP_START              0x10  
// Был потерян приоритет.
#define TWI_ARB_LOST               0x38   

// Статусные коды ведущего передатчика.

// Был передан пакет SLA+W и получено подтверждение.
#define TWI_MTX_ADR_ACK            0x18  
// Был передан пает SLA+W и не получено подтверждение.
#define TWI_MTX_ADR_NACK           0x20  
// Был передан байт данных и получено подтверждение. 
#define TWI_MTX_DATA_ACK           0x28   
// Был передан байт данных и не получено подтверждение.
#define TWI_MTX_DATA_NACK          0x30  

// Статусные коды ведущего приемника.

// Был передан пакет SLA+R и получено подтвеждение.
#define TWI_MRX_ADR_ACK            0x40  
// Был передан пакет SLA+R и не получено подтверждение.
#define TWI_MRX_ADR_NACK           0x48 
// Байт данных принят и передано подтверждение.
#define TWI_MRX_DATA_ACK           0x50 
// Был принят байт данных без подтверждения.
#define TWI_MRX_DATA_NACK          0x58   

// Другие статусные коды.

// Неопределенное состояние (TWINT = "0").
#define TWI_NO_STATE               0xF8 
// Ошибка на шине из-за некоректных состояний СТАРТ или СТОП.
#define TWI_BUS_ERROR              0x00  

// Пользовательские коды.

// Успешное завершение.
#define TWI_SUCCESS                0xff

//********************** /Статусные коды TWI модуля **************************//

//*********************** Функции получения байта адреса *********************//
#define TWI_GET_READ_BYTE(ADDRESS)  ((ADDRESS)<<1)|1;
#define TWI_GET_WRITE_BYTE(ADDRESS) ((ADDRESS)<<1)|0;
//*********************** /Функции получения байта адреса ********************//

//***************************  Публичные функции  ****************************//
/// <summary>
/// Инициализация и установка частоты SCL сигнала.
/// </summary>
void TwiMasterInit(void);

/// <summary>
/// Изменение скорости TWI.
/// </summary>
/// <param name="twiSpeed">Скорость TWI.</param>
void TwiChangeSpeed(uint32_t twiSpeed);

/// <summary>
/// Получить статус TWI модуля.
/// </summary>
/// <returns>Статус TWI модуля.</returns>
uint8_t TwiGetState(void);

/// <summary>
/// Получить статус, работа с TWI завершена.
/// </summary>
/// <returns>Работа с TWI завершена..</returns>
uint8_t TwiGetFinishStep(void);

/// <summary>
/// Передать данные.
/// </summary>
/// <param name="msg">Массив данных для отправки.</param>
/// <param name="msgSize">Размер массива msg.</param>
void TwiSendData(uint8_t *msg, uint8_t msgSize);

/// <summary>
/// Передать данные. С дополнительным ожиданием завершения операции.
/// </summary>
/// <param name="msg">Массив данных для отправки.</param>
/// <param name="msgSize">Размер массива msg.</param>
void TwiSendDataOnInit(uint8_t* msg, uint8_t msgSize);

/// <summary>
/// Устанавливаем начальный регистр.
/// </summary>
/// <param name="address">Адрес устройства.</param>
/// <param name="beginReg">Номер начального регистра.</param>
void TwiSetBeginReg(uint8_t address, uint8_t beginReg);

/// <summary>
/// Устанавливаем начальный регистр. С дополнительным ожиданием завершения операции.
/// </summary>
/// <param name="address">Адрес устройства.</param>
/// <param name="beginReg">Номер начального регистра.</param>
void TwiSetBeginRegOnInit(uint8_t address, uint8_t beginReg);

/// <summary>
/// Отправляем запрос на чтение.
/// </summary>
/// <param name="address">Адрес устройства.</param>
/// <param name="msgSize">Размер массива msg.</param>
void TwiReadBytes(uint8_t address, uint8_t msgSize);

/// <summary>
/// Отправляем запрос на чтение. С дополнительным ожиданием завершения операции.
/// </summary>
/// <param name="address">Адрес устройства.</param>
/// <param name="msg">Массив данных для отправки.</param>
void TwiReadBytesOnInit(uint8_t address, uint8_t msgSize);

/// <summary>
/// Читаем значение регистра по адресу и номеру регистра. С дополнительным ожиданием завершения операции.
/// </summary>
/// <param name="address">Адрес устройства.</param>
/// <param name="numberReg">Номер регистра.</param>
uint8_t TwiGetByteOnInit(uint8_t address, uint8_t numberReg);

/// <summary>
/// Получить принятые данные. Переписываем данные буфера драйвера в свой буфер.
/// </summary>
/// <param name="msg">Массив в который необходимо переписать данные.</param>
/// <param name="msgSize">Размер массива msg.</param>
/// <returns>Статус TWI модуля.</returns>
uint8_t TwiGetData(uint8_t *msg, uint8_t msgSize);

/// <summary>
/// Устанавляем размер регистра адреса в байтах.
/// </summary> 
/// <param name="length">Размер регистра адреса в байтах.</param>
void TwiSetToLengthAddressReg(uint8_t length);
//***************************  /Публичные функции  ***************************//

#ifdef TWI_DEBUG 
//*****************************  Функции отладки  ****************************//
/// <summary>
/// Возращает кол-во ошибок со старта приложения.
/// </summary>
uint16_t TwiGetCountError();

/// <summary>
/// Возращает последнюю ошибку.
/// </summary>
uint8_t TwiGetLastError();

/// <summary>
/// Возращает последний обработанный индекс массива в прерывании.
/// </summary>
uint8_t TwiGetTwiBufIndex();
//****************************  /Функции отладки  ****************************//
#endif

#endif //TWIM_H_
