% Discrete frequency function with Cholesky decomposition and FFT coded by

% Dae Kun Kwon, Ph.D
% Research Assistant Professor
% NatHaz Modeling Laboratory
% Department of Civil & Environmental Engineering and Earth Sciences
% University of Notre Dame
% Notre Dame, IN 46556, USA

% Reference
% Wittig, L. E. and Sinha, A. K. (1975). "Simulation of multicorrelated random processes using the FFT algorithm." The Journal of the Acoustical Society of America, 58(3), 630-633. 

%
% inputs and outputs
%

% clear all;
% close all;
function [windspeed_out,windforce_out] = getWind(EC,V10,drag,Height,Width,Nfloor)

tic;    % check execution time

% Input parameters: units in English such as inch (length related variables) and mph (mile per hour for Gust wind speed)
% Please note that the inputs are made in English units but the outputs are calculated as SI units
% EC = 'B';                       % Exposure Category: A, B, C, or D based on ASCE 7. However, only B, C, and D are currently defined in ASCE 7 as Exposure A was removed since ASCE 7-02 (2002)
% V10 = 90;                       % Gust wind speed [mph]
% drag = 1.3;                     % Drag Coefficient
% Height = 7200;                   % Building Height [inch]
% Width = 1200;                     % Building Width [inch]
% Nfloor = 60;                     % Number of Floors

%
% Call main function
% Outputs: (N is currently set to 3000 for 10-min simulation)
% windspeed_out: wind speed  - [(dt*2*N) X Nfloor] matrix
% windforce_out: wind force matrix [(dt*2*N) X Nfloor] matrix
%
% Note that currently, it is set to: time interval (dt) = 0.1 sec, total time (T) = 600 sec
% thus, [6000 X Nfloor] matrices for wind speed and force are simulated
% If needing T = 3600 sec (1-hour), then change variable N value in the main code (windsim_dk1_main.m) to N = 18000

[windspeed_out,windforce_out] = windsim_dk1_main(EC,V10,drag,Height,Width,Nfloor);

%%% Please keep in mind that output units are SI as follows
% windspeed_out = wind speeds, m/s
% windforce_out = wind force, Newton (N)

% Accordingly, if necessary, those should be converted to English unit (e.g., m/s to mph & Newton to kip)


% Test plot for dt=0.1, T=600 showing fluctuating wind speed and force at top
% dt=0.1;
% T=300;
% figure(1)
% plot(dt:dt:T,windspeed_out(:,Nfloor))
% xlabel('Time [sec]')
% ylabel('Fluctuating wind speed at building top [m/s]')
% 
% figure(2)
% plot(dt:dt:T,windforce_out(:,Nfloor))
% xlabel('Time [sec]')
% ylabel('Fluctuating wind force at building top [N]')


toc;    % check execution time
