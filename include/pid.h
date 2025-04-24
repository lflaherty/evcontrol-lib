/**
 * pid.h
 *
 * Only PI implemented, as no PID controllers used yet.
 *
 *      Author: Liam Flaherty
 */

#ifndef _CONTROLLIB_PID_H_
#define _CONTROLLIB_PID_H_

#include "types.h"
#include <stdbool.h>

typedef struct {
  float setpoint;
  float measurement;
  float upperLimit; // Output upper saturation
  float lowerLimit; // Output lower saturation
} PI_Input_t;

typedef struct {
  float output;
} PI_Output_t;

typedef struct {
  float T; // sample time
  bool enableOutputSaturation;
  float upperLimitInt; // Upper limit on integrator
  float lowerLimitInt; // Lower limit on integrator
  float Kp;            // Proportional gain
  float Ki;            // integral gain
} PI_Params_t;

typedef struct {
  PI_Params_t params;
  float integrator;
  float prevError;
} PI_t;

/**
 * Initializes the internal fields.
 * Does not update the parameters - these must be manually set.
 */
void piInit(PI_t *pi, const PI_Params_t *params);

/**
 * Performs an update of the PI controller
 * @param PI data used for input, output, and parameters
 */
void piStep(PI_t *pi, const PI_Input_t *in, PI_Output_t *out);

#endif
