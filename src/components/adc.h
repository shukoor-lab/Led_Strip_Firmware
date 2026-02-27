#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>
#include <stdint.h>

/* Public API */
uint16_t ADC0_read(void);   // PB5
uint16_t ADC1_read(void);   // PB2
uint16_t ADC2_read(void);   // PB4
uint16_t ADC3_read(void);   // PB3

#endif