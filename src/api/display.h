#ifndef DISPLAY_H
#define DISPLAY_H

#include <avr/io.h>

/* ============================= */
/* Animation Type (Public)       */
/* ============================= */

typedef enum {
    ANIM_OFF = 0,
    ANIM_BLINK
} anim_t;

/* ============================= */
/* Public API                    */
/* ============================= */

void display_init(void);

/* Call every 10ms */
void display_update(void);

/* Change current animation */
void display_set_animation(anim_t anim);

#endif