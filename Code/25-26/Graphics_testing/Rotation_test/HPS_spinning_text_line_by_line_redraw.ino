#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_RA8875.h>

#define RA8875_CS 10
#define RA8875_RESET 9

Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);

const float Hpoints[][3] = { 
  {99,6,0},{281,8,0},{281,246,0},{526,247,0},{528,7,0},{702,5,0},
  {702,599,0},{529,600,0},{527,345,0},{282,341,0},{280,598,0},{98,598,0} 
};

const float Ppoints[][3] = { 
  {101,7,0},{620,9,0},{619,82,0},{699,83,0},{699,265,0},{620,266,0},
  {620,346,0},{284,346,0},{282,599,0},{101,600,0} 
};

const float Pholepoints[][3] = { 
  {282,95,0},{281,250,0},{525,251,0},{528,97,0} 
};

const float Spoints[][3] = { 
  {190,7,0},{618,9,0},{618,82,0},{699,83,0},{699,177,0},{527,176,0},
  {527,99,0},{283,95,0},{282,168,0},{364,170,0},{363,249,0},{530,252,0},
  {530,338,0},{620,341,0},{619,421,0},{698,419,0},{699,513,0},{620,513,0},
  {618,600,0},{189,601,0},{189,515,0},{100,513,0},{99,417,0},{282,419,0},
  {281,508,0},{524,507,0},{528,435,0},{439,430,0},{440,349,0},{270,344,0},
  {271,267,0},{189,262,0},{189,176,0},{101,176,0},{101,82,0},{187,81,0} 
};

uint16_t color565(uint8_t r, uint8_t g, uint8_t b){
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

void drawPolygon3D(const float points[][3], int n, float scale, float angleY, float prevAngleY, int screenCenterX, int screenCenterY, uint16_t color, float zAxis, float globalOffsetX, float globalOffsetY, float focalLength){
  float cosAnew = cos(angleY);
  float sinAnew = sin(angleY);
  float cosAold = cos(prevAngleY);
  float sinAold = sin(prevAngleY);

  for(int i=0;i<n;i++){
    int next = (i+1)%n;

    // previous frame coordinates
    float x1_old = points[i][0]-400 + globalOffsetX;
    float y1_old = points[i][1]-300 + globalOffsetY;
    float z1_old = points[i][2];
    float x2_old = points[next][0]-400 + globalOffsetX;
    float y2_old = points[next][1]-300 + globalOffsetY;
    float z2_old = points[next][2];

    int sx1_old = screenCenterX + (cosAold*x1_old + sinAold*z1_old)*scale*(focalLength/(-sinAold*x1_old + cosAold*z1_old + zAxis));
    int sy1_old = screenCenterY + y1_old*scale*(focalLength/(-sinAold*x1_old + cosAold*z1_old + zAxis));
    int sx2_old = screenCenterX + (cosAold*x2_old + sinAold*z2_old)*scale*(focalLength/(-sinAold*x2_old + cosAold*z2_old + zAxis));
    int sy2_old = screenCenterY + y2_old*scale*(focalLength/(-sinAold*x2_old + cosAold*z2_old + zAxis));

    // erase previous line (slightly thicker to avoid gaps)
    tft.drawLine(sx1_old, sy1_old, sx2_old, sy2_old, 0);
    tft.drawLine(sx1_old, sy1_old-1, sx2_old, sy2_old-1, 0);
    tft.drawLine(sx1_old, sy1_old+1, sx2_old, sy2_old+1, 0);

    // new frame coordinates
    int sx1_new = screenCenterX + (cosAnew*x1_old + sinAnew*z1_old)*scale*(focalLength/(-sinAnew*x1_old + cosAnew*z1_old + zAxis));
    int sy1_new = screenCenterY + y1_old*scale*(focalLength/(-sinAnew*x1_old + cosAnew*z1_old + zAxis));
    int sx2_new = screenCenterX + (cosAnew*x2_old + sinAnew*z2_old)*scale*(focalLength/(-sinAnew*x2_old + cosAnew*z2_old + zAxis));
    int sy2_new = screenCenterY + y2_old*scale*(focalLength/(-sinAnew*x2_old + cosAnew*z2_old + zAxis));

    // draw new line
    tft.drawLine(sx1_new, sy1_new, sx2_new, sy2_new, color);
  }
}


float angle = 0;
float prevAngle = 0;

void setup(){
  Serial.begin(9600);
  if(!tft.begin(RA8875_800x480)) while(1);
  tft.displayOn(true);
  tft.GPIOX(true);
  tft.PWM1config(true, RA8875_PWM_CLK_DIV1024);
  tft.PWM1out(255);
  tft.fillScreen(0);
}

void loop(){
  int screenCenterX = tft.width()/2;
  int screenCenterY = tft.height()/2;

  float scale = 0.1;
  float zAxis = 2000;
  float focalLength = 2000;

  float hOffset = -800;
  float pOffset = 0;
  float sOffset = 800;

  drawPolygon3D(Hpoints, sizeof(Hpoints)/sizeof(Hpoints[0]), scale, angle, prevAngle, screenCenterX, screenCenterY, color565(255,255,255), zAxis, hOffset, 0, focalLength);
  drawPolygon3D(Ppoints, sizeof(Ppoints)/sizeof(Ppoints[0]), scale, angle, prevAngle, screenCenterX, screenCenterY, color565(255,255,255), zAxis, pOffset, 0, focalLength);
  drawPolygon3D(Pholepoints, sizeof(Pholepoints)/sizeof(Pholepoints[0]), scale, angle, prevAngle, screenCenterX, screenCenterY, color565(255,255,255), zAxis, pOffset, 0, focalLength);
  drawPolygon3D(Spoints, sizeof(Spoints)/sizeof(Spoints[0]), scale, angle, prevAngle, screenCenterX, screenCenterY, color565(255,255,255), zAxis, sOffset, 0, focalLength);

  prevAngle = angle;
  angle += 0.1;

}
