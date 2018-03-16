// define display libraries
#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

// Define Display parameters
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA);   // pin remapping with ESP8266 HW I2C DISPLAY


// Define Humidity & Temperature sensor libraries
#include <dht.h>
dht DHT;
#define DHT11_PIN 3

// Define Clock Libraries for DS3231 For Pi
#include "DS3231.h"

// Define Clock parameters
DS3231 Clock;
bool Century=false;
bool h12 = false;
bool PM = false;

// End of constructor list
#include "DS3231.h"

void setup(void) {   
   // Start the I2C interface so we can start writing data to the DS3231 RTC Module
  Wire.begin();
    
  
//Setup the time - Use the ONLY once for initialization!!!
  Clock.setSecond(00);//Set the second 
  Clock.setMinute(20);//Set the minute 
  Clock.setHour(03);  //Set the hour 
  Clock.setDoW(5);    //Set the day of the week
  Clock.setDate(29);  //Set the date of the month
  Clock.setMonth(12);  //Set the month of the year
  Clock.setYear(17);  //Set the year (Last two digits of the year)
  
  // Start the serial interface
  Serial.begin(9600);

  u8g2.begin();            // Starting the display lib 
  loading();               // Calling load method to show the demo at the startup
}

void  showSklLogo(void){
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB10_tr); // choose a suitable font
    u8g2.drawStr(0, 15, "SKL Electronics");
    u8g2.sendBuffer();
}

void readingSensorData(void){
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(0, 30, "Reading sensor data...");
    u8g2.sendBuffer();
}


void loading(void){
  for (int i = 0; i < 2; i++) {
    showSklLogo();
    delay(600);
    readingSensorData();
    delay(600);
    u8g2.clearBuffer();
    showSklLogo();
    delay(600);
    readingSensorData();
    delay(600);
    u8g2.clearBuffer();
  }

  int check = DHT.read11 (DHT11_PIN);
  
  if(DHT.temperature <= 0 || DHT.humidity <= 0){
    u8g2.setFont(u8g2_font_ncenB10_tr); // choose a suitable font
    u8g2.drawStr(25, 20, "ERROR");
     u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(0, 32, "H&T sensor fault");
    u8g2.sendBuffer();
    delay(4000);
    u8g2.clearBuffer();
  }else {
    u8g2.setFont(u8g2_font_ncenB14_tr); // choose a suitable font
    u8g2.drawStr(30, 25, "Ready");
    u8g2.sendBuffer();
    delay(4000);
    u8g2.clearBuffer();
  } 
  katrinLogo();
}

void katrinLogo(void){
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB18_tr); // choose a suitable font
  u8g2.drawStr(0, 28, "* Katrin *");
  u8g2.sendBuffer();
  delay(4000);
  u8g2.clearBuffer();
}


int tempCalibration = 4;  // Calibrating with more sensitive dual metal sensor

void temp(void){
  char t_str[3];
  int check = DHT.read11 (DHT11_PIN);
  uint8_t t = DHT.temperature - tempCalibration;
//  u8g2.clearBuffer();
  strcpy(t_str, u8x8_u8toa(t, 2));
  u8g2.setFont(u8g2_font_logisoso20_tn); // choose a suitable font
  u8g2.drawStr(2, 32, t_str);
  u8g2.setFont(u8g2_font_ncenB12_tr);
  u8g2.drawStr(32, 24, "*C");
  u8g2.setFont(u8g2_font_ncenB18_tr);
  u8g2.drawStr(55, 31, "|");
}

void humidity(void){
  char h_str[3];
  int check = DHT.read11 (DHT11_PIN);
  uint8_t h = DHT.humidity;
//  u8g2.clearBuffer();
  strcpy(h_str, u8x8_u8toa(h, 2));
  u8g2.setFont(u8g2_font_logisoso20_tn); // choose a suitable font
  u8g2.drawStr(75, 32, h_str);
  u8g2.setFont(u8g2_font_ncenB18_tr);
  u8g2.drawStr(105, 30, "%");
  u8g2.sendBuffer();
}

// Define the Clock variables (TODO - delete the clock code)
uint8_t m = 9;
uint8_t h = 2; 

void watch(void){
  u8g2.clearBuffer();
  
  char h_str[3];
  char m_str[3];
  
  /* convert m to a string with two digits */
  strcpy(h_str, u8x8_u8toa(h, 2));
  strcpy(m_str, u8x8_u8toa(m, 2));

  u8g2.setFont(u8g2_font_logisoso30_tn);
  u8g2.drawStr(20,32,h_str);
  u8g2.drawStr(55,30,":");
  u8g2.drawStr(65,32, m_str);
  u8g2.sendBuffer();
  delay(4000);
  u8g2.clearBuffer();
}

// RTC CLock for Pi

void readTime(void){
  u8g2.clearBuffer();
  uint8_t h = Clock.getHour();
  uint8_t m = Clock.getMinute();

  char h_str[3];
  char m_str[3];
  
  /* convert m to a string with two digits */
  strcpy(h_str, u8x8_u8toa(h, 2));
  strcpy(m_str, u8x8_u8toa(m, 2));
  
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(2,8,h_str);
  u8g2.drawStr(15,8,":");
  u8g2.drawStr(20,8,m_str);
}

void readData(void){
  uint8_t day = Clock.getDate();
  uint8_t month = Clock.getMonth(Century);
  uint8_t year = Clock.getYear();
   
  char day_str[3];
  char month_str[3];
  char year_str[3];
  
  /* convert m to a string with two digits */
  strcpy(day_str, u8x8_u8toa(day, 2));
  strcpy(month_str, u8x8_u8toa(month, 2));
  strcpy(year_str, u8x8_u8toa(year, 2));
  
//  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(75,8,day_str);
  u8g2.drawStr(90,8,"/");
  u8g2.drawStr(95,8,month_str);
  u8g2.drawStr(110,8,"/");
  u8g2.drawStr(115,8,year_str);
//  u8g2.sendBuffer();
//  delay(4000);
//  u8g2.clearBuffer();
}

void katrinHeader(void){
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(35,8,"Katrin");
}


/* 
 *  Main Void Loop as a main logic process loop
 */
void loop(void) {
  
//    katrinLogo();
// ReadTime has clearBuffer, Humidity() has sendBuffer();
    readTime();
    readData();
    katrinHeader();
    temp();
    humidity();
    
    delay(2000);
    
 }

