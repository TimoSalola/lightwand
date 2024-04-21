#ifndef led_controller_h
#define led_controller_h


class led_controller {
  public:
    led_controller();
    void startupLEDsTest();

    void pixel_at_x_to_rgb(int , int , int , int);

    void use_as_progress_bar(int n, int of);

    int get_cms();
    void set_cms(int cms);
    int get_frame_delay();

    void x_pixels_to_value(int x, int r,int g,int b);

    void update_brightness(int new_brightness);

    void update_ctemp(int ctemp_enum);
    void show();
    void clear();

    void ctemp_visual();
    void brightness_visual();

  private:


};

#endif