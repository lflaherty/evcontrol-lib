%% Parameters for Emrax 268 based powertrain
% Referencing EMRAX 268 Medium Voltage combined cooling (CC).
% https://emrax.com/wp-content/uploads/2025/03/EMRAX_268_datasheet_v1.6.pdf

%% Machine Parameters
Pmax = 210e3;      % Maximum power                        [W]
Tmax = 500;        % Maximum torque                       [N*m]
Imax = 500;        % Maximum current                      [A]
Ld   = 140e-6;     % Stator d-axis inductance             [H]
Lq   = 140e-6;     % Stator q-axis inductance             [H]
L0   = 0;          % Stator zero-sequence inductance      [H]  not modeled
Rs   = 9.85e-3;    % Stator resistance per phase          [Ohm]
psim = 0.06099;    % Permanent magnet flux linkage        [Wb]
p    = 10;         % Number of pole pairs
Jm   = 0.05769;    % Rotor inertia                        [Kg*m^2]
Ke_ds = 0.07823;   % back emf constant in datasheet units [Vrms/RPM]
Ke   = Ke_ds * sqrt(2); % back emf constant in units we want  [V/RPM]

wmax = inf * pi/30;  % Maximum motor speed [rad/s]

%% High-Voltage Battery Parameters
Cdc  = 255e-6;     % DC-link capacitor  [F]
Vnom = 830;        % Nominal DC voltage [V] 
V1   = 800;        % Voltage V1(< Vnom) [V]

%% Control Parameters
Ts   = 5e-6;        % Fundamental sample time               [s]
fsw  = 15e3;        % PMSM drive switching frequency        [Hz]
Tsi  = 1e-4;        % Sample time for current control loops [s]
Tso  = 1e-4;        % Sample time for current ref generator [s]

Kp_id = 0.8779;     % Proportional gain id controller
Ki_id = 710.3004;   % Integrator gain id controller
Kp_iq = 1.0744;     % Proportional gain iq controller
Ki_iq = 1.0615e+03; % Integrator gain iq controller

Mfw_threshold = 0.95;   % Voltage modulation index where field weakening
                        % begins
Mfw_saturation = 1.25;  % Voltage modulation index where field weakening
                        % should saturate


%% Zero-Cancellation Transfer Functions
numd_id = Tsi/(Kp_id/Ki_id);
dend_id = [1 (Tsi-(Kp_id/Ki_id))/(Kp_id/Ki_id)];
numd_iq = Tsi/(Kp_iq/Ki_iq);
dend_iq = [1 (Tsi-(Kp_iq/Ki_iq))/(Kp_iq/Ki_iq)];
