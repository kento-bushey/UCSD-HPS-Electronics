# LCD Information


## Circuit
The The RA8875 driver board is connected to the arduino via:
- RST --> D9
- CS --> D10
- MOSI --> D11
- MISO --> D12
- SCK --> D13
- GND --> GND
- VIN --> 5v

## Code & Libraries
**Adafruit GFX**
- General graphics library for LCD boards. 

**Adafruit RA8875**
- Library specifc to the driver boards we are using. We use a modified version found in `./libraries/` that contains an added function for drawing Bezier curves.

**Adafruit BusIO**
- IO communication library used for communicating with the driver board.   

## Documentation
Documentation for the RA8875 library can be found *here*

