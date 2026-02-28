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
static void render_anim_rainbow(void);
static void render_anim_theater(void);
static void render_anim_fire(void);
static void render_anim_water(void);
static void render_anim_packets(void);

/* ============================= */
/* Animation Tick Lengths       */
/* ============================= */

#define OFF_TICK_LEN     1
#define BLINK_TICK_LEN   100   /* 100 * 10ms = 1 second */
#define RAINBOW_TICK_LEN 256
#define THEATER_TICK_LEN 30
#define FIRE_TICK_LEN 255
#define COLOR_BOUNCE_TICK_LEN 160
#define WATER_TICK_LEN 255
#define PACKET_TICK_LEN  238

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

anim_t display_get_animation(void)
{
    return current_anim;
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

        case ANIM_RAINBOW:
            render_anim_rainbow();
            break;

        case ANIM_THEATER:
            render_anim_theater();
            break;

        case ANIM_FIRE:
            render_anim_fire();
            break;

        case ANIM_WATER:
            render_anim_water();
            break;

        case ANIM_PACKETS:
            render_anim_packets();
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

static uint32_t wheel(uint8_t pos)
{
    uint8_t r, g, b;

    pos = 255 - pos;

    if (pos < 85)
    {
        r = 255 - pos * 3;
        g = 0;
        b = pos * 3;
    }
    else if (pos < 170)
    {
        pos -= 85;
        r = 0;
        g = pos * 3;
        b = 255 - pos * 3;
    }
    else
    {
        pos -= 170;
        r = pos * 3;
        g = 255 - pos * 3;
        b = 0;
    }

    return ((uint32_t)r << 16) |
           ((uint32_t)g << 8)  |
           ((uint32_t)b);
}

static void render_anim_rainbow(void)
{
    for (uint8_t i = 0; i < LED_COUNT; i++)
    {
        uint8_t color_index = (i * 256 / LED_COUNT + anim_tick) & 0xFF;
        uint32_t c = wheel(color_index);

        ws2812_set_pixel(i,
                         (c >> 16) & 0xFF,
                         (c >> 8) & 0xFF,
                         c & 0xFF);
    }

    anim_tick++;
    if (anim_tick >= RAINBOW_TICK_LEN)
        anim_tick = 0;
}

static void render_anim_theater(void)
{
    ws2812_clear();

    uint8_t phase = anim_tick % 3;

    for (uint8_t i = phase; i < LED_COUNT; i += 3)
        ws2812_set_pixel(i, 255, 255, 255);

    anim_tick++;
    if (anim_tick >= THEATER_TICK_LEN)
        anim_tick = 0;
}

static uint16_t lfsr = 0xACE1u;

static uint8_t fast_rand(void)
{
    lfsr ^= lfsr << 7;
    lfsr ^= lfsr >> 9;
    lfsr ^= lfsr << 8;
    return (uint8_t)lfsr;
}


#define FIRE_BASE_HEIGHT     30
#define FIRE_MAX_HEIGHT      40

#define SECOND_CENTER        65
#define SECOND_HALF_WIDTH    8
#define SECOND_EXTRA_HEIGHT  7

static uint8_t main_flame_level = FIRE_BASE_HEIGHT;
static uint8_t second_flame_level = SECOND_HALF_WIDTH;

static void render_anim_fire(void)
{
    /* ===============================
       NATURAL MAIN FLAME DRIFT
    =============================== */

    if ((anim_tick & 0x07) == 0)   // update slowly (every ~80ms)
    {
        int8_t change = (fast_rand() & 0x03) - 1;  // -1,0,+1

        int16_t new_level = main_flame_level + change;

        if (new_level < FIRE_BASE_HEIGHT)
            new_level = FIRE_BASE_HEIGHT;

        if (new_level > FIRE_MAX_HEIGHT)
            new_level = FIRE_MAX_HEIGHT;

        main_flame_level = new_level;
    }

    /* ===============================
       NATURAL SECOND FLAME DRIFT
    =============================== */

    if ((anim_tick & 0x0F) == 0)   // slower drift
    {
        int8_t change2 = (fast_rand() & 0x03) - 1;

        int16_t new_level2 = second_flame_level + change2;

        uint8_t max2 = SECOND_HALF_WIDTH + SECOND_EXTRA_HEIGHT;

        if (new_level2 < SECOND_HALF_WIDTH)
            new_level2 = SECOND_HALF_WIDTH;

        if (new_level2 > max2)
            new_level2 = max2;

        second_flame_level = new_level2;
    }

    /* ===============================
       RENDER LOOP
    =============================== */

    for (uint8_t i = 0; i < LED_COUNT; i++)
    {
        uint16_t total_heat = 0;

        /* ---------- MAIN FIRE ---------- */
        if (i < main_flame_level)
        {
            uint8_t base_heat = 255 - (i * 6);
            uint8_t flicker = fast_rand() & 0x3F;

            uint8_t heat = (base_heat > flicker) ? base_heat - flicker : 0;
            total_heat += heat;
        }

        /* ---------- SECOND FIRE ---------- */

        int8_t dist = i - SECOND_CENTER;
        if (dist < 0) dist = -dist;

        if (dist <= second_flame_level)
        {
            uint8_t local = (second_flame_level - dist) << 4;
            uint8_t flicker2 = fast_rand() & 0x1F;

            uint8_t heat2 = (local > flicker2) ? local - flicker2 : 0;
            total_heat += heat2;
        }

        if (total_heat > 255)
            total_heat = 255;

        /* ---------- COLOR MAP ---------- */

        uint8_t r = total_heat;
        uint8_t g = (total_heat > 100) ? (total_heat - 100) : 0;

        ws2812_set_pixel(i, r, g, 0);
    }

    anim_tick++;
    if (anim_tick >= FIRE_TICK_LEN)
        anim_tick = 0;
}

static uint8_t water_energy = 10;   // small dynamic variation

static uint8_t triwave(uint8_t x)
{
    if (x & 0x80)
        return 255 - ((x & 0x7F) << 1);
    else
        return (x & 0x7F) << 1;
}

static void render_anim_water(void)
{
    /* ===============================
       Slowly vary wave energy
       (natural sea breathing)
    =============================== */
    if ((anim_tick & 0x1F) == 0)   // every ~320ms
    {
        int8_t delta = (fast_rand() & 0x03) - 1;
        int8_t new_energy = water_energy + delta;

        if (new_energy < 6)  new_energy = 6;
        if (new_energy > 22) new_energy = 22;

        water_energy = new_energy;
    }

    for (uint8_t i = 0; i < LED_COUNT; i++)
    {
        uint8_t brightness = 0;

        /* =========================
           LINEAR SHORE (0–40)
        ========================= */
        if (i <= 40)
        {
            uint8_t wave_pos = (i * 5) + (anim_tick * 2);
            uint8_t wave = triwave(wave_pos);

            brightness = (wave >> 2) + water_energy;
        }

        /* =========================
           CIRCULAR RIPPLE (41–80)
        ========================= */
        else if (i >= 41 && i <= 80)
        {
            uint8_t ring_pos = i - 41;   // 0..39

            uint8_t ripple = triwave((ring_pos * 7) + (anim_tick * 3));

            brightness = ripple >> 2;
        }

        /* Subtle shimmer */
        brightness += fast_rand() & 0x05;

        if (brightness > 255)
            brightness = 255;

        /* =========================
           NATURAL OCEAN COLOR MAP
        ========================= */

        uint8_t r, g, b;

        if (brightness > 180)
        {
            /* Wave foam */
            uint8_t foam = brightness - 180;
            foam += fast_rand() & 0x03;  // slight sparkle

            r = 170 + foam;
            g = 200 + foam;
            b = 220 + foam;
        }
        else
        {
            /* Deep teal water */
            r = brightness >> 5;             // very low red
            g = (brightness * 3) >> 2;       // teal bias
            b = brightness;                  // blue body
        }

        ws2812_set_pixel(i, r, g, b);
    }

    anim_tick++;
    if (anim_tick >= WATER_TICK_LEN)
        anim_tick = 0;
}

#define PACKET_LEN       3

static void render_anim_packets(void)
{
    uint8_t pointer[3] = {0, 14, 28};

    uint8_t idx = anim_tick / 2.5;

    if(idx < 81){
        for (uint8_t j = 0; j < 3; j++)
        {
            for(int j = 0; j < 3; j++){
                if(idx - pointer[j] >= 0 && idx - pointer[j] < LED_COUNT) {
                    ws2812_set_pixel(idx - pointer[j], 130, 90, 40);
                }
                if(idx - pointer[j] - PACKET_LEN >= 0 && idx - pointer[j] - PACKET_LEN < LED_COUNT) {
                    ws2812_set_pixel(idx - pointer[j] - PACKET_LEN, 0, 0, 0);
                }
            }
        }
    }else{
        for(int j = 0; j < 3; j++){
            uint16_t pv_idx = idx - pointer[j];
            if (pv_idx >= LED_COUNT) {
                pv_idx -= 42;
            }
            if(pv_idx >= 40 && pv_idx < LED_COUNT) {
                ws2812_set_pixel(pv_idx, 130, 90, 40);
            }
            pv_idx = idx - pointer[j] - PACKET_LEN;
            if (pv_idx >= LED_COUNT) {
                pv_idx -= 42;
            }
            if(pv_idx >= 0 && pv_idx < LED_COUNT) {
                ws2812_set_pixel(pv_idx, 0, 0, 0);
            }
        }
    }
    

    anim_tick++;
    if (anim_tick >= PACKET_TICK_LEN)
    {
        // ws2812_clear();  // Clear strip at end of cycle
        anim_tick = 203;
    }
}