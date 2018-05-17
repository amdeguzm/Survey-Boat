# Survey-Boat

Tasks: 
-plot relationships between throttle and position
-plot relationship between rudder and yaw rate 
-create MATLAB code to parse data 
-try to get an interrupt to work for throttle/rudder


-separate test files                                                      (completed
-check if the heading angle is affected by the motor                      (completed)
-conduct static tests on boat to see if csv files are printed on SD card  (completed)
-Fix header and cpp files to parse transducer and GPS data                (completed)
-collect data from IMU                                                    (completed)
-parse IMU data                                                           (completed)
-collect throttle and rudder servo data                                   (completed)
-tape/velcro arduino to the boat for testing                              (completed)
-redo the wiring from the receiver to the servos                          (completed)
-aquire 3 channel connectors and extra cable                              (completed)


  
  
  Notes:
  
  5/3
  -separated data into test files in "Test Data" folder
  -added an excel doc that contains all the tests 
  
  4/18
  -successful test on the water
  -tests conducted: counter clockwise, closkwise, slow constant, fast constant, acceleration, slow constant
  
  4/8
  -used an external battery to power the servos (servos were drawing too much current from the arduino causing it to reset)
  -code only writes one .csv file 
    
  
  4/2
  -Updated code to get arduino to read PWM signal from the receiver and write the value to the servos
  -reorganized code into sections
  
  
  
  3/8
  Test Plan
    record time at power on
      Time:
    clockwise circles (need start time)
      Start Time:
    counterclockwise circles (need start time)
      Start Time:
    step response slow, medium, fast
      Slow start time:
      Medium start time:
      fast start time:
  finished
  
  2/14
  Applied pulse in to read the throttle and rudder data. Added code to print the roll, pitch, and heading angles. 
  
  
  2/12
  Connected the servos to the arduino. We have a problem because it seems the arduino can't read what position the servo is in. servo.read() simply returns whatever was written last to it which is useless for us. This is something we should bring up to Dr. A. I updated the master branches Surveyboat_2_7_18.ino to the current version of code which includes your additions. 
  
  2/8
  - Uploaded notes about servo. Survey_boat.ino can collect and parse gps, depth, and imu data. It can also print this data to a file on a microSD. Each restart of the arduino creates a new file on the microSD. GPS and Depth transducer update at 1 Hz,magnetic update at 25 Hz, accelerometer update at 50Hz, gyro update 90 Hz. The last three seem to be configurable
  
  2/5
  -for IMU libraries: https://learn.adafruit.com/adafruit-10-dof-imu-breakout-lsm303-l3gd20-bmp180?view=all#connecting-it-up
  
  https://www.sparkfun.com/products/13712
  
  https://www.sparkfun.com/datasheets/GPS/EM-406A_User_Manual.PDF
  
  https://www.sparkfun.com/products/449
  
  
  1/31
  -Meetings: Monday/Wed 1pm-whenever
  
  -Hey Angelica,
  I found some files for a shield template for the mega. 
  If you have altium, or CAD (might need to convert the file) 
  you can view what the layout will look like. Otherwise just
  know it'll be a clean way for most of our sensors to fit 
  compactly on top of the mega. I put all the new files in a
  seperate branch. (I hope)
