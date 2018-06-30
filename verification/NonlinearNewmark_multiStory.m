function [U,V,A,FS] = NonlinearNewmark_multiStory(numStory,M,C,U0,V0,dt,Pt,gamma,beta,...
    structure, algorithm, maxIter, convergenceCheck)
% function to compute response of nonlinear system using Newmark algorithm
%
% Inputs: M  - mass
%         C  - damping coeficient
%         U0 - initial displacement
%         V0 - initial velocity
%         dt - time step, deltaT
%         P  - discretized forcing function, dt time steps P(1) = P0
%         gamma - newmark velocity coeficient
%         beta  - Newmark acceleration coeficient
%         structure -
%         algorithm 'Initial','Modified'or 'Newton'
%         maxIter - max number of iterations per step
%         convergenceCheck - function to evaluate convergence given dU and dR
%
% Outputs: U - displacement vector, discretized dt, U(1) = U0
%          V - velocity vector,     discretized dt, V(1) = V0
%          A - acceleration vector, discretized dt, A(1) = A0
%          Fs - force in structure
%
% written: fmk 09/2016

% Allocation of Returns Values

numDOF = size(M,1);
if (size(Pt,2) == numDOF)
    Pt = Pt';  % in case in SOOF model Pt passed by col!
end;

nSteps=length(Pt);
% numStory = 60;
U=zeros(numStory,nSteps);
V=zeros(numStory,nSteps);
A=zeros(numStory,nSteps);
FS=zeros(numStory,nSteps);

% form tangent if initial stiffness iterations
K = zeros(numStory,numStory);
for j=1:numStory-1
    [k(j,1) Fs(j,1)] = structure{j}.setTrialDisplacement(U0(j));
    K(j,j) = K(j,j) + k(j,1);
    K(j+1,j+1) = K(j+1,j+1) + k(j,1);
    K(j,j+1) = K(j,j+1) - k(j,1);
    K(j+1,j) = K(j,j+1);
end
j = numStory;
[k(j,1) Fs(j,1)] = structure{j}.setTrialDisplacement(U0(j));

if strcmp(algorithm,'Initial')
    Khat = M/(beta*dt^2) + C*gamma/(beta*dt) + K;
end

% set t0 values
U(:,1)=U0;
V(:,1)=V0;
% A(:,1)=(Pt(:,1)-C*V(:,1)-Fs)/M;
A(:,1)=M\(Pt(:,1)-C*V(:,1)-Fs);

% [K Fs] = structure.setTrialDisplacement(U(:,1));
K = zeros(numStory,numStory);
for j=1:numStory-1
    [k(j,1) Fs(j,1)] = structure{j}.setTrialDisplacement(U(j,1));
    K(j,j) = K(j,j) + k(j,1);
    K(j+1,j+1) = K(j+1,j+1) + k(j,1);
    K(j,j+1) = K(j,j+1) - k(j,1);
    K(j+1,j) = K(j,j+1);
end
j = numStory;
[k(j,1) Fs(j,1)] = structure{j}.setTrialDisplacement(U(j,1));

% loop over all time steps computing response
for i=1:nSteps-1
    
    % form tangent if Modified Newton
    if strcmp(algorithm,'Modified')
        Khat = M/(beta*dt^2) + C*gamma/(beta*dt) + K;
    end
    
    % Predict
    U(:,i+1)=U(:,i);
    V(:,i+1)=(1-gamma/beta)*V(:,i)+dt*(1.0-gamma/(2*beta))*A(:,i);
    A(:,i+1)= (-1/(beta*dt))*V(:,i)+(1.0-1.0/(2*beta))*A(:,i);
    dR=Pt(:,i+1)-M*A(:,i+1)-C*V(:,i+1)-Fs;
    
    % iterate until convergence
    for j=1:maxIter
        
        % Correct
        
        % form tangent if Newton-Raphson
        if strcmp(algorithm,'Newton')
            Khat = M/(beta*dt^2) + C*gamma/(beta*dt) + K;
        end
        
        dU=Khat\dR;
        U(:,i+1)=U(:,i+1)+dU;
        V(:,i+1)=V(:,i+1)+gamma/(beta*dt)*dU;
        A(:,i+1)=A(:,i+1)+1.0/(beta*dt^2)*dU;
        %         [K Fs] = structure.setTrialDisplacement(U(:,i+1));
        K = zeros(numStory,numStory);
        for j=1:numStory-1
            [k(j,1) Fs(j,1)] = structure{j}.setTrialDisplacement(U(j,i+1));
            K(j,j) = K(j,j) + k(j,1);
            K(j+1,j+1) = K(j+1,j+1) + k(j,1);
            K(j,j+1) = K(j,j+1) - k(j,1);
            K(j+1,j) = K(j,j+1);
        end
        j = numStory;
        [k(j,1) Fs(j,1)] = structure{j}.setTrialDisplacement(U(j,i+1));
        
        
        dR=Pt(:,i+1)-M*A(:,i+1)-C*V(:,i+1)-Fs;
        ok = convergenceCheck(dU,dR);
        if (ok)
            break;
        end;
    end
    
    % save forces
    FS(:,i+1) = Fs;
    
    if (ok)
        for j=1:numStory
        structure{j}.commitState();
        end
    else % cannot converge
        dU
        disp('cannot converge')
        
        break;
    end
    
end


