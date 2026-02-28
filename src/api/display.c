#include "display.h"
#include "ws2812b.h"

/* ============================= */
/* Private State                 */
/* ============================= */

static anim_t current_anim = ANIM_OFF;
static anim_t previous_anim = ANIM_OFF;

static uint16_t anim_tick = 0;

/* ============================= */
/* Function Declaration          */
/* ============================= */

static void render_anim_off(void);
static void render_anim_blink(void);

/* ============================= */
/* Animation Tick Lengths       */
/* ============================= */

#define OFF_TICK_LEN     1
#define BLINK_TICK_LEN   100   /* 100 * 10ms = 1 second */

/* ============================= */
/* Public API                    */
/* ============================= */

void display_init(void) {
    ws2812_init();
    display_set_animation(ANIM_OFF);
}

void display_set_animation(anim_t anim)
{
    current_anim = anim;
}

void display_update(void)
{
    /* Detect animation change */
    if (current_anim != previous_anim)
    {
        anim_tick = 0;              /* Reset tick on animation change */
        previous_anim = current_anim;
    }

    /* Dispatch correct animation */
    switch (current_anim)
    {
        case ANIM_OFF:
            render_anim_off();
            break;

        case ANIM_BLINK:
            render_anim_blink();
            break;

        default:
            render_anim_off();
            break;
    }
    ws2812_show();
}

/* ============================= */
/* Animation Renderers           */
/* ============================= */

static void render_anim_off(void)
{
    ws2812_clear();

    anim_tick++;
    if (anim_tick >= OFF_TICK_LEN)
    {
        anim_tick = 0;
    }
}

static void render_anim_blink(void)
{
    /* Blink red ON for half period, OFF for half period */

    if (anim_tick < (BLINK_TICK_LEN / 2))
    {
        ws2812_set_pixel(0, 255, 0, 0); // Red
    }
    else
    {
        /* LEDs OFF */
        ws2812_set_pixel(0, 0, 0, 0);
    }

    anim_tick++;
    if (anim_tick >= BLINK_TICK_LEN)
    {
        anim_tick = 0;
    }
}

