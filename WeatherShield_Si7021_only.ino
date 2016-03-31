// *************************************************************************************************************
//                                        WeatherShield sample sketch
// *************************************************************************************************************
// This sketch does periodic reads from a Moteino Weather Shield BMP180 and Si7021 sensors and displays
// information like this (real sample):
//
//        ************ Si7021 *********************************
//        C: 2389   F: 7500   H: 36%   DID: 21  BATT: 1015
//        *****************************************************
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
#include <SI7021.h>        //get it here: https://github.com/LowPowerLab/SI7021
#include <Wire.h>

SI7021 sensor;

void setup() {
  pinMode(A7, INPUT);
  Serial.begin(115200);
  sensor.begin();
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
  digitalWrite(A3, HIGH);
  //*************** READING BATTERY VOLTAGE *********************
    
  char status;
  double T,P,p0,a;

  delay(2000);
}


