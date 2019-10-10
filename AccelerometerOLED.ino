#include <Arduino.h>
#include <U8x8lib.h>                  // SSD1306 Library
#include <SparkFun_ADXL345.h>         // SparkFun ADXL345 Library

#define DELAYMS 500                   // Delay between readings in milliseconds
#define DISPLAYCONTRAST 127           // Display contrast (0 - 255)
#define FLIP 1                        // Set to 1 to rotate display 180

//
// Object constructors
//

// Constructor for generic 128x64 module with hardware I2C
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);
// Use this constructor for "every 2nd line skipped" problem
//U8X8_SSD1306_128X64_ALT0_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

// Constructor for ADXL345 Interface
ADXL345 adxl = ADXL345();             // USE FOR I2C COMMUNICATION

//
// End object constructors
//

void setup() {
  // Serial port for diagnostics
  Serial.begin(115200);
  Serial.println("Orientation sensor test");
  
  // Setup and initialize OLED
  u8x8.begin();
  u8x8.setContrast(DISPLAYCONTRAST);
  u8x8.setFlipMode(FLIP);
  u8x8.setPowerSave(0);
  u8x8.setFont(/*u8x8_font_chroma48medium8_r*/u8x8_font_amstrad_cpc_extended_r);
  // A graphics display with 128x64 pixels
  // has 16 colums and 8 rows.
  // Suitable values for x are 0 to 15
  //            col,row,  string
  //              x,  y,  string
  u8x8.drawString(2,  0, "Orientation");
  u8x8.drawString(0, 1, "X:");
  u8x8.drawString(0, 2, "Y:");
  u8x8.drawString(0, 3, "Z:");
  u8x8.refreshDisplay();    // only required for SSD1606/7  
  Serial.println("OLED SSD1306 display initialized");

  // Setup and initialize the accelerometer
  adxl.powerOn();                     // Power on the ADXL345
  adxl.setRangeSetting( 4);           // Give the range settings
                                      // Accepted values are 2g, 4g, 8g or 16g
                                      // Higher Values = Wider Measurement Range
                                      // Lower Values = Greater Sensitivity
  adxl.setActivityXYZ(1, 1, 1);       // Set to activate movement detection in the axes "adxl.setActivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
  adxl.setActivityThreshold(75);      // 62.5mg per increment   // Set activity   // Inactivity thresholds (0-255)
  //adxl.setInactivityXYZ(1, 1, 1);     // Set to detect inactivity in all the axes "adxl.setInactivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
  //adxl.setInactivityThreshold(75);    // 62.5mg per increment   // Set inactivity // Inactivity thresholds (0-255)
  //adxl.setTimeInactivity(2);          // How many seconds of no activity is inactive?

    // Set values for what is considered a TAP and what is a DOUBLE TAP (0-255)
  //adxl.setTapThreshold(50);           // 62.5 mg per increment
  //adxl.setTapDuration(15);            // 625 μs per increment
  //adxl.setDoubleTapLatency(80);       // 1.25 ms per increment
  //adxl.setDoubleTapWindow(200);       // 1.25 ms per increment
 
  // Set values for what is considered FREE FALL (0-255)
  //adxl.setFreeFallThreshold(7);       // (5 - 9) recommended - 62.5mg per increment
  //adxl.setFreeFallDuration(30);       // (20 - 70) recommended - 5ms per increment
  
}

void loop() {
  int x, y, z;                        // Accelerometer axis readings
  // Read accelerometer
  adxl.readAccel(&x, &y, &z);

  // Display values read on serial monitor
  Serial.print("X: ");
  Serial.print(x);
  Serial.print(", Y: ");
  Serial.print(y);
  Serial.print(", Z: ");
  Serial.println(z);

  // Display values read on OLED
  //            col, row (1st row/col is 0,0)
  u8x8.clearLine(1);
  u8x8.setCursor(0, 1);
  u8x8.print("X:");
  u8x8.setCursor(3, 1);               // Display values read
  u8x8.print(x);

  u8x8.clearLine(2);
  u8x8.setCursor(0, 2);
  u8x8.print("Y: ");
  u8x8.setCursor(3, 2);
  u8x8.print(y);
  
  u8x8.clearLine(3);
  u8x8.setCursor(0, 3);
  u8x8.print("Z: ");
  u8x8.setCursor(3, 3);
  u8x8.print(z);

  u8x8.clearLine(7);
  u8x8.setCursor(0, 7);
  if (z > 130) {
    u8x8.print("Face Up");
  } else if (z < -80) {
    u8x8.print("Face Down");
  } else {
    u8x8.print("Tilted");
  }
  
  u8x8.setCursor(11, 7);
  if (x < -5) {
    u8x8.print("<");
  } else if (x > 5) {
    u8x8.print(">");
  } else {
    u8x8.print("-"); 
  }

  u8x8.setCursor(13, 7);
  if (y < -10) {
    u8x8.print("^");
  } else if (y > 10) {
    u8x8.print("v");
  } else {
    u8x8.print("-");
  }
  
  u8x8.refreshDisplay();
  
  delay(DELAYMS);
}
