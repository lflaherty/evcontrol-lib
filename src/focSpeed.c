/**
 * focSpeed.c
 * 
 *  Created on: Jan 27 2022
 *      Author: Liam Flaherty
 */

#include "focSpeed.h"

#include <string.h>

void focSpeed_Init(focSpeed_T* foc)
{
    // initial values
    foc->rpmReq = 0.0f;
    foc->rpmMeas = 0.0f;
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
    foc->foc.T = foc->T;
    foc->foc.currentRef.Tmax = foc->Tmax;
    foc->pi_speed.T = foc->T;

    // constant params
    foc->pi_speed.upperLimit = foc->Tmax;
    foc->pi_speed.lowerLimit = -foc->Tmax;

    FOC_Init(&foc->foc);
    piInit(&foc->pi_speed);
}

void focSpeed_Step(focSpeed_T* foc)
{
    pi_T* pi_speed = &foc->pi_speed;
    pi_speed->setpoint = foc->rpmReq;
    pi_speed->measurement = foc->rpmMeas;
    piStep(pi_speed);

    float tqRef = 0.0f;
    if (true == foc->tqEnable) {
        tqRef = pi_speed->output;
    }
    
    foc->foc.tqRef = tqRef;
    foc->foc.iabcSense = foc->iabcSense;
    foc->foc.wSense = foc->wSense;
    foc->foc.thetaSense = foc->thetaSense;
    foc->foc.vdcSense = foc->vdcSense;
    FOC_Step(&foc->foc);

    foc->dutyCycle = foc->foc.dutyCycle;
    foc->vdq = foc->foc.vdq;
    foc->idqRef = foc->foc.idqRef;
    foc->tqRefSat = foc->foc.tqRefSat;
    foc->tqLim = foc->foc.tqLim;
    foc->tqEst = foc->foc.tqEst;
    foc->rpmBase = foc->foc.rpmBase;
}