/******************************************************************************
     * Based on the file Protocol.h project Multiwii. 
     * Author project Multiwii - Alexandre Dubus.
     * Version on commit e957a70 on 24 Feb 2016.
     * Project description Multiwii - http://www.multiwii.com. 
     * You can find the code of the Multiwii project here - https://github.com/multiwii/multiwii-firmware.
     * License GNU GPL v3
******************************************************************************/

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

void InitUart(void);

void CheckUart(void);

void SendStruct(uint8_t *st,uint8_t siz, uint8_t c);

#endif /* PROTOCOL_H_ */

