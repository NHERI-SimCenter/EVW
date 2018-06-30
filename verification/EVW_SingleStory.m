clear
clc
close all

% building properties
EC = 'B';                       % Exposure Category: A, B, C, or D based on ASCE 7. However, only B, C, and D are currently defined in ASCE 7 as Exposure A was removed since ASCE 7-02 (2002)
V10 = 90;                       % Gust wind speed [mph]
drag = 1.3;                     % Drag Coefficient
Height = 7200;                   % Building Height [inch]
Width = 1200;                     % Building Width [inch]
numStory = 1;                     % Number of Floors
m = 72000; % weight, klb
m * (9.8*39.3701) % weight, klb force, input for the app

% EQ loads
load ElCentro.txt;
P=-(9.8*39.3701)*m*ElCentro*0.001; % k * in / s^2
P = [P;zeros(1+300/0.02-max(size(P)),1)];
dt = 0.02;

% Wind loads
[windspeed_out,windforce_out] = getWind(EC,V10,drag,Height,Width,numStory); % N


Ts=[.5 1.0 2.0 5.0];
for i = 4

% calculate building parameters    
Tn = Ts(i);
wn = 2*pi/Tn;
k = m*wn^2; % k/s^2


zeta = 0.01;% damping ratio
c=2*zeta*m*wn;% damping coefficient


% linear analysis
gamma=0.5;
beta=0.25;
[u,v,a]=Newmark(m,c,k,0,0,dt,P,gamma,beta);
U0 = max(abs(u))

PW = 0.00022480894387096*windforce_out;%kip
gamma=0.5;
beta=0.25;
dtw = 0.1;
[uw,vw,aw]=Newmark(m,c,k,0,0,dtw,PW,gamma,beta);
U0 = max(abs(uw))

% plot results of linear analysis
nSteps=size(P)-1;
t=[0:dt:nSteps*dt];

nSteps=size(PW)-1;
tw=[0:dtw:nSteps*dtw];


figure('position',[100 300 1800 600])
subplot(2,1,1)
hold on
plot(t,u)
xlabel('Time')
ylabel('Relative displacement(in)')
xlim([0,300])
% ylim([-17,17])
grid
box on
text(250,max(u)*0.8,'Earthquake','FontSize',20)

subplot(2,1,2)
hold on
plot(tw(1:3000),uw(numStory,1:3000))
xlabel('Time')
ylabel('Relative displacement (in)')
grid
box on
text(250,max(uw)-max(abs(uw))*0.2,'Wind','FontSize',20)
max(abs(u(numStory,1:3000)))


% non-linear analysis
U0 = max(abs(u));
F0 = U0*k;
check = @(dU,dR)abs(dR)<1e-12
spring0=ElasticPPSpring(k,30);
[u,v,a,fs]=NonlinearNewmark(m,c,0,0,dt,P,gamma,beta, ...
			    spring0,'Newton',5,check);
            

gamma=0.5;
beta=0.25;
dtw = 0.1;
spring0=ElasticPPSpring(k,30);
[uw,vw,aw,fsw]=NonlinearNewmark(m,c,0,0,dtw,PW,gamma,beta, ...
			    spring0,'Newton',5,check);



% plot results of non-linear analysis
figure('position',[100 300 1800 600])
subplot(2,1,1)
hold on
plot(t,u)
xlabel('Time')
ylabel('Relative displacement(in)')
xlim([0,300])
grid
box on
text(250,max(u)*0.8,'Earthquake','FontSize',20)

subplot(2,1,2)
hold on
plot(tw(1:3000),uw(numStory,1:3000))
xlabel('Time')
ylabel('Relative displacement (in)')
grid
box on
text(250,max(uw)-max(abs(uw))*0.2,'Wind','FontSize',20)
max(abs(u(numStory,1:3000)))


end


