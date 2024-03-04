#!/usr/bin/env bash

FQBN="arduino:avr:uno"
PORT="/dev/cu.usbmodem141401"

./run_unit_tests.sh

# COMPILE

mkdir -p bin/

if arduino-cli compile --clean -b "$FQBN" --output-dir ./bin/; then
    echo "Compilation Succeeded"
else
    echo "Compilation Failed"
    exit 1
fi
