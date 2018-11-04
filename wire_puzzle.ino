/**
 * Wire puzzle implemented on Arduino
    Copyright (C) 2018  Joshua Achermann

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    email: joshua.achermann@gmail.com
 */

#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

const int8_t WIN_LED = 2; // TODO
const int8_t LOSE_LED = 3; // TODO

// correct wire is the fourth wire

const int8_t NUM_WIRES = 6;
const int8_t OUT_WIRE = 22;
const int8_t WIRE_ACTIVE = HIGH;
const int8_t WIRE_LOW = 15;
const int8_t WIRE_HIGH = WIRE_LOW + NUM_WIRES;
typedef struct {
  int8_t pin;
} wire_t;

wire_t wires[NUM_WIRES];
wire_t *correct;

// return wire that needs to be 'cut'
wire_t *trigger_wire(void){
  return &(wires[random(0, NUM_WIRES-1)]);
}

wire_t setup_wire(int8_t pin){
  wire_t w;
  w.pin = pin;
  return w;
}

void intro(void){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write("SECURITY ON PULL");
  lcd.setCursor(0, 1);
  lcd.write("WIRE TO DISABLE");
}

void wait_for_wires(void){
  bool wait = true;
  while(wait){
    wait = false;
    for(int i = 0; i < NUM_WIRES; i++){
      if(digitalRead(wires[i].pin) != WIRE_ACTIVE){
        wait = true;
      }
    }
  }
}

void win(void){
  digitalWrite(WIN_LED, HIGH);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write("READ EVERY");
  lcd.setCursor(0, 1);
  lcd.write("THIRD LETTER");
  while(true){}
}

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(OUT_WIRE, OUTPUT);
  digitalWrite(OUT_WIRE, HIGH);

  // wire setup
  for(int i = 0; i < NUM_WIRES; i++){
    wires[i] = setup_wire(WIRE_LOW + i);
    pinMode(WIRE_LOW + i, INPUT);
  }
  correct = &(wires[3]);//trigger_wire();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write("PLEASE REPLACE");
  lcd.setCursor(0, 1);
  lcd.write("WIRES");
  // wait until all wires are replaced
  wait_for_wires();
  intro();
}

void loop() {
  for(int i = 0; i < NUM_WIRES; i++){

    Serial.print(i);
    Serial.print(" ");
    Serial.println(digitalRead(wires[i].pin));
    
    if(digitalRead(wires[i].pin) != WIRE_ACTIVE){
      if(wires[i].pin == correct->pin){
        win(); // alternatively need to pull them out in a pattern
      } else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("LOSE! RESET");
        lcd.setCursor(0, 1);
        lcd.print("WIRES");
        wait_for_wires();
        for(int x = 0; x < 60; x++){
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(60 - x);
          delay(1000);
        }
        intro();
      }
    }
  }
}
