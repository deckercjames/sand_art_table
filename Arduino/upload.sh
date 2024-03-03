#!/usr/bin/env bash

FQBN="arduino:avr:uno"
PORT="/dev/cu.usbmodem141401"

# CLOSE ANY OEN SCREEN
screen -X quit

# COMPILE
# echo Compiling
mkdir bin

# if arduino-cli compile --clean -b "$FQBN" --output-dir ./bin/; then
#     echo "Compilation Succeeded"
# else
#     echo "Compilation Failed"
#     exit 1
# fi

# Compile unit tests
if g++ unit_tests/unit_tests.cpp src/motor_driver/path_calculator/path_calculator.cpp -o bin/unit_tests -Isrc/motor_driver/path_calculator -Isrc/utils; then
    echo "Unit tests compiled successfully"
else
    echo "UNIT TESTS FAILED TO COMPILE. Aborting uplaod!"
    exit 1
fi

# Run unit tests befor uploading
if ./bin/unit_tests; then
    echo "Unit tests passed"
else
    echo "UNIT TESTS FAILED. Aborting uplaod!"
    exit 1
fi

# exit here to compile only
exit 0

# UPLOAD
echo Uploading

arduino-cli upload -b "$FQBN" -p "$PORT" -i ./bin/Arduino.ino.hex

if [ $? -ne 0 ]; then
    echo "Upload Failed"
    exit 1
fi

echo Upload successful


# Open Serial Terminal
screen "$PORT" 9600