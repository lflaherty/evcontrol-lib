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

void FOC_Init(FOC_T* foc)
{
    // initial values
    foc->tqRef = 0.0f;
    foc->wSense = 0.0f;
    foc->thetaSense = 0.0f;
    foc->vdcSense = 0.0f;
    memset(&foc->iabcSense, 0, sizeof(iabc_T));

    memset(&foc->dutyCycle, 0, sizeof(DutyCycle_T));
    memset(&foc->vdq, 0, sizeof(Vdq_T));
    memset(&foc->idqRef, 0, sizeof(idq_T));
    foc->tqRefSat = 0.0f;
    foc->tqLim = 0.0f;
    foc->tqEst = 0.0f;
    foc->rpmBase = 0.0f;

    // consistent params
    foc->controller.T = foc->T;
    foc->currentRef.polePairs = foc->polePairs;

    PMSMCurrentRefInit(&foc->currentRef);
    PMSMCurrentControllerInit(&foc->controller);
    spwmInit(&foc->spwm);
}

void FOC_Step(FOC_T* foc)
{
    // measurements
    float wElec = foc->polePairs * foc->wSense;
    float theta_e = fmodf(foc->polePairs * foc->thetaSense, TWO_PI);

    foc->currentRef.tqRef = foc->tqRef;
    foc->currentRef.wMech = foc->wSense;
    foc->currentRef.Vdc = foc->vdcSense;
    PMSMCurrentRefStep(&foc->currentRef);

    foc->controller.idqRef = foc->currentRef.idqRef;
    foc->controller.iabcMeas = foc->iabcSense;
    foc->controller.theta_e = theta_e;
    foc->controller.we = wElec;
    foc->controller.Vdc = foc->vdcSense;
    PMSMCurrentControllerStep(&foc->controller);

    foc->spwm.Vdq = foc->controller.vdqOut;
    foc->spwm.theta_e = theta_e;
    foc->spwm.Vdc = foc->vdcSense;
    spwmStep(&foc->spwm);

    // outputs
    foc->dutyCycle = foc->spwm.dutyCycles;
    foc->vdq = foc->controller.vdqOut;
    foc->idqRef = foc->controller.idqRef;
    foc->tqRefSat = foc->currentRef.tqRefSat;
    foc->tqLim = foc->currentRef.tqLim;
    foc->tqEst = 0.0f; // TODO
    foc->rpmBase = 0.0f; // TODO
}