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
   Serial1.begin(115200);         // GPS
   Serial2.begin(115200);         // SD card
   Serial3.begin(9600);         // Transducer
   Wire.begin();

   // millis() returns current millisecond since program start

   if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println(F("Ooops, no LSM303 detected ... Check your wiring!"));
    while(1);
  }

  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  
  }

   if(!gyro.begin())
  {
    /* There was a problem detecting the L3GD20 ... check your connections */
    Serial.print("Ooops, no L3GD20 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
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
       Serial.print(F("ACCEL "));
       Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
       Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
       Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");

       Serial2.print(F("ACCEL "));
       Serial2.print("X: "); Serial2.print(event.acceleration.x); Serial2.print("  ");
       Serial2.print("Y: "); Serial2.print(event.acceleration.y); Serial2.print("  ");
       Serial2.print("Z: "); Serial2.print(event.acceleration.z); Serial2.print("  ");Serial2.println("m/s^2 ");

       mag.getEvent(&event);
       Serial.print(F("MAG   "));
       Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print("  ");
       Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("  ");
       Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  ");Serial.println("uT");

       Serial2.print(F("MAG   "));
       Serial2.print("X: "); Serial2.print(event.magnetic.x); Serial2.print("  ");
       Serial2.print("Y: "); Serial2.print(event.magnetic.y); Serial2.print("  ");
       Serial2.print("Z: "); Serial2.print(event.magnetic.z); Serial2.print("  ");Serial2.println("uT");

       gyro.getEvent(&event);
       Serial.print(F("GYRO  "));
       Serial.print("X: "); Serial.print(event.gyro.x); Serial.print("  ");
       Serial.print("Y: "); Serial.print(event.gyro.y); Serial.print("  ");
       Serial.print("Z: "); Serial.print(event.gyro.z); Serial.print("  ");Serial.println("rad/s ");

       Serial2.print(F("GYRO  "));
       Serial2.print("X: "); Serial2.print(event.gyro.x); Serial2.print("  ");
       Serial2.print("Y: "); Serial2.print(event.gyro.y); Serial2.print("  ");
       Serial2.print("Z: "); Serial2.print(event.gyro.z); Serial2.print("  ");Serial2.println("rad/s ");
      
    while(Serial1.available()){
      old_char = new_char;
      new_char = (char) Serial1.read();
      GPS_message += new_char;
      
      if(old_char == '\r' && new_char == '\n'){           // stop at the end of nmea data packet
          Serial.print(GPS_message);
          Serial2.print(GPS_message);
          GPS_message = "";                                 // clearing message
      }
       
    }

    while(Serial3.available()){
      old_chardepth = new_chardepth;
      new_chardepth = (char) Serial3.read();
      Depth_message += new_chardepth;

      if(old_chardepth == '\r' && new_chardepth == '\n'){
        Serial.print(Depth_message);
        Serial2.print(Depth_message);
        Depth_message = "";
      }
    }
      
    

}

  


