/**
 * SPWM.c
 * 
 *  Created on: Oct 24 2021
 *      Author: Liam Flaherty
 */

#include "SPWM.h"

#include "sat.h"
#include "transforms.h"


void spwmInit(SPWM_T* spwm)
{
    spwm->Vdq.vd = 0.0f;
    spwm->Vdq.vq = 0.0f;
    spwm->theta_e = 0.0f;
    spwm->Vdc = 0.0f;

    spwm->dutyCycles.a = 0.0f;
    spwm->dutyCycles.b = 0.0f;
    spwm->dutyCycles.c = 0.0f;
}

void spwmStep(SPWM_T* spwm)
{
    // Use inverse park transform to obtain the 3 phase voltages to convert to PWM
    Vabc_T vabc;
    inverseParkTransform(&vabc, &spwm->Vdq, spwm->theta_e);

    // Convert to duty cycles
    float Vdc = spwm->Vdc;
    float offset = Vdc/2.0F;
    spwm->dutyCycles.a = sat((vabc.va + offset)/Vdc, 0.0F, 1.0F);
    spwm->dutyCycles.b = sat((vabc.vb + offset)/Vdc, 0.0F, 1.0F);
    spwm->dutyCycles.c = sat((vabc.vc + offset)/Vdc, 0.0F, 1.0F);
}