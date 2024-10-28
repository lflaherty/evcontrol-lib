%% Parameters for Emrax 268 based powertrain
%% Machine Parameters
Pmax = 230e3;      % Maximum power                   [W]
Tmax = 500;        % Maximum torque                  [N*m]
Imax = 565;        % Maximum current                 [A]
Ld   = 292e-6;     % Stator d-axis inductance        [H]
Lq   = 272e-6;     % Stator q-axis inductance        [H]
L0   = 0.00012184; % Stator zero-sequence inductance [H]
Rs   = 0.026;      % Stator resistance per phase     [Ohm]
psim = 0.1014;     % Permanent magnet flux linkage   [Wb]
p    = 10;         % Number of pole pairs
Jm   = 0.0922;     % Rotor inertia                   [Kg*m^2]

wmax = inf * pi/30;  % Maximum motor speed [rad/s]
wbase = 2000 * pi/30; % Base speed [rad/s]

%% High-Voltage Battery Parameters
Cdc  = 255e-6;     % DC-link capacitor  [F]
Vnom = 650;        % Nominal DC voltage [V] 
V1   = 625;        % Voltage V1(< Vnom) [V]

%% Control Parameters
Ts   = 5e-6;        % Fundamental sample time               [s]
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
%genRefLut