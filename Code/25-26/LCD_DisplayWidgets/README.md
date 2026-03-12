## LCD Display - Code Structure & Adding New Widgets:
For general info about the LCD Screen, see [the wiki](https://github.com/kento-bushey/UCSD-HPS-Electronics/wiki/LCD-Documentation)

![subLayout](https://raw.githubusercontent.com/kento-bushey/UCSD-HPS-Electronics/refs/heads/main/Notes/SubLCDLayout.png)

Each of these sensor displays is a `Widget`.

`Widget.h` lays out a framework for drawing and "undrawing" widgets.<br> Any new widget will be a new subclass of `Widget`.<br>
Eg, `class SensorDisplay : public Widget {  /*Specific behavior in here*/ };`

The `Widget` class already implements:
- `void UnDraw()`,
- `void UnDraw(backgroundColor)`,
- `void Draw(sensorValue, color1)`,
- and `void Draw(sensorValue, color1, color2)`

by referencing the virtural method `void Draw(sensorValue, color1, color2, color3)`.
<br><br>This method is the only one that needs to be overridden when creating a new widget. 
<br>It should draw the widget on the screen showing the sensor reading for a given value, using up to 3 color options.
<br>It's important that only colors passed into the method get drawn, otherwise erasing with `UnDraw` will not work.

The `Widget` class gets initialized with the following member variables that should get used in each specific implementation of `Draw()`:
- `RA8868* screen` (this is usually just `tft` in the main program, and should always be passed into the constructor)
- `struct Point pos` ( {x,y} pair referenced via `pos.x` and `pos.y`)
- `int w,h` (width and height respectively. Default value is 100px)
- `int previousValue` (the previous drawn value. Always set this to `val` at the end of `Draw(val)`, so that `UnDraw()` can erase properly.)

eg, you might call `screen->drawRect(pos.x, pos.y, pos.x + w, pos.y + h, color1)` to draw a box around the widget. 


### Calling widgets:
The main .ino program contains an array of `Widget*` pointers. 
Once a new widget is implemented, it can be initialized then added to the array. Each time a relevant sensor reading is taken, the widget will draw it and undraw the previous value. 
