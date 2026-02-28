#ifndef DISPLAY_H
#define DISPLAY_H

#include <avr/io.h>

/* ============================= */
/* Animation Type (Public)       */
/* ============================= */

typedef enum {
    ANIM_OFF = 0,
    ANIM_BLINK,
    ANIM_RAINBOW,
    ANIM_THEATER,
    ANIM_FIRE,
    ANIM_WATER,
    ANIM_PACKETS,
    TOTAL_ANIMS
} anim_t;

/* ============================= */
/* Public API                    */
/* ============================= */

void display_init(void);

/* Call every 10ms */
void display_update(void);

/* Change current animation */
void display_set_animation(anim_t anim);

anim_t display_get_animation(void);

#endif