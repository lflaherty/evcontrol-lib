/**
 * SPWM.h
 *
 *      Author: Liam Flaherty
 */

#ifndef _CONTROLLIB_SPWM_H_
#define _CONTROLLIB_SPWM_H_

#include "types.h"

typedef struct {
  Vdq_T Vdq;
  float theta_e;
  float Vdc;
} SPWM_Input_t;

typedef struct {
  DutyCycle_T dutyCycles;
} SPWM_Output_t;

/**
 * Performs an update of the Sinusoidal PWM
 * @param spwm SPWM data
 */
void spwmStep(const SPWM_Input_t *in, SPWM_Output_t *out);

#endif
