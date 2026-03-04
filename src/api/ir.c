#include "ir.h"
#include "adc.h"
#include "display.h"
// #include "spi_log.h"

uint16_t IR1_value = 0;
uint16_t IR2_value = 0;

uint8_t IR1_staus = 0, IR1_Prev_staus;
uint8_t IR2_staus = 0;

typedef enum {
    DOOR_CLOSED,
    DOOR_OPENING,
    DOOR_OPEN,
    DOOR_CLOSING,
    DOOR_USER_MODE
} door_state_t;

door_state_t door_state = DOOR_CLOSED;

#define IR_THRESHOLD 500

void IR_Task(void) {
    IR1_value = ADC2_read();
    IR1_value = ADC2_read();
    IR2_value = ADC3_read();
    IR2_value = ADC3_read();
    
    if(IR1_value >= IR_THRESHOLD) IR1_staus = 1;
    else IR1_staus = 0;

    if(IR2_value >= IR_THRESHOLD) IR2_staus = 1;
    else IR2_staus = 0;

    // SPI_printf("IR1: %d   IR2: %d\n", IR1_staus, IR2_staus);
    
    switch(door_state){
        case DOOR_CLOSED: 
        {
            // SPI_printf("Closed\n");
            if(IR1_staus && IR2_staus) return;
            if(!IR1_staus && IR2_staus) {
                door_state = DOOR_OPENING;
                // leds_opening();
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
                // leds_on();
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
                return;
            }
            if(!IR1_Prev_staus && IR1_staus){
                // SPI_printf("User command Called\n");
            }
            IR1_Prev_staus = IR1_staus;
        }
        break;
    }
}