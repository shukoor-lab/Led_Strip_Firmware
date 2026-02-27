#include <avr/io.h>
#include <util/delay.h>
#include "ws2812b.h"
#include "spi_log.h"
#include "time.h"
#include "ir.h"


int main(void)
{
    ws2812_init();
    timeInit();

    uint32_t current_time = getTime();

    uint32_t IR_task_prev_time = current_time;
    const uint32_t IR_task_interval = 100; // 100ms

    uint32_t RGB_task_prev_time = current_time;
    const uint32_t RGB_task_interval = 25; // 25ms

    uint8_t rgb1_state = 0;
    uint8_t rgb2_state = 0;


    while(1){
        current_time = getTime();

        if(current_time - IR_task_prev_time >= IR_task_interval) {
            IR_Task();
            IR_task_prev_time = current_time;
        }

        if(current_time - RGB_task_prev_time >= RGB_task_interval) {
            // Update RGB1
            if(rgb1_state == 0) {
                ws2812_set_pixel(0, 255, 0, 0); // Red
                rgb1_state = 1;
                ws2812_show();
            }else{
                ws2812_set_pixel(0, 0, 0, 0); // Off
                rgb1_state = 0;
                ws2812_show();
            }
            RGB_task_prev_time = current_time;
        }
    }
    
}