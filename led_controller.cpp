#include <FastLED.h>

#include "led_controller.h"

// How many leds in your strip?
#define NUM_LEDS 140

#define DATA_PIN 2
#define CLOCK_PIN 3

int brightness = 25; // default is 25
//int max_brightness = 50;

// how fast to update pixel values in image drawing 
int cm_per_s = 100; // how many cm/ per second the wand is supposed to be moved
int speed_case = 3; // which speed case we are on
int delay_in_us = 1000; // delay between frames in microseconds


int gamma2[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 9, 9, 9, 10, 10, 11, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 20, 21, 22, 22, 23, 23, 24, 25, 25, 26, 27, 27, 28, 29, 29, 30, 31, 31, 32, 33, 33, 34, 35, 36, 36, 37, 38, 39, 40, 40, 41, 42, 43, 44, 44, 45, 46, 47, 48, 49, 50, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 79, 80, 81, 82, 83, 84, 85, 87, 88, 89, 90, 91, 93, 94, 95, 96, 97, 99, 100, 101, 102, 104, 105, 106, 108, 109, 110, 112, 113, 114, 116, 117, 118, 120, 121, 122, 124, 125, 127, 128, 129, 131, 132, 134, 135, 137, 138, 140, 141, 143, 144, 146, 147, 149, 150, 152, 153, 155, 156, 158, 160, 161, 163, 164, 166, 168, 169, 171, 172, 174, 176, 177, 179, 181, 182, 184, 186, 188, 189, 191, 193, 195, 196, 198, 200, 202, 203, 205, 207, 209, 211, 212, 214, 216, 218, 220, 222, 224, 225, 227, 229, 231, 233, 235, 237, 239, 241, 243, 245, 247, 249, 251, 253, 255};

// Define the array of leds
CRGB leds[NUM_LEDS];

#define TEMPERATURE1 Candle // 1900  / 0 
#define TEMPERATURE2 0xffac5f // 2200  / 1
#define TEMPERATURE3 Tungsten40W // 2600   2 
#define TEMPERATURE4 Tungsten100W //2850   3
#define TEMPERATURE5 Halogen // 3200       4
#define TEMPERATURE6 CarbonArc //5200       5 
#define TEMPERATURE7 HighNoonSun //5400        6
#define TEMPERATURE8 DirectSunlight // 6000      7
#define TEMPERATURE9 OvercastSky // 7000          8


led_controller::led_controller(){
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS);
  update_brightness(25);
  FastLED.setTemperature(TEMPERATURE3);
  set_cms(100);

}

void led_controller::update_brightness(int new_brightness){
  Serial.println("new brightness");
  Serial.println(new_brightness);
  
  FastLED.setBrightness(new_brightness);
  brightness = new_brightness;
}

void led_controller::update_ctemp(int ctemp_enum){

  if (ctemp_enum == 0){
    FastLED.setTemperature(TEMPERATURE1);
  } else if (ctemp_enum == 1){
    FastLED.setTemperature(TEMPERATURE2);
  }else if (ctemp_enum == 2){
    FastLED.setTemperature(TEMPERATURE3);
  }else if (ctemp_enum == 3){
    FastLED.setTemperature(TEMPERATURE4);
  }else if (ctemp_enum == 4){
    FastLED.setTemperature(TEMPERATURE5);
  }else if (ctemp_enum == 5){
    FastLED.setTemperature(TEMPERATURE6);
  }else if (ctemp_enum == 6){
    FastLED.setTemperature(TEMPERATURE7);
  }else if (ctemp_enum == 7){
    FastLED.setTemperature(TEMPERATURE8);
  }else if (ctemp_enum == 8){
    FastLED.setTemperature(TEMPERATURE9);
  }

}

void led_controller::ctemp_visual(){

  clear();
  pixel_at_x_to_rgb(131, 255, 255, 255);
  pixel_at_x_to_rgb(132, 255, 255, 255);
  pixel_at_x_to_rgb(133, 255, 255, 255);
  pixel_at_x_to_rgb(134, 255, 255, 255);
  pixel_at_x_to_rgb(135, 255, 255, 255);
  pixel_at_x_to_rgb(136, 255, 255, 255);
  pixel_at_x_to_rgb(137, 255, 255, 255);
  pixel_at_x_to_rgb(138, 255, 255, 255);
  pixel_at_x_to_rgb(139, 255, 255, 255);
  pixel_at_x_to_rgb(140, 255, 255, 255);
  show();
}

void led_controller::brightness_visual(){
  clear();
  pixel_at_x_to_rgb(0, 255, 255, 255);
  pixel_at_x_to_rgb(1, 255, 255, 255);
  pixel_at_x_to_rgb(2, 255, 255, 255);
  pixel_at_x_to_rgb(3, 255, 255, 255);
  pixel_at_x_to_rgb(4, 255, 255, 255);
  show();
}

int led_controller::get_cms(){
  return cm_per_s;
}

int led_controller::get_frame_delay(){
  return delay_in_us;
}

void led_controller::set_cms(int cms){
  cm_per_s = cms;
  delay_in_us = 1000/(cms/0.7);
}


void led_controller::x_pixels_to_value(int x, int r,int g,int b){
  fill_solid(leds, x, CRGB(r,g,b));
  FastLED.show();


}

void led_controller::startupLEDsTest() {

  // 8 leds 0.17A at 255 bightness. 
  // 600ma max 24 leds
  // startup blink test to confirm LEDs are working.
  //int BRIGHTNESS = 15;

  //8 leds 0.05A at 50 brightness
  // 96 leds max at 500ma?
  // 72 leds at 60 brightness 0.278A, color shift
  // 50 max with 144 leds, uses 1.6A

  FastLED.setBrightness(brightness);
  fill_solid(leds, NUM_LEDS, CRGB(255,0,0));  // fill red
  FastLED.show();
  delay(5000);
  fill_solid(leds, NUM_LEDS, CRGB(0,255,0));  // fill green
  FastLED.show();
  delay(5000);
  fill_solid(leds, NUM_LEDS, CRGB(0,0,255));  // fill blue
  FastLED.show();
  delay(5000);
  fill_solid(leds, NUM_LEDS, CRGB(255,255,255));  // fill white
  FastLED.show();
  delay(5000);
  FastLED.clear();
  FastLED.show();
  FastLED.setBrightness(10);
}

void led_controller::clear(){

  //fill_solid(leds, NUM_LEDS, CRGB(0,0,0));  // fill black
  // FastLED.show();
  FastLED.clear();
}

void led_controller::show(){
  FastLED.show();
}


void led_controller::use_as_progress_bar(int n, int of){

  int leds = NUM_LEDS;

  int leds_on = (leds*n)/of;

  Serial.print("leds on:");
  Serial.println(leds_on);

  clear();

  for (int i = 0; i < leds_on; i++){
    pixel_at_x_to_rgb(i,255, 119,0 );
  }

  show();
}

void led_controller::pixel_at_x_to_rgb(int x, int r, int g, int b){

  leds[x].r = gamma2[r];
  leds[x].g = gamma2[g];
  leds[x].b = gamma2[b];
}