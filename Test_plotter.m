%% Survey Boat Plotter
clear;clc;close all;
file = "Test_data.xlsx";

% Test1 counterclockwise
rud1 = xlsread(file,'Test1_clockwise','N:N');
yaw1 = xlsread(file,'Test1_clockwise','L:L');
time1 = linspace(0,10,length(rud1));

% Test2 clockwise 
rud2 = xlsread(file,'Test2_clockwise','N:N'); 
yaw2 = xlsread(file,'Test2_clockwise','L:L');
time2 = linspace(0,10,length(rud2));

%% Plotter


% Test 1 counterclockwise plot
figure; 
tlim1 = [0 10]; %choose time interval from 0 to 10
subplot(2,1,1)
plot(time1,yaw1,'b');
xlabel('time(s)');
ylabel('yaw output (deg)');
xlim(tlim1);
title('Test 1: Rudder Input to Yaw Output');
subplot(2,1,2);
plot(time1, rud1,'r');
xlabel('time (s)');
ylabel('rudder input (\mu s)');
xlim(tlim1);

% Test2 clockwise plot
figure; 
tlim2 = [0 10]; %choose time interval from 0 to 10
subplot(2,1,1)
plot(time2,yaw2,'b');
xlabel('time(s)');
ylabel('yaw output (deg)');
xlim(tlim2);
title('Test 2: Rudder Input to Yaw Output');
subplot(2,1,2);
plot(time2, rud2,'r');
xlabel('time (s)');
ylabel('rudder input (\mu s)');
xlim(tlim2);


