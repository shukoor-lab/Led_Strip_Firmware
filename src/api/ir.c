#include "ir.h"

uint16_t IR1_value;
uint16_t IR2_value;

void IR_Task(void) {
    IR1_value = ADC3_read();
    IR1_value = ADC3_read();
    IR2_value = ADC2_read();
    IR2_value = ADC2_read();
}