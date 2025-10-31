// MyPolygon.h
#ifndef MYPOLYGON_H
#define MYPOLYGON_H

#include "MyPoint.h"
#include <Adafruit_RA8875.h>

class MyPolygon {
public:
    MyPoint* vertices;
    int nVerts;
    MyPoint normal;
    uint16_t color;

    MyPolygon(MyPoint* verts, int nVerts, uint16_t color);
    void computeNormal();
    void draw(Adafruit_RA8875 &tft, float focal_length, float plane_offset);
    void fill(Adafruit_RA8875 &tft, float focal_length, float plane_offset);
    void rotate(char axis, float angle);
};

#endif
