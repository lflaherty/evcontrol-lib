/**
 * PMSMFOC.c
 * 
 *  Created on: Nov 27 2021
 *      Author: Liam Flaherty
 */

#include "PMSMFOC.h"

#include <math.h>
#include <string.h>

#include "constants.h"

void FOC_Init(FOC_T* foc, const FOC_Params_t* params)
{
    memcpy(&foc->params, params, sizeof(FOC_Params_t));

    foc->voltageFilterParams = (LowPassFilter_Params_t){
        .T = params->T,
        .timeConst = FOC_VDC_FILTER_TIME_CONST,
    };
    lowPassFilter_Init(&foc->voltageFilter, &foc->voltageFilterParams);

    foc->currentRefParams = (PMSMCurrentRef_Params_t){
        .Vnom = params->Vnom,
        .Pmax = params->Pmax,
        .Tmax = params->Tmax,
        .Imax = params->Imax,
        .polePairs = params->polePairs,
        .fluxLink = params->fluxLink,
        .Ld = params->Ld,
        .Ke = params->Ke,
        .Modulation_Index_Threshold = params->Modulation_Index_Threshold,
        .Modulation_Index_FwMax = params->Modulation_Index_FwMax,
    };
    PMSMCurrentRefInit();

    foc->currentControllerParams = (PMSMCurrentController_Params_t){
        .T = params->T,
        .pi_id = params->pi_id,
        .pi_iq = params->pi_iq,
    };
    PMSMCurrentControllerInit(&foc->currentController, &foc->currentControllerParams);
}

void FOC_Step(FOC_T* foc, const FOC_Input_t* in, FOC_Output_t* out)
{
    // measurements
    float wElec = foc->params.polePairs * in->wSense;
    float theta_e = fmodf(foc->params.polePairs * in->thetaSense, TWO_PI);

    // filter DC bus voltage
    float vdcFilt;
    lowPassFilter_Step(&foc->voltageFilter, in->vdcSense, &vdcFilt);

    // generate idq current references
    PMSMCurrentRef_Input_t currentRefIn = (PMSMCurrentRef_Input_t){
        .tqRef = in->tqRef,
        .we = wElec,
        .Vdc = vdcFilt,
    };
    PMSMCurrentRef_Output_t currentRefOut;
    PMSMCurrentRefStep(&foc->currentRefParams, &currentRefIn, &currentRefOut);

    // control Id and Iq to reference targets
    PMSMCurrentController_Input_t currentControlIn = (PMSMCurrentController_Input_t){
        .idqRef = currentRefOut.idqRef,
        .iabcMeas = in->iabcSense,
        .theta_e = theta_e,
        .we = wElec,
        .Vdc = vdcFilt,
    };
    PMSMCurrentController_Output_t currentControlOut;
    PMSMCurrentControllerStep(&foc->currentController, &currentControlIn, &currentControlOut);

    // convert Vdq to SPWM output
    SPWM_Input_t spwmIn = (SPWM_Input_t){
        .Vdq = currentControlOut.vdqOut,
        .theta_e = theta_e,
        .Vdc = vdcFilt,
    };
    SPWM_Output_t spwmOut;
    spwmStep(&spwmIn, &spwmOut);

    // apply outputs
    *out = (FOC_Output_t){
        .dutyCycle = spwmOut.dutyCycles,
        .vdq = currentControlOut.vdqOut,
        .idqRef = currentRefOut.idqRef,
        .tqRefSat = currentRefOut.tqRefSat,
        .tqLim = currentRefOut.tqLim,
        .tqEst = 0.0f,  // TODO add calculation
    };
}
