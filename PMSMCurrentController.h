/**
 * PMSMCurrentController.h
 * 
 * Implements the Park transform and PI control of id and iq.
 * 
 *  Created on: Oct 24 2021
 *      Author: Liam Flaherty
 */

#ifndef _CONTROLLIB_PMSMCURRENTCONTROLLER_H_
#define _CONTROLLIB_PMSMCURRENTCONTROLLER_H_

#include "types.h"
#include "pid.h"

typedef struct
{
    // Parameters
    real_T T; // sample time

    // Sub-modules
    pi_T pi_iq; // parameters here must be initialized too
    pi_T pi_id; // parameters here must be initialized too
} PMSMCurrentController_T;

/**
 * Initializes the internal fields.
 * Does not update the parameters - these must be manually set.
 * controller->pi parameters must also be set.
 */
void PMSMCurrentControllerInit(PMSMCurrentController_T* controller);

/**
 * Performs an update of the PMSM Current Controller
 */
void PMSMCurrentControllerStep(PMSMCurrentController_T* controller,
                               Vdq_T* vdqOut,
                               const idq_T* idqRef,
                               const iabc_T* iabcMeas,
                               const real_T theta_e,
                               const real_T we,
                               const real_T Vdc);

#endif