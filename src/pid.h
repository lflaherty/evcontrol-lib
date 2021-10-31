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
    float T; // sample time
    float upperLimit; // Output upper saturation
    float lowerLimit; // Output lower saturation
    float upperLimitInt; // Upper limit on integrator
    float lowerLimitInt; // Lower limit on integrator
    float Kaw; // Anti-windup gain
    float Kp; // Proportional gain
    float Ki; // integral gain

    // state data
    float integrator;
    float prevError;
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
float piStep(pi_T* pi, float setpoint, float measurement);

#endif
