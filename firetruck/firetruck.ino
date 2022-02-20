/*
 Firetruck

 This sketch will implement lights to a toy firetruck.

 */

#include <Adafruit_NeoPixel.h>

// Driving lights 3 LEDs on each group.
#define FRONTRIGHTPIN D0
#define FRONTLEFTPIN  D1
#define REARRIGHTPIN  D2
#define REARLEFTPIN   D3

struct lights {
  int frontLightRight = 255;
  int frontLightLeft  = 255;
  int backLightRight  = 255;
  int backLightLeft   = 255;
} Lights;

// Emergency Light
#define NEOPIN      D6
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      18
#define NUMCOLORS       7
int red[NUMCOLORS]   = {20,20, 0, 0, 0,20,20};
int green[NUMCOLORS] = { 0,20,20,20, 0, 0,10};
int blue[NUMCOLORS]  = { 0, 0, 0,20,20,20,20};
/*
int red[NUMCOLORS]   = { 60,  0, 60, 0};
int green[NUMCOLORS] = {  0,  0,  0, 0};
int blue[NUMCOLORS]  = {  0, 60,  0,60};
*/
// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIN, NEO_GRB + NEO_KHZ800);

void fade() {
  static int brightness = 0;    // how bright the LED is
  static int fadeAmount = 10;   // how many points to fade the LED by

  Serial.print("Fade Brightness: ");
  Serial.println(brightness);

  // set the brightness of pin
  analogWrite(FRONTRIGHTPIN, brightness);
  analogWrite(FRONTLEFTPIN,  160-brightness);
  analogWrite(REARRIGHTPIN,  brightness);
  analogWrite(REARLEFTPIN,   brightness);

  // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;

  // reverse the direction of the fading at the ends of the fade:
  if (brightness <= 0 || brightness >= 160) {
    fadeAmount = -fadeAmount;
  }
}

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(115200);

  // declare pin 9 to be an output:
  pinMode(FRONTRIGHTPIN, OUTPUT);
  pinMode(FRONTLEFTPIN,  OUTPUT);
  pinMode(REARRIGHTPIN,  OUTPUT);
  pinMode(REARLEFTPIN,   OUTPUT);

  analogWrite(FRONTRIGHTPIN, 0xff);
  analogWrite(FRONTLEFTPIN,  0xff);

  pixels.begin(); // This initializes the NeoPixel library.
  pixels.show();            // Turn OFF all pixels ASAP
  pixels.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)

  Serial.println("Setup Ready");
}

uint8_t stripColor[NUMPIXELS][3] {};

void setColor() {
  static uint8_t color = 0;
  stripColor[0][0] = red[color];
  stripColor[0][1] = green[color];
  stripColor[0][2] = blue[color];
  Serial.print("Color: ");
  Serial.println(color);
  color++;
  if (color == NUMCOLORS) {
    color = 0;
  }
}

void loop() {
  static uint8_t emergency = 0;
  // Fade front/back lights
  //fade();

  if (emergency++ == 0) {
    emergency = 0;

    // Shift up
    for (int i = NUMPIXELS-2; i >= 0; i = i - 1) {
      stripColor[i+1][0] = stripColor[i][0];
      stripColor[i+1][1] = stripColor[i][1];
      stripColor[i+1][2] = stripColor[i][2];
    }

    setColor();

    // Display neopixels
    for (uint8_t i = 0; i < NUMPIXELS; i=i+1) {
      uint8_t r = random(0,10);
      //if ((i+r) % 3 == 10) { // Random white
      //  pixels.setPixelColor(i, pixels.Color(50, 50, 50));
      //} else {
        pixels.setPixelColor(i, pixels.Color(stripColor[i][0], stripColor[i][1], stripColor[i][2]));
      //}
    }
    pixels.show();
  }
  delay(200);
}
