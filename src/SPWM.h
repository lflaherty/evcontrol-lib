/**
 * SPWM.h
 * 
 *  Created on: Oct 24 2021
 *      Author: Liam Flaherty
 */

#ifndef _CONTROLLIB_SPWM_H_
#define _CONTROLLIB_SPWM_H_

#include "types.h"

void spwmStep(DutyCycle_T* dutyCycles, const Vdq_T* Vdq, const float theta_e, const float Vdc);

#endif
