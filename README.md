# EV Control Library

Implements the necessary components to build a field oriented control system for a PMSM:

* Discrete PI controllers
* Park/Inverse Park transforms
* Sinusoidal PWM (takes 3 phase voltage and generates the required PWM duty cycles)
* Current controller (3phase -> DQ axis PI controllers)
* PMSM Current reference generators (generates D-Q current references for a requested torque)
* Field weakening
* Field Oriented Control (supervising component that puts all of these components together to implement FOC).