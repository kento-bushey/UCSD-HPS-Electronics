#include <MyPoint.h>
#include <MyPolygon.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_RA8875.h>

#define RA8875_CS 10
#define RA8875_RESET 9

Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);

float focal_length = 1000;
float plane_offset = -35;
float speed = 0.02;

uint16_t color565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

class Cube {
public:
    MyPolygon* faces[6];
    uint16_t color;

    Cube(float cx, float cy, float cz, float size, uint16_t c) {
        color = c;
        float hs = size / 2;
        MyPoint tfl(cx - hs, cy + hs, cz - hs);
        MyPoint tfr(cx + hs, cy + hs, cz - hs);
        MyPoint tbr(cx + hs, cy + hs, cz + hs);
        MyPoint tbl(cx - hs, cy + hs, cz + hs);
        MyPoint bfl(cx - hs, cy - hs, cz - hs);
        MyPoint bfr(cx + hs, cy - hs, cz - hs);
        MyPoint bbr(cx + hs, cy - hs, cz + hs);
        MyPoint bbl(cx - hs, cy - hs, cz + hs);

        faces[0] = new MyPolygon(new MyPoint[4]{tfl, tfr, tbr, tbl}, 4, c);
        faces[1] = new MyPolygon(new MyPoint[4]{bfl, bfr, bbr, bbl}, 4, c);
        faces[2] = new MyPolygon(new MyPoint[4]{tfl, tfr, bfr, bfl}, 4, c);
        faces[3] = new MyPolygon(new MyPoint[4]{tbl, tbr, bbr, bbl}, 4, c);
        faces[4] = new MyPolygon(new MyPoint[4]{tfl, tbl, bbl, bfl}, 4, c);
        faces[5] = new MyPolygon(new MyPoint[4]{tfr, tbr, bbr, bfr}, 4, c);
    }

    void rotate(char axis, float angle) {
        for (int i = 0; i < 6; i++)
            faces[i]->rotate(axis, angle);
    }

    void erase(Adafruit_RA8875 &tft) {
        for (int i = 0; i < 6; i++) {
            uint16_t prevColor = faces[i]->color;
            faces[i]->color = 0; // temporarily black
            faces[i]->fill(tft, focal_length, plane_offset);
            faces[i]->color = prevColor;
        }
    }

    void draw(Adafruit_RA8875 &tft) {
        for (int i = 0; i < 6; i++)
            faces[i]->draw(tft, focal_length, plane_offset);
    }
    void fill(Adafruit_RA8875 &tft) {
        for (int i = 0; i < 6; i++)
            faces[i]->fill(tft, focal_length, plane_offset);
    }
};


Cube* cube;

void setup() {
    Serial.begin(9600);
    if (!tft.begin(RA8875_800x480)) while (1);
    tft.displayOn(true);
    tft.GPIOX(true);
    tft.PWM1config(true, RA8875_PWM_CLK_DIV1024);
    tft.PWM1out(255);
    tft.fillScreen(0);

    cube = new Cube(0, 0, 0, 100, color565(255, 0, 0));
}

void loop() {
    cube->erase(tft);
    cube->rotate('y', speed);
    cube->fill(tft);
    cube->draw(tft);
    delay(20);
}
