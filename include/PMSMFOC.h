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
#include "lowPassFilter.h"

#define FOC_VDC_FILTER_TIME_CONST 0.01f

typedef struct {
    float tqRef; // Torque request [Nm]
    iabc_T iabcSense; // 3ph current [A]
    float wSense; // mechnical angular velocity [rad/s]
    float thetaSense; // mechanical angle [rad]
    float vdcSense; // DC bus voltage [V]
} FOC_Input_t;

typedef struct {
    DutyCycle_T dutyCycle;
    Vdq_T vdq;
    idq_T idqRef;
    float tqRefSat;
    float tqLim;
    float tqEst;
} FOC_Output_t;

typedef struct {
    // Motor parameters
    float Vnom; // Nominal voltage [V]
    float Pmax; // Max power [W]
    float Tmax; // Max torque [Nm]
    float Imax; // Max current [A]
    uint16_t polePairs; // Pole pairs
    float fluxLink; // Flux linkage [Webers]
    float Ld; // D axis inductance [H]
    float Ke; // back emf constant [V/rpm]

    // Control parameters
    float T; // sample time [s]
    float Modulation_Index_Threshold; // Field weakening M threshold
    float Modulation_Index_FwMax; // Field weakening M saturation
    PI_Params_t pi_id;
    PI_Params_t pi_iq;
} FOC_Params_t;

typedef struct
{
    FOC_Params_t params;

    PMSMCurrentRef_Params_t currentRefParams;

    PMSMCurrentController_Params_t currentControllerParams;
    PMSMCurrentController_t currentController;

    LowPassFilter_Params_t voltageFilterParams;
    LowPassFilter_t voltageFilter;
} FOC_T;

void FOC_Init(FOC_T* foc, const FOC_Params_t* params);

void FOC_Step(FOC_T* foc, const FOC_Input_t* in, FOC_Output_t* out);


#endif