
CC := arduino-cli
ARDUINO_FLAGS  := --clean
FQBN := arduino:avr:nano
RELEASE_BUILD := "-DRELEASE"

.PHONY: all clean setup

default: sd_card lighting

sd_card:
	@mkdir -p ./bin
	$(CC) compile $(ARDUINO_FLAGS) --build-property "build.extra_flags=-Isrc/config/ -Isrc/utils/ $(RELEASE_BUILD)" --fqbn $(FQBN) --output-dir bin/ "src/sd_card_board"

lighting:
	@mkdir -p ./bin
	$(CC) compile $(ARDUINO_FLAGS) --build-property "build.extra_flags=-Isrc/config/ -Isrc/utils/ $(RELEASE_BUILD)" --fqbn $(FQBN) --output-dir bin/ "src/lighting_board"

clean: 
	@$(RM) ./bin/*