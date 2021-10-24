/**
 * pid.c
 * 
 *  Created on: Oct 24 2021
 *      Author: Liam Flaherty
 */

#include "pid.h"

#include "sat.h"

void piInit(pi_T* pi)
{
    pi->integrator = 0;
    pi->prevError = 0;
}

void piStep(pi_T* pi, real_T* output, real_T setpoint, real_T measurement)
{
    real_T error = setpoint - measurement;

    // Proportional
    real_T p = pi->Kp * error;

    // Integral
    pi->integrator = pi->integrator + 0.5f * pi->Ki * pi->T * (error + pi->prevError);

    // Integral anti-windup
    pi->integrator = sat(pi->integrator, pi->lowerLimitInt, pi->upperLimitInt);

    // Don't perform derivative...

    // Output and saturation
    *output = sat(p + pi->integrator, pi->lowerLimit, pi->upperLimit);

    pi->prevError = error;
}