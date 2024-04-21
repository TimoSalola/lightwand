// importing classes from this project
#include "oled.h"
#include "sd_reader.h"
#include "led_controller.h"

// digital button version, not in use, uncomment pin mode commands in void setup if changing to digital pin readouts.
//const int buttonUp = 22;     // CORRECT
//const int buttonDown = 40;   // CORRECT
//const int buttonLeft = 41;   // CORRECT
//const int buttonRight = 23;  // CORRECT


static int img_index = 0;
const char* file_name;


// fastled doesn't seem to have support for black body color temps, using enumerators instead
int ctemp_enum = 2;

// brightness range in fastled goes from 0 to 255
int brightness_now = 10;


// ######################## global instances of classess
oled* ol = nullptr;
sd_reader* sr = nullptr;
led_controller* lc = nullptr;


void setup() {

  // button setup

  //pinMode(buttonUp, INPUT);
  //pinMode(buttonDown, INPUT);
  //pinMode(buttonLeft, INPUT);
  //pinMode(buttonRight, INPUT);

  //sd_reader sr;
  //led_controller lc;
  //oled ol;
  ol = new oled();
  sr = new sd_reader();
  lc = new led_controller();


  Serial.begin(9600);
  Serial.println("setup");

  main_menu();
}

int read_btn_state() {
  int sensorValue = analogRead(A0);
  //Serial.println(sensorValue);

  // 745 is up
  // 492 right
  // 369 down 
  // 295 left
  // nobtn = 0


  if (sensorValue > 700 && sensorValue < 800){
    // up
    return 1;
  }

  if (sensorValue > 450 && sensorValue < 550){
    // right
    return 2;
  }
  if (sensorValue > 330 && sensorValue < 400){
    return 3;
  }

  if (sensorValue > 250 && sensorValue < 330){
    return 4;
  }

  return 0;

}

int read_btn_verified(){
  // reads button value and verifies that value is not "transitional"
  // eq function waits and attempts to read again if a button is most likely being held down

  int state1 = read_btn_state();

  if (state1 == 0){
    return 0;
  } else {

    delay(50);

    int state2 = read_btn_state();

    if (state1 == state2){
      Serial.println(state1);
      return state1;
    }

  }

  return 0;

}


void settings_menu() {  //oled ol, sd_reader sr
  Serial.println("############################################");
  Serial.println("#################SETTINGS MENU##############");
  Serial.println("############################################");

  delay(1000);

  int settings_item = 0;

  while (true) {
    /*
    // Button layout
    //    1
    //4   0   2
    //    3

    Menu items:
    exit 0 
    brightness 1
    speed 2
    img 3
    color temp 4
    */


    //################################ EXIT MENU LEFTRIGHT BEGINS
    // EXIT menu
    if (settings_item == 0){
      int menu_item_delta = settings_exit_loop();

      if (menu_item_delta == 2){
        break;
      } else{
        settings_item += menu_item_delta;
      }
    }

    //################################ BRIGHTNESS MENU LEFTRIGHT BEGINS
    // brightness menu
    if (settings_item == 1){
      int menu_item_delta = brightness_select_loop();
      settings_item += menu_item_delta;
    }

    //################################ SPEED MENU LEFTRIGHT BEGINS
    if (settings_item == 2){
      int menu_item_delta = cms_select_loop();
      settings_item += menu_item_delta;
    }
    //################################ IMG SELECT MENU LEFTRIGHT BEGINS
    if (settings_item == 3){
      int menu_item_delta = img_select_loop();
      settings_item += menu_item_delta;
    }
    //################################ CTEMP SELECT LEFTRIGHT BEGINS
    if (settings_item == 4){
       int menu_item_delta = ctemp_select_loop();
      settings_item += menu_item_delta;
    }

    // settings item overflow/underflow handling
    if (settings_item > 4){
      settings_item = 0;
    } else if (settings_item < 0){
      settings_item = 4;
    }

  }
}

int settings_exit_loop(){
  Serial.println("Entering exit loop");

  ol->settings_exit();

  lc->clear();
  lc->show();
  delay(600);

  int btn = 0;

  while(true){
    btn = read_btn_verified();
    delay(50);
    if (btn == 2 or btn == 4){
        return 2;
      } else if (btn == 1){
        return -1;
      } else if (btn == 3){
        return 1;
      }
  }
}


int brightness_select_loop(){
  Serial.println("Entering brightness selector loop");

  lc->brightness_visual();

  // drawing menu for the first time
  ol->settings_brightness(brightness_now, 255);

  delay(600);
  // delaying after so that menu item is not instantly changed

  // button value
  int btn = 0;

  while(true){

    // reading btn value
    btn = read_btn_verified();
    delay(50);

    if (btn == 2){
      // right btn pressed, next brightness
      update_brightness(5);
      ol->settings_brightness(brightness_now, 255);
      lc->brightness_visual();
      delay(20);
    } else if (btn == 4){
      update_brightness(-5);
      ol->settings_brightness(brightness_now, 255);
      lc->brightness_visual();
      delay(20);
      // left btn pressed, previous ctemp
    } else if (btn == 1){
      // pressing up, previous menu item
      return -1;
    } else if (btn == 3){
      // pressing down, next menu item
      return 1;
    }

  }


}

void update_brightness(int delta){
  brightness_now += delta;

  if (brightness_now < 1) {
    brightness_now = 1;
  } else if (brightness_now > 255){
    brightness_now = 255;
  }

  lc->update_brightness(brightness_now);


}

int cms_select_loop(){
  // clearing wand
  lc->clear();
  lc->show();

  // drawing menu for the first time
  int cms = lc->get_cms();
  ol->settings_speed(cms);
  

  delay(600);
  // delaying after so that menu item is not instantly changed

  // button value
  int btn = 0;


  int pixel_position = 0;
  int delaytime = 7;

  // while true loop
  while(true){

    // reading btn value
    btn = read_btn_verified();
    if (btn != 0){
      delay(50);
    }


    lc->clear();
    lc->pixel_at_x_to_rgb(pixel_position, 255, 255, 255);
    lc->pixel_at_x_to_rgb(pixel_position-1, 255, 119, 0);
    lc->pixel_at_x_to_rgb(pixel_position-2, 204, 95, 0);
    lc->pixel_at_x_to_rgb(pixel_position-3, 153, 71, 0);
    lc->show();
    pixel_position +=1;

    if (pixel_position > 140){
      pixel_position = 0;
    }

    if (btn == 2){
      // increasing speed
      int current_cms = lc->get_cms();

      int new_speed = current_cms;

      if (current_cms == 50){
        new_speed = 100;
        delaytime = 7;
      }else if (current_cms == 100){
        new_speed = 200;
        delaytime =3;
      }
      lc->set_cms(new_speed);
      ol->settings_speed(new_speed);
    } else if (btn == 4){
      // decreasing speed
      int current_cms = lc->get_cms();
      int new_speed = current_cms;
      if (current_cms == 100){
        new_speed = 50;
        delaytime = 14;
      }else if (current_cms == 200){
        new_speed = 100;
        delaytime = 7;
      }

      lc->set_cms(new_speed);
      ol->settings_speed(new_speed);

    } else if (btn== 1){
      return -1;
    } else if (btn == 3){
      return 1;
    }

    delay(delaytime);
  }



}

int img_select_loop(){

  // clearing wand
  lc->clear();
  lc->show();

  // drawing menu for the first time

  Serial.print("drawing img menu, at index: ");
  Serial.println(img_index);

  const char* im_name = sr->file_n_name(img_index);

  Serial.print(", im name:");
  Serial.println(im_name);
  ol->settings_img_select(im_name);

  delay(600);
  // delaying after so that menu item is not instantly changed

  // button value
  int btn = 0;

  // while true loop
  while(true){

    // reading btn value
    btn = read_btn_verified();
    delay(50);

    if (btn == 2){
      // right btn pressed, next image
      update_img_index(1);

      Serial.print("drawing img menu, at index: ");
      Serial.println(img_index);

      const char* im_name = sr->file_n_name(img_index);

      Serial.print(", im name:");
      Serial.println(im_name);

      ol->settings_img_select(im_name);

      delay(200);
    } else if (btn == 4){
      update_img_index(-1);

      Serial.print("drawing img menu, at index: ");
      Serial.println(img_index);

      const char* im_name = sr->file_n_name(img_index);

      Serial.print(", im name:");
      Serial.println(im_name);

      ol->settings_img_select(im_name);

      delay(200);
      // left btn pressed, previous image
    } else if (btn == 1){
      // pressing up, previous menu item
      return -1;
    } else if (btn == 3){
      // pressing down, next menu item
      return 1;
    }

  }

}

int ctemp_select_loop(){
  Serial.println("Entering ctemp selector loop");

  lc->ctemp_visual();


  // drawing menu for the first time
  ol->settings_ctemp(ctemp_enum);

  delay(600);
  // delaying after so that menu item is not instantly changed

  // button value
  int btn = 0;

  while(true){

    // reading btn value
    btn = read_btn_verified();
    delay(50);

    if (btn == 2){
      // right btn pressed, next ctemp
      update_ctemp_enum(1);
      ol->settings_ctemp(ctemp_enum);
      Serial.println("Ctemp incremented");
      lc->ctemp_visual();
      delay(200);
    } else if (btn == 4){
      update_ctemp_enum(-1);
      ol->settings_ctemp(ctemp_enum);
      ol->show();
      Serial.println("ctemp decreased");
      lc->ctemp_visual();
      delay(200);
      // left btn pressed, previous ctemp
    } else if (btn == 1){
      // pressing up, previous menu item
      return -1;
    } else if (btn == 3){
      // pressing down, next menu item
      return 1;
    }

  }


}

void update_ctemp_enum(int delta){
  // updates ctemp enum and makes sure it does not exceed acceptable range

  int new_ctemp = ctemp_enum + delta;
  Serial.print("New ctemp:");
  Serial.println(new_ctemp);

  if (new_ctemp < 0){
    new_ctemp = 0;
  }

  if (new_ctemp > 8){
    new_ctemp = 8;
  }

  lc->update_ctemp(new_ctemp);

  ctemp_enum = new_ctemp;
}


void update_img_index(int delta){
  // updates image index and makes sure it does not exceed acceptable range

  Serial.print("Updating image index, currently on ");
  Serial.print(img_index);
  Serial.print(" going to ");
  Serial.println(img_index + delta);
  int max_file_count = sr->get_file_count();

  Serial.print("file count: ");
  Serial.println(max_file_count);

  int new_index = img_index + delta;

  if (new_index >= max_file_count){
    Serial.print("Attempted to set file index as:");
    Serial.print(new_index);
    Serial.print(" this is too high. Using last file index:");
    Serial.println(max_file_count-1);
    img_index = max_file_count-1;
  } else if (new_index < 0){
    Serial.print("Attempted to set file index as:");
    Serial.print(new_index);
    Serial.print(" this is too low. Using 0.");
    img_index = 0;
  } else {
    img_index = new_index;
  }

  //updating file name in common variables
  file_name = sr->file_n_name(img_index);

}


void main_menu() {

  Serial.println("############################################");
  Serial.println("##################MAIN MENU#################");
  Serial.println("############################################");

  //sd_reader sr;
  //oled ol;

  const char* img_name = sr->file_n_name(img_index);

  Serial.println("############################################");
  Serial.println("##############SHOWING MAIN MENU#############");
  Serial.println("############################################");
  Serial.println("#FILE: ");
  Serial.println(img_name);

  ol->show_main_menu(img_name);

  while (true) {
    // Main menu loop for print, print with delay, settings and loop selection
    int button_state = read_btn_verified();

    if (button_state == 1) {
      Serial.println("Pressing up.");
      print_once_with_11s_delay(img_name);
    }

    if (button_state == 2) {
      Serial.println("Pressing right. Looping img.");
      print_img(img_name, true);
      delay(300);
    }

    if (button_state == 3) {
      Serial.println("Pressing down.");
      settings_menu();
      // returning from settings menu, some variables have to be updated + menu drawn again.


      img_name = sr->file_n_name(img_index);
      ol->show_main_menu(img_name);
      delay(300); // just got back from another menu, slight delay before accepting another button press
    }
    
    if (button_state == 4) {
      Serial.println("Pressing left. Printing img once.");
      Serial.print("Img index: ");
      Serial.print(img_index);
      Serial.print(" img name: ");
      Serial.println(img_name);
      print_img(img_name, false);
    }

  }
}

void print_once_with_11s_delay(const char* img_name) {

  /*
  Shows a 10 segment timer on wand, 1 segment per second. 
  Delays a second after last segment. Results in a 11 second timer, should go nicely with 10 second timers on cameras
  */


  // 11 second timer
  for (int i = 10; i >= 0; i--) {
    Serial.println(i);
    lc->use_as_progress_bar(i, 10);
    delay(1000);
  }

  // image printing, no looping
  print_img(img_name, false);
}

void print_img(const char* img_name, bool loop) {
  /*
  Prints img with given name. luna.bmp for example. 
  If loop = true, will print image again and again until a button is held down between prints.
  */

  int width = 10;
  int height = 10;

  int* width_pointer = &width;
  int* height_pointer = &height;

  // init pixel array, has to be deleted later
  uint8_t* imgArray = {};

  int framedelay = lc->get_frame_delay();


  // ################# opening img from sd card
  if (sr->loadBMP(img_name, imgArray, width_pointer, height_pointer)) {
    Serial.print("Got file size:");
    Serial.print(*width_pointer);
    Serial.print("x");
    Serial.println(*height_pointer);


    while (true) {
      for (uint16_t y = 0; y < height; y++) { // used to be height


        lc->clear();
        lc->show();

        for (uint16_t x = 0; x < width; x++) { // used to be width

          int arrayPos = ((width * y) + x) * 3;

          uint8_t red = imgArray[arrayPos];
          uint8_t green = imgArray[arrayPos + 1];
          uint8_t blue = imgArray[arrayPos + 2];

          lc->pixel_at_x_to_rgb(x, red, green, blue);
        }

        Serial.print("On row ");
        Serial.print(y);
        Serial.print(" of ");
        Serial.println(height);


        lc->show();
        delay(framedelay);
      }

      // if button held down or if was not supposed to loop, break loop
      int btn = read_btn_state();

      if (btn != 0) {
        break;
      }

      if (!loop) {
        break;
      }
    }

    // loop ends
  }

  // file closes

  // clearing pixels from wand
  lc->clear();
  lc->show();

  // releasing image array from memory
  delete[] imgArray;
}


void loop() {
  //read_btn_state();
  main_menu();
}



