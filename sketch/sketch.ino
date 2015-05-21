#include <WS2812.h>
#include "avr/interrupt.h"
#include <EEPROM.h>

#define MODE_PIN 2
#define MODE_EEPROM_POSITION 0
int mode = 0;
bool mode_change = false;
#define NB_MODES 3

#define VARIANT_PIN 1
#define VARIANT_EEPROM_POSITION 1
int variant = 0;
bool variant_change = false;
#define NB_VARIANTS 13

long nextWindow = millis();

#define WINDOW_SPAN 500 // ms

// Led Stuff
#define LED_COUNT 60
#define STRIP_PIN 4

WS2812 LED(LED_COUNT); // 1 LED Strip
cRGB value;

cRGB cBlack   = { 0, 0, 0 };
cRGB cWhite   = { 185, 185, 185 }; // White with <1,5A consumption

// Primary
cRGB cRed     = { 255, 0, 0 };
cRGB cGreen   = { 0, 255, 0 };
cRGB cBlue    = { 0, 0, 255 };

// Secondary
cRGB cCyan    = { 0, 255, 255 };
cRGB cMagenta = { 255, 0, 255 };
cRGB cYellow  = { 255, 255, 0 };

// Tertiary
cRGB cOrange  = { 255, 125, 0 };
cRGB cPink    = { 255, 0, 125 };
cRGB cViolet  = { 125, 0, 255 };
cRGB cOcean   = { 0, 125, 255 };
cRGB cTurquoise = { 0, 255, 125 };
cRGB cLawn    = { 125, 255, 0 };
    
cRGB colors[13] =  {
  cWhite,
  cRed,
  cGreen,
  cBlue,
  cCyan,
  cMagenta,
  cYellow,
  cOrange,
  cPink,
  cViolet,
  cOcean,
  cTurquoise,
  cLawn
};

// For mode rainbow
int current_color_index = 0;
int next_color_index = current_color_index + 1;

void setup()
{
  
  LED.setOutput(STRIP_PIN);
  LED.setColorOrderRGB();
  setAll(cBlack);
  
  pinMode(MODE_PIN, INPUT);
  pinMode(VARIANT_PIN, INPUT);
  
  // Let's retrieve the mode and variant from EEPROM
  mode = EEPROM.read(MODE_EEPROM_POSITION);
  variant = EEPROM.read(VARIANT_EEPROM_POSITION);
  
  // We enable interrupts for the mode button
  attachInterrupt(0, on_mode_change, RISING); // attach INT0 - D2 interrupt for mode change
 
}

void setAll(cRGB value)
{
  int i = 0;
  while (i < LED_COUNT){
    LED.set_crgb_at(i, value);
    i++;
  }
  LED.sync(); // Sends the value to the LED
}

void on_mode_change()
{
  long digital_state = digitalRead(MODE_PIN);
  for(int i=0;i<100;i++) { digital_state += digitalRead(MODE_PIN); }
  digital_state = digital_state / 100;
  mode_change = (digital_state == 1);
}

void nextVariant()
{
  variant = (variant + 1) % NB_VARIANTS;
  EEPROM.write(VARIANT_EEPROM_POSITION,variant);
}

void nextMode()
{
  mode = (mode + 1) % NB_MODES;
  EEPROM.write(MODE_EEPROM_POSITION,mode);
}

void loop()
{
  // Check for variant button (not interrupt driven)
  long variant_pushed = digitalRead(VARIANT_PIN);
  for(int i=0;i<100;i++) { variant_pushed += digitalRead(VARIANT_PIN); }
  variant_pushed = variant_pushed / 100;
  variant_change = (variant_pushed == 1);
  
  // Event loop
  long now = millis();
  if (mode_change == true && now > nextWindow) {
    // Leave a few ms before next 
    nextWindow = now + WINDOW_SPAN;
    nextMode();
  }
  
  if (variant_change == true && now > nextWindow) {
    // Leave a few ms before next 
    nextWindow = now + WINDOW_SPAN;
    nextVariant();
  }
  
  // ... and reset
  variant_change = false;
  mode_change = false;
  
  // Finally :
  switch (mode) {
    case 0:
      mode0();
      break;
    case 1:
      mode1();
      break;
    case 2:
      mode2();
      break;
  }

  delay(20);

}

// Lights on
void mode0()
{
  setAll(colors[variant]);
}

// Get the actual decay value at the time it's called
float getBreathingValue(){
  
  // Based on :
  // http://sean.voisen.org/blog/2011/10/breathing-led-with-arduino/

  // Breathing pattern time adjustement
  // to make it go faster or slower
  float time_adj = PI/2000.0; 
  
  // The actual value
  float val = (exp(sin(millis()*time_adj)) - 0.36787944)*0.42545949; // with these constants, val ∈ [0;1]
  
  return val;
}

// "Breathing"
void mode1()
{
  
  cRGB color = colors[variant];
  
  float val = getBreathingValue();
  
  cRGB breathing_color = { (int) color.g * val, (int) color.r * val, (int) color.b * val  };
  setAll(breathing_color);
  
}

// "Random changes"
void mode2()
{
  
  cRGB active_color = colors[current_color_index];
  cRGB next_color = colors[next_color_index];
  
  float val = getBreathingValue();
  
  cRGB current_color = { (int) (active_color.g * val + next_color.g * (1 - val)), (int) (active_color.r * val + next_color.r * (1 - val)), (int) (active_color.b * val + next_color.b * (1 - val))  };
  setAll(current_color);
  
  if (val < 0.001) {
    current_color_index =  (current_color_index + 1) % NB_VARIANTS;
  } else if (val > 0.999) {
    next_color_index =  (next_color_index + 1) % NB_VARIANTS;
  }
  
}
