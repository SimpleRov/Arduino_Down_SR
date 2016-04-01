/******************************************************************************
     * Based on the file Serial.cpp project Multiwii. 
     * Author project Multiwii - Alexandre Dubus.
     * Version on commit e957a70 on 24 Feb 2016.
     * Project description Multiwii - http://www.multiwii.com. 
     * You can find the code of the Multiwii project here - https://github.com/multiwii/multiwii-firmware.
     * License GNU GPL v3
******************************************************************************/

//*******************************  Библиотеки  ******************************//
#include <Arduino.h>

#include "UART.h"

#include "Config.h"

#include "Def.h"
//*******************************  /Библиотеки  *****************************//

// UARTHeadRX - буфер RX для хранения "головы массива"
// UARTTailRX - буфер RX для хранения "хвоста массива"
static volatile uint8_t UARTHeadRX[UARTNumber],UARTTailRX[UARTNumber];

// UARTBufferRX - буфер RX для хранения данных
static uint8_t UARTBufferRX[RXBufferSize][UARTNumber];

// UARTHeadTX - буфер TX для хранения "головы массива"
// UARTBufferTX - буфер TX для хранения "хвоста массива"
static volatile uint8_t UARTHeadTX[UARTNumber],UARTTailTX[UARTNumber];

// UARTBufferTX - буфер X для хранения данных
static uint8_t UARTBufferTX[TXBufferSize][UARTNumber];

/// <summary>
/// Прерывание по опустошению буфера UART
/// </summary> 
#if defined(ARDUINO_PRO_MINI) || defined(ARDUINO_MEGA_2560)
  #if defined(ARDUINO_PRO_MINI)
  ISR(USART_UDRE_vect) 
  { 
  #endif
  #if defined(ARDUINO_MEGA_2560)
  ISR(USART0_UDRE_vect) 
  { 
  #endif
    uint8_t t = UARTTailTX[0];
    if (UARTHeadTX[0] != t) 
    {
      if (++t >= TXBufferSize) 
      {
        t = 0;
      }
      UDR0 = UARTBufferTX[t][0];  
      UARTTailTX[0] = t;
    }
    if (t == UARTHeadTX[0]) 
    {
      UCSR0B &= ~(1<<UDRIE0); 
    }
  }
#endif
#if defined(ARDUINO_MEGA_2560) || defined(ARDUINO_PRO_MICRO)
  ISR(USART1_UDRE_vect) 
  { 
    #if defined(ARDUINO_MEGA_2560)
      uint8_t t = UARTTailTX[1];
      if (UARTHeadTX[1] != t) 
      {
        if (++t >= TXBufferSize) 
        {
          t = 0;
        }
        UDR1 = UARTBufferTX[t][1];  
        UARTTailTX[1] = t;
      }
      if (t == UARTHeadTX[1])
      {
        UCSR1B &= ~(1<<UDRIE1);
      }
    #endif

    #if defined(ARDUINO_PRO_MICRO)
      uint8_t t = UARTTailTX[0];
      if (UARTHeadTX[0] != t) 
      {
        if (++t >= TXBufferSize) 
        {
          t = 0;
        }
        UDR1 = UARTBufferTX[t][0];  
        UARTTailTX[0] = t;
      }
      if (t == UARTHeadTX[0])
      {
        UCSR1B &= ~(1<<UDRIE1);
      }
    #endif
  }
#endif
#if defined(ARDUINO_MEGA_2560)
  ISR(USART2_UDRE_vect) 
  { 
    uint8_t t = UARTTailTX[2];
    if (UARTHeadTX[2] != t) 
    {
      if (++t >= TXBufferSize) 
      {
        t = 0;
      }
      UDR2 = UARTBufferTX[t][2];
      UARTTailTX[2] = t;
    }
    if (t == UARTHeadTX[2])
    {
      UCSR2B &= ~(1<<UDRIE2);
    }
  }
  ISR(USART3_UDRE_vect) 
  { 
    uint8_t t = UARTTailTX[3];
    if (UARTHeadTX[3] != t) 
    {
      if (++t >= TXBufferSize) 
      {
        t = 0;
      }
      UDR3 = UARTBufferTX[t][3];
      UARTTailTX[3] = t;
    }
    if (t == UARTHeadTX[3]) 
    {
      UCSR3B &= ~(1<<UDRIE3);
    }
  }
#endif

/// <summary>
/// Прерывание по завершению приема по каналу UART
/// </summary> 
#if defined(ARDUINO_PRO_MINI)
  ISR(USART_RX_vect)  { StoreUARTInBuf(UDR0, 0); }
#endif
#if defined(ARDUINO_PRO_MICRO)
  ISR(USART1_RX_vect)  { StoreUARTInBuf(UDR1, 1); }
#endif
#if defined(ARDUINO_MEGA_2560)
  ISR(USART0_RX_vect)  { StoreUARTInBuf(UDR0, 0); }
  ISR(USART1_RX_vect)  { StoreUARTInBuf(UDR1, 1); }
  ISR(USART2_RX_vect)  { StoreUARTInBuf(UDR2, 2); }
  ISR(USART3_RX_vect)  { StoreUARTInBuf(UDR3, 3); }
#endif

/// <summary>
/// Инициализируем и открываем UART порт 
/// </summary>
/// <param name="port">Номер порта</param>
/// <param name="baud">Скорость работы порта</param>
void UARTOpen(uint8_t port, uint32_t baud) 
{
  // U2X - Удвоение скорости обмена. Если этот разряд установлен в «1», коэффициент деления предделителя контроллера скорости передачи уменьшается с 16 до 8, удваивая тем самым скорость асинхронного обмена по последовательному каналу. В USART разряд U2X используется только при асинхронном режиме работы. В синхронном режиме он должен быть сброшен.
  // UBRRL  и UBRRH - регистры скорости передачи   
  // RXEN - Разрешение приема. При установке этого разряда в «1» разрешается работа приемника USART/UART и переопределяется функционирование вывода RXD.
  // TXEN - Разрешение передачи. При установке этого разряда в «1» разрешается работа передатчика UART и переопределяется функционирование вывода TXD
  // RXCIE - Разрешение прерывания по завершению приема. Если данный разряд установлен в «1», то при установке флага RXC регистра UCSRA генерируется прерывание.

  //TODO: При скорости 56700 убрать U2X.
  uint8_t h = ((F_CPU  / 4 / baud -1) / 2) >> 8;
  uint8_t l = ((F_CPU  / 4 / baud -1) / 2);
  switch (port) 
  {
    #if defined(ARDUINO_PRO_MINI)
      case 0: UCSR0A  = (1<<U2X0); UBRR0H = h; UBRR0L = l; UCSR0B |= (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0); break;
    #endif
    #if defined(ARDUINO_PRO_MICRO)
      case 1: UCSR1A  = (1<<U2X1); UBRR1H = h; UBRR1L = l; UCSR1B |= (1<<RXEN1)|(1<<TXEN1)|(1<<RXCIE1); break;
    #endif
    #if defined(ARDUINO_MEGA_2560)
      case 0: UCSR0A  = (1<<U2X0); UBRR0H = h; UBRR0L = l; UCSR0B |= (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0); break;
      case 1: UCSR1A  = (1<<U2X1); UBRR1H = h; UBRR1L = l; UCSR1B |= (1<<RXEN1)|(1<<TXEN1)|(1<<RXCIE1); break;
      case 2: UCSR2A  = (1<<U2X2); UBRR2H = h; UBRR2L = l; UCSR2B |= (1<<RXEN2)|(1<<TXEN2)|(1<<RXCIE2); break;
      case 3: UCSR3A  = (1<<U2X3); UBRR3H = h; UBRR3L = l; UCSR3B |= (1<<RXEN3)|(1<<TXEN3)|(1<<RXCIE3); break;
    #endif
  }
}

/// <summary>
/// Отправляем данные по UART, включаем прерывание по опустошению буфера UART
/// </summary> 
/// <param name="port">Номер порта</param>
void UARTSendData(uint8_t port) 
{
  // UDRIE - Разрешение прерывания при очистке регистра данных UART. Если данный разряд установлен в «1», то при установке флага UDRE в регистра UCSRA  генерируется прерывание. 
  #if defined(ARDUINO_PRO_MINI)
    UCSR0B |= (1<<UDRIE0);
  #endif
  #if defined(ARDUINO_PRO_MICRO)
    //TODO: Заменить switch на if.
    switch (port) 
    {
      case 1: UCSR1B |= (1<<UDRIE1); break;
    }
  #endif
  #if defined(ARDUINO_MEGA_2560)
    switch (port) 
    {
      case 0: UCSR0B |= (1<<UDRIE0); break;
      case 1: UCSR1B |= (1<<UDRIE1); break;
      case 2: UCSR2B |= (1<<UDRIE2); break;
      case 3: UCSR3B |= (1<<UDRIE3); break;
    }
  #endif
}

/// <summary>
/// Получает количество байт доступных для отправки в буфере TX
/// </summary>
/// <param name="port">Номер порта</param>
uint8_t UARTUsedTXBuff(uint8_t port) 
{
  #if defined(ARDUINO_PRO_MICRO)
    port = 0;
  #endif
  return ((uint8_t)(UARTHeadTX[port] - UARTTailTX[port]))%TXBufferSize;
}

/// <summary>
// // Сохраняем отправляемый байт в буфер TX
/// </summary>
/// <param name="port">Номер порта</param>
/// <param name="a">Отправляемый байт</param>
void UARTSerialize(uint8_t port,uint8_t a) 
{
  #if defined(ARDUINO_PRO_MICRO)
    port = 0;
  #endif
  uint8_t t = UARTHeadTX[port];
  if (++t >= TXBufferSize) 
  {
    t = 0;
  }
  UARTBufferTX[t][port] = a;
  UARTHeadTX[port] = t;
}

/// <summary>
/// Сохраняем отправляемый байт в буфер TX и отправляем данные по UART, включаем прерывание по опустошению буфера UART
/// </summary> 
/// <param name="port">Номер порта</param>
/// <param name="c">Отправляемый байт</param>
void UARTWrite(uint8_t port,uint8_t c)
{
  #if defined(ARDUINO_PRO_MICRO)
    port = 0;
  #endif
  UARTSerialize(port,c);
  UARTSendData(port);
}

/// <summary>
/// Сохраняем полученные данные в буфер RX
/// </summary> 
/// <param name="portnum">Номер порта</param>
static inline void StoreUARTInBuf(uint8_t data, uint8_t port) 
{
  #if defined(ARDUINO_PRO_MICRO)
    port = 0;
  #endif
  
  // Получаем номер "головы массива" RX
  uint8_t h = UARTHeadRX[port];
  
  // Сохраняем полученный байт в буфер RX для хранения данных
  UARTBufferRX[h++][port] = data;
  
  // Проверяем, увеличивая номер "головы массива" RX головы на 1 и если номер "головы массива", станет равным или больше размера буфера, обнуляем переменную h в 0 
  if (h >= RXBufferSize) 
  {
    h = 0;
  }
  
  // Сохраняем новый номер "головы массива" RX
  UARTHeadRX[port] = h;
}

/// <summary>
/// Получает количество байт доступных для чтения из буфера RX
/// </summary> 
/// <param name="port">Номер порта</param>
uint8_t UARTAvailable(uint8_t port) 
{
  #if defined(ARDUINO_PRO_MICRO)
    port = 0;
  #endif
  return ((uint8_t)(UARTHeadRX[port] - UARTTailRX[port]))%RXBufferSize;
}

/// <summary>
/// Читаем данные из буфера RX
/// </summary> 
/// <param name="port">Номер порта</param>
uint8_t UARTRead(uint8_t port) 
{
  #if defined(ARDUINO_PRO_MICRO)
    port = 0;
  #endif
  // Получаем номер "хвоста масива" RX
  uint8_t t = UARTTailRX[port];
  // Получаем байт по номеру "хвоста масива" RX
  uint8_t c = UARTBufferRX[t][port];
  // Проверяем номер "головы масива" RX и номер "хвоста масива" RX если не равен
  if (UARTHeadRX[port] != t) 
  {
    // Проверяем, увеличивая номер "хвоста массива" RX хвоста на 1 и если номер "хвоста массива", станет равным или больше размера буфера, обнуляем переменную t в 0  
    if (++t >= RXBufferSize) 
    {
      t = 0;
    }
    // Сохраняем новый номер "хвоста массива" RX
    UARTTailRX[port] = t;
  }
  return c;
}

/// <summary>
/// Закрываем UART порт
/// </summary> 
/// <param name="port">Номер порта</param>
void UARTClose(uint8_t port) 
{
  // RXEN - Разрешение приема. При установке этого разряда в «1» разрешается работа приемника USART/UART и переопределяется функционирование вывода RXD.
  // TXEN - Разрешение передачи. При установке этого разряда в «1» разрешается работа передатчика UART и переопределяется функционирование вывода TXD
  // RXCIE - Разрешение прерывания по завершению приема. Если данный разряд установлен в «1», то при установке флага RXC регистра UCSRA генерируется прерывание.
  // UDRIE - Разрешение прерывания при очистке регистра данных UART. Если данный разряд установлен в «1», то при установке флага UDRE в регистра UCSRA  генерируется прерывание.  
  
  switch (port) 
  {
    #if defined(ARDUINO_PRO_MINI)
      case 0: UCSR0B &= ~((1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0)|(1<<UDRIE0)); break;
    #endif
    #if defined(ARDUINO_PRO_MICRO)
      case 1: UCSR1B &= ~((1<<RXEN1)|(1<<TXEN1)|(1<<RXCIE1)|(1<<UDRIE1)); break;
    #endif
    #if defined(ARDUINO_MEGA_2560)
      case 0: UCSR0B &= ~((1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0)|(1<<UDRIE0)); break;
      case 1: UCSR1B &= ~((1<<RXEN1)|(1<<TXEN1)|(1<<RXCIE1)|(1<<UDRIE1)); break;
      case 2: UCSR2B &= ~((1<<RXEN2)|(1<<TXEN2)|(1<<RXCIE2)|(1<<UDRIE2)); break;
      case 3: UCSR3B &= ~((1<<RXEN3)|(1<<TXEN3)|(1<<RXCIE3)|(1<<UDRIE3)); break;
    #endif
  }
}

