/**
 * pid.c
 *
 *      Author: Liam Flaherty
 */

#include "pid.h"

#include "sat.h"
#include <string.h>

void piInit(PI_t *pi, const PI_Params_t *params) {
  pi->integrator = 0;
  pi->prevError = 0;
  memcpy(&pi->params, params, sizeof(PI_Params_t));
}

void piStep(PI_t *pi, const PI_Input_t *in, PI_Output_t *out) {
  const PI_Params_t *params = &pi->params;

  float error = in->setpoint - in->measurement;

  // Proportional
  float p = params->Kp * error;

  // Integral
  pi->integrator =
      pi->integrator + 0.5f * params->Ki * params->T * (error + pi->prevError);

  // Integral anti-windup
  pi->integrator =
      sat(pi->integrator, params->lowerLimitInt, params->upperLimitInt);

  // Don't perform derivative... (PI not PID)

  // Output and saturation
  out->output = p + pi->integrator;
  if (params->enableOutputSaturation) {
    out->output = sat(out->output, in->lowerLimit, in->upperLimit);
  }

  pi->prevError = error;
}