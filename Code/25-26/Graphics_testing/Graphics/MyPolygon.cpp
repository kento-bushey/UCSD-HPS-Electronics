// MyPolygon.cpp
#include "MyPolygon.h"

MyPolygon::MyPolygon(MyPoint* verts, int nVerts, uint16_t color) {
    this->vertices = verts;
    this->nVerts = nVerts;
    this->color = color;
    computeNormal();
}

void MyPolygon::computeNormal() {
    float ux = vertices[1].x - vertices[0].x;
    float uy = vertices[1].y - vertices[0].y;
    float uz = vertices[1].z - vertices[0].z;
    float vx = vertices[2].x - vertices[0].x;
    float vy = vertices[2].y - vertices[0].y;
    float vz = vertices[2].z - vertices[0].z;
    normal.x = uy * vz - uz * vy;
    normal.y = uz * vx - ux * vz;
    normal.z = ux * vy - uy * vx;
}

void MyPolygon::draw(Adafruit_RA8875 &tft, float focal_length, float plane_offset) {
    float sx[10], sy[10];
    for (int i = 0; i < nVerts; i++) {
        vertices[i].project(sx[i], sy[i], focal_length, plane_offset);
    }
    for (int i = 0; i < nVerts; i++) {
        int j = (i + 1) % nVerts;
        tft.drawLine((int)sx[i], (int)sy[i], (int)sx[j], (int)sy[j], RA8875_WHITE);
    }
}

void MyPolygon::fill(Adafruit_RA8875 &tft, float focal_length, float plane_offset) {
    float sx[10], sy[10];
    for (int i = 0; i < nVerts; i++) {
        vertices[i].project(sx[i], sy[i], focal_length, plane_offset);
    }
    for (int i = 1; i < nVerts - 1; i++) {
        tft.fillTriangle((int)sx[0], (int)sy[0],
                         (int)sx[i], (int)sy[i],
                         (int)sx[i+1], (int)sy[i+1],
                         color);
    }
}

void MyPolygon::rotate(char axis, float angle) {
    for (int i = 0; i < nVerts; i++) {
        vertices[i].rotate(axis, angle);
    }
    computeNormal();
}
