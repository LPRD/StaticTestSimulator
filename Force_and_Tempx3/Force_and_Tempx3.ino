#include <SPI.h>
#include "Adafruit_MAX31855.h"
#include "HX711.h"

#define CLK 2
#define MAXDO 3
#define MAXCS1 4 
#define MAXCS2 5
#define MAXCS3 6
#define LCDO 8
#define PRESSURE_PIN 10 
#define PERIOD 100
#define calibration_factor 20400

Adafruit_MAX31855 thermocouple_1(CLK, MAXCS1, MAXDO);
Adafruit_MAX31855 thermocouple_2(CLK, MAXCS2, MAXDO);
Adafruit_MAX31855 thermocouple_3(CLK, MAXCS3, MAXDO);

HX711 loadcell(LCDO, CLK);

unsigned long last_run_time = 0;


void setup() 
{
  while (!Serial); // wait for Serial on Leonardo/Zero, etc
  Serial.begin(9600);
  pinMode(PRESSURE_PIN, INPUT);
  pinMode(LCDO, INPUT);

  loadcell.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  loadcell.tare();  //Assuming there is no weight on the scale at start up, reset the scale to 0
  
  delay (500); // wait for MAX chip to stabilize 
}

void loop() 
{
  if (millis () > last_run_time + PERIOD) 
  {
    last_run_time = millis ();
    /*Serial.print ("PSIG = ");
    Serial.println (getPressure(analogRead(PRESSURE_PIN)));
    //error message
    */
    if (isnan(getTemperature_1_Celsius()) || isnan(getTemperature_2_Celsius()) || isnan(getTemperature_3_Celsius()))
    {
      Serial.println ("Something wrong with thermocouple !");
    }
    else 
    {
      /*double CTemp = getTemperature_1_Celsius();
      Serial.print("C_1= ");
      Serial.println(CTemp); 
      CTemp = getTemperature_2_Celsius();
      Serial.print("C_2= ");
      Serial.println(CTemp); 
      CTemp = getTemperature_3_Celsius();
      Serial.print("C_3= ");
      Serial.println(CTemp);*/ 
      Serial.print("Force= ");
      Serial.println(loadcell.get_units(),1);
      //Serial.print ("F = ");
      //Serial.println (CTemp * 1.8 + 32);    
    }
  }
}

double getPressure (double reading) 
{
  double PSIG = getVDC(reading) * 246.58 - 118.33;
  return PSIG;
}

double getVDC (double reading) 
{
  double VDC = reading *5 / 1024;
  return VDC;  
}

double getInternalTemperature () 
{
  return thermocouple_1.readInternal();
}

double getTemperature_1_Celsius() 
{
  return thermocouple_1.readCelsius();
}

double getTemperature_2_Celsius() 
{
  return thermocouple_2.readCelsius();
}

double getTemperature_3_Celsius() 
{
  return thermocouple_3.readCelsius();
}



 

