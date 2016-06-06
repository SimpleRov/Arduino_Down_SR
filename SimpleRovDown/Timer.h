#ifndef TIMER_H_
#define TIMER_H_

/// <summary>
/// Проверка таймера на основе millis
/// </summary>
uint8_t CheckTimerMillis(uint32_t PreviousTimmer, uint32_t Time);

/// <summary>
/// Получение разницы между 2 uint32_t
/// </summary>
uint32_t GetDifferenceULong(uint32_t BeginTime, uint32_t EndTime);

#endif /* TIMER_H_ */
