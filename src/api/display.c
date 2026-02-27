#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

/* ============================= */
/* Configuration                 */
/* ============================= */

#define LED_COUNT 16

/* ============================= */
/* Animation Type (Public)       */
/* ============================= */

typedef enum {
    ANIM_OFF = 0,
    ANIM_BLINK
} anim_t;

/* ============================= */
/* Public Framebuffer            */
/* ============================= */

/* Format: led_buffer[led_index][0=R,1=G,2=B] */
extern uint8_t led_buffer[LED_COUNT][3];

/* ============================= */
/* Public API                    */
/* ============================= */

/* Call every 10ms */
void display_update(void);

/* Change current animation */
void display_set_animation(anim_t anim);

#endif