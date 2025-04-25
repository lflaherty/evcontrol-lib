# EV Motor Control Library

Implements the necessary components to build a field oriented control system for a PMSM:

* Discrete PI controllers
* Park/Inverse Park transforms
* Sinusoidal PWM (3 phase voltage -> PWM duty cycles)
* Current controller (3 phase -> DQ axis PI controllers)
* PMSM Current reference generators (generates D-Q current references for a requested torque)
* Field weakening
* Field Oriented Control (supervising component that puts all of these components together to implement full FOC system).

An example of simulation output:
![Torque](doc/images/sim_outputs/sim1/torque.png)

## Repo Layout

* `src/`, `include/` C implementation of field oriented control
* `sim/` MATLAB simulation of controlling a motor and vehicle motion
  * `src/` MATLAB script implementation of field oriented control
  * `parameters/` Various parameters for the simulation
    * `motor_emrax268.m` Simulation parameters for the Emrax 268 motor - [https://emrax.com/e-motors/emrax-268/](https://emrax.com/e-motors/emrax-268/)
    * `vehicle.m` Constant parameters used for the vehicle sim
    * `loadOnStartup.m` Invokes the two other params scripts
  * `scripts/` scripts used to assist simulation
  * `models/` Simulink models (more detail below)
* `doc/` supporting images for this readme

## Control Theory

A permanent magnet synchronous motor (PMSM) is constructed using two magnets: a permanent magnet on the rotor, and a three phase electromagnet on the stator. The three stator electromagnets are spaced at 120 degree offsets, so can be used to steer a magnetic field.These two magnetic fields, from the stator and the rotor, interact and produce torque as they attempt to align.

The maximum torque is obtained when the two fields are opposed at 90 degree offsets.

Consider the following three phase system, with the dotted vector showing the current angle of the rotor (0 degrees).

![Three Phase](doc/images/threephase.png)

To exert maximum torque, the stator field should be applied as:

![Three Phase](doc/images/threephase_control.png).

To provide more convenient control, we can perform a park transform (or direct-quadrature-zero transform).

```math
\begin{bmatrix}
d \\
q \\
0
\end{bmatrix}
= \frac{2}{3}
\begin{bmatrix}
\sin{\theta} & \sin{(\theta - \frac{2\pi}{3})} & \sin{(\theta + \frac{2\pi}{3})} \\
\cos{\theta} & \cos{(\theta - \frac{2\pi}{3})} & \cos{(\theta + \frac{2\pi}{3})} \\
\frac{1}{2} & \frac{1}{2} & \frac{1}{2} 
\end{bmatrix}
\begin{bmatrix}
a \\
b \\
c
\end{bmatrix}
```

This converts the three phase vector space into a vector space for direct and quadrature (with respect to the rotor). The reference frame basis vectors are now orthogonal, and so can be controlled independently, which we do via two separate PI controllers.

![Three Phase](doc/images/threephase_dq.png)

And when the rotor rotates, the reference frame for our controller also rotates:

![Three Phase](doc/images/threephase_dq_rotating.png)

In DQ axis, motor torque is given as:
```math
T = \frac{3}{2} p \phi_f I_q + (L_d - L_q) I_d I_q
```
Where $p$ is the number of number of pole pairs, $\phi_f$ is flux linkage, $L_d$ and $L_q$ are D and Q axis inductance respectively, and $I_d$ and $I_q$ are D and Q axis current respectively. 

Zero D Axis Control observes maximum torque output with all of the current directed into the Q axis and none into the 0 axis, which sets our reference generation scheme. Our reference currents are:
```math
i_q = 0 \\\\
i_d = \frac{2 T}{3 p \phi_f}
```

An inverse park transform can be applied to these to obtain the three phase voltage outputs, which can be converted into PWM duty cycles.

The torque output is limited to available current and voltage:
```math
\sqrt{I_d^2 + I_q^2} \le I_{max} \\\\
\sqrt{V_d^2 + V_q^2} \le V_{max}
```
where
```math
V_{max} = \frac{V_{DC}}{\sqrt{3}}
```

The motor will produce a back emf voltage,
```math
v_e = K_e \omega
```
where $K_e$ is the back emf constant, and $\omega$ is the electrical speed of the motor. Eventually, the back emf voltage will reach the phase voltage, at which point the inverter has no remaining voltage to continue to increase the speed.

After this point, we can employ field weakening. By applying a negative $I_d$ current, we can reduce the effective back emf while increasing the speed.

Source: Leopold Sepulchre, Maurice Fadel, Maria Pietrzak-David, Guillaume Porte. New High Speed PMSM
Flux-Weakening Strategy. 19th International Conference on Electrical Machines and Systems (ICEMS
2016), Nov 2016, Chiba, Japan. 6 p. hal-03545045 https://ut3-toulouseinp.hal.science/hal-03545045v1

This can be combined into a control system as follows:

![FOC](doc/images/pmsm_field_oriented_control_block_diagram.png)
Reference: Simulink documentation https://au.mathworks.com/help/sps/ref/pmsmfieldorientedcontrol.html

These components are implemented in `src/` and `include/` (and script versions in `sim/script/`).

## EV Drivetrain Simulation

`sim/` contains a simulink based physical simulation for an EV drivetrain.

The Simulink model supports simulation for:
 * Field oriented controller
 * Arbitrary pedal inputs
 * PMSM motor model
 * IGBT gates
 * Basic vehicle dynamics

### Models

The design flow for the library and simulations was as follows:
1. Build the control system purely in Simulink using pre-existing Simulink components
2. Port each of these components to MATLAB script, replacing the Simulink block components with invocations to these scripts.
3. Bundle each of the components into a complete end-to-end MATLAB script and have one single Simulink block which implements the control system end-to-end
4. Port the components into C, and replace step 2 with S Functions invoking the C implementations.
5. Unify the C implementation into an end-to-end implementation, as in step 3.

As such the following models exist:
| Filename | Model |
| -------- | ----- |
| sim/models/vehiclesim_simblocks.slx | Vehicle simulation model with control system built using Simulink components |
| sim/models/vehiclesim_mscript_blocks.slx | Vehicle simulation model with control system built using separate MATLAB script components |
| sim/models/vehiclesim_mscript.slx | Vehicle simulation model with control system built using end-to-end controller implemented in MATLAB script |
| sim/models/vehiclesim_c_blocks.slx | Vehicle simulation model with control system built using separate components written in C |
| sim/models/vehiclesim_c.slx | Vehicle simulation model with control system built using end-to-end controller implemented in C |
| sim/models/pid_mscript.slx | An experimentation of replacing the Simulink PID block with MATLAB script |

### Model Detail

#### Top Level
The basic simulation is adapted from a Simulink example with the "Inverter Controller" being what this repo aims to implement.

![Top Level Simulation Model](doc/images/simulink_toplevel.png)

#### Controller

Controller block is mostly just an S Function wrapper for the FOC implementation. The remainder is test probes/measurements and small output conversions for the connected simulation.

![Controller](doc/images/simulink_controller.png)

#### HV System
The HV system implements models for:
* Permanent Magnet Synchronous Motor (PMSM)
* HV switching gates (IGBTs)
* DC link capacitor
* HV DC battery
* and various sensors to mimic what sensors a physical system would have available

These are implemented using the Simulink Simscape plugin and are adapted from an example.

![HV Model](doc/images/simulink_hv.png)

#### Vehicle

The simple 1D vehicle dynamics model is also adapted from example. This mainly provides the relation to vehicle speed, and provides a physical load on the motor's rotational dynamics simulation.

![Vehicle Model](doc/images/simulink_vehicle.png)

### Simulation Input

The follow scenarios are applied to the simulation:

#### Simulation 1: Hard Acceleration
This scenario will accelerate long enough to enter field weakening mode.
![Sim Input 1 - Hard Acceleration](doc/images/input_1.png)

#### Simulation 2: Gradual Acceleration and Deceleration
![Sim Input 2 - Gradual Acceleration and Deceleration](doc/images/input_2.png)

#### Simulation 3: Rapid Acceleration, Deceleration, and Repeat
![Sim Input 1 - Rapid Acceleration, Deceleration, and Repeat](doc/images/input_3.png)

### Simulation Results

The results are captured from the `sim/models/vehiclesim_c.slx` implementation, showing the operation of the C implementation in this repo. Though the output of the other models is identical.

#### Simulation 1: Hard Acceleration

In this simulation, we can see that 0-100km/h (0-60mph) takes 1.43 seconds. Field weakening can be seen to begin at t = 1.8 sec.

Due to field weakening, the speed increase drops off and plateaus as expected.

* Driver Inputs ![Driver Inputs](doc/images/sim_outputs/sim1/driver_inputs.png)
* Torque ![Torque](doc/images/sim_outputs/sim1/torque.png)
* Motor RPM ![Motor RPM](doc/images/sim_outputs/sim1/motor_rpm.png)
* Vehicle Speed ![Vehicle Speed](doc/images/sim_outputs/sim1/vehicle_speed.png)
* IQ current ![IQ](doc/images/sim_outputs/sim1/iq.png)
* ID current ![ID](doc/images/sim_outputs/sim1/id.png)

#### Simulation 2: Gradual Acceleration and Deceleration

* Driver Inputs ![Driver Inputs](doc/images/sim_outputs/sim2/driver_inputs.png)
* Torque ![Torque](doc/images/sim_outputs/sim2/torque.png)
* Motor RPM ![Motor RPM](doc/images/sim_outputs/sim2/motor_rpm.png)
* Vehicle Speed ![Vehicle Speed](doc/images/sim_outputs/sim2/vehicle_speed.png)
* IQ current ![IQ](doc/images/sim_outputs/sim2/iq.png)
* ID current ![ID](doc/images/sim_outputs/sim2/id.png)

#### Simulation 3: Rapid Acceleration, Deceleration, and Repeat

* Driver Inputs ![Driver Inputs](doc/images/sim_outputs/sim3/driver_inputs.png)
* Torque ![Torque](doc/images/sim_outputs/sim3/torque.png)
* Motor RPM ![Motor RPM](doc/images/sim_outputs/sim3/motor_rpm.png)
* Vehicle Speed ![Vehicle Speed](doc/images/sim_outputs/sim3/vehicle_speed.png)
* IQ current ![IQ](doc/images/sim_outputs/sim3/iq.png)
* ID current ![ID](doc/images/sim_outputs/sim3/id.png)
