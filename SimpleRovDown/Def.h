#ifndef DEF_H_
#define DEF_H_

//***********************  Определяем тип ардуины  **************************//
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega2560__)
  
  // Частота процессора.
  #ifndef F_CPU
    #define F_CPU 16000000UL   
  #endif                             

  // Определяем тип ардуины.
  #if defined(__AVR_ATmega328P__)
    // Если ArduinoProMini.
    #define ARDUINO_PRO_MINI
  #endif
  #if defined(__AVR_ATmega32U4__)
    // Если ArduinoProMicro.
    #define ARDUINO_PRO_MICRO
  #endif
  #if defined(__AVR_ATmega2560__)
    // Если ArduinoMega.
    #define ARDUINO_MEGA_2560
  #endif
#else
  #error This board not support!
#endif
//***********************  /Определяем тип ардуины  *************************//

//*********************  Макросы для атомарных операций  ********************//
#ifndef MC_CRITICAL_SECTION_START
  #define MC_CRITICAL_SECTION_START  cli();      
#endif

#ifndef MC_CRITICAL_SECTION_END     
  #define MC_CRITICAL_SECTION_END    sei();
#endif
//*********************  /Макросы для атомарных операций  *******************//

//******************  Soft Serial в ATmega32U4 (Promicro)  ******************//
// Настройки пинов для виртуального UART.
#if defined(ARDUINO_PRO_MICRO)
  #define USB_CDC_TX        3
  #define USB_CDC_RX        2
#endif
//******************  /Soft Serial в ATmega32U4 (Promicro)  *****************//

//********************************  TWI (I2C)  ******************************//
#if defined(ARDUINO_PRO_MINI)
  // Если ArduinoProMini.
  
  // PIN A4&A5 (SDA&SCL).
  #define TWI_PULLUPS_ENABLE         PORTC |= 1<<4; PORTC |= 1<<5;   
  #define TWI_PULLUPS_DISABLE        PORTC &= ~(1<<4); PORTC &= ~(1<<5);
#endif
#if defined(ARDUINO_PRO_MICRO)
  // Если ArduinoProMicro.
  
  // PIN 2&3 (SDA&SCL).
  #define TWI_PULLUPS_ENABLE         PORTD |= 1<<0; PORTD |= 1<<1;   
  #define TWI_PULLUPS_DISABLE        PORTD &= ~(1<<0); PORTD &= ~(1<<1);
#endif
#if defined(ARDUINO_MEGA_2560)
  // Если ArduinoMega.
  
  // PIN 20&21 (SDA&SCL).
  #define TWI_PULLUPS_ENABLE         PORTD |= 1<<0; PORTD |= 1<<1;       
  #define TWI_PULLUPS_DISABLE        PORTD &= ~(1<<0); PORTD &= ~(1<<1);
#endif
//*******************************  /TWI (I2C)  ******************************//

//****************************  Объявление пинов  ***************************//
#define BTN_1_PIN            13  

#define BTN_2_PIN            13

#define BTN_3_PIN            13  

#define BTN_4_PIN            13  

#define BTN_5_PIN            13

#define BTN_6_PIN            13
//****************************  /Объявление пинов  **************************//

#endif /* DEF_H_ */
