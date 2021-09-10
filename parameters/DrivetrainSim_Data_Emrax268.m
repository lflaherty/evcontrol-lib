%% Parameters for Axle-Drive Electric Vehicle Example

% This example shows an interior permanent magnet synchronous machine
% (IPMSM) propelling a simplified axle-drive electric vehicle. A high-
% voltage battery feeds the IPMSM through a controlled three-phase
% converter. The IPMSM operates in both motoring and generating modes.
% The vehicle transmission and differential are implemented using a
% fixed-ratio gear reduction model. The Vehicle Controller subsystem
% converts the driver inputs into a relevant torque command. The
% EM Controller subsystem controls the torque of the IPMSM. The controller
% includes a multi-rate PI-based control structure. The rate of the open-
% loop torque control is slower than the rate of the closed-loop current 
% control. The task scheduling for the controller is implemented as a 
% Stateflow(R) state machine. The Scopes subsystem contains scopes 
% that allow you to see the simulation results.

% Copyright 2016 The MathWorks, Inc.

%% Machine Parameters
Pmax = 200e3;      % Maximum power                   [W]
Tmax = 500;        % Maximum torque                  [N*m]
Ld   = 292e-6;     % Stator d-axis inductance        [H]
Lq   = 272e-6;     % Stator q-axis inductance        [H]
L0   = 0.00012184; % Stator zero-sequence inductance [H]
Rs   = 0.026;      % Stator resistance per phase     [Ohm]
psim = 0.1014;     % Permanent magnet flux linkage   [Wb]
p    = 10;         % Number of pole pairs
Jm   = 0.0922;     % Rotor inertia                   [Kg*m^2]

wmax = inf;

%% High-Voltage Battery Parameters
Cdc  = 0.001;      % DC-link capacitor  [F]
Vnom = 650;        % Nominal DC voltage [V] 
V1   = 625;        % Voltage V1(< Vnom) [V]

%% Control Parameters
Ts   = 1e-5;        % Fundamental sample time               [s]
fsw  = 15e3;        % PMSM drive switching frequency        [Hz]
Tsi  = 1e-4;        % Sample time for current control loops [s]
Tso  = 1e-4;        % Sample time for current ref generator [s]

Kp_id = 0.8779;     % Proportional gain id controller
Ki_id = 710.3004;   % Integrator gain id controller
Kp_iq = 1.0744;     % Proportional gain iq controller
Ki_iq = 1.0615e+03; % Integrator gain iq controller

%% Zero-Cancellation Transfer Functions
numd_id = Tsi/(Kp_id/Ki_id);
dend_id = [1 (Tsi-(Kp_id/Ki_id))/(Kp_id/Ki_id)];
numd_iq = Tsi/(Kp_iq/Ki_iq);
dend_iq = [1 (Tsi-(Kp_iq/Ki_iq))/(Kp_iq/Ki_iq)];

%% Current References
%load pe_ipmsm_35kW_ref_idq;
genRefLut

%% Vehicle Parameters
Mv    = 250;   % Vehicle mass                   [kg]
g     = 9.8;    % Gravitational acceleration     [m/s^2]
rho_a = 1.2;    % Air density                    [kg/m^3]
AL    = 0.9;    % Max vehicle cross section area [m^2]
Cd    = 0.4;    % Air drag coefficient           [N*s^2/kg*m]
cr1   = 0.1;    % Rolling coefficient            
cr2   = 0.2;    % Rolling coefficient            
i_t   = 2.3;    % Gear reduction ratio           
Rw    = 0.3;    % Wheel radius                   [m]
