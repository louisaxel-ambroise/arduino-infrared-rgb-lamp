/*
  Arduino-based RGB led lamp, controlled by infrared remote

  Components: Arduino UNO
              RGB led
              Infrared sensor

  Usage:
  Use (hardcoded) IR remote to control the color of the RGB led.

  TODO: make a timer to automatically change the color after a given time
  TODO: use interruptions for IR?
*/
#include <IRremote.h>
#include <avr/sleep.h>
#include <avr/power.h>

// Pins declaration used for LED and IR sensor
const int red_pin = 6;
const int blue_pin = 9;
const int green_pin = 10;

int red_cur, green_cur, blue_cur, red_dest, green_dest, blue_dest;
int msSinceLastUpdate = 0;

const int maxMsBetweenColorChange = 10000; // ms
const int transitionDuration = 20; //ms

const int ir_sensor_pin = 8;
IRrecv irrecv(ir_sensor_pin);
decode_results results;

void setup() {
  randomSeed(analogRead(A3));
  irrecv.enableIRIn(); 
  Serial.begin(115200);

  setColor(0x4B, 0x00, 0x82);
}

/*
 * Main loop: apply the transition if needed, check for IR key pressed, and wait. 
 * Repeat. 
 * Forever.
 */
void loop() {
  verifyIRSensor();
  updateColor();

  if(msSinceLastUpdate > maxMsBetweenColorChange){
    setRandomColor();
  }
  
  delay(transitionDuration);
  msSinceLastUpdate += transitionDuration;
}

/*
 * TODO: set right colors for key pressed.
 */
void verifyIRSensor(){
  if (irrecv.decode(&results)) 
  {
    Serial.println(results.value);
    if(results.value == 16755285){
      setColor(0xFF, 0x00, 0x00);
    }
    else if(results.value == 16754775){
      setColor(0x00, 0xFF, 0x00);
    }
    else if(results.value == 16759365){
      setColor(0x4B, 0x00, 0x82);
    }
    else if(results.value == 16726215){
      setColor(0x00, 0x00, 0xFF);
    }
    
    irrecv.resume();
  }
}

void updateColor(){
  if(red_cur == red_dest && green_cur == green_dest && blue_cur == blue_dest) return;
  
  if(red_cur < red_dest) red_cur++;
  if(red_cur > red_dest) red_cur--;
  if(green_cur < green_dest) green_cur++;
  if(green_cur > green_dest) green_cur--;
  if(blue_cur < blue_dest) blue_cur++;
  if(blue_cur > blue_dest) blue_cur--;
  
  analogWrite(red_pin, red_cur);
  analogWrite(green_pin, green_cur);
  analogWrite(blue_pin, blue_cur);
  msSinceLastUpdate = 0;
}

void setRandomColor(){
  setColor(random(0,255), random(0,255), random(0,255));
}

/*
 * Set the LED color to the R/G/B given values
 */
void setColor(int red, int green, int blue){
  red_dest = red;
  green_dest = green;
  blue_dest = blue;
}

