/*
Sketch uses 6 104 bytes (2%) of program storage space. Maximum is 253 952 bytes.
Global variables use 513 bytes (6%) of dynamic memory, leaving 7 679 bytes for local variables. Maximum is 8 192 bytes.
*/

//*******************************  Библиотеки  *******************************//
#include "Arduino.h"

#include "UARTDebug.h"

#include "Protocol.h"

#include "Config.h"

#include "Def.h"

#include "Type.h"

#include "GlobalVar.h"

#include "BitsMacros.h"

#include "FastIO.h"

#include "Timer.h"

#include "Sensors.h"

#include "Motor.h"

//#include <Servo.h>

#include <util/delay.h>
//*******************************  /Библиотеки  ******************************//

//********************** Объявление переменных, констант *********************//
// Переменная хранящая предыдущие значение первого массива кнопок джойстика.
static uint8_t previousBFirst = 0;      

// Переменная хранящая предыдущие значение второго массива кнопок джойстика.
static uint8_t previousBSecond = 0; 

// Состояние обработки команды стика джойстика, стик LY. 
static uint8_t joystickLYWorkStatus = 0;

// Состояние обработки команды стика джойстика, стик RY. 
static uint8_t joystickRYWorkStatus = 0;

// Состояние обработки команды стика джойстика, стик RX.
static uint8_t joystickRXWorkStatus = 0;

// Переменные для определения максимального времени цикла.
uint32_t timeCycleBegin = 0;
uint32_t timeCycle = 0;
uint32_t timeCycleMax = 0;    

/*
// Камера.
Servo CameraTiltServo;  
  
// Тяговый мотор правый. 
Servo ESCMotorRight;   

// Тяговый мотор левый. 
Servo ESCMotorLeft; 

// Мотор подъема\спуска. 
Servo ESCMotorUp;  

// Мотор подъема\спуска. 
Servo ESCMotorLag;  
*/

// Угол поворота сервы камеры.
uint8_t cameraTiltServoAngle = CAMERA_TILT_SERVO_BEGIN_ANGLE;

// Переменная для хранения предыдущего значения millis(), используется для движения камеры вверх.
uint32_t cameraTiltUpPreviousT = 0;
// Переменная для хранения предыдущего значения millis(), используется для движения камеры вниз.
uint32_t cameraTiltDownPreviousT = 0;
//********************** /Объявление переменных, констант ********************//

//******************** Функции работы с кнопками PS2 Joystick ****************//
#define PS2_JOYSTICK_BTN_PRESS(IO, REG, BIT_NUMBER) if (MC_BIT_IS_SET(REG, BIT_NUMBER))\
                                                    {\
                                                      MC_WRITE_PIN(IO, !MC_READ_PIN(IO));\
                                                    }
#define PS2_JOYSTICK_BTN_HOLD(IO, REG, PREV, BIT_NUMBER) if (MC_BIT_IS_SET(REG, BIT_NUMBER))\
                                                    {\
                                                      MC_WRITE_PIN(IO, HIGH);\
                                                    }\
                                                    else if (MC_BIT_IS_SET(PREV, BIT_NUMBER)) \
                                                    {\
                                                      MC_WRITE_PIN(IO, LOW);\
                                                    }
//******************** /Функции работы с кнопками PS2 Joystick ***************//

//****************************** Основные функции ****************************//
/// <summary>
/// Основная функция main.
/// </summary>
int main(void)
{
    init();

    #if defined(USBCON)
      USBDevice.attach();
    #endif
   
    setup();
   
    for (;;) 
    {
      loop();
      // В проекте не используется стандартная библиотека Serial. 
      //if (serialEventRun) serialEventRun();
    }
       
    return 0;
}

/// <summary>
/// Настройка при запуске arduino
/// </summary>
void setup()
{  
  // Инициализация UART.
  InitUart();

  #ifdef DEBUGGING_THROUGH_UART
    DEBUG_PRINTLN(F("Setup Start"));
  #endif

  // Инициализация пинов.
  InitPin();

  // Инициализация ESC, Servo и моторов.
  InitEscServoMotor();

  // Инициализация датчиков.
  InitSensors();
  
  // Немного подождем, вдруг, что-то не успело инициализировать.
  _delay_ms(1000);

  #ifdef DEBUGGING_THROUGH_UART
    DEBUG_PRINTLN(F("Setup End"));
  #endif
}

/// <summary>
/// Основной цикл.
/// </summary>
void loop() 
{
  // Начальное время цикла.
  timeCycleBegin = micros();
  
  CheckUart();

  // Пришли данные с джойстика.
  if (ps2S.statuswork == 1)
  { 
    // Кнопка 1 с фиксацией, нажата кнопка "треугольник", 1 массив.  
    PS2_JOYSTICK_BTN_PRESS(BTN_1_PIN, ps2S.bfirst, 4);
          
    // Кнопка 2 с фиксацией, нажата кнопка "круг", 1 массив.
    PS2_JOYSTICK_BTN_PRESS(BTN_2_PIN, ps2S.bfirst, 5);
          
    // Кнопка 3 с фиксацией, нажата кнопка "крест", 1 массив.
    PS2_JOYSTICK_BTN_PRESS(BTN_3_PIN, ps2S.bfirst, 6);
          
    // Кнопка 4 с фиксацией, нажата кнопка "квадрат", 1 массив.
    PS2_JOYSTICK_BTN_PRESS(BTN_4_PIN, ps2S.bfirst, 7);
          
    // Кнопка 5 без фиксации, нажата кнопка вправо, 1 массив.   
    PS2_JOYSTICK_BTN_HOLD(BTN_5_PIN, ps2S.bfirst, previousBFirst, 1);  
          
    // Кнопка 6 без фиксации, нажата кнопка влево, 1 массив.
    PS2_JOYSTICK_BTN_HOLD(BTN_6_PIN, ps2S.bfirst, previousBFirst, 2);   

    // Поднимаем камеру устанавливаем значение таймера.
    CameraTiltSetTimmer(ps2S.bfirst, 0, &cameraTiltUpPreviousT);
        
    // Опускаем камеру устанавливаем значение таймера.
    CameraTiltSetTimmer(ps2S.bfirst, 3, &cameraTiltDownPreviousT);

    // Работаем со стиками. 
    // Далее идут стики левый и правый. Информация с левого стика должна выполняться только тогда, когда нажата кнопка 9, а с правого - когда. 
    // нажата кнопка 12.
    // Стик 13 (вверх - это всплытие, вниз - погружение).
    // Чем сильнее отклоняется стик вверх - тем быстрее крутятся 2 мотора в одну сторону, чем сильнее отклоняется вниз - тем быстрее вращение в обратную сторону.
    if (MC_BIT_IS_SET(ps2S.bsecond, 0) && ps2S.ly != PS2_JOYSTICK_DEF_VALUE)
    {          
      if (ps2S.ly > PS2_JOYSTICK_DEF_VALUE)
      {
        //ESCMotorUp.writeMicroseconds(map(ps2S.ly, PS2_JOYSTICK_DEF_VALUE-1, 0, ESC_MOTOR_UP_SIGNAL_REVERSE_MIN, ESC_MOTOR_UP_SIGNAL_REVERSE_MAX));
      }
      else
      {
        //ESCMotorUp.writeMicroseconds(map(ps2S.ly, PS2_JOYSTICK_DEF_VALUE+1, 255, ESC_MOTOR_UP_SIGNAL_FORWARD_MIN, ESC_MOTOR_UP_SIGNAL_FORWARD_MAX));
      }
         
      joystickLYWorkStatus = 1; 
    }
    else 
    {
      if (joystickLYWorkStatus == 1)
      {
        joystickLYWorkStatus++;
      }
    }
        
    // Далее идут стики левый и правый. 
    // Информация с левого стика должна выполняться только тогда, когда нажата кнопка 9, а с правого - когда нажата кнопка 12.
    // Стик 14 (движение в горизонтали).
    // вверх - движение вперед.
    // вниз - движение назад.
    if (MC_BIT_IS_SET(ps2S.bsecond, 1) && ps2S.ry != PS2_JOYSTICK_DEF_VALUE && ps2S.rx == PS2_JOYSTICK_DEF_VALUE)
    {          
      if (ps2S.ry > PS2_JOYSTICK_DEF_VALUE)
      {
        //ESCMotorRight.writeMicroseconds(map(ps2S.ry, PS2_JOYSTICK_DEF_VALUE-1, 0, ESC_MOTOR_RIGHT_SIGNAL_REVERSE_MIN, ESC_MOTOR_RIGHT_SIGNAL_REVERSE_MAX));
        //ESCMotorLeft.writeMicroseconds(map(ps2S.ry, PS2_JOYSTICK_DEF_VALUE-1, 0, ESC_MOTOR_LEFT_SIGNAL_REVERSE_MIN, ESC_MOTOR_LEFT_SIGNAL_REVERSE_MAX));
      }
      else
      {
        //ESCMotorRight.writeMicroseconds(map(ps2S.ry, PS2_JOYSTICK_DEF_VALUE+1, 255, ESC_MOTOR_RIGHT_SIGNAL_FORWARD_MIN, ESC_MOTOR_RIGHT_SIGNAL_FORWARD_MAX));
        //ESCMotorLeft.writeMicroseconds(map(ps2S.ry, PS2_JOYSTICK_DEF_VALUE+1, 255, ESC_MOTOR_LEFT_SIGNAL_FORWARD_MIN, ESC_MOTOR_LEFT_SIGNAL_FORWARD_MAX));
      }
        
      joystickRYWorkStatus = 1;
    }
    else 
    {
      if (joystickRYWorkStatus == 1)
      {
        joystickRYWorkStatus++;
      }
    }
        
    // Далее идут стики левый и правый. 
    // Информация с левого стика должна выполняться только тогда, когда нажата кнопка 9, а с правого - когда нажата кнопка 12.
    // Стик 14 (движение в горизонтали).
    // влево - вращение на месте налево.
    // вправо - движение на месте направо.
    if (MC_BIT_IS_SET(ps2S.bsecond, 1) && ps2S.rx != PS2_JOYSTICK_DEF_VALUE && ps2S.ry == PS2_JOYSTICK_DEF_VALUE)
    {
      if (ps2S.rx < PS2_JOYSTICK_DEF_VALUE)
      {
        //ESCMotorLeft.writeMicroseconds(map(ps2S.rx, 0, PS2_JOYSTICK_DEF_VALUE-1, ESC_MOTOR_LEFT_SIGNAL_REVERSE_MIN, ESC_MOTOR_LEFT_SIGNAL_REVERSE_MAX));
        //ESCMotorRight.writeMicroseconds(map(ps2S.rx, 0, PS2_JOYSTICK_DEF_VALUE-1, ESC_MOTOR_RIGHT_SIGNAL_FORWARD_MIN, ESC_MOTOR_RIGHT_SIGNAL_FORWARD_MAX));
      }
      else
      {     
        //ESCMotorLeft.writeMicroseconds(map(ps2S.rx, PS2_JOYSTICK_DEF_VALUE+1, 255, ESC_MOTOR_LEFT_SIGNAL_FORWARD_MIN, ESC_MOTOR_LEFT_SIGNAL_FORWARD_MAX));
        //ESCMotorRight.writeMicroseconds(map(ps2S.rx, PS2_JOYSTICK_DEF_VALUE+1, 255, ESC_MOTOR_RIGHT_SIGNAL_REVERSE_MIN, ESC_MOTOR_RIGHT_SIGNAL_REVERSE_MAX));
      }
          
      joystickRXWorkStatus = 1;
    }
    else 
    {
      if (joystickRXWorkStatus == 1)
      {
        joystickRXWorkStatus++;
      }
    }
    
    #ifdef DEBUGGING_THROUGH_UART
      DEBUG_PRINTLN(F("Data Exist"));
    #endif
    
    previousBFirst = ps2S.bfirst;
    previousBSecond = ps2S.bsecond;
        
    ps2S.statuswork = 2;  
    ps2S.bfirst = ps2S.bsecond = 0;
  }

  if (ps2S.statuswork == 2)
  {
    RovSendAnswer();
    ps2S.statuswork = 3;
  }

  // Работаем с камерой.
  if ((MC_BIT_IS_SET(previousBFirst, 0)) || (MC_BIT_IS_SET(previousBFirst, 3)))
  {
    // Поднимаем камеру.
    if (MC_BIT_IS_SET(previousBFirst, 0) && CheckTimerMillis(cameraTiltUpPreviousT, CAMERA_TILT_SERVO_ANGLE_CHANGE_DELAY))
    {
      cameraTiltUpPreviousT = millis();
          
      if (cameraTiltServoAngle < 180)
      {
        cameraTiltServoAngle++;  
        //CameraTiltServo.write(cameraTiltServoAngle); 
      }
    }
      
    // Опускаем камеру.
    if (MC_BIT_IS_SET(previousBFirst, 3) && CheckTimerMillis(cameraTiltDownPreviousT, CAMERA_TILT_SERVO_ANGLE_CHANGE_DELAY))
    {   
      cameraTiltDownPreviousT = millis();

      if (cameraTiltServoAngle)
      {
        cameraTiltServoAngle--;
        //CameraTiltServo.write(cameraTiltServoAngle);
      }
    }
  }

  // Работаем со стиками. 
  // Далее идут стики левый и правый.
  // Информация с левого стика должна выполняться только тогда, когда нажата кнопка 9, а с правого - когда нажата кнопка 12.
  // Стик 13 (вверх - это всплытие, вниз - погружение).
  // Чем сильнее отклоняется стик вверх - тем быстрее крутятся 2 мотора в одну сторону, чем сильнее отклоняется вниз - тем быстрее вращение в обратную сторону.
  //TODO: Вынести в отдельную функцию
  if (joystickLYWorkStatus == 2)
  {      
    //ESCMotorUp.writeMicroseconds(ESC_MOTOR_UP_SIGNAL_STOP);
    //ESCMotorUp.writeMicroseconds(ESC_MOTOR_UP_SIGNAL_STOP);
        
    joystickLYWorkStatus = 0;
  }
    
  // Далее идут стики левый и правый. 
  // Информация с левого стика должна выполняться только тогда, когда нажата кнопка 9, а с правого - когда нажата кнопка 12.
  // Стик 14 (движение в горизонтали).
  // вверх - движение вперед.
  // вниз - движение назад.

  //TODO: Вынести в отдельную функцию
  if (joystickRYWorkStatus == 2)
  {      
    //ESCMotorRight.writeMicroseconds(ESC_MOTOR_RIGHT_SIGNAL_STOP);
    //ESCMotorLeft.writeMicroseconds(ESC_MOTOR_LEFT_SIGNAL_STOP);
        
    joystickRYWorkStatus = 0;
  }
    
  // Далее идут стики левый и правый. 
  // Информация с левого стика должна выполняться только тогда, когда нажата кнопка 9, а с правого - когда нажата кнопка 12.
  // Стик 14 (движение в горизонтали).
  // влево - вращение на месте налево.
  // вправо - движение на месте направо.
  //TODO: Вынести в отдельную функцию
  if (joystickRXWorkStatus == 2)
  {      
    //ESCMotorRight.writeMicroseconds(ESC_MOTOR_RIGHT_SIGNAL_STOP);
    //ESCMotorLeft.writeMicroseconds(ESC_MOTOR_LEFT_SIGNAL_STOP);
        
    joystickRXWorkStatus = 0;
  }
  
  // Тест ADC.
  //AdcSensorsSendDataInStruct();
  
  // Тест MS580330BA.
  //Ms580330BaSendDataInStruct();
  
  #ifdef DEBUG_SPEED_CYCLE
    // Определяем скорость работы.
    timeCycle = GetDifferenceULong(timeCycleBegin, micros());
    // Если текущее значение больше, последнего максимального, отображаем его.
    if (timeCycle > timeCycleMax)
    {
      timeCycleMax = timeCycle;

      #ifdef DEBUGGING_THROUGH_UART
        DEBUG_PRINT("Max - ");
        DEBUG_PRINTLN(timeCycleMax);
      #endif
    }
  #endif
}
//************************** Функции ответа Rov по UART **********************//
void RovSendAnswer(void)
{ 
  rovDataS.errore = 1;    

  SendStruct((uint8_t*)&rovDataS, 1, 2);
}
//************************** /Функции ответа Rov по UART *********************//

//************************* Функции инициализации портов *********************//
// Инициализация пинов.
void InitPin()
{
  MC_SET_PIN_OUTPUT(BTN_1_PIN);
    
  MC_SET_PIN_OUTPUT(BTN_2_PIN);
  
  MC_SET_PIN_OUTPUT(BTN_3_PIN);

  MC_SET_PIN_OUTPUT(BTN_4_PIN);
  
  MC_SET_PIN_OUTPUT(BTN_5_PIN);

  MC_SET_PIN_OUTPUT(BTN_6_PIN);
}
//************************* /Функции инициализации портов ********************//

//****************************** Функции Servo и ESC  ************************//
/// <summary>
//// Инициализация ESC, Servo и моторов.
///// </summary>
//void InitEscServoMotor(void)
//{
//  SetupTimer1ForServo();
//  SetupTimer3ForESC();
//  SetupTimer4ForESC();
//
//  /*
//  // Инициализируем камеру.
//  // Устанавливаем сервопривод камеры в среднее положение.
//  ServoEscAttachAndInitInitialPosition(&CameraTiltServo, CAMERA_TILT_SERVO_PIN, CAMERA_TILT_SERVO_BEGIN_ANGLE);
//
//  // Инициализируем тяговый мотор правый.
//  ServoEscAttachAndInitInitialPosition(&ESCMotorRight, ESC_MOTOR_RIGHT_PIN, ESC_MOTOR_RIGHT_SIGNAL_STOP);
//
//  // Инициализируем тяговый мотор левый.
//  ServoEscAttachAndInitInitialPosition(&ESCMotorLeft, ESC_MOTOR_LEFT_PIN, ESC_MOTOR_LEFT_SIGNAL_STOP);
//
//  // Инициализируем мотор подъема\спуска.
//  ServoEscAttachAndInitInitialPosition(&ESCMotorUp, ESC_MOTOR_UP_PIN, ESC_MOTOR_UP_SIGNAL_STOP);
//
//  // Инициализируем лаговый мотор.
//  ServoEscAttachAndInitInitialPosition(&ESCMotorLag, ESC_MOTOR_LAG_PIN, ESC_MOTOR_LAG_SIGNAL_STOP);
//  */
//}
//****************************** /Функции Servo и ESC  ***********************//

//************************* Функции работы с Servo и ESC *********************//
/*void ServoEscAttachAndInitInitialPosition(Servo* servo, uint8_t servoPin, uint16_t initialPosition)
{
  servo->attach(servoPin);
  servo->write(initialPosition);
}*/

//**************************** Функции работы с камерой ***********************//
void CameraTiltSetTimmer(uint8_t reg, uint8_t bitNumber, uint32_t* previousT)
{
  if (MC_BIT_IS_SET(reg, bitNumber))
  { 
    *previousT = millis();
  }
}
//**************************** /Функции работы с камерой **********************//

//************************* /Функции работы с Servo и ESC *********************//
