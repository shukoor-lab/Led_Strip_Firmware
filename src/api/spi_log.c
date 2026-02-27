#include "spi_log.h"

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
        
        _delay_us(1);
    }
    while (!(USISR & (1 << USIOIF)));
}

void SPI_send_string(const char *data, uint16_t len)
{
    for (uint16_t i = 0; i < len; i++)
    {
        SPI_transfer(data[i]);
    }
}

void SPI_log(LogType type, uint8_t value)
{
    SPI_transfer(HEADER1);
    SPI_transfer(HEADER2);
    SPI_transfer((uint8_t)type);
    SPI_transfer(value);
    SPI_transfer(FOOTER1);
    SPI_transfer(FOOTER2);
    _delay_ms(10);
}