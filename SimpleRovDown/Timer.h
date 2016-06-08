#ifndef TIMER_H_
#define TIMER_H_

//********* Функции работы с таймерами на основе millis(), micros() *********//

/// <summary>
/// Проверка таймера на основе millis
/// </summary>
uint8_t CheckTimerMillis(uint32_t PreviousTimmer, uint32_t Time);

/// <summary>
/// Получение разницы между 2 uint32_t
/// </summary>
uint32_t GetDifferenceULong(uint32_t BeginTime, uint32_t EndTime);

//********* /Функции работы с таймерами на основе millis(), micros() ********//

//**************** Функции работы с таймерами для Servo и ESC ***************//
void SetupTimer1ForServo();

void SetupTimer3ForESC();

void SetupTimer4ForESC();
//*************** /Функции работы с таймерами для Servo и ESC ***************//

#endif /* TIMER_H_ */
