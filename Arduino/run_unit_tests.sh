#!/usr/bin/env bash

# COMPILE
mkdir -p bin

# Compile unit tests
if g++ unit_tests/unit_tests.cpp src/motor_driver/path_calculator/path_calculator.cpp -o bin/unit_tests -Isrc/motor_driver/path_calculator -Isrc/config; then
    echo "Unit tests compiled successfully"
else
    echo "UNIT TESTS FAILED TO COMPILE!"
    exit 1
fi

# Run unit tests
./bin/unit_tests || exit 1
