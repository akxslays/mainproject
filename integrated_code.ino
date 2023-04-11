#include <AsyncDelay.h>
#include <SoftWire.h>
#include <LiquidCrystal_I2C.h>
#include <Math.h>
//---------------------------------------------------------------
#define SDA_P 20
#define SCL_P 21

//---------------------------------------------------------------
#include <Wire.h>
#define LIS3DH_ADDR 0x18 // The I2C address of the LIS3DH module

//---------------------------------------------------------------
LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftWire SWire(SDA_P,SCL_P);
//---------------------------------------------------------------

void setup() {
  
//---------------------------------------------------------------
  SWire.begin();
  lcd.init(); // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight
  lcd.setCursor(0,0); // Position of Cursor on LCD Screen
//---------------------------------------------------------------
  
  Wire.begin(); // Start the I2C communication
  Serial.begin(9600); // Initialize serial communication at 9600 baud
  // Configure the LIS3DH module
  Wire.beginTransmission(LIS3DH_ADDR);
  Wire.write(0x20); // CTRL_REG1 address
  Wire.write(0x27); // Enable all axes, set data rate to 10Hz, normal mode
  Wire.endTransmission();
}

void loop() {
  // Read acceleration data from the LIS3DH module
  Wire.beginTransmission(LIS3DH_ADDR);
  Wire.write(0x28 | 0x80); // OUT_X_L address with auto-increment bit set
  Wire.endTransmission();
  Wire.requestFrom(LIS3DH_ADDR, 6); // Request 6 bytes of data (2 bytes for each axis)
  if (Wire.available() >= 6) {
    int x = Wire.read() | (Wire.read() << 8); // Combine the two bytes for X axis
    int y = Wire.read() | (Wire.read() << 8); // Combine the two bytes for Y axis
    int z = Wire.read() | (Wire.read() << 8); // Combine the two bytes for Z axis
    
    float x_g = x ; // 1 LSB = 0.061 mg, 1 g = 9.81 m/s^2
    float y_g = y ;
    float z_g = z ;
    // Print the acceleration values to the serial monitor
    Serial.print("X = ");
    Serial.print(x_g);
    Serial.print("Y = ");
    Serial.print(y_g);
    Serial.print("Z = ");
    Serial.print(z_g);
    Serial.print("\n");

    int arg = y/sqrt((x*x+z*z));
    int theta = atan(arg)* 180/3.14159265;

    lcd.setCursor(0,0);
    lcd.print("Angle");
    lcd.print(":");
    lcd.print(theta);
  }
  delay(1000); // Wait for 100 milliseconds before requesting new data
  lcd.clear(); 
}
