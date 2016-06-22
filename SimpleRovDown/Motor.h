#ifndef MOTOR_H_
#define MOTOR_H_

//**************** Функции работы с таймерами для Servo и ESC ***************//
/// <summary>
/// Инициализация ESC, Servo и моторов.
/// </summary>
void InitEscServoMotor();

static void SetupTimer1ForServo();

static void SetupTimer3ForESC();

static void SetupTimer4ForESC();
//*************** /Функции работы с таймерами для Servo и ESC ***************//

#endif /* MOTOR_H_ */
