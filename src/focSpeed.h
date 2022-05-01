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

typedef struct
{
    // inputs
    float rpmReq; // Requested rpm [rpm]
    float rpmMeas; // Measured rpm [rpm]
    bool tqEnable; // Enable torque (false to coast)
    iabc_T iabcSense; // 3ph current [A]
    float wSense; // mechnical angular velocity [rad/s]
    float thetaSense; // mechanical angle [rad]
    float vdcSense; // DC bus voltage [V]

    // outputs
    DutyCycle_T dutyCycle;
    Vdq_T vdq;
    idq_T idqRef;
    float tqRefSat;
    float tqLim;
    float tqEst;
    float rpmBase;

    // params
    float T; // sample time
    float Tmax; // Max torque [Nm]

    // sub-modules (params must also be set)
    FOC_T foc;
    pi_T pi_speed;
} focSpeed_T;

void focSpeed_Init(focSpeed_T* foc);

void focSpeed_Step(focSpeed_T* foc);


#endif