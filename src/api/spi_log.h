#ifndef SPI_LOG_H
#define SPI_LOG_H

#include <avr/io.h>
#include <util/delay.h>

#define MOSI        PB1
#define SCK         PB2


void SPI_init(void);
void SPI_send_buffer(const char *data, uint16_t len);
void SPI_printf(const char *format, ...);

#endif // SPI_LOG_H