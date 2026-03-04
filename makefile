# ==== Configuration ====
MCU        = attiny85
F_CPU      = 16000000UL
PROGRAMMER = usbasp
PORT       = usb
TARGET     = main
BUILD_DIR  = build

# ==== Tools ====
CC         = avr-gcc
OBJCOPY    = avr-objcopy
AVRDUDE    = avrdude

# ==== Compiler Flags ====
CFLAGS     = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os
LDFLAGS    = -mmcu=$(MCU)

# ==== Include directories ====
INCLUDES   = -I. \
             -Idrivers \
             -Ilib \
             -Isrc/components \
             -Isrc/api

# ==== Source files ====
SRC = src/main.c \
      src/components/adc.c \
      src/components/time.c \
      src/components/ws2812b.c \
      src/components/time.c \
      src/api/display.c \
      src/api/ir.c \
#       src/api/spi_log.c

# ==== Object files inside build/ ====
OBJ = $(SRC:%.c=$(BUILD_DIR)/%.o)

# ==== Final outputs ====
ELF = $(BUILD_DIR)/$(TARGET).elf
HEX = $(BUILD_DIR)/$(TARGET).hex

# ==== Build rules ====
all: $(HEX)

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile C → build/...o
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Link
$(ELF): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

# Convert to HEX
$(HEX): $(ELF)
	$(OBJCOPY) -O ihex -R .eeprom $< $@
	@echo "Build successful ✔"

# Flash
flash: $(HEX)
	$(AVRDUDE) -v -p $(MCU) -c $(PROGRAMMER) -U flash:w:$<:i

# ===== Fuses =====
fuses16:
	$(AVRDUDE) -c $(PROGRAMMER) -p $(MCU) \
	-U lfuse:w:0xE1:m -U hfuse:w:0xDD:m -U efuse:w:0xFE:m

fuses8:
	$(AVRDUDE) -c $(PROGRAMMER) -p $(MCU) \
	-U lfuse:w:0xE2:m -U hfuse:w:0xDF:m -U efuse:w:0xFE:m

# Clean everything
clean:
	rm -rf $(BUILD_DIR)