#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include "time.h"

volatile uint32_t g_ms = 0;

void timeInit(void)
{
    cli();  // Disable interrupts during setup

    // CTC mode (Clear Timer on Compare)
    TCCR0A = (1 << WGM01);

    // Prescaler = 64
    TCCR0B = (1 << CS01) | (1 << CS00);

    // 16MHz / 64 = 250kHz
    // 250 ticks = 1ms
    OCR0A = 249;

    // Enable compare match interrupt
    TIMSK |= (1 << OCIE0A);

    sei();  // Enable global interrupts
}

ISR(TIMER0_COMPA_vect)
{
    g_ms++;
}

uint32_t getTime(void)
{
    uint32_t time;

    cli();          // Prevent partial read (8-bit MCU)
    time = g_ms;
    sei();

    return time;
}