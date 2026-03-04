#include "spi_log.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

char buffer[50];

void SPI_init(void)
{
    // MOSI, SCK, SS as output
    DDRB |= (1 << MOSI) | (1 << SCK);

    // USI in 3-wire mode
    USICR = (1 << USIWM0);
}

void SPI_transfer(uint8_t data)
{
    USIDR = data;                  // Load byte
    USISR = (1 << USIOIF);         // Clear overflow flag

    // Clock 8 bits (16 toggles)
    do
    {
        USICR = (1 << USIWM0) |    // 3-wire mode
                (1 << USICS1) |    // Software clock
                (1 << USICLK) |    // Enable clock strobe
                (1 << USITC);      // Toggle clock
        
        _delay_us(100);
    }
    while (!(USISR & (1 << USIOIF)));
}

void SPI_send_buffer(const char *data, uint16_t len)
{
    for (uint16_t i = 0; i < len; i++)
    {
        SPI_transfer(data[i]);
    }
}

void SPI_printf(const char *format, ...)
{

    va_list args;
    va_start(args, format);

    // Safe formatted print into buffer
    vsnprintf(buffer, sizeof(buffer), format, args);

    va_end(args);

    // Send generated string over SPI
    for (uint16_t i = 0; i < strlen(buffer); i++)
    {
        SPI_transfer(buffer[i]);
    }
}