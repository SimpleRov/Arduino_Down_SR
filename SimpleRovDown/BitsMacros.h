#ifndef BITSMACROS_H_
#define BITSMACROS_H_

/// <summary>
// Устанавливаем определенный бит регистра или переменой.
/// </summary>
/// <param name="reg">Регистр или переменная.</param>
/// <param name="bitNumber">Номер бита, который будет установлен в 1.</param>
#define MC_SET_BIT(reg, bitNumber)              reg |= (1<<(bitNumber))

/// <summary>
// Получаем определенный бит регистра или переменой.
/// </summary>
/// <param name="reg">Регистр или переменная.</param>
/// <param name="bitNumber">Номер бита, который будет получен.</param>
#define MC_GET_BIT(reg, bitNumber)              (reg & (1<<(bitNumber)))

/// <summary>
// Очищаем определенный бит регистра или переменой.
/// </summary>
/// <param name="reg">Регистр или переменная.</param>
/// <param name="bitNumber">Номер бита, который будет установлен в 0.</param>
#define MC_CLEAR_BIT(reg, bitNumber)            reg &= (~(1<<(bitNumber)))

/// <summary>
// Инвертируем определенный бит регистра или переменой.
/// </summary>
/// <param name="reg">Регистр или переменная.</param>
/// <param name="bitNumber">Номер бита, который будет инвертирован.</param>
#define MC_INV_BIT(reg, bitNumber)              reg ^= (1<<(bitNumber))

/// <summary>
// Проверяем установлен определенный бит регистра или переменой в 1-цу.
/// </summary>
/// <param name="reg">Регистр или переменная.</param>
/// <param name="bitNumber">Номер бита, который будет проверен.</param>
#define MC_BIT_IS_SET(reg, bitNumber)          ((reg & (1<<(bitNumber))) != 0)  

/// <summary>
// Проверяем установлен определенный бит регистра или переменой в 0.
/// </summary>
/// <param name="reg">Регистр или переменная.</param>
/// <param name="bitNumber">Номер бита, который будет проверен.</param>
#define MC_BIT_IS_CLEAR(reg, bitNumber)        ((reg & (1<<(bitNumber))) == 0)

#endif /* BITSMACROS_H_ */
