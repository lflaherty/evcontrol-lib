/**
 * SPWM.c
 * 
 *  Created on: Oct 24 2021
 *      Author: Liam Flaherty
 */

#include "SPWM.h"

#include "sat.h"
#include "transforms.h"

void spwmStep(DutyCycle_T* dutyCycles, const Vdq_T* Vdq, const real_T theta_e, const real_T Vdc)
{
    // Use inverse park transform to obtain the 3 phase voltages to convert to PWM
    Vabc_T vabc;
    inverseParkTransform(&vabc, Vdq, theta_e);

    // Convert to duty cycles
    real_T offset = Vdc/2.0F;
    dutyCycles->a = sat((vabc.va + offset)/Vdc, 0.0F, 1.0F);
    dutyCycles->b = sat((vabc.vb + offset)/Vdc, 0.0F, 1.0F);
    dutyCycles->c = sat((vabc.vc + offset)/Vdc, 0.0F, 1.0F);
}