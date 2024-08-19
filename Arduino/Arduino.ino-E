/*
  Sand Table
  
  Driver for all compiling all sand table scripts
*/

#include "src/lighting_board.h"
#include "src/motor_board.h"
#include "src/sd_card_board.h"

void setup() {
  Serial.begin(9600);
  Serial.println("\n\n**************************************");
  motor_board_setup();
}

void loop() {
  motor_board_loop();
}
