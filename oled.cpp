#include "oled.h"
#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_SSD1306.h>
// see https://learn.adafruit.com/adafruit-gfx-graphics-library/using-fonts
// for a list of fonts available in the GFX lib
// or custom fonts from http://oleddisplay.squix.ch/#/home
#include<Fonts/FreeSans9pt7b.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    0  // Reset pin for display (0 or -1 if no reset pin)

#include <string>

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); //wire1?



oled::oled(){

  // DISPLAY CODE:

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println("Display setup failed");
    while (true);
  }
    // set fonts botforh display:
  display.setFont(&FreeSans9pt7b);
  Serial.println("Display is good to go");
  // DISPLAY CODE END

}

void oled::display_text(String s1){
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 12);
  // Display static text
  display.println(s1);
  display.display(); 

}


void oled::append_txt(String s1){
  display.println(s1);
}

void oled::clear(){
  display.clearDisplay();
}

void oled::show(){
  display.display(); 
}


void oled::settings_brightness(int n, int of){


  // INITS
  display.clearDisplay();
  display.setTextSize(0);
  display.setTextColor(WHITE);


  // top print
  display.setCursor(28, 12);
  display.println("Previous");

  // center Brightness: value
  display.setCursor(0, 36); // 40 too low 
  display.print("Bright:");
  int progress = (100*n)/of;
  display.print(progress);
  display.print("%");

  // left print
  //display.setCursor(0, 28);
  //display.println("-");

  // right print
  //display.setCursor(80, 28);
  //display.println("+");

  // bottom print
  display.setCursor(35, 60);
  display.println("Next");

  display.display();

}



void oled::settings_exit(){
  display.clearDisplay();

  display.setTextSize(0);
  display.setTextColor(WHITE);


  // top print
  display.setCursor(28, 12);
  display.println("Previous");

  // center print

  display.setCursor(28, 36); // 40 too low 
  display.println("<-Exit->");

  // left print
  //display.setCursor(0, 28);
  //display.println("-");

  // right print
  //display.setCursor(80, 28);
  //display.println("+");

  // bottom print
  display.setCursor(35, 60);
  display.println("Next");

  display.display();
}

void oled::settings_speed(int cms){

   // INITS
  display.clearDisplay();
  display.setTextSize(0);
  display.setTextColor(WHITE);


  // top print
  display.setCursor(28, 12);
  display.println("Previous");

  // center speed in ms
  display.setCursor(0, 36); // 40 too low 
  display.print("Speed:");
  display.print(cms);
  display.print("cm/s");


  // bottom print
  display.setCursor(35, 60);
  display.println("Next");

  display.display();
}

void oled::settings_ctemp(int ctemp_var){


  // kelvin number

  int kelvin_number = 4000;

  if (ctemp_var == 0){
    kelvin_number = 1900;
  }

  if (ctemp_var == 1){
    kelvin_number = 2200;
  }

  if (ctemp_var == 2){
    kelvin_number = 2600;
  }

  if (ctemp_var == 3){
    kelvin_number = 2850;
  }

  if (ctemp_var == 4){
    kelvin_number = 3200;
  }

  if (ctemp_var == 5){
    kelvin_number = 5200;
  }

  if (ctemp_var == 6){
    kelvin_number = 5400;
  }

  if (ctemp_var == 7){
    kelvin_number = 6000;
  }
  if (ctemp_var == 8){
    kelvin_number = 7000;
  }

   // INITS
  display.clearDisplay();
  display.setTextSize(0);
  display.setTextColor(WHITE);


  // top print
  display.setCursor(28, 12);
  display.println("Previous");

  // center speed in ms
  display.setCursor(0, 36); // 40 too low 
  display.print("C_temp:");
  display.print(kelvin_number);
  display.print("K");


  // bottom print
  display.setCursor(35, 60);
  display.println("Next");

  display.display();
}


void oled::settings_img_select(const char* imname){
   // INITS
  display.clearDisplay();
  display.setTextSize(0);
  display.setTextColor(WHITE);


  // top print
  display.setCursor(28, 12);
  display.println("Previous");

  // center speed in ms
  display.setCursor(0, 36); // 40 too low 
  display.print("IMG:");
  display.print(imname);

  // bottom print
  display.setCursor(35, 60);
  display.println("Next");

  display.display();


}



void oled::show_main_menu(const char* s1){

  /*
  Shows following menu on display:
        11s
  Now       Loop
      Settings
  IMG: s1
  
  */
  Serial.print("Printing menu, got file:");
  Serial.println(s1);
  display.clearDisplay();

  display.setTextSize(0);
  display.setTextColor(WHITE);


  // ############################# 4 MAIN MENU ITEMS
  // top print
  display.setCursor(45, 12);
  display.println("11s");

  // left print
  display.setCursor(0, 24);
  display.println("Now");

  // right print
  display.setCursor(80, 24);
  display.println("Loop");

  // bottom print
  display.setCursor(30, 40);
  display.println("Settings");

  // ################################ BOTTOM TEXT FILE NAME

  // bottom print 2
  display.setCursor(0, 58);
  display.println("IMG:");
  display.setCursor(40, 58);
  display.println(s1);
  display.display(); 
}

void oled::progressbar(int progress, int ydelta){

  int bar_width = 128;
  int progress_pixels = (128.0/100.0) * progress;

  display.drawRect(0, ydelta, bar_width, 5, WHITE);
  display.fillRect(0, ydelta, progress_pixels, 5, WHITE);

}
