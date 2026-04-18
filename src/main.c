#include <avr/io.h>
#include <util/delay.h>
#include "display.h"
#include "spi_log.h"
#include "time.h"
#include "ir.h"


int main(void)
{
    display_init();
    timeInit();
    SPI_init();
    // display_set_animation(ANIM_FILL);

    uint32_t current_time = getTime();

    uint32_t IR_task_prev_time = current_time;
    const uint32_t IR_task_interval = 100; // 100ms

    uint32_t DOOR_task_prev_time = current_time;
    const uint32_t DOOR_task_interval = DOOR_TASK_INTERVAL; // 100ms

    uint32_t DISPLAY_task_prev_time = current_time;
    const uint32_t DISPLAY_task_interval = 10; // 10ms

    uint8_t rgb1_state = 0;
    uint8_t rgb2_state = 0;


    while(1){
        current_time = getTime();

        if(current_time - IR_task_prev_time >= IR_task_interval) {
            IR_Task();
            IR_task_prev_time = current_time;
        }

        if(current_time - DOOR_task_prev_time >= DOOR_task_interval) {
            Door_Task();
            DOOR_task_prev_time = current_time;
        }

        if(current_time - DISPLAY_task_prev_time >= DISPLAY_task_interval) {
            display_update();   
            DISPLAY_task_prev_time = current_time;
        }
    }
    
}