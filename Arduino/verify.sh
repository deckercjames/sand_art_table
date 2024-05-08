#!/usr/bin/env bash

FQBN="arduino:avr:nano"

# ./run_unit_tests.sh

# COMPILE

function compile_board()
{
    sed -i'' -E "s/.*_setup/  ${1}_setup/g" Arduino.ino
    sed -i'' -E "s/.*_loop/  ${1}_loop/g" Arduino.ino
    if arduino-cli compile --clean -b "$FQBN" --output-dir ./bin/; then
        echo "Compilation succeeded for ${1}"
    else
        echo "Compilation failed for ${1}"
        exit 1
    fi
    rm -f ./bin/Arduino.ino.eep
    rm -f ./bin/Arduino.ino.elf
    rm -f ./bin/Arduino.ino.with_bootloader.bin
    rm -f ./bin/Arduino.ino.with_bootloader.hex
    mv ./bin/Arduino.ino.hex "./bin/${1}.ino.hex"
    return 0
}

echo

mkdir -p bin/

if [ "$#" -eq 0 ]; then
    compile_board "lighting_board"
    compile_board "sd_card_board"
    compile_board "motor_board"
    exit 0
fi

if [ "$1" != "lighting" ] && [ "$1" != "sd_card" ] && [ "$1" != "motor" ]; then
    echo "invalid board"
    exit 1
fi

compile_board "${1}_board"