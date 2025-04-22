/**
 * SPWM.h
 * 
 *  Created on: Oct 24 2021
 *      Author: Liam Flaherty
 */

#ifndef _CONTROLLIB_SPWM_H_
#define _CONTROLLIB_SPWM_H_

#include "types.h"

typedef struct
{
    // Input
    Vdq_T Vdq;
    float theta_e;
    float Vdc;

    // Output
    DutyCycle_T dutyCycles;
} SPWM_T;

/**
 * Initializes the internal fields.
 */
void spwmInit(SPWM_T* spwm);

/**
 * Performs an update of the Sinusoidal PWM
 * @param spwm SPWM data
 */
void spwmStep(SPWM_T* spwm);

#endif
