
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_RA8875.h>


//Structure of widgets: base class inherited by specific subclasses. Include them here.
  /* 
   *  The main function to override is Draw(value, color). 
   * Undraw(bgColor) calls Draw(prevValue, bgColor);
   * As long as Draw(value) sets prevalue=value at the end
   * It might be useful to implement Init_MyWidget(var helper1, var helper2) as well.
   */
#include "Widget.h"
#include "PitchMeter.h"


#define RA8875_CS 10
#define RA8875_RESET 9

//800 x 480 pixel LCD screen.
Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);

//Main widget array, push subclasses individulally in setup, and call Draw+UnDraw for each in loop.
//change both of these constants when adding new widgets
Widget* Widgets[1];
int numWidgets = 1; 

//Initialize widgets with pos, w, h, and screen;
PitchMeter pitchMeter = PitchMeter(100, 100, 200,200, tft);

struct Point subPoints[] = { {100,100},{100,300},{60,120},{140,120}, {70,250}, {80,250}, {130,250}, {120,250},{100,250}, {100,260},{90,300}, {110,300} };
uint16_t curves[] = {0,1,2,0,1,3};
uint16_t lines[] = {4,5,6,7,8,9,10,11};
int clen=sizeof(curves)/sizeof(curves[0]);
int llen=sizeof(lines)/sizeof(lines[0]);


uint16_t color565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

void setup(){
    Serial.begin(9600);
    if(!tft.begin(RA8875_800x480)) while(1);
    tft.displayOn(true);
    tft.GPIOX(true);
    tft.PWM1config(true, RA8875_PWM_CLK_DIV1024);
    tft.PWM1out(255);
    tft.fillScreen(0);

    //Specific initialization methods for each widget
    pitchMeter.InitPitchMeter(100,120,1);            

    //Add widgets to fixed length pointer array
    Widgets[0] = &pitchMeter;
}

float angle=0;

void loop(){
  //Draw widgets at values:
   
   //ex, pitch meter at 20 degrees in white
  pitchMeter.Draw(20, color565(0xFF,0xFF,0xFF));



  //UnDraw all widgets:
  //TODO: compare screen clear with undrawing
   //schedule unDraw()'s to only call on changed sensor values?
  for(int i=0; i<numWidgets; i++){
    Widgets[i]->UnDraw(0);
  }
}
