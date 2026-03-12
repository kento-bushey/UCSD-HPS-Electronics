#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_RA8875.h>

///All widgets should inherit this class, and need to override the Draw(val, c1, c2, c3) method.
  //Instead of making a custom constructor for each, 
  //create a method Init_myWdiget(args)that sets specific subclass variables & initilizes
#ifndef WIDGET_H
#define WIDGET_H
class Widget{
    public:
          //reference as pos.x and pos.y (top left corner in pixels)
        struct Point pos; 
        uint16_t w;
        uint16_t h;
        int previousValue;
        Adafruit_RA8875* screen;

          //Constructors can optionally set position (default 0,0) and width/height (default 100,100)
          //Must always pass in screen, (in main file, this is named tft)
        Widget(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Adafruit_RA8875& screen): screen(&screen), pos({x,y}), w(w), h(h){};
        Widget(Adafruit_RA8875& screen): screen(&screen), pos({0,0}), w(100), h(100){};
        Widget(uint16_t x, uint16_t y, Adafruit_RA8875& screen): screen(&screen), pos({x,y}), w(100), h(100){};

          //OVERRIDE THIS ONE:
          //Main draw function, only draw with color1, color2, and/or color3 so that UnDraw(bgColor) works
          //Always set previousValue=val at the end of the Draw(), again so that UnDraw() works. 
        virtual void Draw(int val, uint16_t color1, uint16_t color2, uint16_t color3) = 0;

          //singe and double color methods. Also used for UnDraw(bgColor)
          //These don't need to be overridden, 
          //If using less than 3 colors, simply ignore the last color args in the main Draw()method
          //so that either of these methods will work automatically.
        void Draw(int val, uint16_t color){Draw(val,color,color,color);}
        void Draw(int val, uint16_t color1, uint16_t color2){Draw(val,color1,color2,color2);}

          //Draws back over the widget in black (or given backgound color)
        void UnDraw(uint16_t bgColor){Draw(previousValue, bgColor);}
        void UnDraw(){UnDraw(0);}
        
        
};

#endif
