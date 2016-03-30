#ifndef CONFIG_H_
#define CONFIG_H_

//************************ Тип конфигурации Rov *****************************//
// Simple Rov.
#define ROV_CONF_SIMPLE                                
//************************ /Тип конфигурации Rov ****************************//

//********************** Тип конфигурации моторов ***************************//   
// 2 ходовых мотора, 1 на подъем, смещен к ходовым.
#define MOTOR_CONF_2FORWARD_1UP_DISPLACED
//********************** Тип конфигурации моторов ***************************//

//*************************** Настройки UART ********************************//
// Порт UART.
#define UART_PORT           1       

// Скорость порта UART.
#define UART_PORT_SPEED     115200UL                      
//*************************** /Настройки UART *******************************//

//*****************************  TWI скорость  *******************************//
// TWI скорость - 400kHz 
//#define TWI_SPEED                       400000UL    
// TWI скорость - 100kHz
#define TWI_SPEED                       100000UL
//*****************************  /TWI скорость  ******************************//

//******************** Встроенные подтягивающие рез. I2C ********************//
#define TWI_INTERNAL_PULLUPS
//******************* /Встроенные подтягивающие рез. I2C ********************//

//******************************** Тесты ************************************//
// Отладка через UART.
#define DEBUGGING_THROUGH_UART

// Отладка через LED.
//#define LED_DEBUG

// Тест скорости цикла.
//#define DEBUG_SPEED_CYCLE
//******************************** /Тесты ***********************************//

//************************ Настройки Debug UART *****************************//
#ifdef DEBUGGING_THROUGH_UART
  // Скорость порта UART.
  #define UART_DEBUG_SPEED        115200UL    
#endif        
//************************ /Настройки Debug UART ****************************//

#endif /* CONFIG_H_ */

