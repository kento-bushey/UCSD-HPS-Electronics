#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_RA8875.h>

#define RA8875_CS 10
#define RA8875_RESET 9

//800 x 480;

Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);


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
    
}

float angle=0;

void loop(){

  
    int screenCenterX = tft.width()/2;
    int screenCenterY = tft.height()/2;
    uint16_t white=color565(255,255,255);
    uint16_t black=0;
    float scale=0.3;                          
    float zAxis=1500;
    float focalLength=300;

    float hOffset=-800;
    float pOffset=0;
    float sOffset=800;

    
    for(int i =0; i<clen; i+=3){
       tft.drawBezier( subPoints[curves[i]], subPoints[curves[i+1]], subPoints[curves[i+2]], 20, color565(255, 255, 255) );
    }
    for(int i =0; i<llen; i+=2){
      tft.drawLine(subPoints[lines[i]].x,subPoints[lines[i]].y,subPoints[lines[i+1]].x,subPoints[lines[i+1]].y,color565(255,255,255));
    }
   
    // advance angle
    angle+=0.05f;
    
    

    

}
