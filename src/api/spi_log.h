#ifndef SPI_LOG_H
#define SPI_LOG_H

#include <avr/io.h>
#include <util/delay.h>

#define MOSI        PB1
#define SCK         PB2

#define HEADER1     0xff
#define HEADER2     0xf9
#define FOOTER1     0xe7
#define FOOTER2     0xc3

typedef enum {
    DATA_LOG,
    BREAKPOINT_LOG,
    ERROR_LOG
} LogType;

void SPI_init(void);
void SPI_log(LogType type, uint8_t value);

#endif // SPI_LOG_H