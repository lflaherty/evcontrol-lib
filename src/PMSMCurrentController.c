/**
 * PMSMCurrentController.c
 * 
 *  Created on: Oct 24 2021
 *      Author: Liam Flaherty
 */

#include "PMSMCurrentController.h"

#include <math.h>
#include "constants.h"
#include "transforms.h"

void PMSMCurrentControllerInit(PMSMCurrentController_T* controller)
{
    controller->pi_id.T = controller->T; // make sure these are consistent
    controller->pi_iq.T = controller->T;

    piInit(&controller->pi_id);
    piInit(&controller->pi_iq);
}

void PMSMCurrentControllerStep(PMSMCurrentController_T* controller,
                               Vdq_T* vdqOut,
                               const idq_T* idqRef,
                               const iabc_T* iabcMeas,
                               const float theta_e,
                               const float we,
                               const float Vdc)
{
    idq_T idqMeas;
    parkTransform(&idqMeas, iabcMeas, theta_e);

    float vphMax = Vdc * ONE_SQRT3;

    // D axis PI controller
    controller->pi_id.upperLimit = vphMax;
    controller->pi_id.lowerLimit = -vphMax;
    float vd = piStep(&controller->pi_id,  // D axis PID controller
                       idqRef->id,          // Setpoint current
                       idqMeas.id);         // Measured current
    
    // Q axis PI controller
    controller->pi_iq.upperLimit = vphMax;
    controller->pi_iq.lowerLimit = -vphMax;
    float vq = piStep(&controller->pi_iq,  // Q axis PID controller
                       idqRef->iq,          // Setpoint current
                       idqMeas.iq);         // Measured current
    
    // TODO feedforward control

    // voltage limiter
    // Don't need to saturate vq - the PI controller already does this
    float vdLim = sqrtf(vphMax*vphMax - vq*vq);
    float vdSat = sat(vd, -vdLim, vdLim);

    vdqOut->vd = vdSat;
    vdqOut->vq = vq; // saturated by PI controller
}