//#-#-# Weather Station #-#-#
//Version:
//  1.0
//Authors:
//  Damien Kusters
//  Sven Engelen
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
#include "DHT.h"          //Temperature and Humidity
#define DHTPIN A0         //Temperature and Humidity
#define DHTTYPE DHT11     //Temperature and Humidity
DHT dht(DHTPIN, DHTTYPE);

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

int timerMainMax = 100;
int timerMainCount = 0;
bool timerMainCountChanged = false;

int nameId = 0;

int displayMode = 0;
//displayMode = 0 | Main Menu
//displayMode = 1 | Temperature
//displayMode = 2 | Humidity
//displayMode = 3 | Wind direction
//displayMode = 4 | Air pressure 
//displayMode = 5 | Yaw, pitch and roll

//Switch button connected to port: 8
int btnSwitch = 8;
//Debounce variables.
int debOldSwitch = LOW;
int debNewSwitch = LOW;
int debStartSwitch;
int debStopSwitch;

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
          Serial.println("BMP Device error!");
        }
  //Stop Setup Thermometer
  //Start Setup LED display
        screen.begin(16, 2);
        screen.setRGB(colorR, colorG, colorB);
        screen.print("Weather  Station");
  //Stop Setup LED display
  //Start Setup Temperature and Humidity Sensor
        dht.begin();
  //Stop Setup Temperature and Humidity Sensor

  pinMode(btnSwitch, INPUT);
}

void loop()
{
  checkTimer();

  if(timerMainCountChanged && displayMode == 0)
  {
    nameSwitch();
  }

  if(displayMode == 1)
  {
    screen.setCursor(0, 1);
    screen.print(bmp280.getTemperature());
    screen.print("C");
  }
  if(displayMode == 2)
  {
    screen.setCursor(0, 1);
    screen.print(dht.readHumidity());
    screen.print("%");
  }
  if(displayMode == 3)
  {
    screen.setCursor(0, 1);
    screen.print("???");
  }
  if(displayMode == 4)
  {
    float press;
    screen.setCursor(0, 1);
    screen.print(press = bmp280.getPressure());
    screen.print("Pa");
  }
  if(displayMode == 5)
  {
    MagnetometerScaled scaled = compass.readScaledAxis();
    
    screen.setCursor(0, 1);
    screen.print((int)scaled.YAxis);
    screen.print(" ");
    screen.print((int)scaled.ZAxis);
    screen.print(" ");
    screen.print((int)scaled.XAxis);
    //Y=Yaw | Z=Pitch | X=Roll
  }
  
  //checkButtonState() will call switchIsPressed()
  //if conditions apply
  checkButtonState();

  timerMainCountChanged = false;
}

void checkTimer()
{
  timerMainMax--;
  if(timerMainMax <= 0)
  {
    timerMainCount++;
    timerMainMax = 200;
    timerMainCountChanged = true;
  }
}

void checkButtonState()
{
  //Used for debouncing Switch(:
        debNewSwitch = digitalRead(btnSwitch);
      
        debStartSwitch = digitalRead(btnSwitch);
        delay(10);
        debStopSwitch = digitalRead(btnSwitch);
  //:)

  if (debStartSwitch == debStopSwitch) //if Switch is pressed
  {
    if (debNewSwitch == HIGH  && debOldSwitch == LOW)
    {
      //Function will fire. If button is pressed.
      switchIsPressed();
    }
  }

  debOldSwitch = debNewSwitch;
}

void switchIsPressed()
{
  screen.setCursor(0, 1);
  screen.print("               ");
  screen.setCursor(1, 0);
  screen.print("               ");
  
  displayMode++;

  switch (displayMode)
  {
    case 0:
      screen.setCursor(0, 0);
      screen.print("Weather  Station");
      break;
    case 1:
      screen.setCursor(0, 0);
      screen.print("Temperature");
      break;
    case 2:
      screen.setCursor(0, 0);
      screen.print("Humidity");
      break;
    case 3:
      screen.setCursor(0, 0);
      screen.print("Wind direction");
      break;
    case 4:
      screen.setCursor(0, 0);
      screen.print("Air pressure");
      break;
    case 5:
      screen.setCursor(0, 0);
      screen.print("Yaw,pitch,roll");
      break;
    case 6:
      screen.setCursor(0, 0);
      screen.print("Weather  Station");
      displayMode = 0;
      break;
  }
}

void nameSwitch()
{
  screen.setCursor(0, 1);
  screen.print("               ");
  screen.setCursor(0, 1);
  screen.print("by: ");
  
  switch (nameId)
  {
    case 0:
      screen.print("Damien K.");
      nameId++;
      break;
    case 1:
      screen.print("Sven E.");
      nameId++;
      break;
    case 2:
      screen.print("William V.");
      nameId++;
      break;
    case 3:
      screen.print("Anthony S.");
      nameId = 0;
      break;
  }
}
