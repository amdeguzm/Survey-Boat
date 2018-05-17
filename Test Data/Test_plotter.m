%% Survey Boat Plotter
clear;clc;close all;
file = "Test_data2.xlsx";

% % Test1 counterclockwise
% rud1 = xlsread(file,'Test1_counterclockwise','N:N');
% yaw1 = xlsread(file,'Test1_counterclockwise','I:I');
% 
% % Test2 clockwise 
% rud2 = xlsread(file,'Test2_clockwise','N:N'); 
% yaw2 = xlsread(file,'Test2_clockwise','I:I');
% 
% 
% % Test 3 slow
% thr3 = xlsread(file,'Test3_slow','M:M');
% vel3 = xlsread(file,'Test3_slow','H:H');
% 
% % Test 4 fast
% thr4 = xlsread(file,'Test4_fast','M:M');
% vel4 = xlsread(file,'Test4_fast','H:H');

% % Test 5 medium
% thr5 = xlsread(file,'Test5_medium','M:M');
% vel5 = xlsread(file,'Test5_medium','H:H');

load test.mat
%% Parser

% Test 1 counterclockwise data
rud1(find(rud1<=0)) = []; rud1(find(isnan(rud1))) = [];
yaw1(find(yaw1==0)) = []; yaw1(find(isnan(yaw1))) = [];
t1r = linspace(0,29,length(rud1));
t1y = linspace(0,29,length(yaw1));

% Test 2 clockwise data
rud2(find(rud2<=0)) = []; rud2(find(isnan(rud2))) = [];
yaw2(find(yaw2> 2)) = []; yaw2(find(isnan(yaw2))) = [];
t2r = linspace(0,60,length(rud2));
t2y = linspace(0,60,length(yaw2));

% Test 3 
thr3(find(thr3<=0)) = []; thr3(find(isnan(thr3))) = [];
vel3(find(vel3<=0)) = []; vel3(find(isnan(vel3))) = [];
vel3 = vel3*0.514; %vel in m/s
pos3 = cumsum([0; vel3(2:end)]);
t3t = linspace(0,8,length(thr3));

% Test 4
thr4(find(thr4<=0)) = []; thr4(find(isnan(thr4))) = [];
vel4(find(vel4<=1)) = []; vel4(find(isnan(vel4))) = [];
vel4 = vel4*0.514; %vel in m/s
pos4 = cumsum([0; vel4(2:end)]);
t4t = linspace(0,7,length(thr4));

% Test 5
thr5(find(thr5<=0)) = []; thr5(find(isnan(thr5))) = [];
vel5(find(vel5<=1)) = []; vel5(find(isnan(vel5))) = [];
vel5 = vel5*0.514; %vel in m/s
pos5 = cumsum([0; vel5(2:end)]);
t5t = linspace(0,8,length(thr5));

%% Model 
% G(s) = [(K/tau) + y0]/(s + 1/tau)
Y = mean(yaw2(782:end));
U = mean(rud2(782:end));
K = (Y/U) + (.03/1500);
dY = (1.21- 0.23)/(26.41 - 25.2);
tau = K/dY;
rud2yaw = tf((K/tau),[1 1/tau]);
yaw2sim = lsim(rud2yaw,rud2-1500,t2r);
yaw1sim = lsim(rud2yaw,rud1-1500,t1r);


%% Plotter

% Test 1 counterclockwise plot
figure; 
tlim1 = [0 29]; %choose time interval from 0 to 29
subplot(2,1,1);plot(t1y,yaw1,'r',t1r,yaw1sim,'b');legend('Measured','Simulated');
xlabel('time(s)');ylabel('yaw rate output (rad/s)');xlim(tlim1);
title('Test 1: Rudder Input to Yaw Output');
subplot(2,1,2);plot(t1r, rud1,'r');
xlabel('time (s)');ylabel('rudder input (\mu s)');xlim(tlim1);

% Test2 clockwise plot
figure; 
tlim2 = [0 60]; %choose time interval from 0 to 60
subplot(2,1,1);plot(t2y,yaw2,'r',t2y,yaw2sim,'b');legend('Measured','Simulated');
xlabel('time(s)');ylabel('yaw rate output (rad/s)');xlim(tlim2);
title('Test 2: Rudder Input to Yaw Output');
subplot(2,1,2);plot(t2r, rud2,'r');
xlabel('time (s)');ylabel('rudder input (\mu s)');xlim(tlim2); 


% Test 3 Varying Speed
figure;
subplot(3,1,1);plot(0:6,pos4,'r',0:7,pos5,'g',0:8,pos3,'b');xlabel('time (s)');ylabel('Distance (m)');
title('Test 3: Throttle Input at Varying \mu s');
subplot(3,1,2);plot(0:6,vel4,'r',0:7,vel5,'g',0:8,vel3,'b');xlabel('time (s)');ylabel('Velocity (m/s)');
subplot(3,1,3);plot(t4t,thr4,'r',t5t,thr5,'g',t3t,thr3,'b');xlabel('time(s)');ylabel('throttle input (\mu s)');
legend('mean = 1679 \mu s','mean = 1657 \mu s','mean = 1577 \mu s');

