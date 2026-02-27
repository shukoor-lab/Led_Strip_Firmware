#include "adc.h"

/* ---------- One-Time ADC Setup ---------- */
static void ADC_hw_init(void)
{
    static uint8_t initialized = 0;

    if (!initialized)
    {
        /* Vref = Vcc, Right Adjust */
        ADMUX = (0 << REFS1) | (0 << REFS0) |
                (0 << ADLAR);

        /* Enable ADC, Prescaler = 128 (16MHz → 125kHz) */
        ADCSRA = (1 << ADEN)  |
                 (1 << ADPS2) |
                 (1 << ADPS1) |
                 (1 << ADPS0);

        initialized = 1;
    }
}

/* ---------- Select Channel ---------- */
static void ADC_select_channel(uint8_t channel)
{
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
}

/* ---------- Blocking Read ---------- */
static uint16_t ADC_read_blocking(void)
{
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADC;
}

/* ===================================================== */

uint16_t ADC0_read(void)
{
    ADC_hw_init();
    ADC_select_channel(0);
    return ADC_read_blocking();
}

uint16_t ADC1_read(void)
{
    ADC_hw_init();
    ADC_select_channel(1);
    return ADC_read_blocking();
}

uint16_t ADC2_read(void)
{
    ADC_hw_init();
    ADC_select_channel(2);
    return ADC_read_blocking();
}

uint16_t ADC3_read(void)
{
    ADC_hw_init();
    ADC_select_channel(3);
    return ADC_read_blocking();
}