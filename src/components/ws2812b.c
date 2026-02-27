#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "ws2812b.h"

/* ===== Static LED Buffer ===== */
/* 3 bytes per LED (GRB order) */
static uint8_t led_buffer[LED_COUNT * 3];

/* ===== Low-level send byte (16MHz timing) ===== */
static void ws2812_send_buffer(uint8_t *data, uint16_t len)
{
    cli();  // disable interrupts

    for(uint16_t i = 0; i < len; i++)
    {
        uint8_t byte = data[i];

        for(uint8_t j = 0; j < 8; j++)
        {
            if(byte & 0x80)
            {
                // ----- SEND 1 -----
                __asm__ __volatile__ (
                    "sbi %[port], %[bit] \n\t"  // 2 cycles
                    "nop\n\t nop\n\t nop\n\t nop\n\t nop\n\t nop\n\t" // 6 cycles
                    "cbi %[port], %[bit] \n\t"  // 2 cycles
                    "nop\n\t nop\n\t nop\n\t"   // 3 cycles
                    :
                    : [port] "I" (_SFR_IO_ADDR(PORTB)),
                      [bit]  "I" (0)
                );
            }
            else
            {
                // ----- SEND 0 -----
                __asm__ __volatile__ (
                    "sbi %[port], %[bit] \n\t"  // 2 cycles
                    "nop\n\t nop\n\t nop\n\t"   // 3 cycles
                    "cbi %[port], %[bit] \n\t"  // 2 cycles
                    "nop\n\t nop\n\t nop\n\t nop\n\t nop\n\t nop\n\t nop\n\t" // 7 cycles
                    :
                    : [port] "I" (_SFR_IO_ADDR(PORTB)),
                      [bit]  "I" (0)
                );
            }

            byte <<= 1;
        }
    }

    sei();  // enable interrupts
}

/* ===== Initialize ===== */
void ws2812_init(void)
{
    WS_DDR |= WS_PIN_MASK;
    ws2812_clear();
}

/* ===== Set Pixel (buffer only) ===== */
void ws2812_set_pixel(uint16_t index, uint8_t r, uint8_t g, uint8_t b)
{
    if(index >= LED_COUNT)
        return;

    uint16_t base = index * 3;

    // WS2812 expects GRB order
    led_buffer[base + 0] = g;
    led_buffer[base + 1] = r;
    led_buffer[base + 2] = b;
}

/* ===== Clear Buffer ===== */
void ws2812_clear(void)
{
    for(uint16_t i = 0; i < LED_COUNT * 3; i++)
        led_buffer[i] = 0;
}

/* ===== Send Entire Buffer ===== */
void ws2812_show(void)
{
    ws2812_send_buffer(led_buffer, LED_COUNT * 3);
    _delay_us(60);
}