# LCD Information

![pilot display](https://github.com/kento-bushey/UCSD-HPS-Electronics/blob/main/Notes/SubLCDLayout.png)

## Circuit
The The RA8875 driver board is connected to the arduino via:
- RST --> D9
- CS --> D10
- MOSI --> D11
- MISO --> D12
- SCK --> D13
- GND --> GND
- VIN --> 5v

## Libraries
[**Adafruit GFX**](https://github.com/kento-bushey/UCSD-HPS-Electronics/tree/main/libraries/Adafruit_GFX_Library)
- General graphics library for LCD boards. 

[**Adafruit RA8875**](https://github.com/kento-bushey/UCSD-HPS-Electronics/tree/main/libraries/Adafruit_RA8875)
- Library specifc to the driver boards we are using. We are currently using a modified version found in `./libraries/` that contains an added function for drawing Bezier curves.

[**Adafruit BusIO**](https://github.com/adafruit/Adafruit_BusIO)
- IO communication library used for communicating with the driver board.   

## Documentation
Documentation for the RA8875 library can be found [here](https://cdn-learn.adafruit.com/downloads/pdf/adafruit-gfx-graphics-library.pdf)

In addition, the added method `drawBezier(struct Point start, struct Point, end, struct Point control, tuint_16 color)` draws a bezier curve between two points with the control point controlling the curvture. SVG files can be broken down into bezier curves and line segments. 








