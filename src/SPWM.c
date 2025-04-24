/**
 * SPWM.c
 * 
 *  Created on: Oct 24 2021
 *      Author: Liam Flaherty
 */

#include "SPWM.h"

#include <string.h>
#include "sat.h"
#include "transforms.h"


void spwmStep(const SPWM_Input_t *in, SPWM_Output_t *out)
{
    // Use inverse park transform to obtain the 3 phase voltages to convert to PWM
    Vabc_T vabc;
    inverseParkTransform(&vabc, &in->Vdq, in->theta_e);

    // Convert to duty cycles
    float Vdc = in->Vdc;
    float offset = Vdc/2.0F;
    out->dutyCycles.a = sat((vabc.va + offset)/Vdc, 0.0F, 1.0F);
    out->dutyCycles.b = sat((vabc.vb + offset)/Vdc, 0.0F, 1.0F);
    out->dutyCycles.c = sat((vabc.vc + offset)/Vdc, 0.0F, 1.0F);
}
