/******************************************************************************
     * Based on the file fastio.h project Marlin. 
     * Author project Marlin - Erik van der Zalm (Email - erik@vdzalm.eu).
     * Version on commit 122bdd1 25 Mar 2016.
     * Project description Marlin - http://reprap.org/wiki/Marlin. 
     * You can find the code of the Marlin project here - https://github.com/MarlinFirmware/Marlin.
     * License GNU GPL v3
******************************************************************************/

#ifndef FastIO_H_
#define FastIO_H_

//*******************************  Библиотеки  ******************************//

// Библиотека ввода вывода.
#include <avr/io.h>

//******************************* /Библиотеки  ******************************//

#ifndef MC_MASK_PIN
  #define MC_MASK_PIN(PIN)  (1 << PIN)
#endif

#ifndef MC_CRITICAL_SECTION_START
  #define MC_CRITICAL_SECTION_START  unsigned char _sreg = SREG; cli();         
#endif

#ifndef MC_CRITICAL_SECTION_END
  #define MC_CRITICAL_SECTION_END    SREG = _sreg;
#endif

#define MC_READ_PIN_(IO) ((bool)(DIO ## IO ## _RPORT & MC_MASK_PIN(DIO ## IO ## _PIN)))

#define MC_WRITE_PIN_NC_(IO, v)  do { if (v) {DIO ##  IO ## _WPORT |= MC_MASK_PIN(DIO ## IO ## _PIN); } else {DIO ##  IO ## _WPORT &= ~MC_MASK_PIN(DIO ## IO ## _PIN); }; } while (0)

#define MC_WRITE_PIN_C_(IO, v)   do { if (v) { \
                                         MC_CRITICAL_SECTION_START; \
                                         {DIO ##  IO ## _WPORT |= MC_MASK_PIN(DIO ## IO ## _PIN); }\
                                         MC_CRITICAL_SECTION_END; \
                                       }\
                                       else {\
                                         MC_CRITICAL_SECTION_START; \
                                         {DIO ##  IO ## _WPORT &= ~MC_MASK_PIN(DIO ## IO ## _PIN); }\
                                         MC_CRITICAL_SECTION_END; \
                                       }\
                                     }\
                                     while (0)
                                     
#define MC_WRITE_PIN_(IO, v)  do {  if (&(DIO ##  IO ## _RPORT) >= (uint8_t *)0x100) {MC_WRITE_PIN_C_(IO, v); } else {MC_WRITE_PIN_NC_(IO, v); }; } while (0)

#define MC_TOGGLE_PIN_(IO) do {DIO ##  IO ## _RPORT = MC_MASK_PIN(DIO ## IO ## _PIN); } while (0)

#define MC_SET_PIN_INPUT_(IO) do {DIO ##  IO ## _DDR &= ~MC_MASK_PIN(DIO ## IO ## _PIN); } while (0)

#define MC_SET_PIN_OUTPUT_(IO) do {DIO ##  IO ## _DDR |=  MC_MASK_PIN(DIO ## IO ## _PIN); } while (0)

#define MC_GET_PIN_INPUT_(IO) ((DIO ## IO ## _DDR & MC_MASK_PIN(DIO ## IO ## _PIN)) == 0)

#define MC_GET_PIN_OUTPUT_(IO) ((DIO ## IO ## _DDR & MC_MASK_PIN(DIO ## IO ## _PIN)) != 0)

#define MC_READ_PIN(IO) MC_READ_PIN_(IO)

#define MC_WRITE_PIN(IO, v) MC_WRITE_PIN_(IO, v)

#define MC_TOGGLE_PIN(IO) MC_TOGGLE_PIN_(IO)

#define MC_SET_PIN_INPUT(IO) MC_SET_PIN_INPUT_(IO)

#define MC_SET_PIN_OUTPUT(IO) MC_SET_PIN_OUTPUT_(IO)

#define MC_GET_PIN_INPUT(IO) MC_GET_PIN_INPUT_(IO)

#define MC_GET_PIN_OUTPUT(IO) MC_GET_PIN_OUTPUT_(IO)

#define MC_SET_PIN_OUTPUT_SET_PIN(IO, v) (MC_SET_PIN_OUTPUT(IO) MC_WRITE_PIN(IO, v))

#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
//Pins
#define DIO0_PIN            PIND0
#define DIO0_RPORT          PIND
#define DIO0_WPORT          PORTD
#define DIO0_DDR            DDRD

#define DIO1_PIN            PIND1
#define DIO1_RPORT          PIND
#define DIO1_WPORT          PORTD
#define DIO1_DDR            DDRD

#define DIO2_PIN            PIND2
#define DIO2_RPORT          PIND
#define DIO2_WPORT          PORTD
#define DIO2_DDR            DDRD

#define DIO3_PIN            PIND3
#define DIO3_RPORT          PIND
#define DIO3_WPORT          PORTD
#define DIO3_DDR            DDRD

#define DIO4_PIN            PIND4
#define DIO4_RPORT          PIND
#define DIO4_WPORT          PORTD
#define DIO4_DDR            DDRD

#define DIO5_PIN            PIND5
#define DIO5_RPORT          PIND
#define DIO5_WPORT          PORTD
#define DIO5_DDR            DDRD

#define DIO6_PIN            PIND6
#define DIO6_RPORT          PIND
#define DIO6_WPORT          PORTD
#define DIO6_DDR            DDRD

#define DIO7_PIN            PIND7
#define DIO7_RPORT          PIND
#define DIO7_WPORT          PORTD
#define DIO7_DDR            DDRD

#define DIO8_PIN            PINB0
#define DIO8_RPORT          PINB
#define DIO8_WPORT          PORTB
#define DIO8_DDR            DDRB

#define DIO9_PIN            PINB1
#define DIO9_RPORT          PINB
#define DIO9_WPORT          PORTB
#define DIO9_DDR            DDRB

#define DIO10_PIN           PINB2
#define DIO10_RPORT         PINB
#define DIO10_WPORT         PORTB
#define DIO10_DDR           DDRB

#define DIO11_PIN           PINB3
#define DIO11_RPORT         PINB
#define DIO11_WPORT         PORTB
#define DIO11_DDR           DDRB

#define DIO12_PIN           PINB4
#define DIO12_RPORT         PINB
#define DIO12_WPORT         PORTB
#define DIO12_DDR           DDRB

#define DIO13_PIN           PINB5
#define DIO13_RPORT         PINB
#define DIO13_WPORT         PORTB
#define DIO13_DDR           DDRB

#define DIO14_PIN           PINC0
#define DIO14_RPORT         PINC
#define DIO14_WPORT         PORTC
#define DIO14_DDR           DDRC

#define DIO15_PIN           PINC1
#define DIO15_RPORT         PINC
#define DIO15_WPORT         PORTC
#define DIO15_DDR           DDRC

#define DIO16_PIN           PINC2
#define DIO16_RPORT         PINC
#define DIO16_WPORT         PORTC
#define DIO16_DDR           DDRC

#define DIO17_PIN           PINC3
#define DIO17_RPORT         PINC
#define DIO17_WPORT         PORTC
#define DIO17_DDR           DDRC

#define DIO18_PIN           PINC4
#define DIO18_RPORT         PINC
#define DIO18_WPORT         PORTC
#define DIO18_DDR           DDRC

#define DIO19_PIN           PINC5
#define DIO19_RPORT         PINC
#define DIO19_WPORT         PORTC
#define DIO19_DDR           DDRC

#define DIO20_PIN           PINC6
#define DIO20_RPORT         PINC
#define DIO20_WPORT         PORTC
#define DIO20_DDR           DDRC

#define DIO21_PIN           PINC7
#define DIO21_RPORT         PINC
#define DIO21_WPORT         PORTC
#define DIO21_DDR           DDRC

#undef PB0
#define PB0_PIN             PINB0
#define PB0_RPORT           PINB
#define PB0_WPORT           PORTB
#define PB0_DDR             DDRB

#undef PB1
#define PB1_PIN             PINB1
#define PB1_RPORT           PINB
#define PB1_WPORT           PORTB
#define PB1_DDR             DDRB

#undef PB2
#define PB2_PIN             PINB2
#define PB2_RPORT           PINB
#define PB2_WPORT           PORTB
#define PB2_DDR             DDRB

#undef PB3
#define PB3_PIN             PINB3
#define PB3_RPORT           PINB
#define PB3_WPORT           PORTB
#define PB3_DDR             DDRB

#undef PB4
#define PB4_PIN             PINB4
#define PB4_RPORT           PINB
#define PB4_WPORT           PORTB
#define PB4_DDR             DDRB

#undef PB5
#define PB5_PIN             PINB5
#define PB5_RPORT           PINB
#define PB5_WPORT           PORTB
#define PB5_DDR             DDRB

#undef PB6
#define PB6_PIN             PINB6
#define PB6_RPORT           PINB
#define PB6_WPORT           PORTB
#define PB6_DDR             DDRB

#undef PB7
#define PB7_PIN             PINB7
#define PB7_RPORT           PINB
#define PB7_WPORT           PORTB
#define PB7_DDR             DDRB

#undef PC0
#define PC0_PIN             PINC0
#define PC0_RPORT           PINC
#define PC0_WPORT           PORTC
#define PC0_DDR             DDRC

#undef PC1
#define PC1_PIN             PINC1
#define PC1_RPORT           PINC
#define PC1_WPORT           PORTC
#define PC1_DDR             DDRC

#undef PC2
#define PC2_PIN             PINC2
#define PC2_RPORT           PINC
#define PC2_WPORT           PORTC
#define PC2_DDR             DDRC

#undef PC3
#define PC3_PIN             PINC3
#define PC3_RPORT           PINC
#define PC3_WPORT           PORTC
#define PC3_DDR             DDRC

#undef PC4
#define PC4_PIN             PINC4
#define PC4_RPORT           PINC
#define PC4_WPORT           PORTC
#define PC4_DDR             DDRC

#undef PC5
#define PC5_PIN             PINC5
#define PC5_RPORT           PINC
#define PC5_WPORT           PORTC
#define PC5_DDR             DDRC

#undef PC6
#define PC6_PIN             PINC6
#define PC6_RPORT           PINC
#define PC6_WPORT           PORTC
#define PC6_DDR             DDRC

#undef PC7
#define PC7_PIN             PINC7
#define PC7_RPORT           PINC
#define PC7_WPORT           PORTC
#define PC7_DDR             DDRC

#undef PD0
#define PD0_PIN             PIND0
#define PD0_RPORT           PIND
#define PD0_WPORT           PORTD
#define PD0_DDR             DDRD

#undef PD1
#define PD1_PIN             PIND1
#define PD1_RPORT           PIND
#define PD1_WPORT           PORTD
#define PD1_DDR             DDRD

#undef PD2
#define PD2_PIN             PIND2
#define PD2_RPORT           PIND
#define PD2_WPORT           PORTD
#define PD2_DDR             DDRD

#undef PD3
#define PD3_PIN             PIND3
#define PD3_RPORT           PIND
#define PD3_WPORT           PORTD
#define PD3_DDR             DDRD

#undef PD4
#define PD4_PIN             PIND4
#define PD4_RPORT           PIND
#define PD4_WPORT           PORTD
#define PD4_DDR             DDRD

#undef PD5
#define PD5_PIN             PIND5
#define PD5_RPORT           PIND
#define PD5_WPORT           PORTD
#define PD5_DDR             DDRD

#undef PD6
#define PD6_PIN             PIND6
#define PD6_RPORT           PIND
#define PD6_WPORT           PORTD
#define PD6_DDR             DDRD

#undef PD7
#define PD7_PIN             PIND7
#define PD7_RPORT           PIND
#define PD7_WPORT           PORTD
#define PD7_DDR             DDRD

#undef PF0
#define PF0_PIN             PINF0
#define PF0_RPORT           PINF
#define PF0_WPORT           PORTF
#define PF0_DDR             DDRF

#undef PF1
#define PF1_PIN             PINF1
#define PF1_RPORT           PINF
#define PF1_WPORT           PORTF
#define PF1_DDR             DDRF

#undef PF2
#define PF2_PIN             PINF2
#define PF2_RPORT           PINF
#define PF2_WPORT           PORTF
#define PF2_DDR             DDRF

#undef PF3
#define PF3_PIN             PINF3
#define PF3_RPORT           PINF
#define PF3_WPORT           PORTF
#define PF3_DDR             DDRF

#undef PF4
#define PF4_PIN             PINF4
#define PF4_RPORT           PINF
#define PF4_WPORT           PORTF
#define PF4_DDR             DDRF

#undef PF5
#define PF5_PIN             PINF5
#define PF5_RPORT           PINF
#define PF5_WPORT           PORTF
#define PF5_DDR             DDRF

#undef PF6
#define PF6_PIN             PINF6
#define PF6_RPORT           PINF
#define PF6_WPORT           PORTF
#define PF6_DDR             DDRF

#undef PF7
#define PF7_PIN             PINF7
#define PF7_RPORT           PINF
#define PF7_WPORT           PORTF
#define PF7_DDR             DDRF

#endif

#if defined (__AVR_ATmega32U4__)
//Pins
#define DIO0_PIN            PIND2
#define DIO0_RPORT          PIND
#define DIO0_WPORT          PORTD
#define DIO0_DDR            DDRD

#define DIO1_PIN            PIND3
#define DIO1_RPORT          PIND
#define DIO1_WPORT          PORTD
#define DIO1_DDR            DDRD

#define DIO2_PIN            PIND1
#define DIO2_RPORT          PIND
#define DIO2_WPORT          PORTD
#define DIO2_DDR            DDRD

#define DIO3_PIN            PIND3
#define DIO3_RPORT          PIND
#define DIO3_WPORT          PORTD
#define DIO3_DDR            DDRD

#define DIO4_PIN            PIND4
#define DIO4_RPORT          PIND
#define DIO4_WPORT          PORTD
#define DIO4_DDR            DDRD

#define DIO5_PIN            PINC6
#define DIO5_RPORT          PINC
#define DIO5_WPORT          PORTC
#define DIO5_DDR            DDRC

#define DIO6_PIN            PIND7
#define DIO6_RPORT          PIND
#define DIO6_WPORT          PORTD
#define DIO6_DDR            DDRD

#define DIO7_PIN            PINE6
#define DIO7_RPORT          PINE
#define DIO7_WPORT          PORTE
#define DIO7_DDR            DDRE

#define DIO8_PIN            PINB4
#define DIO8_RPORT          PINB
#define DIO8_WPORT          PORTB
#define DIO8_DDR            DDRB

#define DIO9_PIN            PINB5
#define DIO9_RPORT          PINB
#define DIO9_WPORT          PORTB
#define DIO9_DDR            DDRB

#define DIO10_PIN           PINB6
#define DIO10_RPORT         PINB
#define DIO10_WPORT         PORTB
#define DIO10_DDR           DDRB

#define DIO11_PIN           PINB7
#define DIO11_RPORT         PINB
#define DIO11_WPORT         PORTB
#define DIO11_DDR           DDRB

#define DIO12_PIN           PIND6
#define DIO12_RPORT         PIND
#define DIO12_WPORT         PORTD
#define DIO12_DDR           DDRD

#define DIO13_PIN           PINC7
#define DIO13_RPORT         PINC
#define DIO13_WPORT         PORTC
#define DIO13_DDR           DDRC

#define DIO14_PIN           PINB3
#define DIO14_RPORT         PINB
#define DIO14_WPORT         PORTB
#define DIO14_DDR           DDRB

#define DIO15_PIN           PINB1
#define DIO15_RPORT         PINB
#define DIO15_WPORT         PORTB
#define DIO15_DDR           DDRB

#define DIO16_PIN           PINB2
#define DIO16_RPORT         PINB
#define DIO16_WPORT         PORTB
#define DIO16_DDR           DDRB

#define DIO17_PIN           PINB0
#define DIO17_RPORT         PINB
#define DIO17_WPORT         PORTB
#define DIO17_DDR           DDRB

#define DIO18_PIN           PINF7
#define DIO18_RPORT         PINF
#define DIO18_WPORT         PORTF
#define DIO18_DDR           DDRF

#define DIO19_PIN           PINF6
#define DIO19_RPORT         PINF
#define DIO19_WPORT         PORTF
#define DIO19_DDR           DDRF

#define DIO20_PIN           PINF5
#define DIO20_RPORT         PINF
#define DIO20_WPORT         PORTF
#define DIO20_DDR           DDRF

#define DIO21_PIN           PINF4
#define DIO21_RPORT         PINF
#define DIO21_WPORT         PORTF
#define DIO21_DDR           DDRF

#define DIO22_PIN           PINF1
#define DIO22_RPORT         PINF
#define DIO22_WPORT         PORTF
#define DIO22_DDR           DDRF

#define DIO23_PIN           PINF0
#define DIO23_RPORT         PINF
#define DIO23_WPORT         PORTF
#define DIO23_DDR           DDRF

#undef PB0
#define PB0_PIN             PINB0
#define PB0_RPORT           PINB
#define PB0_WPORT           PORTB
#define PB0_DDR             DDRB

#undef PB1
#define PB1_PIN             PINB1
#define PB1_RPORT           PINB
#define PB1_WPORT           PORTB
#define PB1_DDR             DDRB

#undef PB2
#define PB2_PIN             PINB2
#define PB2_RPORT           PINB
#define PB2_WPORT           PORTB
#define PB2_DDR             DDRB

#undef PB3
#define PB3_PIN             PINB3
#define PB3_RPORT           PINB
#define PB3_WPORT           PORTB
#define PB3_DDR             DDRB

#undef PB4
#define PB4_PIN             PINB4
#define PB4_RPORT           PINB
#define PB4_WPORT           PORTB
#define PB4_DDR             DDRB

#undef PB5
#define PB5_PIN             PINB5
#define PB5_RPORT           PINB
#define PB5_WPORT           PORTB
#define PB5_DDR             DDRB

#undef PB6
#define PB6_PIN             PINB6
#define PB6_RPORT           PINB
#define PB6_WPORT           PORTB
#define PB6_DDR             DDRB

#undef PB7
#define PB7_PIN             PINB7
#define PB7_RPORT           PINB
#define PB7_WPORT           PORTB
#define PB7_DDR             DDRB

#undef PC0
#define PC0_PIN             PINC0
#define PC0_RPORT           PINC
#define PC0_WPORT           PORTC
#define PC0_DDR             DDRC

#undef PC1
#define PC1_PIN             PINC1
#define PC1_RPORT           PINC
#define PC1_WPORT           PORTC
#define PC1_DDR             DDRC

#undef PC2
#define PC2_PIN             PINC2
#define PC2_RPORT           PINC
#define PC2_WPORT           PORTC
#define PC2_DDR             DDRC

#undef PC3
#define PC3_PIN             PINC3
#define PC3_RPORT           PINC
#define PC3_WPORT           PORTC
#define PC3_DDR             DDRC

#undef PC4
#define PC4_PIN             PINC4
#define PC4_RPORT           PINC
#define PC4_WPORT           PORTC
#define PC4_DDR             DDRC

#undef PC5
#define PC5_PIN             PINC5
#define PC5_RPORT           PINC
#define PC5_WPORT           PORTC
#define PC5_DDR             DDRC

#undef PC6
#define PC6_PIN             PINC6
#define PC6_RPORT           PINC
#define PC6_WPORT           PORTC
#define PC6_DDR             DDRC

#undef PC7
#define PC7_PIN             PINC7
#define PC7_RPORT           PINC
#define PC7_WPORT           PORTC
#define PC7_DDR             DDRC

#undef PD0
#define PD0_PIN             PIND0
#define PD0_RPORT           PIND
#define PD0_WPORT           PORTD
#define PD0_DDR             DDRD

#undef PD1
#define PD1_PIN             PIND1
#define PD1_RPORT           PIND
#define PD1_WPORT           PORTD
#define PD1_DDR             DDRD

#undef PD2
#define PD2_PIN             PIND2
#define PD2_RPORT           PIND
#define PD2_WPORT           PORTD
#define PD2_DDR             DDRD

#undef PD3
#define PD3_PIN             PIND3
#define PD3_RPORT           PIND
#define PD3_WPORT           PORTD
#define PD3_DDR             DDRD

#undef PD4
#define PD4_PIN             PIND4
#define PD4_RPORT           PIND
#define PD4_WPORT           PORTD
#define PD4_DDR             DDRD

#undef PD5
#define PD5_PIN             PIND5
#define PD5_RPORT           PIND
#define PD5_WPORT           PORTD
#define PD5_DDR             DDRD

#undef PD6
#define PD6_PIN             PIND6
#define PD6_RPORT           PIND
#define PD6_WPORT           PORTD
#define PD6_DDR             DDRD

#undef PD7
#define PD7_PIN             PIND7
#define PD7_RPORT           PIND
#define PD7_WPORT           PORTD
#define PD7_DDR             DDRD

#endif

#if defined (__AVR_ATmega1280__) || defined (__AVR_ATmega2560__)
//Pins
#define DIO0_PIN            PINE0
#define DIO0_RPORT          PINE
#define DIO0_WPORT          PORTE
#define DIO0_DDR            DDRE

#define DIO1_PIN            PINE1
#define DIO1_RPORT          PINE
#define DIO1_WPORT          PORTE
#define DIO1_DDR            DDRE

#define DIO2_PIN            PINE4
#define DIO2_RPORT          PINE
#define DIO2_WPORT          PORTE
#define DIO2_DDR            DDRE

#define DIO3_PIN            PINE5
#define DIO3_RPORT          PINE
#define DIO3_WPORT          PORTE
#define DIO3_DDR            DDRE

#define DIO4_PIN            PING5
#define DIO4_RPORT          PING
#define DIO4_WPORT          PORTG
#define DIO4_DDR            DDRG

#define DIO5_PIN            PINE3
#define DIO5_RPORT          PINE
#define DIO5_WPORT          PORTE
#define DIO5_DDR            DDRE

#define DIO6_PIN            PINH3
#define DIO6_RPORT          PINH
#define DIO6_WPORT          PORTH
#define DIO6_DDR            DDRH

#define DIO7_PIN            PINH4
#define DIO7_RPORT          PINH
#define DIO7_WPORT          PORTH
#define DIO7_DDR            DDRH

#define DIO8_PIN            PINH5
#define DIO8_RPORT          PINH
#define DIO8_WPORT          PORTH
#define DIO8_DDR            DDRH

#define DIO9_PIN            PINH6
#define DIO9_RPORT          PINH
#define DIO9_WPORT          PORTH
#define DIO9_DDR            DDRH

#define DIO10_PIN           PINB4
#define DIO10_RPORT         PINB
#define DIO10_WPORT         PORTB
#define DIO10_DDR           DDRB

#define DIO11_PIN           PINB5
#define DIO11_RPORT         PINB
#define DIO11_WPORT         PORTB
#define DIO11_DDR           DDRB

#define DIO12_PIN           PINB6
#define DIO12_RPORT         PINB
#define DIO12_WPORT         PORTB
#define DIO12_DDR           DDRB

#define DIO13_PIN           PINB7
#define DIO13_RPORT         PINB
#define DIO13_WPORT         PORTB
#define DIO13_DDR           DDRB

#define DIO14_PIN           PINJ1
#define DIO14_RPORT         PINJ
#define DIO14_WPORT         PORTJ
#define DIO14_DDR           DDRJ

#define DIO15_PIN           PINJ0
#define DIO15_RPORT         PINJ
#define DIO15_WPORT         PORTJ
#define DIO15_DDR           DDRJ

#define DIO16_PIN           PINH1
#define DIO16_RPORT         PINH
#define DIO16_WPORT         PORTH
#define DIO16_DDR           DDRH

#define DIO17_PIN           PINH0
#define DIO17_RPORT         PINH
#define DIO17_WPORT         PORTH
#define DIO17_DDR           DDRH

#define DIO18_PIN           PIND3
#define DIO18_RPORT         PIND
#define DIO18_WPORT         PORTD
#define DIO18_DDR           DDRD

#define DIO19_PIN           PIND2
#define DIO19_RPORT         PIND
#define DIO19_WPORT         PORTD
#define DIO19_DDR           DDRD

#define DIO20_PIN           PIND1
#define DIO20_RPORT         PIND
#define DIO20_WPORT         PORTD
#define DIO20_DDR           DDRD

#define DIO21_PIN           PIND0
#define DIO21_RPORT         PIND
#define DIO21_WPORT         PORTD
#define DIO21_DDR           DDRD

#define DIO22_PIN           PINA0
#define DIO22_RPORT         PINA
#define DIO22_WPORT         PORTA
#define DIO22_DDR           DDRA

#define DIO23_PIN           PINA1
#define DIO23_RPORT         PINA
#define DIO23_WPORT         PORTA
#define DIO23_DDR           DDRA

#define DIO24_PIN           PINA2
#define DIO24_RPORT         PINA
#define DIO24_WPORT         PORTA
#define DIO24_DDR           DDRA

#define DIO25_PIN           PINA3
#define DIO25_RPORT         PINA
#define DIO25_WPORT         PORTA
#define DIO25_DDR           DDRA

#define DIO26_PIN           PINA4
#define DIO26_RPORT         PINA
#define DIO26_WPORT         PORTA
#define DIO26_DDR           DDRA

#define DIO27_PIN           PINA5
#define DIO27_RPORT         PINA
#define DIO27_WPORT         PORTA
#define DIO27_DDR           DDRA

#define DIO28_PIN           PINA6
#define DIO28_RPORT         PINA
#define DIO28_WPORT         PORTA
#define DIO28_DDR           DDRA

#define DIO29_PIN           PINA7
#define DIO29_RPORT         PINA
#define DIO29_WPORT         PORTA
#define DIO29_DDR           DDRA

#define DIO30_PIN           PINC7
#define DIO30_RPORT         PINC
#define DIO30_WPORT         PORTC
#define DIO30_DDR           DDRC

#define DIO31_PIN           PINC6
#define DIO31_RPORT         PINC
#define DIO31_WPORT         PORTC
#define DIO31_DDR           DDRC

#define DIO32_PIN           PINC5
#define DIO32_RPORT         PINC
#define DIO32_WPORT         PORTC
#define DIO32_DDR           DDRC

#define DIO33_PIN           PINC4
#define DIO33_RPORT         PINC
#define DIO33_WPORT         PORTC
#define DIO33_DDR           DDRC

#define DIO34_PIN           PINC3
#define DIO34_RPORT         PINC
#define DIO34_WPORT         PORTC
#define DIO34_DDR           DDRC

#define DIO35_PIN           PINC2
#define DIO35_RPORT         PINC
#define DIO35_WPORT         PORTC
#define DIO35_DDR           DDRC

#define DIO36_PIN           PINC1
#define DIO36_RPORT         PINC
#define DIO36_WPORT         PORTC
#define DIO36_DDR           DDRC

#define DIO37_PIN           PINC0
#define DIO37_RPORT         PINC
#define DIO37_WPORT         PORTC
#define DIO37_DDR           DDRC

#define DIO38_PIN           PIND7
#define DIO38_RPORT         PIND
#define DIO38_WPORT         PORTD
#define DIO38_DDR           DDRD

#define DIO39_PIN           PING2
#define DIO39_RPORT         PING
#define DIO39_WPORT         PORTG
#define DIO39_DDR           DDRG

#define DIO40_PIN           PING1
#define DIO40_RPORT         PING
#define DIO40_WPORT         PORTG
#define DIO40_DDR           DDRG

#define DIO41_PIN           PING0
#define DIO41_RPORT         PING
#define DIO41_WPORT         PORTG
#define DIO41_DDR           DDRG

#define DIO42_PIN           PINL7
#define DIO42_RPORT         PINL
#define DIO42_WPORT         PORTL
#define DIO42_DDR           DDRL

#define DIO43_PIN           PINL6
#define DIO43_RPORT         PINL
#define DIO43_WPORT         PORTL
#define DIO43_DDR           DDRL

#define DIO44_PIN           PINL5
#define DIO44_RPORT         PINL
#define DIO44_WPORT         PORTL
#define DIO44_DDR           DDRL

#define DIO45_PIN           PINL4
#define DIO45_RPORT         PINL
#define DIO45_WPORT         PORTL
#define DIO45_DDR           DDRL

#define DIO46_PIN           PINL3
#define DIO46_RPORT         PINL
#define DIO46_WPORT         PORTL
#define DIO46_DDR           DDRL

#define DIO47_PIN           PINL2
#define DIO47_RPORT         PINL
#define DIO47_WPORT         PORTL
#define DIO47_DDR           DDRL

#define DIO48_PIN           PINL1
#define DIO48_RPORT         PINL
#define DIO48_WPORT         PORTL
#define DIO48_DDR           DDRL

#define DIO49_PIN           PINL0
#define DIO49_RPORT         PINL
#define DIO49_WPORT         PORTL
#define DIO49_DDR           DDRL

#define DIO50_PIN           PINB3
#define DIO50_RPORT         PINB
#define DIO50_WPORT         PORTB
#define DIO50_DDR           DDRB

#define DIO51_PIN           PINB2
#define DIO51_RPORT         PINB
#define DIO51_WPORT         PORTB
#define DIO51_DDR           DDRB

#define DIO52_PIN           PINB1
#define DIO52_RPORT         PINB
#define DIO52_WPORT         PORTB
#define DIO52_DDR           DDRB

#define DIO53_PIN           PINB0
#define DIO53_RPORT         PINB
#define DIO53_WPORT         PORTB
#define DIO53_DDR           DDRB

#define DIO54_PIN           PINF0
#define DIO54_RPORT         PINF
#define DIO54_WPORT         PORTF
#define DIO54_DDR           DDRF

#define DIO55_PIN           PINF1
#define DIO55_RPORT         PINF
#define DIO55_WPORT         PORTF
#define DIO55_DDR           DDRF

#define DIO56_PIN           PINF2
#define DIO56_RPORT         PINF
#define DIO56_WPORT         PORTF
#define DIO56_DDR           DDRF

#define DIO57_PIN           PINF3
#define DIO57_RPORT         PINF
#define DIO57_WPORT         PORTF
#define DIO57_DDR           DDRF

#define DIO58_PIN           PINF4
#define DIO58_RPORT         PINF
#define DIO58_WPORT         PORTF
#define DIO58_DDR           DDRF

#define DIO59_PIN           PINF5
#define DIO59_RPORT         PINF
#define DIO59_WPORT         PORTF
#define DIO59_DDR           DDRF

#define DIO60_PIN           PINF6
#define DIO60_RPORT         PINF
#define DIO60_WPORT         PORTF
#define DIO60_DDR           DDRF

#define DIO61_PIN           PINF7
#define DIO61_RPORT         PINF
#define DIO61_WPORT         PORTF
#define DIO61_DDR           DDRF

#define DIO62_PIN           PINK0
#define DIO62_RPORT         PINK
#define DIO62_WPORT         PORTK
#define DIO62_DDR           DDRK

#define DIO63_PIN           PINK1
#define DIO63_RPORT         PINK
#define DIO63_WPORT         PORTK
#define DIO63_DDR           DDRK

#define DIO64_PIN           PINK2
#define DIO64_RPORT         PINK
#define DIO64_WPORT         PORTK
#define DIO64_DDR           DDRK

#define DIO65_PIN           PINK3
#define DIO65_RPORT         PINK
#define DIO65_WPORT         PORTK
#define DIO65_DDR           DDRK

#define DIO66_PIN           PINK4
#define DIO66_RPORT         PINK
#define DIO66_WPORT         PORTK
#define DIO66_DDR           DDRK

#define DIO67_PIN           PINK5
#define DIO67_RPORT         PINK
#define DIO67_WPORT         PORTK
#define DIO67_DDR           DDRK

#define DIO68_PIN           PINK6
#define DIO68_RPORT         PINK
#define DIO68_WPORT         PORTK
#define DIO68_DDR           DDRK

#define DIO69_PIN           PINK7
#define DIO69_RPORT         PINK
#define DIO69_WPORT         PORTK
#define DIO69_DDR           DDRK

#define DIO70_PIN           PING4
#define DIO70_RPORT         PING
#define DIO70_WPORT         PORTG
#define DIO70_DDR           DDRG

#define DIO71_PIN           PING3
#define DIO71_RPORT         PING
#define DIO71_WPORT         PORTG
#define DIO71_DDR           DDRG

#define DIO72_PIN           PINJ2
#define DIO72_RPORT         PINJ
#define DIO72_WPORT         PORTJ
#define DIO72_DDR           DDRJ

#define DIO73_PIN           PINJ3
#define DIO73_RPORT         PINJ
#define DIO73_WPORT         PORTJ
#define DIO73_DDR           DDRJ

#define DIO74_PIN           PINJ7
#define DIO74_RPORT         PINJ
#define DIO74_WPORT         PORTJ
#define DIO74_DDR           DDRJ

#define DIO75_PIN           PINJ4
#define DIO75_RPORT         PINJ
#define DIO75_WPORT         PORTJ
#define DIO75_DDR           DDRJ

#define DIO76_PIN           PINJ5
#define DIO76_RPORT         PINJ
#define DIO76_WPORT         PORTJ
#define DIO76_DDR           DDRJ

#define DIO77_PIN           PINJ6
#define DIO77_RPORT         PINJ
#define DIO77_WPORT         PORTJ
#define DIO77_DDR           DDRJ

#define DIO78_PIN           PINE2
#define DIO78_RPORT         PINE
#define DIO78_WPORT         PORTE
#define DIO78_DDR           DDRE

#define DIO79_PIN           PINE6
#define DIO79_RPORT         PINE
#define DIO79_WPORT         PORTE
#define DIO79_DDR           DDRE

#define DIO80_PIN           PINE7
#define DIO80_RPORT         PINE
#define DIO80_WPORT         PORTE
#define DIO80_DDR           DDRE

#define DIO81_PIN           PIND4
#define DIO81_RPORT         PIND
#define DIO81_WPORT         PORTD
#define DIO81_DDR           DDRD

#define DIO82_PIN           PIND5
#define DIO82_RPORT         PIND
#define DIO82_WPORT         PORTD
#define DIO82_DDR           DDRD

#define DIO83_PIN           PIND6
#define DIO83_RPORT         PIND
#define DIO83_WPORT         PORTD
#define DIO83_DDR           DDRD

#define DIO84_PIN           PINH2
#define DIO84_RPORT         PINH
#define DIO84_WPORT         PORTH
#define DIO84_DDR           DDRH

#define DIO85_PIN           PINH7
#define DIO85_RPORT         PINH
#define DIO85_WPORT         PORTH
#define DIO85_DDR           DDRH

#undef PA0
#define PA0_PIN             PINA0
#define PA0_RPORT           PINA
#define PA0_WPORT           PORTA
#define PA0_DDR             DDRA

#undef PA1
#define PA1_PIN             PINA1
#define PA1_RPORT           PINA
#define PA1_WPORT           PORTA
#define PA1_DDR             DDRA

#undef PA2
#define PA2_PIN             PINA2
#define PA2_RPORT           PINA
#define PA2_WPORT           PORTA
#define PA2_DDR             DDRA

#undef PA3
#define PA3_PIN             PINA3
#define PA3_RPORT           PINA
#define PA3_WPORT           PORTA
#define PA3_DDR             DDRA

#undef PA4
#define PA4_PIN             PINA4
#define PA4_RPORT           PINA
#define PA4_WPORT           PORTA
#define PA4_DDR             DDRA

#undef PA5
#define PA5_PIN             PINA5
#define PA5_RPORT           PINA
#define PA5_WPORT           PORTA
#define PA5_DDR             DDRA

#undef PA6
#define PA6_PIN             PINA6
#define PA6_RPORT           PINA
#define PA6_WPORT           PORTA
#define PA6_DDR             DDRA

#undef PA7
#define PA7_PIN             PINA7
#define PA7_RPORT           PINA
#define PA7_WPORT           PORTA
#define PA7_DDR             DDRA

#undef PB0
#define PB0_PIN             PINB0
#define PB0_RPORT           PINB
#define PB0_WPORT           PORTB
#define PB0_DDR             DDRB

#undef PB1
#define PB1_PIN             PINB1
#define PB1_RPORT           PINB
#define PB1_WPORT           PORTB
#define PB1_DDR             DDRB

#undef PB2
#define PB2_PIN             PINB2
#define PB2_RPORT           PINB
#define PB2_WPORT           PORTB
#define PB2_DDR             DDRB

#undef PB3
#define PB3_PIN             PINB3
#define PB3_RPORT           PINB
#define PB3_WPORT           PORTB
#define PB3_DDR             DDRB

#undef PB4
#define PB4_PIN             PINB4
#define PB4_RPORT           PINB
#define PB4_WPORT           PORTB
#define PB4_DDR             DDRB

#undef PB5
#define PB5_PIN             PINB5
#define PB5_RPORT           PINB
#define PB5_WPORT           PORTB
#define PB5_DDR             DDRB

#undef PB6
#define PB6_PIN             PINB6
#define PB6_RPORT           PINB
#define PB6_WPORT           PORTB
#define PB6_DDR             DDRB

#undef PB7
#define PB7_PIN             PINB7
#define PB7_RPORT           PINB
#define PB7_WPORT           PORTB
#define PB7_DDR             DDRB

#undef PC0
#define PC0_PIN             PINC0
#define PC0_RPORT           PINC
#define PC0_WPORT           PORTC
#define PC0_DDR             DDRC

#undef PC1
#define PC1_PIN             PINC1
#define PC1_RPORT           PINC
#define PC1_WPORT           PORTC
#define PC1_DDR             DDRC

#undef PC2
#define PC2_PIN             PINC2
#define PC2_RPORT           PINC
#define PC2_WPORT           PORTC
#define PC2_DDR             DDRC

#undef PC3
#define PC3_PIN             PINC3
#define PC3_RPORT           PINC
#define PC3_WPORT           PORTC
#define PC3_DDR             DDRC

#undef PC4
#define PC4_PIN             PINC4
#define PC4_RPORT           PINC
#define PC4_WPORT           PORTC
#define PC4_DDR             DDRC

#undef PC5
#define PC5_PIN             PINC5
#define PC5_RPORT           PINC
#define PC5_WPORT           PORTC
#define PC5_DDR             DDRC

#undef PC6
#define PC6_PIN             PINC6
#define PC6_RPORT           PINC
#define PC6_WPORT           PORTC
#define PC6_DDR             DDRC

#undef PC7
#define PC7_PIN             PINC7
#define PC7_RPORT           PINC
#define PC7_WPORT           PORTC
#define PC7_DDR             DDRC

#undef PD0
#define PD0_PIN             PIND0
#define PD0_RPORT           PIND
#define PD0_WPORT           PORTD
#define PD0_DDR             DDRD

#undef PD1
#define PD1_PIN             PIND1
#define PD1_RPORT           PIND
#define PD1_WPORT           PORTD
#define PD1_DDR             DDRD

#undef PD2
#define PD2_PIN             PIND2
#define PD2_RPORT           PIND
#define PD2_WPORT           PORTD
#define PD2_DDR             DDRD

#undef PD3
#define PD3_PIN             PIND3
#define PD3_RPORT           PIND
#define PD3_WPORT           PORTD
#define PD3_DDR             DDRD

#undef PD4
#define PD4_PIN             PIND4
#define PD4_RPORT           PIND
#define PD4_WPORT           PORTD
#define PD4_DDR             DDRD

#undef PD5
#define PD5_PIN             PIND5
#define PD5_RPORT           PIND
#define PD5_WPORT           PORTD
#define PD5_DDR             DDRD

#undef PD6
#define PD6_PIN             PIND6
#define PD6_RPORT           PIND
#define PD6_WPORT           PORTD
#define PD6_DDR             DDRD

#undef PD7
#define PD7_PIN             PIND7
#define PD7_RPORT           PIND
#define PD7_WPORT           PORTD
#define PD7_DDR             DDRD

#undef PE0
#define PE0_PIN             PINE0
#define PE0_RPORT           PINE
#define PE0_WPORT           PORTE
#define PE0_DDR             DDRE

#undef PE1
#define PE1_PIN             PINE1
#define PE1_RPORT           PINE
#define PE1_WPORT           PORTE
#define PE1_DDR             DDRE

#undef PE2
#define PE2_PIN             PINE2
#define PE2_RPORT           PINE
#define PE2_WPORT           PORTE
#define PE2_DDR             DDRE

#undef PE3
#define PE3_PIN             PINE3
#define PE3_RPORT           PINE
#define PE3_WPORT           PORTE
#define PE3_DDR             DDRE

#undef PE4
#define PE4_PIN             PINE4
#define PE4_RPORT           PINE
#define PE4_WPORT           PORTE
#define PE4_DDR             DDRE

#undef PE5
#define PE5_PIN             PINE5
#define PE5_RPORT           PINE
#define PE5_WPORT           PORTE
#define PE5_DDR             DDRE

#undef PE6
#define PE6_PIN             PINE6
#define PE6_RPORT           PINE
#define PE6_WPORT           PORTE
#define PE6_DDR             DDRE

#undef PE7
#define PE7_PIN             PINE7
#define PE7_RPORT           PINE
#define PE7_WPORT           PORTE
#define PE7_DDR             DDRE

#undef PF0
#define PF0_PIN             PINF0
#define PF0_RPORT           PINF
#define PF0_WPORT           PORTF
#define PF0_DDR             DDRF

#undef PF1
#define PF1_PIN             PINF1
#define PF1_RPORT           PINF
#define PF1_WPORT           PORTF
#define PF1_DDR             DDRF

#undef PF2
#define PF2_PIN             PINF2
#define PF2_RPORT           PINF
#define PF2_WPORT           PORTF
#define PF2_DDR             DDRF

#undef PF3
#define PF3_PIN             PINF3
#define PF3_RPORT           PINF
#define PF3_WPORT           PORTF
#define PF3_DDR             DDRF

#undef PF4
#define PF4_PIN             PINF4
#define PF4_RPORT           PINF
#define PF4_WPORT           PORTF
#define PF4_DDR             DDRF

#undef PF5
#define PF5_PIN             PINF5
#define PF5_RPORT           PINF
#define PF5_WPORT           PORTF
#define PF5_DDR             DDRF

#undef PF6
#define PF6_PIN             PINF6
#define PF6_RPORT           PINF
#define PF6_WPORT           PORTF
#define PF6_DDR             DDRF

#undef PF7
#define PF7_PIN             PINF7
#define PF7_RPORT           PINF
#define PF7_WPORT           PORTF
#define PF7_DDR             DDRF

#undef PG0
#define PG0_PIN             PING0
#define PG0_RPORT           PING
#define PG0_WPORT           PORTG
#define PG0_DDR             DDRG

#undef PG1
#define PG1_PIN             PING1
#define PG1_RPORT           PING
#define PG1_WPORT           PORTG
#define PG1_DDR             DDRG

#undef PG2
#define PG2_PIN             PING2
#define PG2_RPORT           PING
#define PG2_WPORT           PORTG
#define PG2_DDR             DDRG

#undef PG3
#define PG3_PIN             PING3
#define PG3_RPORT           PING
#define PG3_WPORT           PORTG
#define PG3_DDR             DDRG

#undef PG4
#define PG4_PIN             PING4
#define PG4_RPORT           PING
#define PG4_WPORT           PORTG
#define PG4_DDR             DDRG

#undef PG5
#define PG5_PIN             PING5
#define PG5_RPORT           PING
#define PG5_WPORT           PORTG
#define PG5_DDR             DDRG

#undef PG6
#define PG6_PIN             PING6
#define PG6_RPORT           PING
#define PG6_WPORT           PORTG
#define PG6_DDR             DDRG

#undef PG7
#define PG7_PIN             PING7
#define PG7_RPORT           PING
#define PG7_WPORT           PORTG
#define PG7_DDR             DDRG

#undef PH0
#define PH0_PIN             PINH0
#define PH0_RPORT           PINH
#define PH0_WPORT           PORTH
#define PH0_DDR             DDRH

#undef PH1
#define PH1_PIN             PINH1
#define PH1_RPORT           PINH
#define PH1_WPORT           PORTH
#define PH1_DDR             DDRH

#undef PH2
#define PH2_PIN             PINH2
#define PH2_RPORT           PINH
#define PH2_WPORT           PORTH
#define PH2_DDR             DDRH

#undef PH3
#define PH3_PIN             PINH3
#define PH3_RPORT           PINH
#define PH3_WPORT           PORTH
#define PH3_DDR             DDRH

#undef PH4
#define PH4_PIN             PINH4
#define PH4_RPORT           PINH
#define PH4_WPORT           PORTH
#define PH4_DDR             DDRH

#undef PH5
#define PH5_PIN             PINH5
#define PH5_RPORT           PINH
#define PH5_WPORT           PORTH
#define PH5_DDR             DDRH

#undef PH6
#define PH6_PIN             PINH6
#define PH6_RPORT           PINH
#define PH6_WPORT           PORTH
#define PH6_DDR             DDRH

#undef PH7
#define PH7_PIN             PINH7
#define PH7_RPORT           PINH
#define PH7_WPORT           PORTH
#define PH7_DDR             DDRH

#undef PJ0
#define PJ0_PIN             PINJ0
#define PJ0_RPORT           PINJ
#define PJ0_WPORT           PORTJ
#define PJ0_DDR             DDRJ

#undef PJ1
#define PJ1_PIN             PINJ1
#define PJ1_RPORT           PINJ
#define PJ1_WPORT           PORTJ
#define PJ1_DDR             DDRJ

#undef PJ2
#define PJ2_PIN             PINJ2
#define PJ2_RPORT           PINJ
#define PJ2_WPORT           PORTJ
#define PJ2_DDR             DDRJ

#undef PJ3
#define PJ3_PIN             PINJ3
#define PJ3_RPORT           PINJ
#define PJ3_WPORT           PORTJ
#define PJ3_DDR             DDRJ

#undef PJ4
#define PJ4_PIN             PINJ4
#define PJ4_RPORT           PINJ
#define PJ4_WPORT           PORTJ
#define PJ4_DDR             DDRJ

#undef PJ5
#define PJ5_PIN             PINJ5
#define PJ5_RPORT           PINJ
#define PJ5_WPORT           PORTJ
#define PJ5_DDR             DDRJ

#undef PJ6
#define PJ6_PIN             PINJ6
#define PJ6_RPORT           PINJ
#define PJ6_WPORT           PORTJ
#define PJ6_DDR             DDRJ

#undef PJ7
#define PJ7_PIN             PINJ7
#define PJ7_RPORT           PINJ
#define PJ7_WPORT           PORTJ
#define PJ7_DDR             DDRJ

#undef PK0
#define PK0_PIN             PINK0
#define PK0_RPORT           PINK
#define PK0_WPORT           PORTK
#define PK0_DDR             DDRK

#undef PK1
#define PK1_PIN             PINK1
#define PK1_RPORT           PINK
#define PK1_WPORT           PORTK
#define PK1_DDR             DDRK

#undef PK2
#define PK2_PIN             PINK2
#define PK2_RPORT           PINK
#define PK2_WPORT           PORTK
#define PK2_DDR             DDRK

#undef PK3
#define PK3_PIN             PINK3
#define PK3_RPORT           PINK
#define PK3_WPORT           PORTK
#define PK3_DDR             DDRK

#undef PK4
#define PK4_PIN             PINK4
#define PK4_RPORT           PINK
#define PK4_WPORT           PORTK
#define PK4_DDR             DDRK

#undef PK5
#define PK5_PIN             PINK5
#define PK5_RPORT           PINK
#define PK5_WPORT           PORTK
#define PK5_DDR             DDRK

#undef PK6
#define PK6_PIN             PINK6
#define PK6_RPORT           PINK
#define PK6_WPORT           PORTK
#define PK6_DDR             DDRK

#undef PK7
#define PK7_PIN             PINK7
#define PK7_RPORT           PINK
#define PK7_WPORT           PORTK
#define PK7_DDR             DDRK

#undef PL0
#define PL0_PIN             PINL0
#define PL0_RPORT           PINL
#define PL0_WPORT           PORTL
#define PL0_DDR             DDRL

#undef PL1
#define PL1_PIN             PINL1
#define PL1_RPORT           PINL
#define PL1_WPORT           PORTL
#define PL1_DDR             DDRL

#undef PL2
#define PL2_PIN             PINL2
#define PL2_RPORT           PINL
#define PL2_WPORT           PORTL
#define PL2_DDR             DDRL

#undef PL3
#define PL3_PIN             PINL3
#define PL3_RPORT           PINL
#define PL3_WPORT           PORTL
#define PL3_DDR             DDRL

#undef PL4
#define PL4_PIN             PINL4
#define PL4_RPORT           PINL
#define PL4_WPORT           PORTL
#define PL4_DDR             DDRL

#undef PL5
#define PL5_PIN             PINL5
#define PL5_RPORT           PINL
#define PL5_WPORT           PORTL
#define PL5_DDR             DDRL

#undef PL6
#define PL6_PIN             PINL6
#define PL6_RPORT           PINL
#define PL6_WPORT           PORTL
#define PL6_DDR             DDRL

#undef PL7
#define PL7_PIN             PINL7
#define PL7_RPORT           PINL
#define PL7_WPORT           PORTL
#define PL7_DDR             DDRL

#endif

#ifndef DIO0_PIN
  #error Pins for this chip not defined in arduino.h! If you write an appropriate pin definition and have this firmware work on your chip, please submit a pull request
#endif

#endif /* FastIO_H_ */

