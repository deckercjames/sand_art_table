#!/usr/bin/env bash

FQBN="arduino:avr:nano"
PORT="/dev/cu.usbserial-$1"

shift

# Validate scripts args
if [ $# -eq 0 ]; then
    echo "Requires one argument for board selection"
    exit 1
fi
if [ "$1" != "lighting" ] && [ "$1" != "sd_card" ] && [ "$1" != "motor" ]; then
    echo "invalid board"
    exit 1
fi

# CLOSE ANY OPEN SCREENS
# screen -X quit
pkill SCREEN

# UPLOAD
echo Uploading

arduino-cli upload -b "$FQBN" -p "$PORT" -i ./bin/${1}_board.ino.hex

if [ $? -ne 0 ]; then
    echo "Upload Failed"
    exit 1
fi

echo Upload successful


# Open Serial Terminal
screen "$PORT" 9600