#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_10DOF.h>

Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_BMP085_Unified       bmp   = Adafruit_BMP085_Unified(18001);
Adafruit_L3GD20_Unified       gyro  = Adafruit_L3GD20_Unified(20);


String GPS_message = "";       // GPS nmea message string
String Depth_message = "";     // Depth nmea message string

void setup() {
   Serial.begin(9600);
   Serial1.begin(4800);         // GPS
   Serial2.begin(9600);         // SD card
   Serial3.begin(9600);         // Transducer
   Wire.begin();
}

void loop() {
  
    record_SENSORS();
  
  }



void record_SENSORS(){
  static char new_char;
  static char old_char;
  static char new_chardepth;
  static char old_chardepth;
  static bool GPS_print = true;

  sensors_event_t event;

    accel.getEvent(&event);
    Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
    Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
    Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");

  while(GPS_print){
    if(Serial1.available()){
      old_char = new_char;
      new_char = (char) Serial1.read();
      GPS_message += new_char;

      if(GPS_message.charAt(0) == '$' && old_char == '\r' && new_char == '\n'){           // stop at the end of nmea data packet
          Serial.print(GPS_message);
          Serial2.print(GPS_message);
          GPS_message = "";                                 // clearing message
          GPS_print = false;
      }
    }

    if(Serial3.available()){
      old_chardepth = new_chardepth;
      new_chardepth = (char) Serial3.read();
      Depth_message += new_chardepth;

      if(old_chardepth == '\r' && new_chardepth == '\n'){
        Serial.print(Depth_message);
        Serial3.print(Depth_message);
        Depth_message = "";
      }
    }
      
    
  }
}

  


