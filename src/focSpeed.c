/**
 * focSpeed.c
 * 
 *  Created on: Jan 27 2022
 *      Author: Liam Flaherty
 */

#include "focSpeed.h"

#include <string.h>

void FOCSpeed_Init(FOCSpeed_t* fs, const FOCSpeed_Params_t* params)
{
    FOC_Init(&fs->foc, &params->foc);
    piInit(&fs->pi_speed, &params->pi_speed);
}

void FOCSpeed_Speed(FOCSpeed_t* fs, const FOCSpeed_Input_t* in, FOCSpeed_Output_t* out)
{
    PI_Input_t piSpeedInput = (PI_Input_t){
        .setpoint = in->rpmReq,
        .measurement = in->rpmMeas,
        .upperLimit = fs->params.foc.Tmax,
        .lowerLimit = -fs->params.foc.Tmax,
    };
    PI_Output_t piSpeedOutput;
    piStep(&fs->pi_speed, &piSpeedInput, &piSpeedOutput);

    float tqRef = 0.0f;
    if (piSpeedOutput.output > fs->params.tqEnableThreshold) {
        tqRef = fs->params.tqEnableThreshold;
    }

    FOC_Input_t focInput = (FOC_Input_t){
        .tqRef = tqRef,
        .iabcSense = in->iabcSense,
        .wSense = in->wSense,
        .thetaSense = in->thetaSense,
        .vdcSense = in->vdcSense,
    };
    FOC_Output_t focOutput;
    FOC_Step(&fs->foc, &focInput, &focOutput);

    *out = (FOCSpeed_Output_t){
        .dutyCycle = focOutput.dutyCycle,
        .vdq = focOutput.vdq,
        .idqRef = focOutput.idqRef,
        .tqRefSat = focOutput.tqRefSat,
        .tqLim = focOutput.tqLim,
        .tqEst = focOutput.tqEst,
    };
}