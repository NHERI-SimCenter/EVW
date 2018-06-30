clear 
clc
close all

addpath('WindLoad')


EC = 'B';                       % Exposure Category: A, B, C, or D based on ASCE 7. However, only B, C, and D are currently defined in ASCE 7 as Exposure A was removed since ASCE 7-02 (2002)
V10 = 90;                       % Gust wind speed [mph]
drag = 1.3;                     % Drag Coefficient
Height = 7200;                   % Building Height [inch]
Width = 1200;                     % Building Width [inch]
numStory = 60;                     % Number of Floors
[windspeed_out,windforce_out] = getWind(EC,V10,drag,Height,Width,numStory); % N


rho_bldg = 12/12^3/1000;% k/in3
L = 100*12;% depth, in
V = Height * Width * L;% volume, in3
m_bldg = rho_bldg * V;% weight, k
m = m_bldg / numStory;% floor weight, k
aa=m * (9.8*39.3701)*numStory % weight, klb force, input for the app 2.777954256000000e+07


k = 2811230; %k/s^2  (force)






alpha = 1.0;
beta = 1.0;

zeta = 0.01;

M=eye(numStory)*m; 
M(numStory,numStory) = alpha*m;

K=eye(numStory)*2*k; 
for i=1:numStory-2
    K(i+1,i)=-k;
    K(i,i+1)=-k;
end

K(numStory-1,numStory-1)=k+beta*k;
K(numStory,numStory)=beta*k;
K(numStory-1,numStory)=-beta*k;
K(numStory,numStory-1)=-beta*k;


[eigVectors, eigValues]=eig(K,M);

eigValues=diag(eigValues);
natFrequencies=sqrt(eigValues);% wn
eigVectors;

max(2*pi./natFrequencies) % fundamental periods 


C=zeros(numStory,numStory);
for i=1:numStory
  C=C+2*zeta*natFrequencies(i)*eigVectors(:,i)*eigVectors(:,i)';
end
C = M*C*M;


% EQ 
g = 9.8*39.3701;
ElCentro = load('ElCentro.txt');
ug = g*ElCentro;% kip
P=-M*ones(numStory,1)*ug';
P = [P,zeros(numStory,20000)];

P= P*0.001;% scaling

gamma = 0.5;
beta=0.25;
dt = 0.02;
[u,v,a] = Newmark(M,C,K,zeros(numStory,1),zeros(numStory,1),dt,P,gamma,beta);

% Wind
Pw = 0.00022480894387096*windforce_out;% kip
gamma = 0.5;
beta=0.25;
dtw = 0.1;
[uw,vw,aw] = Newmark(M,C,K,zeros(numStory,1),zeros(numStory,1),dtw,Pw,gamma,beta);






nSteps=size(P,2)-1;
t=[0:dt:nSteps*dt];

nSteps=size(Pw,1)-1;
tw=[0:dtw:nSteps*dtw];


close all

figure('position',[100 300 1800 600])
subplot(2,1,1)
hold on
plot(t,u(numStory,:))
xlabel('Time')
ylabel('Relative displacement at top story (in)')
xlim([0,300])
% ylim([-17,17])
grid
box on
text(250,max(u(numStory,:))*0.8,'Earthquake','FontSize',20)

subplot(2,1,2)
hold on
plot(tw(1:3000),uw(numStory,1:3000))
xlabel('Time')
ylabel('Relative displacement at top story (in)')
% ylim([-17,17])
grid
box on
text(250,max(uw(numStory,:))*0.8,'Wind','FontSize',20)
% plot([0 300],[5 5])
% plot([0 300],[-5 -5])
max(abs(u(numStory,1:3000)))


