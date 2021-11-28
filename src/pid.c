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

void piStep(pi_T* pi)
{
    float error = pi->setpoint - pi->measurement;

    // Proportional
    float p = pi->Kp * error;

    // Integral
    pi->integrator = pi->integrator + 0.5f * pi->Ki * pi->T * (error + pi->prevError);

    // Integral anti-windup
    pi->integrator = sat(pi->integrator, pi->lowerLimitInt, pi->upperLimitInt);

    // Don't perform derivative... (PI not PID)

    // Output and saturation
    pi->output = sat(p + pi->integrator, pi->lowerLimit, pi->upperLimit);

    pi->prevError = error;
}