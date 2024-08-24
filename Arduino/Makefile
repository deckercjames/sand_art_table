
ARDUINO_CC := arduino-cli
UTEST_CC := g++
ARDUINO_FLAGS  := --clean
FQBN := arduino:avr:nano
BUILD_DIR := bin
# RELEASE_BUILD := "-DRELEASE"
RELEASE_BUILD := ""
CONFIG_SRC := $(shell find ./src/config -type f)
UTIL_SRC := $(shell find ./src/utils -type f)

.PHONY: all clean setup

default: lighting sd_card

lighting: $(BUILD_DIR)/lighting_board.ino.hex
sd_card: $(BUILD_DIR)/sd_card_board.ino.hex
test: $(BUILD_DIR)/test_gcode_parser

$(BUILD_DIR)/lighting_board.ino.hex: $(CONFIG_SRC) $(UTIL_SRC) $(shell find ./src/lighting_board -type f)
	@mkdir -p ./bin
	$(ARDUINO_CC) compile $(ARDUINO_FLAGS) --build-property "build.extra_flags=-Isrc/config/ -Isrc/utils/ $(RELEASE_BUILD)" --fqbn $(FQBN) --output-dir $(BUILD_DIR) "src/lighting_board"
	@rm $(BUILD_DIR)/lighting_board.ino.eep
	@rm $(BUILD_DIR)/lighting_board.ino.elf
	@rm $(BUILD_DIR)/lighting_board.ino.with_bootloader.*

$(BUILD_DIR)/sd_card_board.ino.hex: $(CONFIG_SRC) $(UTIL_SRC) $(shell find ./src/sd_card_board -type f)
	@mkdir -p ./bin
	$(ARDUINO_CC) compile $(ARDUINO_FLAGS) --build-property "build.extra_flags=-Isrc/config/ -Isrc/utils/ $(RELEASE_BUILD)" --fqbn $(FQBN) --output-dir $(BUILD_DIR) "src/sd_card_board"
	@rm $(BUILD_DIR)/sd_card_board.ino.eep
	@rm $(BUILD_DIR)/sd_card_board.ino.elf
	@rm $(BUILD_DIR)/sd_card_board.ino.with_bootloader.*
	
	
# Unit tests
$(BUILD_DIR)/test_gcode_parser: test/unit_test_utils.h test/test_gcode_parser.cpp $(wildcard src/sd_card_board/src/test_gcode_parser.*)
	$(UTEST_CC) -o $(BUILD_DIR)/test_gcode_parser -Isrc/sd_card_board/src/ -Isrc/config -Isrc/utils test/test_gcode_parser.cpp src/sd_card_board/src/gcode_parser.cpp

clean: 
	@$(RM) ./bin/*