/**
 * pid.h
 * 
 * Only PI implemented, as no PID controllers used yet.
 * 
 *  Created on: Oct 24 2021
 *      Author: Liam Flaherty
 */

#ifndef _CONTROLLIB_PID_H_
#define _CONTROLLIB_PID_H_

#include "types.h"

typedef struct
{
    // Parameters
    real_T T; // sample time
    real_T upperLimit; // Output upper saturation
    real_T lowerLimit; // Output lower saturation
    real_T upperLimitInt; // Upper limit on integrator
    real_T lowerLimitInt; // Lower limit on integrator
    real_T Kaw; // Anti-windup gain
    real_T Kp; // Proportional gain
    real_T Ki; // integral gain

    // state data
    real_T integrator;
    real_T prevError;
} pi_T;

/**
 * Initializes the internal fields.
 * Does not update the parameters - these must be manually set.
 */
void piInit(pi_T* pi);

/**
 * Performs an update of the PI controller
 * @returns PI controller output
 */
real_T piStep(pi_T* pi, float setpoint, float measurement);

#endif
