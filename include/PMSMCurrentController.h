/**
 * PMSMCurrentController.h
 *
 * Implements the Park transform and PI control of id and iq.
 *
 *      Author: Liam Flaherty
 */

#ifndef _CONTROLLIB_PMSMCURRENTCONTROLLER_H_
#define _CONTROLLIB_PMSMCURRENTCONTROLLER_H_

#include "pid.h"
#include "types.h"

typedef struct {
  idq_T idqRef;
  iabc_T iabcMeas;
  float theta_e;
  float we;
  float Vdc;
} PMSMCurrentController_Input_t;

typedef struct {
  Vdq_T vdqOut;
  idq_T idqMeas;
} PMSMCurrentController_Output_t;

typedef struct {
  float T; // sample time
  PI_Params_t pi_id;
  PI_Params_t pi_iq;
} PMSMCurrentController_Params_t;

typedef struct {
  PMSMCurrentController_Params_t params;
  PI_t pi_id;
  PI_t pi_iq;
} PMSMCurrentController_t;

/**
 * Initializes the internal fields.
 * Does not update the parameters - these must be manually set.
 * controller->pi parameters must also be set.
 */
void PMSMCurrentControllerInit(PMSMCurrentController_t *controller,
                               const PMSMCurrentController_Params_t *params);

/**
 * Performs an update of the PMSM Current Controller
 * @param controller data used for input, output, and parameters
 */
void PMSMCurrentControllerStep(PMSMCurrentController_t *controller,
                               const PMSMCurrentController_Input_t *in,
                               PMSMCurrentController_Output_t *out);

#endif