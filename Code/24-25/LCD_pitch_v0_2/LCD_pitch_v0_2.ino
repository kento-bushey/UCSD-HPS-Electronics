#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
Adafruit_BNO055 bno = Adafruit_BNO055(55);// green goes to 21, yellow goes to 20
// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x03C8
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define BROWN   0x9A62

Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

uint8_t rotation = 1;
// 0 -> bottom is where it's plugged in from
// 1 -> bottom is DIGITAL(PWM), 
int frame_count = 0;

int frame_Rate = 25;

// Dead man stuff below
bool on;
int stage;
unsigned long lastPressTime;
unsigned long lastBlinkTime;

int elMag = 49;
int blinkBulb = 51;
int btn = 53;
int delayTime = 6000; // delay until release
int blinkDelay = 500; // 10 blinks

bool blackout = false;

int resetBtn = btn;

bool buttonPressed = false; 


void reset_dead_man_system() {
  stage = 1;
  lastPressTime = millis(); // Initialize with current time
  lastBlinkTime = millis(); // Initialize with current time
  buttonPressed = false;
  digitalWrite(elMag, HIGH);
  digitalWrite(blinkBulb, LOW);
  printStage();
  tft.fillScreen(BLACK);
  blackout = false;
}

void dead_man_setup(){
  pinMode(elMag, OUTPUT);
  pinMode(blinkBulb, OUTPUT);
  pinMode(btn, INPUT_PULLUP);
  stage = 1;
  on = true;
  digitalWrite(elMag, HIGH);
  digitalWrite(blinkBulb, LOW);
}

void printStage() {
  Serial.print("Stage ");
  Serial.println(stage);
}

int alert_triangle1_points[][2] = {
  {60,80}, {20,160}, {100,160}
};

double sub_points[][2] = {
    {2.923108, -55.945691},
    {6.400080, -52.686700},
    {9.767072, -47.640878},
    {12.024809, -41.928336},
    {13.607672, -35.989905},
    {14.766838, -29.953000},
    {15.571795, -23.860302},
    {15.923632, -17.723367},
    {16.007022, -11.576276},
    {15.997172, -5.428399},
    {15.984528, 0.719420},
    {15.780539, 6.863253},
    {15.145444, 12.976158},
    {13.950358, 19.005423},
    {12.389277, 24.950971},
    {10.636571, 30.841372},
    {12.550802, 35.924202},
    {17.228589, 39.225261},
    {21.445643, 43.380618},
    {18.710779, 47.877776},
    {14.247843, 44.366371},
    {9.602669, 41.028526},
    {6.777449, 44.394608},
    {5.092944, 50.307183},
    {2.741441, 55.930592},
    {-2.812767, 55.945691},
    {-5.213475, 50.341790},
    {-6.952666, 44.445390},
    {-9.553063, 41.156411},
    {-14.170865, 44.570686},
    {-18.656020, 47.895458},
    {-21.445643, 43.431413},
    {-17.335919, 39.190205},
    {-12.681972, 35.962955},
    {-10.672790, 30.991855},
    {-12.455726, 25.109669},
    {-13.923105, 19.140871},
    {-14.968157, 13.083664},
    {-15.628913, 6.972265},
    {-15.960575, 0.833950},
    {-16.030750, -5.313215},
    {-16.034878, -11.461092},
    {-16.010774, -17.608921},
    {-15.749825, -23.749377},
    {-14.852288, -29.830039},
    {-13.592711, -35.844705},
    {-11.805974, -41.725796},
    {-9.651475, -47.481886},
    {-6.528590, -52.754667},
    {-3.086911, -55.847023},
};
int num_sub_points = 50;

double pitch_points[][2] = {
  {-20,0},
  {-6,0},
  {0,-6},
  {6,0},
  {20,0}
};
int num_pitch_points = 5;
double pitch_offset = 1.25;
double pitch_window = 90;

void bno_setup(){
  Serial.println("Orientation Sensor Test"); Serial.println("");

  /* Initialise the sensor */
  if (!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }
  bno.setExtCrystalUse(true);
  dead_man_setup();
}

void LCD_setup(){
  Serial.println(F("TFT LCD test"));

  #ifdef USE_Elegoo_SHIELD_PINOUT
    Serial.println(F("Using Elegoo 2.8\" TFT Arduino Shield Pinout"));
  #else
    Serial.println(F("Using Elegoo 2.8\" TFT Breakout Board Pinout"));
  #endif

  Serial.print("TFT size is "); Serial.print(tft.width()); Serial.print("x"); Serial.println(tft.height());

  tft.reset();

  uint16_t identifier = tft.readID();
   if(identifier == 0x9325) {
    Serial.println(F("Found ILI9325 LCD driver"));
  } else if(identifier == 0x9328) {
    Serial.println(F("Found ILI9328 LCD driver"));
  } else if(identifier == 0x4535) {
    Serial.println(F("Found LGDP4535 LCD driver"));
  }else if(identifier == 0x7575) {
    Serial.println(F("Found HX8347G LCD driver"));
  } else if(identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  } else if(identifier == 0x8357) {
    Serial.println(F("Found HX8357D LCD driver"));
  } else if(identifier==0x0101)
  {     
      identifier=0x9341;
       Serial.println(F("Found 0x9341 LCD driver"));
  }else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    Serial.println(F("If using the Elegoo 2.8\" TFT Arduino shield, the line:"));
    Serial.println(F("  #define USE_Elegoo_SHIELD_PINOUT"));
    Serial.println(F("should appear in the library header (Elegoo_TFT.h)."));
    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
    Serial.println(F("Also if using the breakout, double-check that all wiring"));
    Serial.println(F("matches the tutorial."));
    identifier=0x9341;
  
  }

  tft.begin(identifier);

  Serial.println(F("Done!"));
}

void setup(void) {
  Serial.begin(9600);
  bno_setup();
  LCD_setup();
}

double** get_ellipse(double center[2], int width, int height, int num_points) {
    double** ellipse_points = new double*[num_points];
    for (int i = 0; i < num_points; i++) {
        ellipse_points[i] = new double[2];
    }
    
    double a = width / 2.0;  // Semi-major axis
    double b = height / 2.0; // Semi-minor axis

    // Generate points on the ellipse
    for (int i = 0; i < num_points; ++i) {
        double t = (2 * M_PI / num_points) * i; 
        ellipse_points[i][0] = a * cos(t) + center[0]; // x-coordinate
        ellipse_points[i][1] = b * sin(t) + center[1]; // y-coordinate
    }
    
    return ellipse_points;
}

class sub {
public:
    int num_points;
    double** points;
    double** rotated_points;
    double pos[2];
    double radius;

    sub(double start_pos[2], double r) : radius(r) {
        num_points = num_sub_points;

        points = new double*[num_points];
        rotated_points = new double*[num_points];
        for (int i = 0; i < num_points; i++) {
            points[i] = new double[2];
            rotated_points[i] = new double[2];
        }

        pos[0] = start_pos[0];
        pos[1] = start_pos[1]; 

        // Compute scaling factor based on the furthest point distance
        double max_dist = 0;
        for (int i = 0; i < num_points; i++) {
            double x = sub_points[i][0];
            double y = sub_points[i][1];
            double dist = sqrt(x * x + y * y);
            if (dist > max_dist) {
                max_dist = dist;
            }
        }

        double scale = radius / max_dist;

        // Scale and translate the points
        for (int i = 0; i < num_points; i++) {
            points[i][0] = sub_points[i][0] * scale + pos[0];
            points[i][1] = sub_points[i][1] * scale + pos[1];
        }
    }
};


void draw_polygon(double** points, int num_points, uint16_t color);
double** rotated_polygon(double** points, int num_points, double angle, double center[2]);

class yaw_display{
  public:
  sub sub1;
  uint16_t sub_color = YELLOW;
  double prevangle = 0;
  double angle=0;
  int numpoints = 80;
  double** circle;
  uint16_t circle_color = GREEN;
  double radius;
  yaw_display(double start_pos[2],double radius): sub1(start_pos,radius){
    this->radius = radius;
    sub temp(start_pos,radius);
    sub1=temp;
    circle = get_ellipse(start_pos,radius*2*1.1,radius*2*1.1,numpoints);
  }
  void draw_line(double ang,uint16_t color){
    int lineLength = radius*1.9;
    double cos_angle = cos(ang);
    double sin_angle = sin(ang);
    
    double x1 = sub1.pos[0];
    double y1 = sub1.pos[1] - lineLength / 2.0;

    double x2 = sub1.pos[0];
    double y2 = sub1.pos[1] + lineLength / 2.0;

    double x1_rot = (x1 - sub1.pos[0]) * cos_angle - (y1 - sub1.pos[1]) * sin_angle + sub1.pos[0];
    double y1_rot = (x1 - sub1.pos[0]) * sin_angle + (y1 - sub1.pos[1]) * cos_angle + sub1.pos[1];

    double x2_rot = (x2 - sub1.pos[0]) * cos_angle - (y2 - sub1.pos[1]) * sin_angle + sub1.pos[0];
    double y2_rot = (x2 - sub1.pos[0]) * sin_angle + (y2 - sub1.pos[1]) * cos_angle + sub1.pos[1];

    tft.drawLine(x1_rot, y1_rot, x2_rot, y2_rot, color);
  }
  void draw_sub() {
    if (frame_count == 1) {
        draw_polygon(circle, numpoints, circle_color);
    }
    
    double** new_points = rotated_polygon(sub1.points, sub1.num_points, angle, sub1.pos);
    for (int i = 0; i < sub1.num_points; i++) {
        int x1_old = sub1.rotated_points[i][0];
        int y1_old = sub1.rotated_points[i][1];
        
        int x2_old = sub1.rotated_points[(i + 1) % sub1.num_points][0];
        int y2_old = sub1.rotated_points[(i + 1) % sub1.num_points][1];
        
        tft.drawLine(x1_old, y1_old, x2_old, y2_old, BLACK);
        
        int x1_new = new_points[i][0];
        int y1_new = new_points[i][1];
        
        int x2_new = new_points[(i + 1) % sub1.num_points][0];
        int y2_new = new_points[(i + 1) % sub1.num_points][1];
        
        tft.drawLine(x1_new, y1_new, x2_new, y2_new, sub_color);
    }
    
    draw_line(prevangle,BLACK);
    draw_line(angle,sub_color);

    tft.drawLine(sub1.pos[0],sub1.pos[1] + radius,sub1.pos[0],sub1.pos[1] - radius,circle_color);

    for (int i = 0; i < sub1.num_points; ++i) {
        delete[] sub1.rotated_points[i];
    }
    delete[] sub1.rotated_points;
    sub1.rotated_points = new_points;
  }
  void draw_text(){
    tft.setTextSize(1);
    tft.setCursor(sub1.pos[0]-10,sub1.pos[1]-radius - 10);
    tft.setTextColor(BLACK);
    double prev_print_angle = 180*prevangle/M_PI;
    if (prev_print_angle > 180) {
        prev_print_angle -= 360;
    }
    tft.print(prev_print_angle);
    tft.setCursor(sub1.pos[0]-10,sub1.pos[1]-radius - 10);
    tft.setTextColor(WHITE);
    double print_angle = 180*angle/M_PI;
    if (print_angle > 180) {
        print_angle -= 360;
    }
    tft.print(print_angle);
  }
  void draw(){
    draw_sub();
    draw_text();
  }
};

class rectangle {
public:
    double left, right, top, bottom; 
    double** corners; 
    double** old_corners;

    rectangle(double left, double right, double top, double bottom)
        : left(left), right(right), top(top), bottom(bottom) {
        // Allocate memory for corners and old_corners
        corners = new double*[4];
        old_corners = new double*[4];
        for (int i = 0; i < 4; ++i) {
            corners[i] = new double[2];
            old_corners[i] = new double[2];
        }

        update_corners();
    }

    // Update the corners of the rectangle
    void update_corners() {
        // Copy current corners to old_corners
        for (int i = 0; i < 4; ++i) {
            old_corners[i][0] = corners[i][0];
            old_corners[i][1] = corners[i][1];
        }

        // Define the new corners clockwise
        corners[0][0] = left;   corners[0][1] = top;    // Top-left
        corners[1][0] = right;  corners[1][1] = top;    // Top-right
        corners[2][0] = right;  corners[2][1] = bottom; // Bottom-right
        corners[3][0] = left;   corners[3][1] = bottom; // Bottom-left
    }

    // Function to draw a single line segment between two points
    void draw_line(double* start, double* end, uint16_t color) {
        tft.drawLine(start[0], start[1], end[0], end[1], color);
    }

    // Draw the rectangle, erasing old and drawing new
    void draw(uint16_t color) {
      for (int i = 0; i < 4; ++i) {
          
          draw_line(old_corners[i], old_corners[(i + 1) % 4], BLACK);
          draw_line(corners[i], corners[(i + 1) % 4], color);
      }
    }

};


class pitch_display{
  public:
  double radius;
  double angle=0;
  double prevangle;
  rectangle border;
  rectangle sky;
  rectangle earth;
  double start_pos[2];
  double height_ratio = 1.5;
  pitch_display(double start_pos[2], double radius): border(0, 0, 0, 0), sky(0, 0, 0, 0), earth(0, 0, 0, 0){
    this->radius = radius;
    this->start_pos[0] = start_pos[0];
    this->start_pos[1] = start_pos[1];

    double width = 2 * radius;
    double height = 2*height_ratio*radius;

    // Define border rectangle (centered at start_pos)
    double left = start_pos[0] - width / 2.0;
    double right = start_pos[0] + width / 2.0;
    double top = start_pos[1] - height / 2.0;
    double bottom = start_pos[1] + height / 2.0;

    border = rectangle(left-1, right+1, top-1, bottom+1);
    earth = rectangle(left, right, (top + bottom) / 2.0 + 2, bottom);
    sky = rectangle(left, right, top, (top + bottom) / 2.0-2);
  }
  void draw_crosshair() {
        // Scale the pitch points to fit the width of the pitch display
        double width = 2 * radius;
        double scaling_factor = width / 40.0;  // Scale based on the width of the pitch display

        for (int i = 0; i < num_pitch_points-1; i++) {
            // Scale each point of the crosshair
            double x = pitch_points[i][0] * scaling_factor + start_pos[0];
            double y = pitch_points[i][1] * scaling_factor + start_pos[1];

            tft.drawLine(x, y, pitch_points[(i + 1) % num_pitch_points][0] * scaling_factor + start_pos[0], pitch_points[(i + 1) % num_pitch_points][1] * scaling_factor + start_pos[1], YELLOW);
            
        }
    }
  double angle_to_y(double angle){
    double raw = start_pos[1]+((180*angle/M_PI)/pitch_window)*(height_ratio*radius);
    
    return fmin(fmax(raw, start_pos[1]-height_ratio*radius),start_pos[1]+height_ratio*radius);
  }
  void draw() {
        if (frame_count == 1) {
            border.draw(GREEN);
        }
        draw_crosshair();
        earth.top = angle_to_y(angle)+1;
        earth.update_corners();
        sky.bottom = angle_to_y(angle)-1;
        sky.update_corners();

       
        sky.draw(CYAN); 

        earth.draw(BROWN); 
        draw_text();
  }
  void draw_text(){
    tft.setTextSize(1);
    tft.setCursor(start_pos[0]-radius-40,start_pos[1]-5);
    tft.setTextColor(BLACK);
    double prev_print_angle = 180*prevangle/M_PI;
    if (prev_print_angle > 180) {
        prev_print_angle -= 360;
    }
    tft.print(prev_print_angle);
    tft.setCursor(start_pos[0]-radius-40,start_pos[1]-5);
    tft.setTextColor(WHITE);
    double print_angle = 180*angle/M_PI;
    if (print_angle > 180) {
        print_angle -= 360;
    }
    tft.print(print_angle);
  }
};

void loop_prep(){
  tft.setRotation(rotation);
  frame_count++;
  if (frame_count == 1){
    tft.fillScreen(BLACK);
  }
}

void draw_polygon(double** points, int num_points, uint16_t color) {
    for (int i = 0; i < num_points; i++) {
        // Current point coordinates
        int x1 = points[i][0];
        int y1 = points[i][1];
        
        // Next point coordinates (wraps around to the first point)
        int x2 = points[(i + 1) % num_points][0];
        int y2 = points[(i + 1) % num_points][1];
        
        // Draw line between current point and next point
        tft.drawLine(x1, y1, x2, y2, color);
    }
}

double** rotated_polygon(double** points, int num_points, double angle, double center[2]) {
    // Allocate memory for the rotated points array
    double** rotated_points = new double*[num_points];
    for (int i = 0; i < num_points; ++i) {
        rotated_points[i] = new double[2];
    }

    // Calculate rotation matrix components
    double cos_angle = cos(angle);
    double sin_angle = sin(angle);

    // Rotate each point around the center
    for (int i = 0; i < num_points; ++i) {
        // Translate point to origin (relative to center)
        double x = points[i][0] - center[0];
        double y = points[i][1] - center[1];

        // Apply rotation
        rotated_points[i][0] = x * cos_angle - y * sin_angle + center[0];
        rotated_points[i][1] = x * sin_angle + y * cos_angle + center[1];
    }

    return rotated_points;
}

class warning_display {
  private:
  int lastx = 0;
  int lasty = 0;
  String last_m = "";

  int last_count = "";

  int last_countx = 0;
  int last_county = 0;

  public:
  int countdown = 0;
  bool showing = false;
  bool showing_count = false;

  int press_dmb_button_message_lastx = 0;
  int press_dmb_button_message_lasty = 0;

  String warning_status;

  void draw_triangle() {
    // Fill the triangle with red color
    tft.fillTriangle(alert_triangle1_points[0][0], alert_triangle1_points[0][1],
                     alert_triangle1_points[1][0], alert_triangle1_points[1][1],
                     alert_triangle1_points[2][0], alert_triangle1_points[2][1], RED);
  }

  void clear_triangle() {
      // Fill the triangle with black color (clear color)
      tft.fillTriangle(alert_triangle1_points[0][0], alert_triangle1_points[0][1],
                      alert_triangle1_points[1][0], alert_triangle1_points[1][1],
                      alert_triangle1_points[2][0], alert_triangle1_points[2][1], BLACK);
  }


  void clear_count(){
    tft.setCursor(last_countx,last_county);
    tft.setTextColor(BLACK);
    tft.print(last_count);
    showing_count = false;
  }

  void clear(){
    tft.setCursor(lastx,lasty);
    tft.setTextColor(BLACK);
    tft.print(last_m);
    showing = false;
    clear_count();
  }

  void update() {
    if (warning_status == "none") {

      return;
    }

    if (warning_status == "button_release") {
      int x = 10;
      int y=10;
      tft.setCursor(x,y);
      tft.setTextColor(RED);
      int countdown_seconds = countdown / 1000; // Convert ms to seconds
      String message = "Button Released! Releasing buoy in: ";
      tft.print(message);
      lastx = x;
      lasty = y;
      last_m = message;
      showing = true;
      if (showing_count){
        clear_triangle();
        showing_count = !showing_count;
      }else{
        draw_triangle();
        showing_count = !showing_count;
      }
      
    }

    if (warning_status == "buoy_released"){
      int x = 10;
        int y=10;
        tft.setCursor(x,y);
        tft.setTextColor(RED);
        String message = "Buoy released.";
        tft.print(message);
        lastx = x;
        lasty = y;
        last_m = message;
        showing = true;
    }
  }
};

warning_display wd;

double yaw_pos[2] = {80, 120};
double radius = 30;
yaw_display yaw(yaw_pos,radius);

double pitch_pos[2] = {200,120};
double pitch_display_width = 30;
pitch_display pitch(pitch_pos,pitch_display_width);

void text_handler() {
  if (Serial.available()) {
    String input = Serial.readString();
    input.trim();

    if (input.startsWith("trigger")) {
      int spaceIndex = input.indexOf(' ', 7);
      
      if (spaceIndex != -1) {
        String nextWord = input.substring(spaceIndex + 1);
        if (nextWord == "none") {
          wd.warning_status = nextWord;
        } else if (nextWord == "button_release") {
          Serial.println("button_release warning test");
          wd.warning_status = nextWord;
        } else {
          Serial.println(nextWord + " Invalid. Proper usage: trigger {warning}");
        }
      } else {
        Serial.println("Proper usage: trigger {warning}"); 
      }
    }
  }
}

void caseBtn() {

  switch (stage) {
    case 1:
      blackout = false;
      digitalWrite(elMag, HIGH);
      if (digitalRead(btn) == LOW) {
        stage = 2;
        lastPressTime = millis();
        printStage();
      }
      break;

    case 2:
    	digitalWrite(blinkBulb, LOW);
      if (digitalRead(btn) == HIGH) {
        stage = 3;
        lastBlinkTime = millis();
        lastPressTime = millis();
        printStage();
        wd.clear();
        wd.warning_status = "button_release";
        tft.fillScreen(BLACK);
        blackout = true;
      } else {
        lastPressTime = millis();
      }
      break;

    case 3:
      if (millis() - lastPressTime >= delayTime) {
        digitalWrite(elMag, LOW);
        stage = 4;
        printStage();
        wd.clear();
        wd.warning_status = "buoy_released";
        wd.update();
      } else if (digitalRead(btn) == LOW) {
        wd.clear();
        wd.warning_status = "none";
        blackout = false;
        stage = 2;
        printStage();
      }

      if (millis() - lastBlinkTime >= blinkDelay) {
        wd.countdown =  -millis() + lastPressTime + delayTime+1000;
        wd.update();
        digitalWrite(blinkBulb, !digitalRead(blinkBulb));
        lastBlinkTime = millis();
      }
      break;

    case 4:
      blackout = true;
      digitalWrite(blinkBulb, HIGH);
      if (digitalRead(resetBtn) == LOW && !buttonPressed) {
        buttonPressed = true;
      }

      if (digitalRead(resetBtn) == HIGH && buttonPressed) { 
        buttonPressed = false;  // Button is released, trigger reset
        reset_dead_man_system();
        wd.clear();
        wd.warning_status = "none";
        
        return; 
      }
      break;
  }
}

void bno_loop(){
  sensors_event_t event;
  bno.getEvent(&event);
  yaw.prevangle = yaw.angle;
  yaw.angle=M_PI*(event.orientation.x)/180;
  pitch.prevangle = pitch.angle;
  pitch.angle=-(M_PI*(event.orientation.y)/180 + pitch_offset*M_PI/180);
}

void loop(void) {
    loop_prep();
    bno_loop();
    if (!blackout){
      yaw.draw();
      pitch.draw();
    }
    text_handler();
    caseBtn();
    delay(frame_Rate);
    if (frame_count%50==0){
      Serial.println(blackout);
    }
}
