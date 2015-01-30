// *************************************************************************************************************
//                                        WeatherShield sample sketch
// *************************************************************************************************************
// This sketch does periodic reads from a Moteino Weather Shield BMP180 and Si7021 sensors and displays
// information like this (real sample):
//
//        ************ Si7021 *********************************
//        C: 2389   F: 7500   H: 36%   DID: 21  BATT: 1015
//        ************ BMP180 *********************************
//        provided altitude: 218 meters, 716 feet
//        C: 24.14    F:75.44
//        abs pressure: 992.23 mb, 29.30 inHg
//        relative (sea-level) pressure: 1018.31 mb, 30.07 inHg
//        computed altitude: 218 meters, 716 feet
//
//   You can also read the battery reading from the VIN pin through a resistor divider (VIN - 4.7K + 10K -GND).
//   This is enabled through a P-mosfet that is turned ON through A3. To save power, you only need to turn this 
//   circuit ON when you need a reading. To turn ON set A3 to OUTPUT and LOW. To turn it OFF for power saving set
//   A3 to INPUT (this makes A3 HIGH-Z).
// *************************************************************************************************************
// Copyright Felix Rusu (2015), felix@lowpowerlab.com
// http://lowpowerlab.com/
// *************************************************************************************************************
// License
// *************************************************************************************************************
// This program is free software; you can redistribute it 
// and/or modify it under the terms of the GNU General    
// Public License as published by the Free Software       
// Foundation; either version 2 of the License, or        
// (at your option) any later version.                    
//                                                        
// This program is distributed in the hope that it will   
// be useful, but WITHOUT ANY WARRANTY; without even the  
// implied warranty of MERCHANTABILITY or FITNESS FOR A   
// PARTICULAR PURPOSE.  See the GNU General Public        
// License for more details.                              
//                                                        
// You should have received a copy of the GNU General    
// Public License along with this program; if not, write 
// to the Free Software Foundation, Inc.,                
// 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//                                                        
// Licence can be viewed at                               
// http://www.fsf.org/licenses/gpl.txt                    
//
// Please maintain this license information along with authorship
// and copyright notices in any redistribution of this code
// *************************************************************************************************************
#include <SFE_BMP180.h>    //get it here: https://github.com/LowPowerLab/SFE_BMP180
#include <SI7021.h>        //get it here: https://github.com/LowPowerLab/SI7021
#include <Wire.h>

SI7021 sensor;
SFE_BMP180 pressure;
#define ALTITUDE 218.3 // Altitude in meters

void setup() {
  pinMode(A7, INPUT);
  Serial.begin(115200);
  sensor.begin();
  
  if (pressure.begin())
    Serial.println("BMP180 init success");
  else
  {
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.

    Serial.println("BMP180 init fail\n\n");
    while(1); // Pause forever.
  }
}

void loop() {
  int temperature = sensor.getCelsiusHundredths();
  Serial.println("************ Si7021 *********************************");
  Serial.print("C: ");Serial.print(temperature);

  temperature = sensor.getFahrenheitHundredths();
  Serial.print("   F: ");Serial.print(temperature);

  int humidity = sensor.getHumidityPercent();
  Serial.print("   H: ");Serial.print(humidity);Serial.print("%   ");

  // this driver should work for SI7020 and SI7021, this returns 20 or 21
  int deviceid = sensor.getDeviceId();
  Serial.print("DID: ");Serial.print(deviceid);

  //*************** READING BATTERY VOLTAGE *********************
  //turn MOSFET ON and read voltage, should give a valid reading
  pinMode(A3, OUTPUT);
  digitalWrite(A3, LOW);
  Serial.print("  BATT: ");
  Serial.println(analogRead(A7));
  pinMode(A3, INPUT); //put A3 in HI-Z mode (to allow mosfet gate pullup to turn it OFF)
  //*************** READING BATTERY VOLTAGE *********************
    
  char status;
  double T,P,p0,a;

  // Loop here getting pressure readings every 10 seconds.

  // If you want sea-level-compensated pressure, as used in weather reports,
  // you will need to know the altitude at which your measurements are taken.
  // We're using a constant called ALTITUDE in this sketch:
  Serial.println("************ BMP180 *********************************");
  Serial.print("provided altitude: ");
  Serial.print(ALTITUDE,0);
  Serial.print(" meters, ");
  Serial.print(ALTITUDE*3.28084,0);
  Serial.println(" feet");
  
  // If you want to measure altitude, and not pressure, you will instead need
  // to provide a known baseline pressure. This is shown at the end of the sketch.
  // You must first get a temperature measurement to perform a pressure reading.
  // Start a temperature measurement:
  // If request is successful, the number of ms to wait is returned.
  // If request is unsuccessful, 0 is returned.
  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Function returns 1 if successful, 0 if failure.

    status = pressure.getTemperature(T);
    if (status != 0)
    {
      // Print out the measurement:
      Serial.print("C: ");
      Serial.print(T,2);
      Serial.print("    F:");
      Serial.print((9.0/5.0)*T+32.0,2);
      Serial.println("");
      
      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.

      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          // Print out the measurement:
          Serial.print("abs pressure: ");
          Serial.print(P,2);
          Serial.print(" mb, ");
          Serial.print(P*0.0295333727,2);
          Serial.println(" inHg");

          // The pressure sensor returns abolute pressure, which varies with altitude.
          // To remove the effects of altitude, use the sealevel function and your current altitude.
          // This number is commonly used in weather reports.
          // Parameters: P = absolute pressure in mb, ALTITUDE = current altitude in m.
          // Result: p0 = sea-level compensated pressure in mb

          p0 = pressure.sealevel(P,ALTITUDE); // we're at 1655 meters (Boulder, CO)
          Serial.print("relative (sea-level) pressure: ");
          Serial.print(p0,2);
          Serial.print(" mb, ");
          Serial.print(p0*0.0295333727,2);
          Serial.println(" inHg");

          // On the other hand, if you want to determine your altitude from the pressure reading,
          // use the altitude function along with a baseline pressure (sea-level or other).
          // Parameters: P = absolute pressure in mb, p0 = baseline pressure in mb.
          // Result: a = altitude in m.

          a = pressure.altitude(P,p0);
          Serial.print("computed altitude: ");
          Serial.print(a,0);
          Serial.print(" meters, ");
          Serial.print(a*3.28084,0);
          Serial.println(" feet");
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");
  Serial.println("*****************************************************\n");
  delay(2000);
}


