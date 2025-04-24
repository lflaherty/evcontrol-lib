/**
 * focSpeed.h
 * 
 * Implemented speed based field oriented control.
 * 
 *  Created on: Jan 27 2022
 *      Author: Liam Flaherty
 */

#ifndef _CONTROLLIB_FOCSPEED_H_
#define _CONTROLLIB_FOCSPEED_H_

#include "PMSMFOC.h"
#include "pid.h"

#include <stdbool.h>

typedef struct {
    float rpmReq; // Requested rpm [rpm]
    float rpmMeas; // Measured rpm [rpm]
    bool tqEnable; // Enable torque (false to coast)
    iabc_T iabcSense; // 3ph current [A]
    float wSense; // mechnical angular velocity [rad/s]
    float thetaSense; // mechanical angle [rad]
    float vdcSense; // DC bus voltage [V]
} FOCSpeed_Input_t;

typedef struct {
    DutyCycle_T dutyCycle;
    Vdq_T vdq;
    idq_T idqRef;
    float tqRefSat;
    float tqLim;
    float tqEst;
} FOCSpeed_Output_t;

typedef struct {
  float tqEnableThreshold; // torque threshold for enabling torque output
  FOC_Params_t foc;
  PI_Params_t pi_speed;
} FOCSpeed_Params_t;

typedef struct {
  FOCSpeed_Params_t params;
  FOC_T foc;
  PI_t pi_speed;
} FOCSpeed_t;

void FOCSpeed_Init(FOCSpeed_t* fs, const FOCSpeed_Params_t* params);

void FOCSpeed_Speed(FOCSpeed_t* fs, const FOCSpeed_Input_t* in, FOCSpeed_Output_t* out);


#endif