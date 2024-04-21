#ifndef oled_h
#define oled_h


#include "Arduino.h"


class oled{
  public:
    oled();
    void display_text(String);
    void append_txt(String s1);
    void clear();
    void show();
    void show_main_menu(const char* s1);

    void settings_img_select(const char* imname);

    
    void settings_exit();

    void settings_speed(int cms);

    void settings_ctemp(int ctemp_var);

    void settings_brightness(int n, int of);

    void progressbar(int, int);

  private:

};


#endif