#include "ir.h"
#include "adc.h"
#include "display.h"
#include "spi_log.h"

uint16_t IR1_value = 0;
uint16_t IR2_value = 0;

uint8_t IR1_staus = 0, IR1_Prev_staus;
uint8_t IR2_staus = 0;

door_state_t door_state = DOOR_CLOSED;

#define IR_THRESHOLD 200

static uint8_t user_anim_index = ANIM_START_INDEX;

void IR_Task(void) {
    IR1_value = ADC3_read();
    IR1_value = ADC3_read();
    IR2_value = ADC2_read();
    IR2_value = ADC2_read();
    
    if(IR1_value >= IR_THRESHOLD) IR1_staus = 1;
    else IR1_staus = 0;

    if(IR2_value >= IR_THRESHOLD) IR2_staus = 1;
    else IR2_staus = 0;

    SPI_printf("IR1: %d   IR2: %d\n", IR1_staus, IR2_staus);
    
    switch(door_state){
        case DOOR_CLOSED: 
        {
            // SPI_printf("Closed\n");
            if(IR1_staus && IR2_staus) return;
            if(!IR1_staus && IR2_staus) {
                door_state = DOOR_OPENING;
                return;
            }
            if(IR1_staus && !IR2_staus); //user is asshole
            break;
        }
        case DOOR_OPENING:
        {
            // SPI_printf("Opening\n");
            if(!IR1_staus && !IR2_staus) {
                door_state = DOOR_OPEN;
                return;
            }
            if(IR1_staus && IR2_staus) {
                // leds_off();
                door_state = DOOR_CLOSED;
                return;
            }
            break;
        }
        case DOOR_OPEN:
        {
            // SPI_printf("Open\n");
            if(!IR1_staus && IR2_staus) {
                // leds_closing();
                door_state = DOOR_CLOSING;
                return;
            }
            if(IR1_staus && !IR2_staus) {
                IR1_Prev_staus = 1;
                // led_user_mode();
                display_set_animation(user_anim_index);
                user_anim_index++;
                fill_progress = 0;
                door_state = DOOR_USER_MODE;
                return;
            }
            break;
        }
        case DOOR_CLOSING:
        {
            // SPI_printf("Closing\n");
            if(!IR1_staus && !IR2_staus) {
                // leds_on();
                door_state = DOOR_OPEN;
                return;
            }
            if(IR1_staus && IR2_staus) {
                // leds_off();
                door_state = DOOR_CLOSED;
                return;
            }
            break;
        }
        case DOOR_USER_MODE:
        {
            
            // SPI_printf("User\n");
            if(!IR1_staus && IR2_staus) {
                // leds_closing();
                door_state = DOOR_CLOSING;
                user_anim_index = ANIM_START_INDEX;
                return;
            }
            if(!IR1_Prev_staus && IR1_staus){
                // SPI_printf("User command Called\n");
                display_set_animation(user_anim_index);
                user_anim_index++;
                if(user_anim_index == TOTAL_ANIMS) user_anim_index = ANIM_START_INDEX;

            }
            IR1_Prev_staus = IR1_staus;
            break;
        }

        default:
            break;
    }
}

static uint16_t door_open_count = 0;
static uint16_t door_half_open_count = 0;
static uint16_t start_packets_wait_time = 7000;
static uint16_t end_packets_wait_time2 = 7000;
static uint16_t door_half_open_off_timeout = 5000;

void Door_Task(void) {
    switch(door_state) {

        case DOOR_CLOSED:
            door_open_count = 0;
            door_half_open_count = 0;
            display_set_animation(ANIM_OFF);
            break;
        
        case DOOR_OPENING:
            if(door_half_open_count*DOOR_TASK_INTERVAL > door_half_open_off_timeout) {
                display_set_animation(ANIM_UNFILL);
            }else{
                if(fill_progress < 1.0) display_set_animation(ANIM_FILL);
                else door_half_open_count++;
            }
            break;
        
        case DOOR_CLOSING:
            door_open_count = 0;
            if(fill_progress > 0) display_set_animation(ANIM_UNFILL);
            break;

        case DOOR_OPEN:

            if(fill_progress == 1.0 ){
                if(door_open_count*DOOR_TASK_INTERVAL > start_packets_wait_time) {
                    display_set_animation(ANIM_UNFILL);
                }else{
                    door_open_count++;
                }
            }else if(fill_progress == 0) {
                if(door_open_count*DOOR_TASK_INTERVAL > start_packets_wait_time + end_packets_wait_time2){
                    display_set_animation(ANIM_OFF);
                }else{
                    door_open_count++;
                    display_set_animation(ANIM_PACKETS);
                }
            }
            break;

        case DOOR_USER_MODE:
            break;

        default:
            break;
    }
}