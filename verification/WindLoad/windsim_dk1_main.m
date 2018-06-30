% Discrete frequency function with Cholesky decomposition and FFT coded by

% Dae Kun Kwon, Ph.D
% Research Assistant Professor
% NatHaz Modeling Laboratory
% Department of Civil & Environmental Engineering and Earth Sciences
% University of Notre Dame
% Notre Dame, IN 46556, USA

% Reference
% Wittig, L. E. and Sinha, A. K. (1975). "Simulation of multicorrelated random processes using the FFT algorithm." The Journal of the Acoustical Society of America, 58(3), 630-633. 

function [windspeed_out,windforce_out] = windsim_dk1_main(EC,V10,drag,Height,Width,Nfloor)

%% Input parameters: units in English such as inch (length related variables) and mph (mile per hour for Gust wind speed)
%% Please note that the inputs are made in English units but the outputs are calculated as SI units
%EC = 'B';                       % Exposure Category: A, B, C, or D based on ASCE 7. However, only B, C, and D are currently defined as Exposure A was removed since ASCE 7-02 (2002)
%V10 = 90;                       % Gust wind speed [mph]
%drag = 1.3;                     % Drag Coefficient
%Height = 720;                   % Building Height [inch]
%Width = 40;                     % Building Width [inch]
%Nfloor = 5;                     % Number of Floors

%%%% convert English to Metric
%%%% this main code is run based on SI units
Height=Height.*0.0254;  % inch to m
Width=Width.*0.0254;    % inch to m
V10=V10*0.44704;        % mph to m/s

% calculate vertical height per node
z=(Height/Nfloor:Height/Nfloor:Height);    % inches

N=3000;                   % with fc, generating 5 min (300 sec) winds (N=18000 for 1-hr simulation)
fc=5;                     % cut-off frequency
nt=2*N;

n=(1:N)/N*fc;                 % frequency range
dt=1/2/fc;
T=dt*nt;                      % total time
samp= 1;			%leave it
mnk = 1;            %leave it

% Exposure category
if(EC == 'A')                 
    z0=2;                       % roughness height
    bbb=0.30;                   % factor for power law
    alpha=1/3;                  % exponent of power law

elseif(EC == 'B')
    z0=0.3;                     % roughness height
    bbb=0.45;                   % factor for power law
    alpha=1/4;                  % exponent of power law

elseif(EC == 'C')
    z0=0.02;                    % roughness height
    bbb=0.65;                   % factor for power law
    alpha=1/6.5;                % exponent of power law

elseif(EC == 'D')
    z0=0.005;                   % roughness height
    bbb=0.80;                   % factor for power law
    alpha=1/9;                  % exponent of power law
end   % if(EC == ...)

k=0.4;                        % Von Karman's constant
Cz=10;                        % Coefficient for coherence function, set to 10 in this code

% Calculate wind speeds and friction velocity according to heights
  VV = V10 .* bbb .* (z./10).^alpha;
  fricV = V10 * bbb * k / log(10/z0);   % Friction velocity u*

zsize=size(z,2);
fsize=size(n,2);              % be careful not to use size(n) because n is matrix, anyway
nc=zsize^2-sum(1:zsize);      % total number of upper triangle matrix components, if 5x5, nc=10

windspeed_out=zeros(nt*samp,zsize);          % simulated wind velocity
windforce_out=zeros(nt*samp,zsize);     % simulated wind force
White=zeros(N,zsize);
G=zeros(zsize,zsize);
X=zeros(N,zsize);
XW=zeros(2*N,zsize);
distb=zeros(N*samp,zsize*2);
Rel=zeros(N,zsize);
Im=zeros(N,zsize);

% Calculate wind speeds and friction velocity according to heights
  VV = V10 .* bbb .* (z./10).^alpha;
  fricV = V10 * bbb * k / log(10/z0);   % Friction velocity u*

%   randn('seed',sum(100*clock));
   randn('seed',100);
  distb = randn(N*samp,zsize*2); 
  Rel = distb(:,1:2:2*zsize-1).*sqrt(.5);
  Im =distb(:,2:2:2*zsize).*sqrt(-.5);

			%%%%%%%%%%%%%%% loop %%%%%%%%%%%%%%
  for i=1:zsize;	
    	White(1:N,i)=Rel(1:N,i)+Im(1:N,i);  
  end


for jj=1:N,
        G=diag(200 .*fricV.^2.*z./VV./((1+50.*n(jj).*z./VV).^(5/3)));
% Calculate off-diagonal terms
    k=1;
    for ii=1:zsize, 
      for jjj=(ii+1):zsize,
         G(ii,jjj) = sqrt(G(ii,ii)*G(jjj,jjj))*exp(-Cz*n(jj) * (abs(z(ii)-z(jjj))) / (0.5*(VV(ii)+VV(jjj))))*0.999;
         G1(ii,jjj) = exp(-Cz*n(jj) * (abs(z(ii)-z(jjj))) / (0.5*(VV(ii)+VV(jjj))));
         k=k+1;
      end
    end
    
    G=G'+G-diag(diag(G));
    Cholesky = (chol(G))';
    X(jj,1:zsize) = N*(2*fc/N)^0.5*(Cholesky*White(jj,1:zsize)')'; 
end	% end of jj=1:N

for k=1:zsize
	XW(2:N+1,k)=X(:,k);
	XW(N+2:2*N,k)=conj(X(N-1:-1:1,k));
	XW(N+1,k)=abs(X(N,k));
	windspeed_out((mnk-1)*nt+1:mnk*nt,k)=real(ifft(XW(:,k)));  
end

%%% calculate fluctuating/dynamic wind force, windforce_out: [unit in Newton, N]
rho=1.226;                    % air density  kg/m^3
for ii=1:zsize,
    windforce_out(:,ii) = rho*VV(ii)*windspeed_out(:,ii)*drag*(Height/Nfloor)*Width;
end

%%% Output variables and their units
%windspeed_out = wind speeds, m/s
%windforce_out = wind force, Newton (N)

%figure(1)
%plot(dt:dt:T,windspeed_out(:,1))
%
