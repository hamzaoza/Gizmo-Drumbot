#include <Stepper.h>

// Create motor setup constants.
const int STEPS = 200;
const int SECTIONS = 4;
const int SPEED = 50;

// Calculate gear ratios and setup motor variables.
float OUTER_RATIO = (float) 99/12;
float INNER_RATIO = (float) 45/12;

float angle_outer;
float angle_inner;

int angle_ratio;
int sector = 1;

// Setup Stepper motors.
Stepper innerStepper(STEPS, 6, 7, 8, 9); // Inner Ring Stepper motor pins.
Stepper outerStepper(STEPS, 2, 3, 4, 5); // Outer Ring Stepper motor pins.

/*
 * Begin Drumming Code.
 */

// 12 - Xylo Right - Green
// 13 - Xylo Left - Orange
// 11 - Drum Right - Red
// 10 - Drum Left - Yellow

const int drumPins[] = {12, 13, 11, 10}; // Pins for xylophone keys and drum keys. (1st pair for xylo. 2nd pair for drumsticks.)
const int beatNumber = 8;

int tempo;

const int xyloNotes[4][beatNumber] = {
  
  {0b10, 0b00, 0b01, 0b00, 0b10, 0b00, 0b01, 0b00}, // Section 1
  {0b10, 0b00, 0b10, 0b00, 0b01, 0b00, 0b01, 0b00}, // Section 2
  {0b00, 0b00, 0b11, 0b00, 0b00, 0b00, 0b11, 0b00}, // Section 3
  {0b01, 0b00, 0b10, 0b00, 0b01, 0b00, 0b01, 0b00} // Section 4
  
}; // Xylohpone rhythm composition, including rest beats - 8 elements = 4 strokes

const int drumBeats[4][beatNumber] = {
  
  {0b00, 0b00, 0b00, 0b00, 0b00, 0b00, 0b00, 0b00}, // Section 1
  {0b01, 0b00, 0b11, 0b00, 0b10, 0b00, 0b11, 0b00}, // Section 2
  {0b01, 0b00, 0b00, 0b00, 0b00, 0b00, 0b11, 0b00}, // Section 3
  {0b01, 0b00, 0b10, 0b00, 0b01, 0b00, 0b01, 0b00} // Section 4
  
}; // Drum rhythm compoistion, including rest beats - 8 elements = 4 drum beats

void setup() {

  Serial.begin(9600);

  angle_outer = OUTER_RATIO * STEPS / SECTIONS;
  angle_inner = INNER_RATIO * STEPS / SECTIONS;

  angle_ratio = angle_outer / angle_inner;

  innerStepper.setSpeed(SPEED);
  outerStepper.setSpeed(SPEED / 2); // Set speed of outer ring by half to help alignment issues.

  // Set drum pins as output.
  for(int p = 0; p < 4; p++) {
    pinMode(drumPins[p], OUTPUT);  
  }

  playDemo(); // Call demo player.

}

void loop() {
  

}

void motorTurn() {

  int diff = angle_outer - (angle_ratio * angle_inner); // Calculate the loss of steps to compensate later.
   
  for(int s = 1; s < angle_inner; s++){
    innerStepper.step(1);
    outerStepper.step(-angle_ratio);
  }

  outerStepper.step(-diff); // Move lost steps.
  sector++;

  sectorLimit();
  delay(750);
  
}

void sectorLimit() {

  // Function to catch sector controls.

  if(sector > SECTIONS)
    sector = 1;

  if(sector < 1)
    sector = SECTIONS;
    
}

void playDrums(int times) {

  for(int j = 0; j < times; j++) {

    for(int i = 0; i < beatNumber; i++){
  
      digitalWrite(drumPins[0], (xyloNotes[sector-1][i] & 0b01)); // Xylo Right
      digitalWrite(drumPins[1], (xyloNotes[sector-1][i] & 0b10)); // Xylo Left
      
      digitalWrite(drumPins[2], (drumBeats[sector-1][i] & 0b01)); // Drum Right
      digitalWrite(drumPins[3], (drumBeats[sector-1][i] & 0b10)); // Drum Left

      Serial.println(sector);
      Serial.println(tempo);
      Serial.println(i);
      
      delay(tempo);
      
    }

  }
  
}

void playDemo() {

  tempo = 250;
  playDrums(8);
  delay(500);

  tempo = 500;
  motorTurn();
  playDrums(4);
  delay(500);

  tempo = 120;
  motorTurn();
  playDrums(8);
  delay(500);

  tempo = 80;
  motorTurn();
  playDrums(10);
  delay(500);

  motorTurn();
  delay(500);
  
}
