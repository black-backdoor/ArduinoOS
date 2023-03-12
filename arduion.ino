/****************************************/
/*              Arduino OS              */
/*                                      */
/****************************************/

/*
  ---------------------------------------------------------------
  Arduino OS

  Version: 1.1.15
  Plugin Version Suport: 0.0.0

  Development Started: 4.5.2022
  ---------------------------------------------------------------
*/

//https://www.reddit.com/r/arduino/comments/f71bfv/arduino_cpugpufps_monitor/

//------------------------ Libarys ------------------------
#include <LiquidCrystal_I2C.h> //LCD Display Libary
#include <Wire.h> //Wire is required used for LiquidCrystal_I2C

#include <SoftwareSerial.h> //used to create device serial

//------------------------ Settings ------------------------
const bool no_useless_usage = true; //disables every useless function which needs resources or time

//------------------------ Serial ------------------------
//------------ USER Serial ------------
//can be connected to bluetooth
const bool user_serial_connection = true; //allow serial connection for User

//Set up a new SoftwareSerial object
SoftwareSerial user_serial(4, 5);

//------------ DEVICE Serial ------------
bool device_serial_connection = false; //connection with device astabliced & plugin loaded
String device_name = "";



//------------------------ Buttons ------------------------
//------ Pins ------
const unsigned int pin_button_up = 5;
const unsigned int pin_button_down = 6;
const unsigned int pin_button_right = 7;
const unsigned int pin_button_left = 8;
const unsigned int pin_button_ok = 9;
const unsigned int pin_button_back = 10;



//menu table integer
unsigned int menu = 1;

//--------- Time ---------
unsigned int hours, minutes, seconds;
//--------- Date ---------
unsigned int years, months, days;

//--------- Clock ---------
const bool clock_blink = false;

bool clock_shown = false;
bool clock_dots_activ = false;
unsigned int last_minutes = 61;

unsigned int clock_last_millis = 0;
unsigned int clock_button_mode = 0;


//unsigned long int from 0 to 4,294,967,295
//4,294,967,295ms = 1193
unsigned long int wait_on_second = 0;
//------------------------ LCD Display ------------------------

//Display Size
const unsigned int lcd_rows = 4; //rows starting at one
const unsigned int lcd_colums = 20;  //colums starting at one

/*
  ------------------------ LCD Display Size ------------------------
  the display rows and colums start at 0 not 1

  Examples:

  if LCD Display has 4 rows and 20 colums

  ------for the code---------
  ----------------------
  0 |                    |
  1 |                    |
  2 |                    |
  3 |                    |
  ----------------------
   0123456789
        10 + 0123456789


  if LCD Display has 2 rows and 16 colums

  ------for the code---------
  -------------------
  0 |                 |
  1 |                 |
  -------------------
   0123456789
        10 + 0123456

*/

//LCD Display Init
LiquidCrystal_I2C lcd(0x27, lcd_colums, lcd_rows);   //I2C Adresse: "0x3F" or "0x27"


/*
  ------------------------ creat custom Characters ------------------------
  just creat one with the link down hier

  custom Character Generator


  https://maxpromer.github.io/LCD-Character-Creator/
  https://diyusthad.com/custom-character-generator-lcd

  custom Character

  byte Skull[8] = {
  B00000,
  B01110,
  B10101,
  B11011,
  B01110,
  B01110,
  B00000,
  B00000
  };

  or short term

  byte Skull[8] = {B00000,B01110,B10101,B11011,B01110,B01110,B00000,B00000};

*/

//Arows
const byte character_arow_up[8] = {B00000, B00000, B00100, B01110, B11111, B01110, B01110, B00000};
const byte character_arow_down[8] = {B00000, B01110, B01110, B11111, B01110, B00100, B00000, B00000};

const byte character_arow_left[8] = {B00000, B00010, B00110, B01110, B01110, B00110, B00010, B00000};
const byte character_arow_right[8] = {B00000, B01000, B01100, B01110, B01110, B01100, B01000, B00000};

//settings characters
const byte character_cross[8] = {B00000, B00000, B11011, B01110, B00100, B01110, B11011, B00000};
const byte character_check[8] = {B00000, B00001, B00011, B10110, B11100, B01000, B00000, B00000};

const byte character_lock[8] = {B01110, B10001, B10001, B11111, B11011, B11011, B11111, B00000};
const byte character_key[8] = {B00000, B00000, B00000, B01000, B10111, B01001, B00000, B00000};

const byte character_speaker[8] = {B00001, B00011, B01111, B01111, B01111, B00011, B00001, B00000};
const byte character_speaker_sparkle[8] = {B10000, B10000, B00000, B11000, B00000, B10000, B10000, B0000};

const byte character_notes[8] = {B00001, B00011, B00101, B01001, B01001, B01011, B11011, B11000};
const byte character_note[8] = {B00000, B00010, B00011, B00010, B01110, B11110, B01100, B00000};

const byte character_bell[8] = {B00100, B01110, B01110, B01110, B11111, B00000, B00100, B00000};


const byte character_plug[8] = {B00000, B01010, B01010, B11111, B11111, B01110, B00100, B00100};
const byte character_plug_down[8] = {B00100, B00100, B01110, B11111, B11111, B01010, B01010, B00000};
/*
  or

  const byte character_plug[2][8] = {
  {B00000, B01010, B01010, B11111, B11111, B01110, B00100, B00100},
  {B00100, B00100, B01110, B11111, B11111, B01010, B01010, B00000} //up
  };

  or

  void rotateArray(byte arr[], int size) {
  int temp;
  for (int i = 0; i < size / 2; i++) {
    temp = arr[i];
    arr[i] = arr[size - i - 1];
    arr[size - i - 1] = temp;
  }
  }

  byte arr[SIZE] = {B00000, B01010, B01010, B11111, B11111, B01110, B00100, B00100};

  rotateArray(arr, SIZE);

  #define SIZE 8

*/


const byte character_clock[8] = {B00000, B01110, B10101, B10111, B10001, B01110, B00000, B00000};

const byte character_info[8] = {B00000, B01100, B01100, B00000, B01100, B01100, B01100, B11110};

const byte character_celsius[] = {B11100, B10100, B11100, B00000, B00000, B00000, B00000, B00000};

/*
  const byte batteryLevels[6][8] = {
  {B01110,B11111,B10001,B10001,B10001,B10001,B10001,B11111},
  {B01110,B11111,B10001,B10001,B10001,B10001,B11111,B11111},
  {B01110,B11111,B10001,B10001,B10001,B11111,B11111,B11111},
  {B01110,B11111,B10001,B10001,B11111,B11111,B11111,B11111},
  {B01110,B11111,B10001,B11111,B11111,B11111,B11111,B11111},
  {B01110,B11111,B11111,B11111,B11111,B11111,B11111,B11111}
  };

  batteryLevels[1]
  batteryLevels[2]
  batteryLevels[3]
  batteryLevels[4]
  batteryLevels[5]
  batteryLevels[6]


*/


//################ Progressbar ################
/*
  ------------------------ Progressbar ------------------------
  Download: https://www.carnetdumaker.net/snippets/4/
  Best Explanation https://www.carnetdumaker.net/articles/faire-une-barre-de-progression-avec-arduino-et-liquidcrystal/
  Short Explanation https://www.carnetdumaker.net/articles/faire-une-barre-de-progression-avec-arduino-et-liquidcrystal/#barre-de-progression-v3
*/

//------------------------ progress bar animation characters ------------------------
const byte START_DIV_0_OF_4[8] = {B01111, B11000, B10000, B10000, B10000, B10000, B11000, B01111};
const byte START_DIV_1_OF_4[8] = {B01111, B11000, B10000, B10000, B10000, B10011, B11000, B01111};
const byte START_DIV_2_OF_4[8] = {B01111, B11000, B10000, B10000, B10111, B10011, B11000, B01111};
const byte START_DIV_3_OF_4[8] = {B01111, B11000, B10000, B10111, B10111, B10011, B11000, B01111};
const byte START_DIV_4_OF_4[8] = {B01111, B11000, B10011, B10111, B10111, B10011, B11000, B01111};
const byte DIV_0_OF_8[8] = {B11111, B00000, B00000, B00000, B00000, B00000, B00000, B11111};
const byte DIV_1_OF_8[8] = {B11111, B00000, B00000, B00000, B00000, B11000, B00000, B11111};
const byte DIV_2_OF_8[8] = {B11111, B00000, B00000, B00000, B11000, B11000, B00000, B11111};
const byte DIV_3_OF_8[8] = {B11111, B00000, B00000, B11000, B11000, B11000, B00000, B11111};
const byte DIV_4_OF_8[8] = {B11111, B00000, B11000, B11000, B11000, B11000, B00000, B11111};
const byte DIV_5_OF_8[8] = {B11111, B00000, B11000, B11000, B11000, B11011, B00000, B11111};
const byte DIV_6_OF_8[8] = {B11111, B00000, B11000, B11000, B11011, B11011, B00000, B11111};
const byte DIV_7_OF_8[8] = {B11111, B00000, B11000, B11011, B11011, B11011, B00000, B11111};
const byte DIV_8_OF_8[8] = {B11111, B00000, B11011, B11011, B11011, B11011, B00000, B11111};
const byte END_DIV_0_OF_4[8] = {B11110, B00011, B00001, B00001, B00001, B00001, B00011, B11110};
const byte END_DIV_1_OF_4[8] = {B11110, B00011, B00001, B00001, B00001, B11001, B00011, B11110};
const byte END_DIV_2_OF_4[8] = {B11110, B00011, B00001, B00001, B11101, B11001, B00011, B11110};
const byte END_DIV_3_OF_4[8] = {B11110, B00011, B00001, B11101, B11101, B11001, B00011, B11110};
const byte END_DIV_4_OF_4[8] = {B11110, B00011, B11001, B11101, B11101, B11001, B00011, B11110};

void start_progressbar() {
  lcd.createChar(0, START_DIV_4_OF_4);
  lcd.createChar(1, DIV_0_OF_8);
  lcd.createChar(2, DIV_8_OF_8);
  lcd.createChar(3, END_DIV_0_OF_4);
}

void print_progressbar(bool text_percent, String text, int delay_ms) { //Main Progressbar funktion
  start_progressbar();
  while (true) {
    static byte percent = 0;
    draw_progressbar(text_percent, percent, text, lcd_rows - 2, lcd_rows - 1);
    if (++percent == 101) {
      percent = 0;
      break;
    }
    delay(delay_ms);
  }
}

void switch_progressbar_bank(byte bank) { //Progressbar Symbol Bank
  switch (bank) {
    case 0:
      lcd.createChar(4, START_DIV_0_OF_4);
      lcd.createChar(5, START_DIV_1_OF_4);
      lcd.createChar(6, START_DIV_2_OF_4);
      lcd.createChar(7, START_DIV_3_OF_4);
      break;

    case 1:
      lcd.createChar(4, DIV_1_OF_8);
      lcd.createChar(5, DIV_2_OF_8);
      lcd.createChar(6, DIV_3_OF_8);
      lcd.createChar(7, DIV_4_OF_8);
      break;

    case 2:
      lcd.createChar(4, DIV_4_OF_8);
      lcd.createChar(5, DIV_5_OF_8);
      lcd.createChar(6, DIV_6_OF_8);
      lcd.createChar(7, DIV_7_OF_8);
      break;

    case 3:
      lcd.createChar(4, END_DIV_1_OF_4);
      lcd.createChar(5, END_DIV_2_OF_4);
      lcd.createChar(6, END_DIV_3_OF_4);
      lcd.createChar(7, END_DIV_4_OF_4);
      break;
  }
}

void draw_progressbar(bool show_text_percent, byte percent, String text, int position_text, int position_bar) { //draw Progressbar
  if (show_text_percent) {
    if (text != "") {
      lcd.setCursor(0, position_text);
      lcd.print(text);
      lcd.setCursor(lcd_colums - 4, position_text);
      lcd.print(percent);
      lcd.print(F("%"));
    }
    else {
      lcd.setCursor(0, position_text);
      lcd.print(percent);
      lcd.print("%");
    }
  }
  lcd.setCursor(0, position_bar);
  byte nb_columns = map(percent, 0, 100, 0, lcd_colums * 2 * 4 - 2 * 4);
  for (byte i = 0; i < lcd_colums; ++i) {
    if (i == 0) {
      if (nb_columns > 4) {
        lcd.write((byte) 0);
        nb_columns -= 4;
      } else if (nb_columns == 4) {
        lcd.write((byte) 0);
        nb_columns = 0;
      } else {
        switch_progressbar_bank(0);
        lcd.setCursor(i, position_bar);
        lcd.write(nb_columns + 4);
        nb_columns = 0;
      }
    } else if (i == lcd_colums - 1) {
      if (nb_columns > 0) {
        switch_progressbar_bank(3);
        lcd.setCursor(i, position_bar);
        lcd.write(nb_columns + 3);
      } else {
        lcd.write(3);
      }
    } else {
      if (nb_columns == 0) {
        lcd.write(1);
      } else if (nb_columns >= 8) {
        lcd.write(2);
        nb_columns -= 8;
      } else if (nb_columns >= 4 && nb_columns < 8) {
        switch_progressbar_bank(2);
        lcd.setCursor(i, position_bar);
        lcd.write(nb_columns);
        nb_columns = 0;
      } else if (nb_columns < 4) {
        switch_progressbar_bank(1);
        lcd.setCursor(i, position_bar);
        lcd.write(nb_columns + 3);
        nb_columns = 0;
      }
    }
  }
}


//################ CLOCK ################

// the 8 arrays that form each segment of the custom numbers
const byte bar1[8] = {0b11100, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11100};
const byte bar2[8] = {0b00111, 0b01111, 0b01111, 0b01111, 0b01111, 0b01111, 0b01111, 0b00111};
const byte bar3[8] = {0b11111, 0b11111, 0b00000, 0b00000, 0b00000, 0b00000, 0b11111, 0b11111};
const byte bar4[8] = {0b11110, 0b11100, 0b00000, 0b00000, 0b00000, 0b00000, 0b11000, 0b11100};
const byte bar5[8] = {0b01111, 0b00111, 0b00000, 0b00000, 0b00000, 0b00000, 0b00011, 0b00111};
const byte bar6[8] = {0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b11111, 0b11111};
const byte bar7[8] = {0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00111, 0b01111};
const byte bar8[8] = {0b11111, 0b11111, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000};

void clock_display(){
  //blink
  if (clock_blink) {
    if ((millis() - clock_last_millis) > 499) {
      if (!clock_dots_activ) {
        if (clock_button_mode == 0) {
          lcd.setCursor(6, 0);
          lcd.print(".");
          lcd.setCursor(6, 1);
          lcd.print(".");
        }
        clock_dots_activ = true;
      }
      else {
        if (clock_button_mode == 0) {
          lcd.setCursor(6, 0);
          lcd.print(" ");
          lcd.setCursor(6, 1);
          lcd.print(" ");
        }
        clock_dots_activ = false;
      }
      clock_last_millis = millis();
    }
  }
  if (!clock_blink) {
    lcd.setCursor(6, 0);
    lcd.print(".");
    lcd.setCursor(6, 1);
    lcd.print(".");
  }
  
  //display time
  if(last_minutes != minutes){
    printNumber(hours % 10, 3);
    printNumber(hours / 10, 0);
    printNumber(minutes % 10, 10);
    printNumber(minutes / 10, 7);
    last_minutes = minutes;
  }
}

void printNumber(int value, int col) {
  switch (value) {
    case 0:
      lcd.createChar(2, bar2);
      lcd.createChar(0, bar8);
      lcd.createChar(1, bar1);
      lcd.createChar(6, bar6);
      //--------------------
      lcd.setCursor(col, 0);
      lcd.write(2);
      lcd.write(0);
      lcd.write(1);
      lcd.setCursor(col, 1);
      lcd.write(2);
      lcd.write(6);
      lcd.write(1);
      break;
    case 1:
      lcd.createChar(1, bar1);
      lcd.createChar(2, bar2);
      lcd.createChar(3, bar3);
      //--------------------
      lcd.setCursor(col, 0);
      lcd.write(32);
      lcd.write(32);
      lcd.write(1);
      lcd.setCursor(col, 1);
      lcd.write(32);
      lcd.write(32);
      lcd.write(1);
      break;
    case 2:
      lcd.createChar(5, bar5);
      lcd.createChar(3, bar3);
      lcd.createChar(1, bar1);
      lcd.createChar(2, bar2);
      lcd.createChar(6, bar6);
      //--------------------
      lcd.setCursor(col, 0);
      lcd.write(5);
      lcd.write(3);
      lcd.write(1);
      lcd.setCursor(col, 1);
      lcd.write(2);
      lcd.write(6);
      lcd.write(6);
      break;
    case 3:
      lcd.createChar(5, bar5);
      lcd.createChar(3, bar3);
      lcd.createChar(1, bar1);
      lcd.createChar(7, bar7);
      lcd.createChar(6, bar6);
      //--------------------
      lcd.setCursor(col, 0);
      lcd.write(5);
      lcd.write(3);
      lcd.write(1);
      lcd.setCursor(col, 1);
      lcd.write(7);
      lcd.write(6);
      lcd.write(1);
      break;
    case 4:
      lcd.createChar(2, bar2);
      lcd.createChar(6, bar6);
      lcd.createChar(1, bar1);
      lcd.createChar(3, bar3);
      lcd.createChar(2, bar2);
      //--------------------
      lcd.setCursor(col, 0);
      lcd.write(2);
      lcd.write(6);
      lcd.write(1);
      lcd.setCursor(col, 1);
      lcd.write(32);
      lcd.write(32);
      lcd.write(1);
      break;
    case 5:
      lcd.createChar(2, bar2);
      lcd.createChar(3, bar3);
      lcd.createChar(4, bar4);
      lcd.createChar(7, bar7);
      lcd.createChar(6, bar6);
      lcd.createChar(1, bar1);
      //--------------------
      lcd.setCursor(col, 0);
      lcd.write(2);
      lcd.write(3);
      lcd.write(4);
      lcd.setCursor(col, 1);
      lcd.write(7);
      lcd.write(6);
      lcd.write(1);
      break;
    case 6:
      lcd.createChar(2, bar2);
      lcd.createChar(3, bar3);
      lcd.createChar(4, bar4);
      lcd.createChar(6, bar6);
      lcd.createChar(1, bar1);
      //--------------------
      lcd.setCursor(col, 0);
      lcd.write(2);
      lcd.write(3);
      lcd.write(4);
      lcd.setCursor(col, 1);
      lcd.write(2);
      lcd.write(6);
      lcd.write(1);
      break;
    case 7:
      lcd.createChar(2, bar2);
      lcd.createChar(0, bar8);
      lcd.createChar(1, bar1);
      lcd.createChar(3, bar3);
      lcd.createChar(2, bar2);
      //--------------------
      lcd.setCursor(col, 0);
      lcd.write(2);
      lcd.write(0);
      lcd.write(1);
      lcd.setCursor(col, 1);
      lcd.write(32);
      lcd.write(32);
      lcd.write(1);
      break;
    case 8:
      lcd.createChar(2, bar2);
      lcd.createChar(3, bar3);
      lcd.createChar(1, bar1);
      lcd.createChar(2, bar2);
      lcd.createChar(6, bar6);
      //--------------------
      lcd.setCursor(col, 0);
      lcd.write(2);
      lcd.write(3);
      lcd.write(1);
      lcd.setCursor(col, 1);
      lcd.write(2);
      lcd.write(6);
      lcd.write(1);
      break;
    case 9:
      lcd.createChar(2, bar2);
      lcd.createChar(3, bar3);
      lcd.createChar(1, bar1);
      lcd.createChar(7, bar7);
      lcd.createChar(6, bar6);
      //--------------------
      lcd.setCursor(col, 0);
      lcd.write(2);
      lcd.write(3);
      lcd.write(1);
      lcd.setCursor(col, 1);
      lcd.write(7);
      lcd.write(6);
      lcd.write(1);
      break;
  }
  /*
    //free numbers at the end
    free(byte(1));
    free(byte(2));
    free(byte(3));
    free(byte(4));
    free(byte(5));
    free(byte(6));
    free(byte(7));
    free(byte(8));
  */
}

//################ MENU ################

//----------- Actions -----------
void executeAction_ok() {
  Serial.print(F("ok menu: "));
  Serial.println(menu);
  
  switch (menu) {
    case 4:
      menu = 5011;
      break;
    case 5011:
      //Device Serial -> Info
      menu = 5015;
      break;
    case 5012:
      //User Serial -> Info
      menu = 5019;
      break;
    case 5016:
      //Device Serial Info -> More Info
      menu = 5022;
      break;
  }
  updateMenu();
}

void executeAction_back() {
  switch (menu) {
    case 5015:
      //Info -> Device Serial
      menu = 5011;
      break;
    case 5019:
      //Info -> User Serial
      menu = 5012;
      break;
    case 5022:
      //More Info -> Device Serial Info
      menu = 5016;
      break;
  }
  Serial.print(F("back menu: "));
  Serial.println(menu);

  updateMenu();
}

//Menu

//constant ui

void updateMenu_constant_ui() {
  if (lcd_rows == 3) {
    lcd.createChar(0, character_arow_right);
    lcd.createChar(1, character_arow_left);
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.write(byte(0));
    switch (menu) {
      case 0:
        menu = 1;
        break;
      case 1:
        lcd.setCursor(1, 1);
        lcd.print(F("MenuItem1")); // <--
        lcd.setCursor(1, 2);
        lcd.print(F("MenuItem2"));
        break;
      case 2:
        lcd.setCursor(1, 0);
        lcd.print(F("MenuItem1"));
        lcd.setCursor(1, 1);
        lcd.print(F("MenuItem2")); // <--
        lcd.setCursor(1, 2);
        lcd.print(F("MenuItem3"));
        break;
      case 3:
        lcd.setCursor(1, 0);
        lcd.print(F("MenuItem2"));
        lcd.setCursor(1, 1);
        lcd.print(F("MenuItem3")); // <--
        lcd.setCursor(1, 2);
        lcd.print(F("Settings"));
        break;
      case 4:
        lcd.setCursor(1, 1);
        lcd.print(F("MenuItem3"));
        lcd.setCursor(1, 1);
        lcd.print(F("Settings")); // <--
        break;
      case 5:
        menu = 4;
        break;

      //################ TASKMANAGER ################
      case 10000:
        menu = 10001;
        break;

      case 10001:
        lcd.setCursor(1, 1);
        lcd.print(F("Work"));
        lcd.setCursor(1, 2);
        lcd.print(F("FPS"));
        break;

      case 10002:
        lcd.setCursor(1, 0);
        lcd.print(F("Work"));
        lcd.setCursor(1, 1);
        lcd.print(F("FPS"));
        lcd.setCursor(1, 2);
        lcd.print(F("Graphics"));
        break;

      case 10003:
        lcd.setCursor(1, 0);
        lcd.print(F("FPS"));
        lcd.setCursor(1, 1);
        lcd.print(F("Graphics"));
        lcd.setCursor(1, 2);
        lcd.print(F("Gaming"));
        break;

      case 10004:
        lcd.setCursor(1, 0);
        lcd.print(F("Graphics"));
        lcd.setCursor(1, 1);
        lcd.print(F("Gaming"));
        lcd.setCursor(1, 2);
        lcd.print(F("Advanced Gaming"));
        break;

      case 10005:
        lcd.setCursor(1, 0);
        lcd.print(F("Gaming"));
        lcd.setCursor(1, 1);
        lcd.print(F("Advanced Gaming"));
        lcd.setCursor(1, 2);
        lcd.print(F("RAM"));
        break;

      case 10006:
        lcd.setCursor(1, 0);
        lcd.print(F("Advanced Gaming"));
        lcd.setCursor(1, 1);
        lcd.print(F("RAM"));
        lcd.setCursor(1, 2);
        lcd.print(F("CPU"));
        break;

      case 10007:
        lcd.setCursor(1, 0);
        lcd.print(F("RAM"));
        lcd.setCursor(1, 1);
        lcd.print(F("CPU"));
        lcd.setCursor(1, 2);
        lcd.print(F("GPU"));
        break;

      case 10008:
        lcd.setCursor(1, 0);
        lcd.print(F("CPU"));
        lcd.setCursor(1, 1);
        lcd.print(F("GPU"));
        lcd.setCursor(1, 2);
        lcd.print(F("Temperatures"));
        break;

      //------------------------------
      case 10009:
        lcd.setCursor(1, 0);
        lcd.print(F("GPU"));
        lcd.setCursor(1, 1);
        lcd.print(F("Temperatures"));
        lcd.setCursor(1, 2);
        lcd.print(F("Processors"));
        break;

      case 10010:
        lcd.setCursor(1, 0);
        lcd.print(F("Temperatures"));
        lcd.setCursor(1, 1);
        lcd.print(F("Processors"));
        break;

      case 10011:
        menu = 10012;
        break;
    }
  }
  else if (lcd_rows >= 4) {
    lcd.createChar(0, character_arow_right);
    lcd.clear();
    lcd.setCursor(0, 2);
    lcd.write(byte(0));
    switch (menu) {
      case 0:
        menu = 1;
        break;
      case 1:
        lcd.setCursor((lcd_colums - 4) / 2, 0);
        lcd.print(F("MENU"));
        lcd.setCursor(1, lcd_rows - 2);
        lcd.print(F("MenuItem1")); // <--
        lcd.setCursor(1, lcd_rows - 1);
        lcd.print(F("MenuItem2"));
        break;
      case 2:
        lcd.setCursor((lcd_colums - 4) / 2, 0);
        lcd.print(F("MENU"));
        lcd.setCursor(1, lcd_rows - 3);
        lcd.print(F("MenuItem1"));
        lcd.setCursor(1, lcd_rows - 2);
        lcd.print(F("MenuItem2")); // <--
        lcd.setCursor(1, lcd_rows - 1);
        lcd.print(F("MenuItem3"));
        break;
      case 3:
        lcd.setCursor((lcd_colums - 4) / 2, 0);
        lcd.print(F("MENU"));
        lcd.setCursor(1, lcd_rows - 3);
        lcd.print(F("MenuItem2"));
        lcd.setCursor(1, lcd_rows - 2);
        lcd.print(F("MenuItem3")); // <--
        lcd.setCursor(1, lcd_rows - 1);
        lcd.print(F("Settings"));
        break;
      case 4:
        lcd.setCursor((lcd_colums - 4) / 2, 0);
        lcd.print(F("MENU"));
        lcd.setCursor(1, lcd_rows - 3);
        lcd.print(F("MenuItem3"));
        lcd.setCursor(1, lcd_rows - 2);
        lcd.print(F("Settings")); // <--
        break;
      case 5:
        menu = 4;
        break;

      //################ TASKMANAGER ################
      case 10000:
        menu = 10001;
        break;

      case 10001:
        lcd.setCursor((lcd_colums - 8) / 2, 0);
        lcd.print(F("PROFILES"));
        lcd.setCursor(1, lcd_rows - 2);
        lcd.print(F("Work"));
        lcd.setCursor(1, lcd_rows - 1);
        lcd.print(F("FPS"));
        break;

      case 10002:
        lcd.setCursor((lcd_colums - 8) / 2, 0);
        lcd.print(F("PROFILES"));
        lcd.setCursor(1, lcd_rows - 3);
        lcd.print(F("Work"));
        lcd.setCursor(1, lcd_rows - 2);
        lcd.print(F("FPS"));
        lcd.setCursor(1, lcd_rows - 3);
        lcd.print(F("Graphics"));
        break;

      case 10003:
        lcd.setCursor((lcd_colums - 8) / 2, 0);
        lcd.print(F("PROFILES"));
        lcd.setCursor(1, lcd_rows - 1);
        lcd.print(F("FPS"));
        lcd.setCursor(1, lcd_rows - 2);
        lcd.print(F("Graphics"));
        lcd.setCursor(1, lcd_rows - 3);
        lcd.print(F("Gaming"));
        break;

      case 10004:
        lcd.setCursor(1, 0);
        lcd.print(F("Graphics"));
        lcd.setCursor(1, lcd_rows - 3);
        lcd.print(F("Gaming"));
        lcd.setCursor(1, lcd_rows - 2);
        lcd.print(F("Advanced Gaming"));
        break;

      case 10005:
        lcd.setCursor((lcd_colums - 8) / 2, 0);
        lcd.print(F("PROFILES"));
        lcd.setCursor(1, lcd_rows - 3);
        lcd.print(F("Gaming"));
        lcd.setCursor(1, lcd_rows - 2);
        lcd.print(F("Advanced Gaming"));
        lcd.setCursor(1, lcd_rows - 1);
        lcd.print(F("RAM"));
        break;

      case 10006:
        lcd.setCursor((lcd_colums - 8) / 2, 0);
        lcd.print(F("PROFILES"));
        lcd.setCursor(1, lcd_rows - 3);
        lcd.print(F("Advanced Gaming"));
        lcd.setCursor(1, lcd_rows - 2);
        lcd.print(F("RAM"));
        lcd.setCursor(1, lcd_rows - 3);
        lcd.print(F("CPU"));
        break;

      case 10007:
        lcd.setCursor((lcd_colums - 8) / 2, 0);
        lcd.print(F("PROFILES"));
        lcd.setCursor(1, lcd_rows - 3);
        lcd.print(F("RAM"));
        lcd.setCursor(1, lcd_rows - 2);
        lcd.print(F("CPU"));
        lcd.setCursor(1, lcd_rows - 1);
        lcd.print(F("GPU"));
        break;

      case 10008:
        lcd.setCursor((lcd_colums - 8) / 2, 0);
        lcd.print(F("PROFILES"));
        lcd.setCursor(1, lcd_rows - 3);
        lcd.print(F("CPU"));
        lcd.setCursor(1, lcd_rows - 2);
        lcd.print(F("GPU"));
        lcd.setCursor(1, lcd_rows - 1);
        lcd.print(F("Temperatures"));
        break;

      //------------------------------
      case 10009:
        lcd.setCursor((lcd_colums - 8) / 2, 0);
        lcd.print(F("PROFILES"));
        lcd.setCursor(1, lcd_rows - 3);
        lcd.print(F("GPU"));
        lcd.setCursor(1, lcd_rows - 2);
        lcd.print(F("Temperatures"));
        lcd.setCursor(1, lcd_rows - 1);
        lcd.print(F("Processors"));
        break;

      case 10010:
        lcd.setCursor((lcd_colums - 8) / 2, 0);
        lcd.print(F("PROFILES"));
        lcd.setCursor(1, lcd_rows - 3);
        lcd.print(F("Temperatures"));
        lcd.setCursor(1, lcd_rows - 2);
        lcd.print(F("Processors"));
        break;

      case 10011:
        menu = 10012;
        break;
    }
  }
}

void updateMenu_roll_ui() {
  lcd.createChar(0, character_arow_right);
  lcd.createChar(1, character_arow_left);
  lcd.clear();
  if (lcd_rows == 2) {
    switch (menu) {
      case 0:
        menu = 1;
        break;
      case 1:
        lcd.setCursor(0, 0);
        lcd.write(byte(0));
        lcd.print(F("MenuItem1"));
        lcd.setCursor(1, 1);
        lcd.print(F("MenuItem2"));
        break;
      case 2:
        lcd.setCursor(1, 0);
        lcd.print(F("MenuItem1"));
        lcd.setCursor(0, 1);
        lcd.write(byte(4));
        lcd.print(F("MenuItem2"));
        break;
      case 3:
        lcd.setCursor(0, 0);
        lcd.write(byte(0));
        lcd.print(F("MenuItem3"));
        lcd.setCursor(1, 1);
        lcd.print(F("Settings"));
        break;
      case 4:
        lcd.setCursor(1, 0);
        lcd.print(F("MenuItem3"));
        lcd.setCursor(0, 1);
        lcd.write(byte(0));
        lcd.print(F("Settings"));
        break;
      case 5:
        menu = 4;
        break;

      //Level 1 (5010 - 5013)
      case 5010:
        menu = 5011;
        break;

      case 5011:
        lcd.setCursor(0, 0);
        lcd.write(byte(0));
        lcd.setCursor(1, 0);
        lcd.print(F("Device Serial"));

        if ((lcd_colums - 17) > 0) {
          //if display is enough wide

          //if !serial X shown
          //if serial -E (plug) shown

          //if serial & !device_serial_connection -E X shown
          //if serial & device_serial_connection -E and check (plug) shown
          lcd.setCursor(14, 0);
          if (!Serial) {
            lcd.createChar(2, character_cross);
            lcd.write(byte(2));
            free(byte(2));
          }
          else if (Serial) {
            free(byte(1));
            lcd.createChar(1, character_plug);
            lcd.write(byte(1));
            lcd.setCursor(15, 0);
            if (!device_serial_connection) {
              lcd.createChar(2, character_cross);
              lcd.write(byte(2));
              free(byte(2));
            }
            else if (device_serial_connection) {
              lcd.createChar(2, character_check);
              lcd.write(byte(2));
              free(byte(2));
            }
          }
        }

        lcd.setCursor(1, 1);
        lcd.print(F("User Serial"));

      case 5012:
        lcd.setCursor(1, 0);
        lcd.print(F("Device Serial"));
        lcd.setCursor(0, 1);
        lcd.write(byte(0));
        lcd.setCursor(1, 1);
        lcd.print(F("User Serial"));
        if ((lcd_colums - 13) > 0) {
          //if display is enough wide
          lcd.setCursor(12, 0);
          if (!user_serial_connection) {
            lcd.createChar(2, character_cross);
            lcd.write(byte(2));
            free(byte(2));
          }
          else {
            lcd.createChar(2, character_check);
            lcd.write(byte(2));
            free(byte(2));
          }
        }
      case 5013:
        menu = 5012;
        break;

      //Level 2   Device Serial (5014-5017) User Serial (5018 - 5020)
      case 5014:
        menu = 5015;
        break;

      case 5015:
        lcd.createChar(2, character_cross);
        lcd.setCursor(0, 0);
        lcd.write(byte(2));
        lcd.setCursor(1, 0);
        lcd.print(F("Device Serial"));
        lcd.setCursor(0, 1);
        if (!Serial) {
          lcd.print(F("unpluged"));
        }
        else if (Serial && !device_serial_connection) {
          lcd.print(F("plugin disabeld"));
        }
        else if (Serial && device_serial_connection) {
          lcd.print(F("plugin activ"));
        }
        free(byte(2));
        break;

      case 5016:
        lcd.createChar(2, character_plug);
        lcd.setCursor(0, 0);
        lcd.write(byte(2));
        lcd.setCursor(1, 0);
        lcd.print(F("Device Serial"));

        lcd.setCursor(0, 1);
        lcd.write(byte(0));
        lcd.setCursor(1, 1);
        lcd.print(F("More Info"));
        free(byte(2));
        break;

      case 5017:
        menu = 5016;
        break;

      case 5018:
        menu = 5019;
        break;

      case 5019:
        lcd.createChar(2, character_cross);
        lcd.setCursor(0, 0);
        lcd.write(byte(2));
        lcd.setCursor(1, 0);
        lcd.print(F("User Serial"));
        lcd.setCursor(1, 1);
        if (!user_serial_connection) {
          lcd.print(F("disabled"));
        }
        else {
          lcd.print(F("enabled"));
        }
        free(byte(2));
        break;

      case 5020:
        menu = 5019;
        break;


      //Level 3 Device Serial (5021 - 5024)
      case 5021:
        menu = 5022;
        break;
      case 5022:
        lcd.setCursor(0, 0);
        lcd.write(byte(2));
        lcd.setCursor(1, 0);
        lcd.print(F("Cable Status"));
        lcd.setCursor(0, 1);
        if (!Serial) {
          lcd.print(F("unpluged"));
        }
        else if (Serial) {
          lcd.print(F("pluged in"));
        }
        break;

      case 5023:
        lcd.setCursor(0, 0);
        lcd.write(byte(2));
        lcd.setCursor(1, 0);
        lcd.print(F("Plugin Status"));
        lcd.setCursor(0, 1);
        if (device_serial_connection) {
          lcd.print(F("enabled"));
        }
        else {
          lcd.print(F("disabeld"));
        }
        break;

      case 5024:
        menu = 5022;
        break;


      //################ TASKMANAGER ################
      case 10000:
        menu = 10001;
        break;

      case 10001:
        lcd.setCursor(0, 0);
        lcd.write(byte(0));
        lcd.setCursor(1, 0);
        lcd.print(F("Work"));
        lcd.setCursor(1, 1);
        lcd.print(F("FPS"));
        break;

      case 10002:
        lcd.setCursor(1, 0);
        lcd.print(F("Work"));
        lcd.setCursor(0, 1);
        lcd.write(byte(0));
        lcd.setCursor(1, 1);
        lcd.print(F("FPS"));
        break;

      case 10003:
        lcd.setCursor(1, 0);
        lcd.print(F("Graphics"));
        lcd.setCursor(1, 1);
        lcd.print(F("Gaming"));
        break;

      case 10004:
        lcd.setCursor(0, 0);
        lcd.write(byte(0));
        lcd.setCursor(1, 0);
        lcd.print(F("Graphics"));
        lcd.setCursor(1, 1);
        lcd.print(F("Gaming"));
        break;

      //------------------------------
      case 10005:
        lcd.setCursor(0, 0);
        lcd.write(byte(0));
        lcd.setCursor(1, 0);
        lcd.print(F("Advanced Gaming"));
        lcd.setCursor(1, 1);
        lcd.print(F("RAM"));
        break;

      case 10006:
        lcd.setCursor(1, 0);
        lcd.print(F("Advanced Gaming"));
        lcd.setCursor(0, 1);
        lcd.write(byte(0));
        lcd.setCursor(1, 1);
        lcd.print(F("RAM"));
        break;

      case 10007:
        lcd.setCursor(0, 0);
        lcd.write(byte(0));
        lcd.setCursor(1, 0);
        lcd.print(F("CPU"));
        lcd.setCursor(1, 1);
        lcd.print(F("GPU"));
        break;

      case 10008:
        lcd.setCursor(1, 0);
        lcd.print(F("CPU"));
        lcd.setCursor(0, 1);
        lcd.write(byte(0));
        lcd.setCursor(1, 1);
        lcd.print(F("GPU"));
        break;

      //------------------------------
      case 10009:
        lcd.setCursor(0, 0);
        lcd.write(byte(0));
        lcd.setCursor(1, 0);
        lcd.print(F("Temperatures"));
        lcd.setCursor(1, 1);
        lcd.print(F("Processors"));
        break;

      case 10010:
        lcd.setCursor(1, 0);
        lcd.print(F("Temperatures"));
        lcd.setCursor(0, 1);
        lcd.write(byte(0));
        lcd.setCursor(1, 1);
        lcd.print(F("Processors"));
        break;

      case 10011:
        menu = 10012;
        break;
    }
  }
  else if (lcd_rows >= 4) {
    switch (menu) {
      case 0:
        menu = 1;
        break;
      case 1:
        lcd.setCursor(0, 0);
        lcd.write(byte(0)); // <--
        lcd.setCursor(1, 0);
        lcd.print(F("MenuItem1"));
        lcd.setCursor(1, 1);
        lcd.print(F("MenuItem2"));
        lcd.setCursor(1, 2);
        lcd.print(F("MenuItem3"));
        lcd.setCursor(1, 3);
        lcd.print(F("Settings"));
        break;
      case 2:
        lcd.setCursor(1, 0);
        lcd.print(F("MenuItem1"));
        lcd.setCursor(0, 1);
        lcd.write(byte(0)); // <--
        lcd.setCursor(1, 1);
        lcd.print(F("MenuItem2"));
        lcd.setCursor(1, 2);
        lcd.print(F("MenuItem3"));
        lcd.setCursor(1, 3);
        lcd.print(F("Settings"));
        break;
      case 3:
        lcd.setCursor(1, 0);
        lcd.print(F("MenuItem1"));
        lcd.setCursor(1, 1);
        lcd.print(F("MenuItem2"));
        lcd.setCursor(0, 2);
        lcd.write(byte(0)); // <--
        lcd.setCursor(1, 2);
        lcd.print(F("MenuItem3"));
        lcd.setCursor(1, 3);
        lcd.print(F("Settings"));
        break;
      case 4:
        lcd.setCursor(1, 0);
        lcd.print(F("MenuItem1"));
        lcd.setCursor(1, 1);
        lcd.print(F("MenuItem2"));
        lcd.setCursor(1, 2);
        lcd.print(F("MenuItem3"));
        lcd.setCursor(0, 3);
        lcd.write(byte(0)); // <--
        lcd.setCursor(1, 3);
        lcd.print(F("Settings"));
        break;
      case 5:
        menu = 4;
        break;

      //Level 1 (5010 - 5013)
      case 5010:
        menu = 5011;
        break;

      case 5011:
        lcd.setCursor(0, 0);
        lcd.write(byte(0));
        lcd.setCursor(1, 0);
        lcd.print(F("Device Serial"));
        if ((lcd_colums - 15) > 0) {
          lcd.createChar(1, character_plug);
          lcd.createChar(3, character_cross);
          lcd.createChar(4, character_check);

          //if display is enough wide

          //if !serial X shown
          //if serial -E (plug) shown

          //if serial & !device_serial_connection -E X shown
          //if serial & device_serial_connection -E and check (plug) shown
          lcd.setCursor(14, 0);
          if (!Serial) {
            lcd.write(byte(3));
          }
          else if (Serial) {
            lcd.write(byte(1));
            lcd.setCursor(15, 0);
            if (!device_serial_connection) {
              lcd.write(byte(3));
            }
            else if (device_serial_connection) {
              lcd.write(byte(4));
            }
          }
          free(byte(1));
          free(byte(3));
          free(byte(4));
        }
        lcd.setCursor(1, 1);
        lcd.print(F("User Serial"));
        break;
        
      case 5012:
        Serial.println("_Start");
        lcd.createChar(2, character_cross);
        lcd.createChar(3, character_check);

        lcd.setCursor(1, 0);
        lcd.print(F("Device Serial"));

        lcd.setCursor(0, 1);
        lcd.write(byte(0));
        lcd.setCursor(1, 1);
        lcd.print(F("User Serial"));

        if ((lcd_colums - 13) > 0) {
          //if display is enough wide
          lcd.setCursor(12, 0);
          if (!user_serial_connection) {
            lcd.write(byte(2));
          }
          else {
            lcd.write(byte(3));
          }
          free(byte(2));
          free(byte(3));
        }
        Serial.println("_end");
        break;

      case 5013:
        menu = 5012;
        break;

      //Level 2   Device Serial (5014-5017) User Serial (5018 - 5020)
      case 5014:
        menu = 5015;
        break;

      case 5015:
        lcd.createChar(2, character_cross);
        lcd.setCursor(0, 0);
        lcd.write(byte(2));
        lcd.setCursor(1, 0);
        lcd.print(F("Device Serial"));
        lcd.setCursor(0, 1);
        lcd.write(byte(0));
        lcd.setCursor(3, 0);
        if (!Serial) {
          lcd.print(F("unpluged"));
        }
        else if (Serial && !device_serial_connection) {
          lcd.print(F("plugin disabeld"));
        }
        else if (Serial && device_serial_connection) {
          lcd.print(F("plugin activ"));
        }
        free(byte(2));
        break;

      case 5016:
        lcd.createChar(2, character_plug);
        lcd.setCursor(0, 0);
        lcd.write(byte(2));
        lcd.setCursor(1, 0);
        lcd.print(F("Device Serial"));

        lcd.setCursor(0, 1);
        lcd.write(byte(0));
        lcd.setCursor(3, 0);
        lcd.print(F("More Info"));
        free(byte(2));
        break;

      case 5017:
        menu = 5016;
        break;

      case 5018:
        menu = 5019;
        break;

      case 5019:
        lcd.createChar(2, character_cross);
        lcd.setCursor(0, 0);
        lcd.write(byte(2));
        lcd.setCursor(1, 0);
        lcd.print(F("User Serial"));
        lcd.setCursor(0, 1);
        lcd.write(byte(0));
        lcd.setCursor(3, 0);
        if (!user_serial_connection) {
          lcd.print(F("disabled"));
        }
        else {
          lcd.print(F("enabled"));
        }
        free(byte(2));
        break;

      case 5020:
        menu = 5019;
        break;


      //Level 3 Device Serial (5021 - 5024)
      case 5021:
        menu = 5022;
        break;

      case 5022:
        lcd.createChar(2, character_plug);
        lcd.setCursor(0, 0);
        lcd.write(byte(2));
        lcd.setCursor(1, 0);
        lcd.print(F("Device Serial"));

        lcd.setCursor(0, 3);
        lcd.write(byte(2));
        lcd.setCursor(1, 3);
        lcd.print(F("Cable Status"));
        lcd.setCursor(0, 3);
        if (!Serial) {
          lcd.print(F("unpluged"));
        }
        else if (Serial) {
          lcd.print(F("pluged in"));
        }
        break;

      case 5023:
        lcd.createChar(2, character_plug);
        lcd.setCursor(0, 0);
        lcd.write(byte(2));
        lcd.setCursor(1, 0);
        lcd.print(F("Device Serial"));

        lcd.setCursor(0, 3);
        lcd.write(byte(2));
        lcd.setCursor(1, 3);
        lcd.print(F("Plugin Status"));
        lcd.setCursor(0, 4);
        if (device_serial_connection) {
          lcd.print(F("enabled"));
        }
        else {
          lcd.print(F("disabeld"));
        }
        break;

      case 5024:
        menu = 5022;
        break;

      //################ TASKMANAGER ################
      case 10000:
        menu = 10001;
        break;

      case 10001:
        lcd.setCursor(0, 0);
        lcd.write(byte(0));
        lcd.setCursor(1, 0);
        lcd.print(F("Work"));
        lcd.setCursor(1, 1);
        lcd.print(F("FPS"));
        lcd.setCursor(1, 2);
        lcd.print(F("Graphics"));
        lcd.setCursor(1, 3);
        lcd.print(F("Gaming"));
        break;

      case 10002:
        lcd.setCursor(1, 0);
        lcd.print(F("Work"));
        lcd.setCursor(0, 1);
        lcd.write(byte(0));
        lcd.setCursor(1, 1);
        lcd.print(F("FPS"));
        lcd.setCursor(1, 2);
        lcd.print(F("Graphics"));
        lcd.setCursor(1, 3);
        lcd.print(F("Gaming"));
        break;

      case 10003:
        lcd.setCursor(1, 0);
        lcd.print(F("Work"));
        lcd.setCursor(1, 1);
        lcd.print(F("FPS"));
        lcd.setCursor(0, 2);
        lcd.write(byte(0));
        lcd.setCursor(1, 2);
        lcd.print(F("Graphics"));
        lcd.setCursor(1, 3);
        lcd.print(F("Gaming"));
        break;

      case 10004:
        lcd.setCursor(1, 0);
        lcd.print(F("Work"));
        lcd.setCursor(1, 1);
        lcd.print(F("FPS"));
        lcd.setCursor(1, 2);
        lcd.print(F("Graphics"));
        lcd.setCursor(0, 3);
        lcd.write(byte(0));
        lcd.setCursor(1, 3);
        lcd.print(F("Gaming"));
        break;

      //------------------------------
      case 10005:
        lcd.setCursor(0, 0);
        lcd.write(byte(0));
        lcd.setCursor(1, 0);
        lcd.print(F("Advanced Gaming"));
        lcd.setCursor(1, 1);
        lcd.print(F("RAM"));
        lcd.setCursor(1, 2);
        lcd.print(F("CPU"));
        lcd.setCursor(1, 3);
        lcd.print(F("GPU"));
        break;

      case 10006:
        lcd.setCursor(1, 0);
        lcd.print(F("Advanced Gaming"));
        lcd.setCursor(0, 1);
        lcd.write(byte(0));
        lcd.setCursor(1, 1);
        lcd.print(F("RAM"));
        lcd.setCursor(1, 2);
        lcd.print(F("CPU"));
        lcd.setCursor(1, 3);
        lcd.print(F("GPU"));
        break;

      case 10007:
        lcd.setCursor(1, 0);
        lcd.print(F("Advanced Gaming"));
        lcd.setCursor(1, 1);
        lcd.print(F("RAM"));
        lcd.setCursor(0, 2);
        lcd.write(byte(0));
        lcd.setCursor(1, 2);
        lcd.print(F("CPU"));
        lcd.setCursor(1, 3);
        lcd.print(F("GPU"));
        break;

      case 10008:
        lcd.setCursor(1, 0);
        lcd.print(F("Advanced Gaming"));
        lcd.setCursor(1, 1);
        lcd.print(F("RAM"));
        lcd.setCursor(1, 2);
        lcd.print(F("CPU"));
        lcd.setCursor(0, 3);
        lcd.write(byte(0));
        lcd.setCursor(1, 3);
        lcd.print(F("GPU"));
        break;

      //------------------------------
      case 10009:
        lcd.setCursor(0, 0);
        lcd.write(byte(0));
        lcd.setCursor(1, 0);
        lcd.print(F("Temperatures"));
        lcd.setCursor(1, 1);
        lcd.print(F("Processors"));
        break;

      case 10010:
        lcd.setCursor(1, 0);
        lcd.print(F("Temperatures"));
        lcd.setCursor(0, 1);
        lcd.write(byte(0));
        lcd.setCursor(1, 1);
        lcd.print(F("Processors"));
        break;

      case 10011:
        menu = 10012;
        break;
        /*
          case 152:
          lcd.setCursor(0, 0);
          lcd.write(byte(0));
          lcd.setCursor(1, 0);
          lcd.print(F("Version"));
          lcd.setCursor(1, 1);
          lcd.print(F("Serial"));
          break;

          case 153:
          lcd.setCursor(0, 0);
          lcd.setCursor(1, 0);
          lcd.print(F("Version"));
          lcd.setCursor(0, 1);
          lcd.write(byte(0));
          lcd.setCursor(1, 1);
          lcd.print(F("Serial"));
          break;
        */

    }
  }
}


void updateMenu() {
  updateMenu_roll_ui();
  //updateMenu_constant_ui();

  /*
  switch (menu) {
    //############# PROGRAMME #############
    case 8000:
      menu = 8001;
      break;
    case 8001:
      lcd.write(byte(0));
      lcd.setCursor(1, 0);
      lcd.print(F("Taskmanager"));
      lcd.setCursor(1, 1);
      lcd.print(F("Clock"));
    case 8002:
      lcd.setCursor(1, 0);
      lcd.print(F("Taskmanager"));
      lcd.setCursor(0, 1);
      lcd.write(byte(0));
      lcd.setCursor(1, 1);
      lcd.print(F("Clock"));
    case 8003:
      lcd.write(byte(0));
      lcd.setCursor(1, 0);
      lcd.print(F("Timer"));
      lcd.setCursor(1, 1);
      lcd.print(F("Stopwatch"));
    case 8004:
      lcd.setCursor(1, 0);
      lcd.print(F("Timer"));
      lcd.setCursor(0, 1);
      lcd.write(byte(0));
      lcd.setCursor(1, 1);
      lcd.print(F("Stopwatch"));
  }
  */
  //32,767
  /*
    Taskmanager
    Clock
    Stopwatch
    Timer
  */
  /*
    0) -> 1
  	1) MenuItem1
  	2) MenuItem2
  	3) MenuItem3
  	4) Settings
    5) -> 4

    //5000
    Settings -> Diplay -> Backlight


    Settings -> Serial -> Device Serial (5010 - 5013) -> Info (5014 - 5017)    -> More Info (5021 - 5024)
                       -> User Serial   (5010 - 5013) -> Info (5018 - 5022)
             -> Version

    Settings -> Sound -> Notification
                      -> Key Sound
                      -> Music
                      -> Sound

    //10000
    Task Manager -> WORK (10001)
    Task Manager -> FPS (10002)
    Task Manager -> Graphics (10003)
    Task Manager -> Gaming (10004)
    Task Manager -> Advanced Gaming (10005)
    Task Manager -> RAM (10006)
    Task Manager -> CPU (10007)
    Task Manager -> GPU (10008)
    Task Manager -> Temperatures (10009)
    Task Manager -> Processors (10010)

    Stop Wotch

    Temperature
    CO2
    Date & Time
  */
  /*


    //Backlight
    case 100:
      menu = 101;
      break;

    case 101:
      lcd.clear();
      lcd.print("Backlight");
      lcd.setCursor(lcd_colums - 5, 1);
      lcd.write(byte(3));
      lcd.print("ON");
      lcd.write(byte(4));
      lcd.print(" ");
      break;

    case 102:
      lcd.clear();
      lcd.print("Backlight");
      lcd.setCursor(lcd_colums - 5, 1);
      lcd.write(byte(3));
      lcd.print("OFF");
      lcd.write(byte(4));
      break;

    case 103:
      menu = 102;
      break;

    //Sound
    case 108:
      menu = 109;

    case 109:
      lcd.clear();
      lcd.print("Sound");
      lcd.createChar(2, character_speaker);
      lcd.createChar(1, character_speaker_sparkle);
      lcd.write(byte(2));
      lcd.setCursor(0, 1);
      lcd.write(byte(2));
      lcd.setCursor(1, 1);
      lcd.write(byte(1));
      break;

    case 110:
      lcd.clear();
      lcd.print("Sound");
      lcd.createChar(2, character_speaker);
      lcd.createChar(1, character_cross);
      lcd.write(byte(2));
      lcd.setCursor(0, 1);
      lcd.write(byte(2));
      lcd.setCursor(1, 1);
      lcd.write(byte(1));
      break;

    case 111:
      menu = 110;
      break;

    //Music
    case 112:
      menu = 113;
      break;

    case 113:
      lcd.clear();
      lcd.print("Music");
      lcd.createChar(2, character_notes);
      lcd.createChar(1, character_check);
      lcd.write(byte(2));
      lcd.setCursor(0, 1);
      lcd.write(byte(2));
      lcd.setCursor(1, 1);
      lcd.write(byte(1));
      break;

    case 114:
      lcd.clear();
      lcd.print("Music");
      lcd.createChar(2, character_notes);
      lcd.createChar(1, character_cross);
      lcd.write(byte(2));
      lcd.setCursor(0, 1);
      lcd.write(byte(2));
      lcd.setCursor(1, 1);
      lcd.write(byte(1));
      break;

    case 115:
      menu = 114;
      break;

    //Bell
    case 116:
      menu = 117;
      break;

    case 117:
      lcd.clear();
      lcd.print("Notification");
      lcd.createChar(2, character_bell);
      lcd.createChar(1, character_check);
      lcd.write(byte(2));
      lcd.setCursor(0, 1);
      lcd.write(byte(2));
      lcd.setCursor(1, 1);
      lcd.write(byte(1));
      break;

    case 118:
      lcd.clear();
      lcd.print("Notification");
      lcd.createChar(2, character_bell);
      lcd.createChar(1, character_cross);
      lcd.write(byte(2));
      lcd.setCursor(0, 1);
      lcd.write(byte(2));
      lcd.setCursor(1, 1);
      lcd.write(byte(1));
      break;

    case 119:
      menu = 118;
      break;


    //Tone
    case 120:
      menu = 121;
      break;

    case 121:
      lcd.clear();
      lcd.print("Key Sound");
      lcd.createChar(2, character_note);
      lcd.createChar(1, character_check);
      lcd.write(byte(2));
      lcd.setCursor(0, 1);
      lcd.write(byte(2));
      lcd.setCursor(1, 1);
      lcd.write(byte(1));
      break;

    case 122:
      lcd.clear();
      lcd.print("Key Sound");
      lcd.createChar(2, character_note);
      lcd.createChar(1, character_cross);
      lcd.write(byte(2));
      lcd.setCursor(0, 1);
      lcd.write(byte(2));
      lcd.setCursor(1, 1);
      lcd.write(byte(1));
      break;

    case 123:
      menu = 122;
      break;


    //lock
    case 124:
      menu = 125;
      break;

    case 125:
      lcd.clear();
      lcd.print("Lock ");
      lcd.createChar(0, character_lock);
      lcd.write(byte(0));
      lcd.setCursor(lcd_colums - 5, 1);
      lcd.write(byte(3));
      lcd.print("ON");
      lcd.write(byte(4));
      lcd.print(" ");
      break;

    case 126:
      lcd.clear();
      lcd.print("Lock ");
      lcd.createChar(0, character_lock);
      lcd.write(byte(0));
      lcd.setCursor(lcd_colums - 5, 1);
      lcd.write(byte(3));
      lcd.print("OFF");
      lcd.write(byte(4));
      break;

    case 127:
      menu = 126;
      break;

    //info


  */
}

//------------------------------------------------

void temp() {
  if (lcd_rows >= 2) {
    /*
      Task Manager -> WORK
      ---------------
      CPU%
      RAM%
      ----------------
    */

    lcd.setCursor(0, 0);
    lcd.print(F("CPU: "));
    lcd.print(F("100")); //CPU PERCENT
    lcd.print(F("%"));
    //------------------
    lcd.setCursor(0, 1);
    lcd.print(F("GPU: "));
    lcd.print(F("100")); //GPU PERCENT
    lcd.print(F("%"));
  }

  if (lcd_rows >= 1) {
    /*
      Task Manager -> FPS
      -----------------
      FPS

      -----------------
    */
    lcd.setCursor(0, 0);
    lcd.print(F("100")); //FPS
    lcd.print(F(" "));
    lcd.print(F("FPS"));
  }

  if (lcd_rows >= 4) {
    lcd.createChar(2, character_celsius);
    /*
      Task Manager -> Graphics
      -----------------
      FPS

      GPU%
      GPU Temp
      -----------------
    */
    lcd.setCursor(0, 0);
    lcd.print(F("100")); //FPS
    lcd.print(F(" "));
    lcd.print(F("FPS"));
    //------------------
    lcd.setCursor(0, 2);
    lcd.print(F("GPU: "));
    lcd.print(F("100")); //GPU PERCENT
    lcd.print(F("%"));
    lcd.setCursor(0, 3);
    lcd.print(F("GPU: "));
    lcd.print(F("100")); //GPU Temp.
    lcd.write(byte(2));
    lcd.print(F("C"));
    free(byte(2));
  }

  if (lcd_rows >= 4) {
    /*
      Task Manager -> Gaming
      -----------------
      FPS
      GPU%
      CPU%
      RAM%
      -----------------
    */
    lcd.setCursor(0, 0);
    lcd.print(F("100")); //FPS
    lcd.print(F(" "));
    lcd.print(F("FPS"));
    //------------------
    lcd.setCursor(0, 1);
    lcd.print(F("GPU: "));
    lcd.print(F("100")); //GPU PERCENT
    lcd.print(F("%"));
    //------------------
    lcd.setCursor(0, 1);
    lcd.print(F("CPU: "));
    lcd.print(F("100")); //CPU PERCENT
    lcd.print(F("%"));
    //------------------
    lcd.setCursor(0, 1);
    lcd.print(F("RAM: "));
    lcd.print(F("100")); //RAM PERCENT
    lcd.print(F("%"));
  }

  if (lcd_rows >= 4) {
    lcd.createChar(2, character_celsius);
    /*
      Task Manager -> Advanced Gaming
      -----------------
      FPS
      GPU%    GPU Temp.
      CPU%    CPU Temp.
      RAM%
      -----------------
    */
    lcd.setCursor(0, 0);
    lcd.print(F("100")); //FPS
    lcd.print(F(" "));
    lcd.print(F("FPS"));
    //------------------
    lcd.setCursor(0, 1);
    lcd.print(F("GPU: "));
    lcd.print(F("100")); //GPU PERCENT
    lcd.print(F("%"));
    //------------------
    lcd.setCursor(0, 1);
    lcd.print(F("CPU: "));
    lcd.print(F("100")); //CPU PERCENT
    lcd.print(F("%"));
    //------------------
    lcd.setCursor(0, 1);
    lcd.print(F("RAM: "));
    lcd.print(F("100")); //RAM PERCENT
    lcd.print(F("%"));
    //------------------
    lcd.setCursor(10, 1);
    lcd.print(F("100")); //GPU Temp.
    lcd.write(byte(2));
    lcd.print(F("C"));
    //------------------
    lcd.setCursor(10, 2);
    lcd.print(F("100")); //CPU Temp.
    lcd.write(byte(2));
    lcd.print(F("C"));
    free(byte(2));
  }

  if (lcd_rows >= 2) {
    /*
      Task Manager -> RAM
      ---------------
      RAM%
      RAM USED/RAM ALL
      ---------------
    */

    lcd.setCursor(0, 0);
    lcd.print(F("RAM: "));
    lcd.print(F("100")); //RAM PERCENT
    lcd.print(F("%"));
    //------------------
    if (lcd_rows >= 3) {
      lcd.setCursor(0, 2);
    }
    else if (lcd_rows == 2) {
      lcd.setCursor(0, 1);
    }
    lcd.print(F("7.810GB")); //RAM USED
    lcd.print(F("/"));
    lcd.print(F("7.899GB")); //RAM ALL
  }

  if (lcd_rows >= 2) {
    lcd.createChar(2, character_celsius);
    /*
      Task Manager -> CPU
      ---------------
      CPU%
      CPU Temp
      ---------------
    */
    lcd.setCursor(0, 0);
    lcd.print(F("CPU: "));
    lcd.print(F("100")); //CPU PERCENT
    lcd.print(F("%"));
    //------------------
    lcd.setCursor(0, 1);
    lcd.print(F("CPU: "));
    lcd.print(F("100")); //CPU Temp.
    lcd.write(byte(2));
    lcd.print(F("C"));
    free(byte(2));
  }

  if (lcd_rows >= 2) {
    lcd.createChar(2, character_celsius);
    /*
      Task Manager -> GPU
      ---------------
      GPU%
      GPU Temp
      ---------------
    */
    lcd.setCursor(0, 0);
    lcd.print(F("GPU: "));
    lcd.print(F("100")); //GPU PERCENT
    lcd.print(F("%"));
    //------------------
    lcd.setCursor(0, 1);
    lcd.print(F("GPU: "));
    lcd.print(F("100")); //GPU Temp.
    lcd.write(byte(2));
    lcd.print(F("C"));
    free(byte(2));
  }

  if (lcd_rows >= 2) {
    lcd.createChar(2, character_celsius);
    /*
      Task Manager -> CPU Temp / GPU Temp
      ---------------
      CPU Temp
      GPU Temp
      ---------------
    */
    lcd.print(F("CPU: "));
    lcd.print(F("100")); //CPU Temp.
    lcd.write(byte(2));
    lcd.print(F("C"));
    //------------------
    lcd.print(F("GPU: "));
    lcd.print(F("100")); //GPU Temp.
    lcd.write(byte(2));
    lcd.print(F("C"));
    free(byte(2));
  }

  if (lcd_rows >= 2) {
    /*
      Task Manager -> CPU% / GPU%
      ----------------
      CPU%
      GPU%
      ----------------
    */

    lcd.setCursor(0, 0);
    lcd.print(F("CPU: "));
    lcd.print(F("100")); //CPU PERCENT
    lcd.print(F("%"));
    //------------------
    lcd.setCursor(0, 1);
    lcd.print(F("GPU: "));
    lcd.print(F("100")); //GPU PERCENT
    lcd.print(F("%"));
  }



}

void device_serial_command_process(String input) {
  //remove ! from command
  //!display -> display
  input.trim();
  input = input.substring(1);

  String str = input;
  String strs[8];
  int StringCount = 0;

  while (str.length() > 0)
  {
    int index = str.indexOf(' ');
    if (index == -1) //No space found
    {
      strs[StringCount++] = str;
      break;
    }
    else
    {
      strs[StringCount++] = str.substring(0, index);
      str = str.substring(index + 1);
    }
  }
  String args[8] = strs;


  //String command = args[0];
  //String arg = args[1];

  //!menu
  //!menu
  //!date
  //!time

  //!version
  //!home
  //!settings


  //############## Process Commands ##################
  if (args[0] == F("menu")) {
    menu = args[1] .toInt();
    Serial.println(menu);
    updateMenu();
  }
  else if (args[0] == F("time")) {
    if (args[1] == F("set")) {
      hours = args[1].toInt();
      minutes = args[2].toInt();
      seconds = args[3].toInt();
    }
    else if (args[1] == F("get")) {
      Serial.println(hours);
      Serial.println(minutes);
      Serial.println(seconds);
    }
  }
  else if (args[0] == F("date")) {
    if (args[1] == F("set")) {
      days = args[1].toInt();
      months = args[2].toInt();
      years = args[3].toInt();
    }
    else if (args[1] == F("get")) {
      Serial.println(days);
      Serial.println(months);
      Serial.println(years);
    }
  }

  else if (args[0] == "version") {
    Serial.println(F("1.1.1"));
  }

  else if (args[0] == "home") {
    menu = 1;
    updateMenu();
  }
  else if (args[0] == F("settings")) {
    menu = 5001;
    updateMenu();
  }
}

void process_commands(String input) {
  //remove ! from command
  //!display -> display
  input.trim();
  input = input.substring(1);

  String str = input;
  String strs[8];
  int StringCount = 0;

  while (str.length() > 0)
  {
    int index = str.indexOf(' ');
    if (index == -1) //No space found
    {
      strs[StringCount++] = str;
      break;
    }
    else
    {
      strs[StringCount++] = str.substring(0, index);
      str = str.substring(index + 1);
    }
  }
  String args[8] = strs;


  //String command = args[0];
  //String arg = args[1];

  //!display ClearScreen
  //!display Display
  //!display NoDisplay
  //!display ShowCursorON
  //!display ShowCursorOFF
  //!display CursorBlock
  //!display CursorBlinkOFF
  //!display LCDhome
  //!display Autoscroll
  //!display NoAutoscroll
  //!display LeftToRight
  //!display RightToLeft
  //!display Backlight
  //!display NoBacklight
  //set cursor example: !display SetCursor 0,0


  //---------- Cursor Basics ----------
  //!up
  //!down
  //!ok
  //!back

  //!home
  //!settings

  //!date
  //!time

  //!menu

  //!device-name
  //!serial


  //############## Process Commands ##################
  //---------- Cursor Basics ----------
  if (args[0] == F("up")) {
    menu--;
    updateMenu();
  }
  else if (args[0] == F("down")) {
    menu++;
    updateMenu();
  }
  else if (args[0] == F("ok")) {
    executeAction_ok();
  }
  else if (args[0] == F("back")) {
    executeAction_back();
  }
  //------------------------------------

  else if (args[0] == "home") {
    menu = 1;
    updateMenu();
  }
  else if (args[0] == F("settings")) {
    menu = 5001;
    updateMenu();
  }

  else if (args[0] == F("time")) {
    if (args[1] == F("set")) {
      hours = args[1].toInt();
      minutes = args[2].toInt();
      seconds = args[3].toInt();
    }
    else if (args[1] == F("get")) {
      user_serial.print(hours);
      user_serial.print(F(":"));
      user_serial.print(minutes);
      user_serial.print(F(":"));
      user_serial.println(seconds);
    }
  }
  else if (args[0] == F("date")) {
    if (args[1] == F("set")) {
      days = args[1].toInt();
      months = args[2].toInt();
      years = args[3].toInt();
    }
    else if (args[1] == F("get")) {
      user_serial.print(days);
      user_serial.print(F("."));
      user_serial.print(months);
      user_serial.print(F("."));
      user_serial.println(years);
    }
  }

  else if (args[0] == F("menu")) {
    if (sizeof(args) == 2) {
      menu = args[1] .toInt();
      user_serial.print(F("menu set to "));
      user_serial.println(menu);

      updateMenu();
    }
    else {
      user_serial.print(F("menu at "));
      user_serial.println(menu);
      user_serial.println(F("!menu {value}"));
    }
  }

  //##################### INFO #########################
  else if (args[0] == F("device-name")) {
    if (Serial && device_serial_connection && device_name == "") {
      //ask for it
    }
    //else if(!(device_name == "")){}
    user_serial.print(F("device name: "));
    user_serial.println(device_name);
  }
  else if (args[0] == F("serial")) {
    user_serial.print(F("cable status: "));
    if (!Serial) {
      user_serial.print(F("dis"));
    }
    user_serial.println(F("connected"));
    //---------------------------------
    user_serial.print(F("plugin status: "));
    user_serial.println(device_serial_connection);
  }

  else if (args[0] == F("stats")) {

  }

  if (args[0] == F("return")) {
    user_serial.println(input.substring(input.indexOf(" ")));
  }

  else if (args[0] == F("lock")) {
    //lock
  }
  else if (args[0] == F("taskmanager")) {
    if (args[1] == "work") {

    }
  }
  if (args[0] == "display") {
    //#### Display Text ####
    //!display print ""
    if (args[1] == "help") {
      Serial.println(F("--------------------- HELP ---------------------"));
      Serial.println(F("!display ClearScreen"));
      Serial.println(F("----------"));
      Serial.println(F("!display Display"));
      Serial.println(F("!display NoDisplay"));
      Serial.println(F("----------"));
      Serial.println(F("!display ShowCursorON"));
      Serial.println(F("!display ShowCursorOFF"));
      Serial.println(F("----------"));
      Serial.println(F("!display CursorBlock"));
      Serial.println(F("!display CursorBlinkOFF"));
      Serial.println(F("----------"));
      Serial.println(F("!display Autoscroll"));
      Serial.println(F("!display NoAutoscroll"));
      Serial.println(F("----------"));
      Serial.println(F("!display LeftToRight"));
      Serial.println(F("!display RightToLeft"));
      Serial.println(F("----------"));
      Serial.println(F("!display Backlight"));
      Serial.println(F("!display NoBacklight"));
      Serial.println(F("----------"));
      Serial.println(F("set cursor example: !display SetCursor 0,0"));
      Serial.println(F("!display LCDhome"));

    }
    else if (args[1] == "Print") {
      lcd.println(args[2]);
    }
    else if (args[1] == "ClearScreen") {
      lcd.clear();
    }
    else if (args[1] == "Display") {
      lcd.display();
    }
    else if (args[1] == "NoDisplay") {
      lcd.noDisplay();
    }
    else if (args[1] == "ShowCursorON") {
      lcd.cursor();
    }
    else if (args[1] == "ShowCursorOFF") {
      lcd.noCursor();
    }
    else if (args[1] == "CursorBlock") {
      lcd.blink();
    }
    else if (args[1] == "CursorBlinkOFF") {
      lcd.noBlink();
    }
    else if (args[1] == "LCDhome") {
      lcd.home();
    }
    else if (args[1] == "Autoscroll") {
      lcd.autoscroll();
    }
    else if (args[1] == "NoAutoscroll") {
      lcd.noAutoscroll();
    }
    else if (args[1] == "LeftToRight") { //Direction of text (left to right)
      lcd.leftToRight();
    }
    else if (args[1] == "RightToLeft") { //Direction of text (right to left)
      lcd.rightToLeft();
    }
    else if (args[1] == "Backlight") { //Direction of text (left to right)
      lcd.backlight();
    }
    else if (args[1] == "NoBacklight") { //Direction of text (right to left)
      lcd.noBacklight();
    }

    else if (args[1] == ("SetCursor")) {
      int cursorColumn = 0;
      int cursorRow = 0;

      for (int i = 0; i < args[2].length(); i++) {
        if (args[2].substring(i, i + 1) == ",") {
          cursorColumn = args[2].substring(0, i).toInt();
          cursorRow = args[2].substring(i + 1).toInt();
          break;
        }
      }
      Serial.println(cursorColumn);
      Serial.println(cursorRow);
      lcd.setCursor(cursorColumn, cursorRow);
    }
  }
  Serial.println("command end");
}


void setup() {
  //--------- Time ---------
  hours = 0;
  minutes = 0;
  seconds = 0;
  //--------- Date ---------
  years = 0;
  months = 0;
  days = 0;

  lcd.init(); //LCD Display init
  lcd.backlight(); //LCD Display activate Backlight

  //init buttons
  pinMode(pin_button_up, INPUT_PULLUP);
  pinMode(pin_button_down, INPUT_PULLUP);
  pinMode(pin_button_right, INPUT_PULLUP);
  pinMode(pin_button_left, INPUT_PULLUP);
  pinMode(pin_button_ok, INPUT_PULLUP);
  pinMode(pin_button_back, INPUT_PULLUP);


  //------------------------ ONLY FOR SHOW ------------------------
  if (!no_useless_usage) //useless wait for startup
  {
    delay(random(1000, 4001)); //wait for random number between 2000 and 4000 milliseconds (2-4 seconds)
  }

  if (!no_useless_usage) //to load start screen
  {
    if ((lcd_rows % 2) == 0) //if number is even
    {
      lcd.setCursor(0, 0);
      lcd.print(F("Arduino"));
      delay(1000);
      lcd.setCursor(lcd_colums - 12, 1);
      lcd.print(F("OS"));
    }
    else //if number is not even
    {
      if (lcd_rows >= 3) {
        lcd.setCursor(0, 1);
        lcd.print(F("Arduino"));
        delay(1000);
        lcd.setCursor(lcd_colums - 3 - 1, 2);
        lcd.print(F("OS"));
      }
      else {
        if (lcd_colums >= 12) {
          lcd.setCursor(0, 0);
          lcd.print(F("Arduino OS"));
          delay(1000);
        }
      }
    }
    /*
      if Display has two rows

      --Start Display View--
      ------------------
      |Simpel          |
      |   Dynamic Menu |
      ------------------
    */
    /*
      if Display has three rows

      --Start Display View--
      ------------------
      |Simpel          |
      |   Dynamic Menu |
      |(==============)|
      ------------------
    */
    /*
      if Display has four rows

      --Start Display View--
      ------------------
      |Simpel          |
      |   Dynamic Menu |
      |                |
      |(==============)|
      ------------------
    */
  }

  if (!no_useless_usage) //progressbars
  {
    print_progressbar(false, F(""), 50);
    delay(5000);
    lcd.clear();

    delay(2000);

    print_progressbar(false, F(""), 50);
    delay(5000);
    lcd.clear();
  }
  //------------------------------------------------

  //###################### Connections ######################
  //----------- Serial Connection ----------
  start_progressbar();
  draw_progressbar(true, 0, F(""), lcd_rows - 2, lcd_rows - 1);

  Serial.begin(57600);
  Serial.println(F("!online"));
  draw_progressbar(true, 1, F(""), lcd_rows - 2, lcd_rows - 1);

  unsigned int wait_on_serial_connetion = millis();
  //Show Display Message that Serial is not connected
  if (!Serial) {
    if (lcd_rows <= 3) {
      lcd.clear();
      /*
        ------------------
        0 |i connect to pc |
        1 |                |
        ------------------

        --------------------------

          ------------------
        0 |i connect to pc |
        1 |1%              |
        2 |(==============)|
          ------------------
      */
    }
    /*
        ------------------
      0 |i connect to pc |
      1 |                |
      2 |1%              |
      3 |(==============)|
        ------------------
    */

    //lcd.setCursor(0, 0);

    //lcd.createChar(7, character_info); //load small Info Symbol
    //lcd.write(byte(7)); //show small Info Symbol on start of message

    lcd.setCursor(2, 0);
    lcd.print(F("Connect to PC")); //show message
  }
  draw_progressbar(true, 2, F(""), lcd_rows - 2, lcd_rows - 1);

  //Wait for the serial port to connect
  while (!Serial && (millis() - wait_on_serial_connetion) < 30000) {
    //Wait for the serial port to connect. Required for native USBs.
  }

  //serial port Connected
  draw_progressbar(true, 3, F(""), lcd_rows - 2, lcd_rows - 1);

  //serial connection activ
  if (Serial) {
    wait_on_serial_connetion = millis();
    //if(millis() - wait_on_serial_connetion) < 30000)
    //connection must use less than 30 seconds
    while ((millis() - wait_on_serial_connetion) < 30000 && !device_serial_connection) {
      //Wait for pc to send !pc_online
      //------------------------------
      //if Serial.available() something to read on serial port
      //if Serial.readString() == "!pc_online"
      if (Serial.available()) {
        String temp = Serial.readString();
        temp.trim();
        if (temp == F("!online")) {
          //pc connected and online and plugin loaded
          device_serial_connection = true;
          Serial.print(F("!connected"));
        }
      }
    }
  }
  free(wait_on_serial_connetion);
  draw_progressbar(true, 4, F("Bluetooth"), lcd_rows - 2, lcd_rows - 1);

  if (device_serial_connection) {
    //start PC Connection Protocol
  }
  //--------------------------------------------------------


  //----------- User Connection ----------
  if (user_serial_connection) {
    //Define pin modes for TX and RX
    pinMode(4, INPUT);
    pinMode(5, OUTPUT);

    //Set the baud rate for the SoftwareSerial object
    user_serial.begin(9600);
  }
  //--------------------------------------------------------

  draw_progressbar(true, 100, F("Finsih"), lcd_rows - 2, lcd_rows - 1);
  Serial.println(F("Finsih"));

  lcd.clear();

  //updateMenu();
  clock_shown = true;
}

void loop() {
  //Buttons
  /*

  //up Button
  if (digitalRead(pin_button_up) == LOW) {
    menu = menu - 1;
    updateMenu();
    while (digitalRead(pin_button_down) == LOW);
  }

  //down Button
  if (digitalRead(pin_button_down) == LOW) {
    menu = menu + 1;
    updateMenu();
    while (digitalRead(pin_button_down) == LOW);
  }

  //ok Button
  if (digitalRead(pin_button_ok) == LOW) {
    executeAction_ok();
    while (digitalRead(pin_button_ok) == LOW);
  }

  //back Button
  if (digitalRead(pin_button_back) == LOW) {
    executeAction_back();
    while (digitalRead(pin_button_back) == LOW);
  }

  */

  //------- Clock -------
  //count time
  
  if ((millis() - wait_on_second) > 999) {
    wait_on_second = millis();
    seconds++;
    if (seconds > 59) {
      seconds = 0;
      minutes++;
      if (minutes > 59) {
        minutes = 0;
        hours++;
        if (hours > 23) {
          hours = 0;
          days++;
        }
      }
    }
    Serial.println(minutes);
    Serial.println(seconds);
    Serial.println(last_minutes);
    Serial.println("------------");
  }
  if(clock_shown){
    clock_display();
  }

  //Serial.println();
  if (user_serial.available()) {
    String temp = user_serial.readString();
    Serial.println(temp);
    process_commands(temp);
  }
  /*
    if (!Serial) {
    //USB Serial not Connected
    }
    if (Serial && !device_serial_connection) {
    //USB Serial Connected
    //PC Plugin not Working
    }

    if (user_serial.available()) {
    Serial.write(user_serial.read());
    }
    if (Serial.available()) {
    user_serial.write(Serial.read());
    }
  */
}

/*
  --------------------------Explanation and Info------------------------------

  comments won't get complied by the compiler



  ------------------------ Reduce file size and memory size ------------------------
  Official Arduino Suport: https://support.arduino.cc/hc/en-us/articles/360013825179
  https://forum.arduino.cc/t/compiler-flag^s-for-smallest-code-size/583694/11

  free();
  malloc();

  ------------------------  #randomSeed ------------------------
  if analog input pin 0 is unconnected, random analog
  noise will cause the call to randomSeed() to generate
  different seed numbers each time the sketch runs.
  randomSeed() will then shuffle the random function. (randomSeed(seed);)

  randomSeed(analogRead(0));

  ------------------------ is ist even? ------------------------
  Trying to determine if a number is odd or even on an Arduino is rather simple.
  There are several ways, but the easiest ways are to either take the modulo of the number to be tested,
  or do a bitwise AND on the Least Significant Bit.

  If x % 2 is ‘0’ that means that x is even. If it not zero, it is an uneven number.

  If x & 1 is ‘1’ it must be an odd number. If it is ‘0’ it must be an even number.

  The ‘x & 1’ method works for the standard arduino negative integers as well.

  it will not work on floats !!!!!!!!!!!!!

  ‘1’ = true
  ‘0’ = false

  from https://arduinodiy.wordpress.com/2015/11/13/checking-for-odd-and-even-numbers-on-the-arduino/

  Example:

  if (x % 1)
  {
    it is an uneven number
  }
  else{
    it is an even number
  }
  If 10 % 2 is ‘0’ that means that 10 is even.

  If 9 % 2 is ‘1’ that means that 10 is an uneven.

*/
