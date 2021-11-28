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
#include "sat.h"

void PMSMCurrentControllerInit(PMSMCurrentController_T* controller)
{
    controller->vdqOut.vd = 0.0f;
    controller->vdqOut.vq = 0.0f;

    controller->pi_id.T = controller->T; // make sure these are consistent
    controller->pi_iq.T = controller->T;

    piInit(&controller->pi_id);
    piInit(&controller->pi_iq);
}

void PMSMCurrentControllerStep(PMSMCurrentController_T* controller)
{
    idq_T idqMeas;
    parkTransform(&idqMeas, controller->iabcMeas, *controller->theta_e);

    float vphMax = *controller->Vdc * ONE_SQRT3;

    // D axis PI controller
    pi_T* pi_id = &controller->pi_id;
    pi_id->upperLimit = vphMax;
    pi_id->lowerLimit = -vphMax;
    pi_id->setpoint = controller->idqRef->id;
    pi_id->measurement = idqMeas.id;
    piStep(pi_id);
    float vd = pi_id->output;
    
    // Q axis PI controller
    pi_T* pi_iq = &controller->pi_iq;
    pi_iq->upperLimit = vphMax;
    pi_iq->lowerLimit = -vphMax;
    pi_iq->setpoint = controller->idqRef->iq;
    pi_iq->measurement = idqMeas.iq;
    piStep(pi_iq);
    float vq = pi_iq->output;
    
    // TODO feedforward control

    // voltage limiter
    // Don't need to saturate vq - the PI controller already does this
    float vdLim = sqrtf(vphMax*vphMax - vq*vq);
    float vdSat = sat(vd, -vdLim, vdLim);

    controller->vdqOut.vd = vdSat;
    controller->vdqOut.vq = vq; // saturated by PI controller
}