# Led_Strip_Firmware
## Overview

Firmware for driving addressable LED strips (WS2812 / SK6812) from an ATtiny85-based controller. Designed to be small, efficient, and configurable for different strip lengths, clock speeds, and data pins.

## Features

- Support for WS2812 / SK6812 timing
- Configurable LED count and data pin
- Multiple demo patterns (solid, rainbow, chase, blink)
- Minimal RAM/flash footprint suitable for ATtiny85
- Simple build and flash instructions for common toolchains

## Hardware

- MCU: ATtiny85
- LED strip: WS2812B / SK6812 (5V)
- Power: 5V supply capable of current required by LED count (estimate 60 mA per LED at full white)
- Wiring:
    - LED V+ -> 5V
    - LED GND -> common GND with MCU
    - LED Data -> MCU DATA_PIN (recommend a series resistor 220–470Ω)
    - Large electrolytic capacitor (470 µF) across 5V and GND at strip input
    - If MCU runs at 3.3V, use a level shifter or ensure timings/logic compatibility

Suggested default pin: PB0 (physical pin 5 on DIP8) but configurable in config header.

## Software Requirements

- avr-gcc / avr-libc
- avr-binutils (avr-objcopy, avr-size)
- avrdude (or other flasher)
- make (optional)
- recommended: usbasp / Arduino-as-ISP or other programmer

## Configuration

Edit config.h (or top of main.c) to set:
- LED_COUNT — number of LEDs on the strip
- DATA_PIN — MCU pin used for LED data (example: PB0)
- F_CPU — MCU clock speed (e.g., 8000000UL or 16000000UL)
- COLOR_ORDER — if your strip uses GRB vs RGB

Example config snippet:
```c
#define LED_COUNT 30
#define DATA_PIN PB0
#define F_CPU 16000000UL
#define COLOR_ORDER GRB
```

Ensure clock definition matches fuses; incorrect F_CPU causes timing problems.

## Build

Typical steps (from project root):

1. Compile and link:
     - make (if a Makefile is provided)
     - or manually:
```bash
avr-gcc -Os -DF_CPU=16000000UL -mmcu=attiny85 -c src/main.c -o main.o
avr-gcc -mmcu=attiny85 main.o -o main.elf
avr-objcopy -O ihex -R .eeprom main.elf main.hex
avr-size --format=avr --mcu=attiny85 main.elf
```

2. Verify size fits into ATtiny85 flash (8 KB total, subtract bootloader if present).

## Flashing

Using usbasp:
```bash
avrdude -c usbasp -p attiny85 -U flash:w:main.hex
```

Using Arduino as ISP (example port /dev/ttyACM0):
```bash
avrdude -c arduino -p attiny85 -P /dev/ttyACM0 -b 19200 -U flash:w:main.hex
```

Set fuses if needed (clock/source), e.g., to use internal 8MHz:
```bash
avrdude -c usbasp -p attiny85 -U lfuse:w:0xE2:m -U hfuse:w:0xDF:m
```
(Adjust fuse bytes for your requirements; double-check before writing.)

## Usage

- Power the MCU and strip.
- On reset, firmware runs and starts default pattern.
- Provide options via UART or button (if implemented) — consult project code for available runtime controls.

## Troubleshooting

- No LEDs lit: check 5V/GND continuity, common ground, correct data pin wiring.
- Random colors / flicker: insufficient power, missing capacitor, incorrect data resistor, or timing mismatch (check F_CPU).
- Single LED responds only: data line or wiring may be broken after first LED; verify solder joints.
- Wrong colors: swap color order (RGB vs GRB) in config.
- Flashing fails: verify programmer, permissions, and correct avrdude parameters.

## Testing

- Start with 1 or 2 LEDs to validate wiring before connecting full strip.
- Use low brightness when testing to avoid high current draw.

## Contributing

- Open issues for bugs or feature requests.
- Submit pull requests with focused changes, tests where possible, and clear descriptions.
- Keep code size and RAM usage minimal for ATtiny85 targets.

## License

MIT License — see LICENSE file for details.

## References

- avr-gcc / avr-libc documentation
- avrdude usage and programmer docs
- WS2812 timing and wiring best practices
