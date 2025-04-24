/**
 * PMSMCurrentController.c
 *
 *      Author: Liam Flaherty
 */

#include "PMSMCurrentController.h"

#include "constants.h"
#include "sat.h"
#include "transforms.h"
#include <math.h>

void PMSMCurrentControllerInit(PMSMCurrentController_t *controller,
                               const PMSMCurrentController_Params_t *params) {
  piInit(&controller->pi_id, &params->pi_id);
  piInit(&controller->pi_iq, &params->pi_iq);
}

void PMSMCurrentControllerStep(PMSMCurrentController_t *controller,
                               const PMSMCurrentController_Input_t *in,
                               PMSMCurrentController_Output_t *out) {
  idq_T idqMeas;
  parkTransform(&idqMeas, &in->iabcMeas, in->theta_e);

  float vphMax = in->Vdc * ONE_SQRT3;

  // D axis PI controller
  PI_Input_t pi_id_input = (PI_Input_t){
      .measurement = idqMeas.id,
      .setpoint = in->idqRef.id,
      .upperLimit = vphMax,
      .lowerLimit = -vphMax,
  };
  PI_Output_t pi_id_output;
  piStep(&controller->pi_id, &pi_id_input, &pi_id_output);

  // Q axis PI controller
  PI_Input_t pi_iq_input = (PI_Input_t){
      .measurement = idqMeas.iq,
      .setpoint = in->idqRef.iq,
      .upperLimit = vphMax,
      .lowerLimit = -vphMax,
  };
  PI_Output_t pi_iq_output;
  piStep(&controller->pi_iq, &pi_iq_input, &pi_iq_output);

  float vd = pi_id_output.output;
  float vq = pi_iq_output.output;

  // TODO feedforward control

  // voltage limiter
  // Don't need to saturate vq - the PI controller already does this
  float vdLim = sqrtf(powf(vphMax, 2) - powf(vq, 2));
  float vdSat = sat(vd, -vdLim, vdLim);

  out->vdqOut = (Vdq_T){
      .vd = vdSat,
      .vq = vq, // saturated by PI controller
  };
}