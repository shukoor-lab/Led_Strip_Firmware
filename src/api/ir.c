#include "ir.h"
#include "adc.h"
#include "display.h"

uint16_t IR1_value;
uint16_t IR2_value;

#define IR1_THRESHOLD 500

void IR_Task(void) {
    IR1_value = ADC2_read();
    IR1_value = ADC2_read();
    // IR2_value = ADC2_read();
    // IR2_value = ADC2_read();

    if(IR1_value > IR1_THRESHOLD) {
        display_set_animation(ANIM_BLINK);
    } else {
        display_set_animation(ANIM_OFF);
    }
}