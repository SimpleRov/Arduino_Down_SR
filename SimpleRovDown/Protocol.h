#ifndef PROTOCOL_H_
#define PROTOCOL_H_

void InitUart(void);

void CheckUart(void);

void Serialize8(uint8_t a);

void Serialize16(int16_t a);

void Serialize32(uint32_t a);

void SerializeStruct(uint8_t *cb,uint8_t siz);

void HeadCmdSend(uint8_t err, uint8_t s, uint8_t c);

void TailCmdSend(void);

#endif /* PROTOCOL_H_ */

