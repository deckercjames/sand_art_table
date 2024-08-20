#!/usr/bin/env bash

FQBN="arduino:avr:nano"


# COMPILE

BUILD_PROPERTIY=""
BUILD_PROPERTIY="build.extra_flags=\"-DRELEASE=1\""

function compile_board()
{
    if arduino-cli compile --clean --build-property build.extra_flags=\"-Isrc/utils/\" --fqbn "$FQBN" --output-dir bin/ "src/${1}"; then
        echo "Compilation succeeded for ${1}"
    else
        echo "Compilation failed for ${1}"
        exit 1
    fi
    rm -f ./bin/${1}.ino.eep
    rm -f ./bin/${1}.ino.elf
    rm -f ./bin/${1}.ino.with_bootloader.bin
    rm -f ./bin/${1}.ino.with_bootloader.hex
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