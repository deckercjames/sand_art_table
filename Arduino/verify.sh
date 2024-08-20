#!/usr/bin/env bash

FQBN="arduino:avr:nano"


# COMPILE

function compile_board()
{
    cd "src/$1"
    if arduino-cli compile --clean -b "$FQBN" --output-dir ../../bin/; then
        echo "Compilation succeeded for ${1}"
    else
        echo "Compilation failed for ${1}"
        exit 1
    fi
    cd ../..
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