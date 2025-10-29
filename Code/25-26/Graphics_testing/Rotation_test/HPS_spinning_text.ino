#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_RA8875.h>

#define RA8875_CS 10
#define RA8875_RESET 9

Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);

const float Hpoints[][3] = { 
  {99, 6, 0}, {281, 8, 0}, {281, 246, 0}, {526, 247, 0}, {528, 7, 0}, {702, 5, 0}, 
  {702, 599, 0}, {529, 600, 0}, {527, 345, 0}, {282, 341, 0}, {280, 598, 0}, {98, 598, 0} 
};

const float Ppoints[][3] = { 
  {101, 7, 0}, {620, 9, 0}, {619, 82, 0}, {699, 83, 0}, {699, 265, 0}, {620, 266, 0}, 
  {620, 346, 0}, {284, 346, 0}, {282, 599, 0}, {101, 600, 0} 
};

const float Pholepoints[][3] = { 
  {282, 95, 0}, {281, 250, 0}, {525, 251, 0}, {528, 97, 0} 
};

const float Spoints[][3] = { 
  {190, 7, 0}, {618, 9, 0}, {618, 82, 0}, {699, 83, 0}, {699, 177, 0}, {527, 176, 0}, 
  {527, 99, 0}, {283, 95, 0}, {282, 168, 0}, {364, 170, 0}, {363, 249, 0}, {530, 252, 0}, 
  {530, 338, 0}, {620, 341, 0}, {619, 421, 0}, {698, 419, 0}, {699, 513, 0}, {620, 513, 0}, 
  {618, 600, 0}, {189, 601, 0}, {189, 515, 0}, {100, 513, 0}, {99, 417, 0}, {282, 419, 0}, 
  {281, 508, 0}, {524, 507, 0}, {528, 435, 0}, {439, 430, 0}, {440, 349, 0}, {270, 344, 0}, 
  {271, 267, 0}, {189, 262, 0}, {189, 176, 0}, {101, 176, 0}, {101, 82, 0}, {187, 81, 0} 
};

uint16_t color565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

void drawPolygon3D(const float points[][3], int n, float scale, float angleY, int screenCenterX, int screenCenterY, uint16_t color, float zAxis, float globalOffsetX, float globalOffsetY, float focalLength) {
    float cosA = cos(angleY);
    float sinA = sin(angleY);

    for(int i=0;i<n;i++){
        int next=(i+1)%n;

        float x1 = points[i][0] - 400 + globalOffsetX;
        float y1 = points[i][1] - 300 + globalOffsetY;
        float z1 = points[i][2];

        float x2 = points[next][0] - 400 + globalOffsetX;
        float y2 = points[next][1] - 300 + globalOffsetY;
        float z2 = points[next][2];

        float rx1 = cosA*x1 + sinA*z1;
        float rz1 = -sinA*x1 + cosA*z1 + zAxis;
        float rx2 = cosA*x2 + sinA*z2;
        float rz2 = -sinA*x2 + cosA*z2 + zAxis;

        int sx1 = screenCenterX + rx1*scale*(focalLength / rz1);
        int sy1 = screenCenterY + y1*scale*(focalLength / rz1);
        int sx2 = screenCenterX + rx2*scale*(focalLength / rz2);
        int sy2 = screenCenterY + y2*scale*(focalLength / rz2);

        tft.drawLine(sx1, sy1, sx2, sy2, color);
    }
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

    // erase previous frame
    drawPolygon3D(Hpoints, sizeof(Hpoints)/sizeof(Hpoints[0]), scale, angle, screenCenterX, screenCenterY, black, zAxis, hOffset, 0, focalLength);
    drawPolygon3D(Ppoints, sizeof(Ppoints)/sizeof(Ppoints[0]), scale, angle, screenCenterX, screenCenterY, black, zAxis, pOffset, 0, focalLength);
    drawPolygon3D(Pholepoints, sizeof(Pholepoints)/sizeof(Pholepoints[0]), scale, angle, screenCenterX, screenCenterY, black, zAxis, pOffset, 0, focalLength);
    drawPolygon3D(Spoints, sizeof(Spoints)/sizeof(Spoints[0]), scale, angle, screenCenterX, screenCenterY, black, zAxis, sOffset, 0, focalLength);

    // advance angle
    angle+=0.05;

    // draw new frame
    drawPolygon3D(Hpoints, sizeof(Hpoints)/sizeof(Hpoints[0]), scale, angle, screenCenterX, screenCenterY, white, zAxis, hOffset, 0, focalLength);
    drawPolygon3D(Ppoints, sizeof(Ppoints)/sizeof(Ppoints[0]), scale, angle, screenCenterX, screenCenterY, white, zAxis, pOffset, 0, focalLength);
    drawPolygon3D(Pholepoints, sizeof(Pholepoints)/sizeof(Pholepoints[0]), scale, angle, screenCenterX, screenCenterY, white, zAxis, pOffset, 0, focalLength);
    drawPolygon3D(Spoints, sizeof(Spoints)/sizeof(Spoints[0]), scale, angle, screenCenterX, screenCenterY, white, zAxis, sOffset, 0, focalLength);

}
