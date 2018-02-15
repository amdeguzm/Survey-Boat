#include <Servo.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_10DOF.h>

#define TEST_TIME  1000000    // Test time in milliseconds
#define INTERRUPT_PIN_T 2     // Digital pin 2 designated for throttle interrupts
#define INTERRUPT_PIN_R 3     // Digital pin 3 designated for rudder interrupts

Adafruit_10DOF                dof   = Adafruit_10DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_BMP085_Unified       bmp   = Adafruit_BMP085_Unified(18001);
Adafruit_L3GD20_Unified       gyro  = Adafruit_L3GD20_Unified(20);


String GPS_message = "";       // GPS nmea message string
String Depth_message = "";     // Depth nmea message string

volatile int T_state = 0;           // Variables used for interrupt logging
volatile int R_state = 0;

volatile unsigned long pwm_T = 0;
volatile unsigned long pwm_R = 0;
volatile unsigned long timeA_T = 0, timeB_T = 0, timeC_T = 0;              
volatile unsigned long timeA_R = 0, timeB_R = 0, timeC_R = 0;


Servo throttleServo;           // Servos for control
Servo rudderServo;

void setup() {
   Serial.begin(9600);
   Serial1.begin(4800);         // GPS
   Serial2.begin(9600);         // SD card
   Serial3.begin(9600);         // Transducer
   Wire.begin();
   Serial.println("START OF TEST");
   Serial2.println("START OF TEST"); //To parse the beginning of test, could find a way to make this date/time later?

   attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN_T), servo_pos_throttle, CHANGE);
   attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN_R), servo_pos_rudder, CHANGE);
   
   throttleServo.attach(22);     // Digital pin 22 to white throttle wire
   rudderServo.attach(23);       // Digital pin 23 to white rudder wire
  

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

  record_GPS();
  record_IMU();
  record_DBT();
  pulse_Servo();
  
  //print_Servo();
  
  if(millis()>TEST_TIME){
    Serial.println("END OF TEST");
    Serial2.println("END OF TEST"); //
    while(1);
  }
}


/*
 * This method retrieves and prints Accelerometer, Magnetometer and Gyro data
 * It has a no arg input and returns void
 */
void record_IMU(){
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
       
       if (dof.accelGetOrientation(&event,&orient))
       {
        Serial.print(F("ORIENTATION: ")); 
        Serial.print("ROLL: ");    Serial.print(orient.roll);    Serial.print("  ");
        Serial.print("PITCH: ");   Serial.print(orient.pitch);   Serial.print("  ");
       }
       
       if(dof.magGetOrientation(SENSOR_AXIS_Z, &event, &orient))
       {
         Serial.print("HEADING: "); Serial.print(orient.heading); Serial.print("  "); Serial.println("deg");
       }
       //
       //Serial.print("Throttle: ");
       //Serial.println(throttleServo.read());   // prints angle of servo from 0 to 180 degrees
       //Serial.print("Rudder: ");
       //Serial.println(rudderServo.read());

       //Serial2.print("Throttle: ");
       //Serial2.println(throttleServo.read());   // prints angle of servo from 0 to 180 degrees
       //Serial2.print("Rudder: ");
       //Serial2.println(rudderServo.read());
           
}

 
/*
 * This method retrieves Depth readings from the transducer and prints the full NMEA string
 * No arg input and returns void
 */
void record_DBT(){
  static char new_chardepth;
  static char old_chardepth;
  if(Serial3.available()){
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
}


/*
 * This method retrieves GPS readings from the GPS and prints the full NMEA string 
 * No arg input and returns void
 */
void record_GPS(){
  static char new_char;
  static char old_char;
  if(Serial1.available()){
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
  }
}
/*
 * This method uses digital pins 4 and 5 to record throttle and rudder servo values
 * values are in microseconds
 */

void pulse_Servo(){
  Serial.print("Rudder Servo: ");
  Serial.println(pulseIn(4, HIGH));

  Serial.print("Throttle Servo: ");
  Serial.println(pulseIn(5, HIGH));
}

void print_Servo(){
  Serial.print("Throttle Servo: ");
  Serial.println(pwm_T);

  Serial.print("Rudder Servo: ");
  Serial.println(pwm_R);
}

/*
 * This method records the period of the Throttle Servo into global variable pwm_T
 * Called from interrupts occuring on pin 2, returns void
 */
void servo_pos_throttle(){
  
  if( !T_state){
    timeA_T = timeB_T;
    timeB_T = timeC_T;
    timeC_T = micros();
    pwm_T = timeC_T - timeB_T;
  }
  T_state = !T_state;
  
}

/* 
 * This method records the period of the Rudder Servo into global variable pwm_R
 * Called from interrupts occuring on pin 3, returns void
 */
void servo_pos_rudder(){
  
  if( !R_state){
    timeA_R = timeB_R;
    timeB_R = timeC_R;
    timeC_R = micros();
    pwm_R = timeC_R - timeB_R; 
  }
  R_state = !R_state;
}


  


