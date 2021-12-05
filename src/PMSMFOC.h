/**
 * PMSMFOC.h
 * 
 * Implements Field Oriented Control for a PMSM
 * 
 *  Created on: Nov 27 2021
 *      Author: Liam Flaherty
 */

#ifndef _CONTROLLIB_PMSMFOC_H_
#define _CONTROLLIB_PMSMFOC_H_

#include "PMSMCurrentRef.h"
#include "PMSMCurrentController.h"
#include "SPWM.h"

typedef struct
{
    // inputs
    float tqRef; // Torque request [Nm]
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
    float T; // sample time [s]
    uint16_t polePairs;

    // sub-modules (params must also be set)
    PMSMCurrentRef_T currentRef;
    PMSMCurrentController_T controller;
    SPWM_T spwm;
} FOC_T;

void FOC_Init(FOC_T* foc);

void FOC_Step(FOC_T* foc);


#endif