/******************************************************************************
     * Based on the file Serial.h project Multiwii. 
     * Author project Multiwii - Alexandre Dubus.
     * Version on commit e957a70 on 24 Feb 2016.
     * Project description Multiwii - http://www.multiwii.com. 
     * You can find the code of the Multiwii project here - https://github.com/multiwii/multiwii-firmware.
     * License GNU GPL v3
******************************************************************************/

#ifndef UART_H_
#define UART_H_

// Определяем кол-во UART-тов в зависимости от платы
#if defined(ARDUINO_MEGA_2560)
  // Если ArduinoMega
  #define UARTNumber 4
#elif defined(ARDUINO_PRO_MICRO)
  // Если ArduinoProMicro
  #define UARTNumber 1
#else
  // Иначе предполагается ArduinoProMini
  #define UARTNumber 1
#endif

// Размер буфера RX
#define RXBufferSize 64

// Размер буфера TX
#define TXBufferSize 64

/// <summary>
/// Инициализируем и открываем UART порт 
/// </summary>
/// <param name="port">Номер порта</param>
/// <param name="baud">Скорость работы порта</param>
void UARTOpen(uint8_t port, uint32_t baud);

/// <summary>
/// Отправляем данные по UART, включаем прерывание по опустошению буфера UART
/// </summary> 
/// <param name="port">Номер порта</param>
void UARTSendData(uint8_t port);

/// <summary>
/// Получает количество байт доступных для отправки в буфере TX
/// </summary>
/// <param name="port">Номер порта</param>
uint8_t UARTUsedTXBuff(uint8_t port);

/// <summary>
/// Сохраняем отправляемый байт в буфер TX
/// </summary>
/// <param name="port">Номер порта</param>
/// <param name="a">Отправляемый байт</param>
void UARTSerialize(uint8_t port,uint8_t a);

/// <summary>
/// Сохраняем отправляемый байт в буфер TX и отправляем данные по UART, включаем прерывание по опустошению буфера UART
/// </summary> 
/// <param name="port">Номер порта</param>
/// <param name="c">Отправляемый байт</param>
void UARTWrite(uint8_t port,uint8_t c);

/// <summary>
/// Сохраняем полученные данные в буфер RX
/// </summary> 
/// <param name="portnum">Номер порта</param>
static inline void StoreUARTInBuf(uint8_t data, uint8_t port) __attribute__((always_inline));

/// <summary>
/// Получает количество байт доступных для чтения из буфера RX
/// </summary> 
/// <param name="port">Номер порта</param>
uint8_t UARTAvailable(uint8_t port);

/// <summary>
/// Читаем данные из буфера RX
/// </summary> 
/// <param name="port">Номер порта</param>
uint8_t UARTRead(uint8_t port);

/// <summary>
/// Закрываем UART порт
/// </summary> 
/// <param name="port">Номер порта</param>
void UARTClose(uint8_t port);

#endif /* UART_H_ */
