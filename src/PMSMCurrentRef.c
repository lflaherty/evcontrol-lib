/**
 * PMSMCurrentRef.c
 * 
 *  Created on: Nov 22 2021
 *      Author: Liam Flaherty
 */

#include "PMSMCurrentRef.h"

#include <math.h>
#include "constants.h"
#include "sat.h"


void PMSMCurrentRefInit(PMSMCurrentRef_T* currentRef)
{
    currentRef->tqRef = 0.0f;
    currentRef->wMech = 0.0f;
    currentRef->wElec = 0.0f;
    currentRef->Vdc = 0.0f;

    currentRef->idqRef.id = 0.0f;
    currentRef->idqRef.iq = 0.0f;
    currentRef->tqRefSat = 0.0f;
    currentRef->tqLim = 0.0f;

    currentRef->weBase = currentRef->polePairs * currentRef->wBase;
}

void PMSMCurrentRefStep(PMSMCurrentRef_T* currentRef)
{
    float speed = sat(fabsf(currentRef->wMech), currentRef->Pmax/currentRef->Tmax, 10e3f); // todo sensible large number
    currentRef->tqLim = (currentRef->Vdc/currentRef->Vnom) * fminf(currentRef->Tmax, speed);
    currentRef->tqRefSat = sat(currentRef->tqRef, -(currentRef->tqLim), currentRef->tqLim);

    if (currentRef->wMech < currentRef->wBase) {
        // zero d-axis control (ZDAC)
        currentRef->idqRef.id = 0.0f;

        currentRef->idqRef.iq = 2.0f * (currentRef->tqRefSat) / (3.0f * currentRef->polePairs * currentRef->fluxLink);
        currentRef->idqRef.iq = sat(currentRef->idqRef.iq, -currentRef->Imax, currentRef->Imax);
    } else {
        // Field weakening
        float id_fw = 2.0f * (currentRef->weBase - currentRef->wElec) * currentRef->fluxLink 
                        / (currentRef->wElec * currentRef->Ld);
        float id = fmaxf(id_fw, -currentRef->Imax);

        float iq_fw = 2.0f * (currentRef->tqRefSat) / (3.0f * currentRef->polePairs * currentRef->fluxLink);
        float iq_lim = sqrtf(currentRef->Imax*currentRef->Imax - id*id);
        float iq = sat(iq_fw, -iq_lim, iq_lim);

        currentRef->idqRef.id = id;
        currentRef->idqRef.iq = iq;
    }
}