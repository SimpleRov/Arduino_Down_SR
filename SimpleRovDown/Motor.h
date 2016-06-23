#ifndef MOTOR_H_
#define MOTOR_H_

//**************** Функции работы с таймерами для Servo и ESC ***************//
/// <summary>
/// Инициализация ESC, Servo и моторов.
/// </summary>
void InitEscServoMotor();

static void SetupTimer1ForServo();

void SetNewValueServo(uint8_t servoNubmer, uint16_t newValue);

void SetNewValueESC(uint8_t escNubmer, uint16_t newValue);

static void SetupTimer3ForESC();

static void SetupTimer4ForESC();
//*************** /Функции работы с таймерами для Servo и ESC ***************//

#endif /* MOTOR_H_ */
