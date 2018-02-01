#include <NMEA.h>

String GPS_message = "";       // GPS nmea message string
String Depth_message = "";     // Depth nmea message string
void setup() {
   Serial.begin(9600);
   Serial1.begin(4800);         // GPS
   Serial2.begin(9600);         // SD card
   Serial3.begin(9600);         // Transducer

}

void loop() {
  
  while(Serial3.available()> 0){
    record_NMEA();
  }
}

void record_NMEA(){
  static char new_char;
  static char old_char;
  static char new_chardepth;
  static char old_chardepth;

  while(Serial1.available()){
    old_char = new_char;
    new_char = (char) Serial1.read();
    GPS_message += new_char;

    old_chardepth = new_chardepth;
    new_chardepth = (char) Serial3.read();
    Depth_message += new_chardepth;

    if(old_char == '\r' && new_char == '\n'){           // stop at the end of nmea data packet
      Serial.print(GPS_message);
      Serial2.print(GPS_message);
      GPS_message = "";                                 // clearing message
    }

    if(old_chardepth == '\r' && new_chardepth == '\n'){
      Serial.print(Depth_message);
      Serial2.print(Depth_message);
      Depth_message = "";
    }
  }
}
