#include "ir.h"
#include "adc.h"
#include "display.h"

uint16_t IR1_value = 0, IR1_value_prev = 0;
uint16_t IR2_value;

#define IR1_THRESHOLD 500

void IR_Task(void) {
    IR1_value = ADC2_read();
    IR1_value = ADC2_read();
    // IR2_value = ADC2_read();
    // IR2_value = ADC2_read();

    if(IR1_value > IR1_THRESHOLD && IR1_value_prev <= IR1_THRESHOLD) {
        // IR1 just crossed the threshold, toggle animation
        anim_t current_anim = display_get_animation();
        if(current_anim < TOTAL_ANIMS - 1) {
            display_set_animation(current_anim + 1);
        } else {
            display_set_animation(ANIM_OFF);
        }
    }

    IR1_value_prev = IR1_value;
}