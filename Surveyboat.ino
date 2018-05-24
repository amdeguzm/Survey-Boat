//-----------------------------------LIBRARIES----------------------------------------------
#include <Wire.h>
#include <Adafruit_Sensor.h>         // IMU libraries
#include <Adafruit_LSM303_U.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_10DOF.h>
#include <Servo.h>                   // Servo library
#include <PID_v1.h>                  // PID library



//-----------------------------------MACROS-------------------------------------------------
#define TEST_TIME  10000000          // Test time in milliseconds
#define THR_PULSE  3                 // Throttle digitl input pin from receiver 
#define THR_PIN    8                 // Throttle digital output pin to servo
#define RUD_PULSE  4                 // Rudder digital input pin from receiver 
#define RUD_PIN    9                 // Rudder digital output pin to servo
#define auto_PULSE 5                 // Digital input for autopilot


//-----------------------------------OBJECTS-------------------------------------------------
Adafruit_10DOF                       dof   = Adafruit_10DOF();                     
Adafruit_LSM303_Accel_Unified        accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified          mag   = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_BMP085_Unified              bmp   = Adafruit_BMP085_Unified(18001);
Adafruit_L3GD20_Unified              gyro  = Adafruit_L3GD20_Unified(20);
Servo throttleServo;                 // Servo instantiation and initialization
Servo rudderServo;

//-----------------------------------VARIABLES-----------------------------------------------
volatile unsigned long thrPWM = 0;
volatile unsigned long rudPWM = 0;
volatile unsigned long auto_PWM = 0;

String GPS_message = "";                      // GPS nmea message string
String Depth_message = "";                    // Depth nmea message string

double headingCurr,yawRateCurr, yawRateRef;   // PID parameters 
double rudPWMAuto = 1500; 
double Kp = 5528.9;                           // PID gains 
double Ki = 94008.0; 
double Kd = 46.20;             
PID myPID(&yawRateCurr, &rudPWMAuto, &yawRateRef,Kp,Ki,Kd, DIRECT);

//-----------------------------------SETUP---------------------------------------------------
void setup() {
   Serial.begin(57600);
   Serial1.begin(4800);              // GPS
   Serial2.begin(57600);             // SD card
   Serial3.begin(9600);              // Transducer
   Wire.begin();
 
   Serial.println("START OF TEST");
   Serial2.println("START OF TEST"); //To parse the beginning of test, could find a way to make this date/time later?
   delay(2000);
   
   Serial.println("Survey Boat Data Packet");
   Serial.print("ax");Serial.print(",");Serial.print("ay");Serial.print(",");Serial.print("az");Serial.print(",");
   Serial.print("mx");Serial.print(",");Serial.print("my");Serial.print(",");Serial.print("mz");Serial.print(",");
   Serial.print("rx");Serial.print(",");Serial.print("ry");Serial.print(",");Serial.print("rz");Serial.print(",");
   Serial.print("roll");Serial.print(",");Serial.print("pitch");Serial.print(",");Serial.print("yaw");Serial.print(",");
   Serial.print("throttle");Serial.print(",");Serial.println("rudder");
   Serial2.print("ax");Serial2.print(",");Serial2.print("ay");Serial2.print(",");Serial2.print("az");Serial2.print(",");
   Serial2.print("mx");Serial2.print(",");Serial2.print("my");Serial2.print(",");Serial2.print("mz");Serial2.print(",");
   Serial2.print("rx");Serial2.print(",");Serial2.print("ry");Serial2.print(",");Serial2.print("rz");Serial2.print(",");
   Serial2.print("roll");Serial2.print(",");Serial2.print("pitch");Serial2.print(",");Serial2.print("yaw");Serial2.print(",");
   Serial2.print("throttle");Serial2.print(",");Serial2.println("rudder");
   
 
   throttleServo.attach(THR_PIN);    // Digital pin 8 to white throttle wire
   rudderServo.attach(RUD_PIN);      // Digital pin 9 to white rudder wire
   
   if(!accel.begin()){
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println(F("Ooops, no LSM303 detected ... Check your wiring!"));
    while(1);
   }
   if(!mag.begin()){
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
   }
   if(!gyro.begin()){
    /* There was a problem detecting the L3GD20 ... check your connections */
    Serial.print("Ooops, no L3GD20 detected ... Check your wiring or I2C ADDR!");
    while(1);
   }

   myPID.SetMode(AUTOMATIC);        // Initialize PID
}

//-----------------------------------LOOP---------------------------------------------------
void loop() {
  
  record_GPS();
  record_IMU();
  //record_DBT();
  pulse_Servo();                    // Continues manual control
  if(millis()>TEST_TIME){
    Serial.println("END OF TEST");
    Serial2.println("END OF TEST"); 
    while(1);
  }
  while(autopilot()){

    record_GPS();
    record_IMU();
    pulse_Servoauto();              // Initiate autonomous control

  }
  
}


//-----------------------------------FUNCTIONS-----------------------------------------------
/*
 * This method retrieves and prints Accelerometer, Magnetometer and Gyro data
 * It has a no arg input and returns void
 */
void record_IMU(){
  sensors_event_t accel_event;
  sensors_event_t mag_event; 
  sensors_event_t gyro_event;
  sensors_vec_t orient;
  
  accel.getEvent(&accel_event);
  Serial.print(accel_event.acceleration.x); Serial.print(",");
  Serial.print(accel_event.acceleration.y); Serial.print(",");
  Serial.print(accel_event.acceleration.z); Serial.print(",");
  Serial2.print(accel_event.acceleration.x); Serial2.print(",");
  Serial2.print(accel_event.acceleration.y); Serial2.print(",");
  Serial2.print(accel_event.acceleration.z); Serial2.print(",");
   
  mag.getEvent(&mag_event);
  Serial.print(mag_event.magnetic.x); Serial.print(",");
  Serial.print(mag_event.magnetic.y); Serial.print(",");
  Serial.print(mag_event.magnetic.z); Serial.print(",");
  Serial2.print(mag_event.magnetic.x); Serial2.print(",");
  Serial2.print(mag_event.magnetic.y); Serial2.print(",");
  Serial2.print(mag_event.magnetic.z); Serial2.print(",");
  
  gyro.getEvent(&gyro_event);
  Serial.print(gyro_event.gyro.x); Serial.print(",");
  Serial.print(gyro_event.gyro.y); Serial.print(",");
  Serial.print(gyro_event.gyro.z); Serial.print(",");
  Serial2.print(gyro_event.gyro.x); Serial2.print(",");
  Serial2.print(gyro_event.gyro.y); Serial2.print(",");
  Serial2.print(gyro_event.gyro.z); Serial2.print(",");
  yawRateCurr = double(gyro_event.gyro.z);  
  
  if (dof.accelGetOrientation(&accel_event,&orient)){
    Serial.print(orient.roll); Serial.print(",");
    Serial.print(orient.pitch);Serial.print(",");
    Serial2.print(orient.roll); Serial2.print(",");
    Serial2.print(orient.pitch);Serial2.print(",");
  }
  
  if(dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orient)){
    Serial.print(orient.heading); Serial.print(","); 
    Serial2.print(orient.heading); Serial2.print(","); 
    headingCurr = (double)orient.heading;
  }         
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
          Serial3.print(Depth_message);
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
        if(old_char == '\r' && new_char == '\n'){             // stop at the end of nmea data packet
            Serial.print(GPS_message);
            Serial2.print(GPS_message);
            GPS_message = "";                                 // clearing message
        }
    }
  }
}



/*
 * This method uses digital pin 5 to decide if an auto pilot routine should be executed
 */
bool autopilot(){
  bool mode = false;
  auto_PWM = pulseIn(auto_PULSE, HIGH);
  if(auto_PWM > 1500)
    mode = true;
  else
    mode = false;
  return mode;
}



/*
 * This method uses analog pins 3 and 4 to record throttle and rudder servo values
 * values are in microseconds
 */
void pulse_Servo(){
  thrPWM = pulseIn(THR_PULSE,HIGH);

  if (thrPWM>=1100 && thrPWM<=1900){
    throttleServo.writeMicroseconds(thrPWM);
  }
  else if(thrPWM > 1900){
    thrPWM = 1900;
    throttleServo.writeMicroseconds(thrPWM);
  }
  else if(thrPWM < 1100){
    thrPWM = 1100;
    throttleServo.writeMicroseconds(thrPWM);
  }
  Serial.print(thrPWM);Serial.print(",");                // print the pulsewidth of the throttle when signal is high
  Serial2.print(thrPWM);Serial2.print(","); 


  rudPWM = pulseIn(RUD_PULSE,HIGH);
  if (rudPWM>=1100 && rudPWM<=1900){
    rudderServo.writeMicroseconds(rudPWM);
  }
  else if(rudPWM > 1900){
    rudPWM = 1900;
    rudderServo.writeMicroseconds(rudPWM);
  }
  else if(rudPWM < 1100){
    rudPWM = 1100;
    rudderServo.writeMicroseconds(rudPWM);
  }
  Serial.println(rudPWM);Serial.print("");              // print the pulsewidth of the rudder when signal is high
  Serial2.println(rudPWM);Serial2.println(""); 
}


/*
 * This method autonomously computes rudder input using PID control
 */
void pulse_Servoauto(){
  
  double headingRef = 90.0;                                                           // Deg - (eventually we want lat/lon as our reference instead of heading angle)
  double yawRateRef = ((headingRef - headingCurr)/0.05)*(3.14159/180);                // Assume 20 samples per second average, in rad/s

  myPID.Compute();                                                                    // Computes PID Control: Input and Reference are the yaw rate, output is the rudder control input
  thrPWM = pulseIn(THR_PULSE,HIGH);                                                   // we still have control over throttle
  if (rudPWMAuto < 1100){
    rudPWMAuto = 1100;
  }
  else if (rudPWMAuto > 1900){
    rudPWMAuto = 1900; 
  }

  throttleServo.writeMicroseconds(thrPWM);                                             // Write microseconds to throttle and rudder
  rudderServo.writeMicroseconds(rudPWMAuto);

  
  Serial.print(rudPWMAuto); Serial.print(",");                                         // Print indicates we're in autonomous mode 
  Serial.print(thrPWM); Serial.print(",");
  Serial2.print(rudPWMAuto); Serial2.print(",");
  Serial2.print(thrPWM); Serial2.print(",");
  Serial.print("Auto");
  Serial2.println("Auto");
  Serial.print(",");Serial.print(headingCurr); Serial.print(",");                      // Delete once verified
  Serial.print(yawRateRef); Serial.print(",");
  Serial.println(rudPWMAuto);
}
