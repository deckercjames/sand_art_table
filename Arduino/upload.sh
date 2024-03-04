#!/usr/bin/env bash

FQBN="arduino:avr:uno"
PORT="/dev/cu.usbmodem141401"

# Verify

mkdir -p bin/

./verify.sh

# CLOSE ANY OEN SCREEN
screen -X quit

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