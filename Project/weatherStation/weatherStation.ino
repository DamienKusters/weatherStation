//#-#-# Weather Station #-#-#
//Version:
//  1.0
//Authors:
//  Damien Kusters
//  Sven van Engelen
//  William Verheijen
//  Anthony Schmitt
//Date:
//  Creation: 17-03-2017
//  Completion: 

//Reference the libraries for the peripherals
#include <Wire.h>         //I2C (Base Shield)
#include <HMC5883L.h>     //Compass
#include <Seeed_BMP280.h> //Thermometer
#include <rgb_lcd.h>      //LED display

//Init components
HMC5883L compass;
BMP280 bmp280;
rgb_lcd screen;

//Color channels the LED display is going to use
const int colorR = 255;
const int colorG = 255;
const int colorB = 255;

//Used for Compas Setup
int error = 0;

void setup()
{
  Serial.begin(9600);//This starts the Serial monitor
  
  Wire.begin();//Start the I2C interface

  //Start Setup Compass
        error = compass.setScale(1.3); //Set the scale of the compass
        if(error != 0)//If there is an error, print it out
        {
          Serial.println(compass.getErrorText(error));
        }
        error = compass.setMeasurementMode(MEASUREMENT_CONTINUOUS); //Set the measurement mode to Continuous
        if(error != 0)//If there is an error, print it out
        {
          Serial.println(compass.getErrorText(error));
        }
  //Stop Setup Compass
  //Start Setup Thermometer
        if(!bmp280.init())
        {
          Serial.println("Device error!");
        }
  //Stop Setup Thermometer
  //Start Setup LED display
        screen.begin(16, 2);
        screen.setRGB(colorR, colorG, colorB);
        screen.print("Hello World!");
  //Stop Setup LED display
}

void loop()
{
  
}
