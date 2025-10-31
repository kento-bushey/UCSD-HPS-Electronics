#include "MyPoint.h"
#include <Arduino.h>

void MyPoint::rotate(char axis, float angle) {
    float tmp;
    if (axis == 'x') {
        tmp = y;
        y = y * cos(angle) - z * sin(angle);
        z = tmp * sin(angle) + z * cos(angle);
    } else if (axis == 'y') {
        tmp = x;
        x = x * cos(angle) + z * sin(angle);
        z = -tmp * sin(angle) + z * cos(angle);
    } else if (axis == 'z') {
        tmp = x;
        x = x * cos(angle) - y * sin(angle);
        y = tmp * sin(angle) + y * cos(angle);
    }
}

void MyPoint::project(float &px, float &py, float focal_length, float plane_offset) {
    px = 400 + (x * focal_length) / (z + plane_offset + focal_length);
    py = 240 + (y * focal_length) / (z + plane_offset + focal_length);
}
