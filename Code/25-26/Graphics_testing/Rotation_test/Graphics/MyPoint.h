#ifndef MYPOINT_H
#define MYPOINT_H

class MyPoint {
public:
    float x, y, z;

    MyPoint() : x(0), y(0), z(0) {}
    MyPoint(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

    void rotate(char axis, float angle);
    void project(float &px, float &py, float focal_length, float plane_offset);
};

#endif
