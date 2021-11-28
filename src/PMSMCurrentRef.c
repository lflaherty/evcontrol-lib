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
    currentRef->idqRef.id = 0.0f;
    currentRef->idqRef.iq = 0.0f;
    currentRef->tqRefSat = 0.0f;
    currentRef->tqLim = 0.0f;
}

void PMSMCurrentRefStep(PMSMCurrentRef_T* currentRef)
{
    float speed = sat(fabsf(*currentRef->wMech), currentRef->Pmax/currentRef->Tmax, 10e3f); // todo sensible large number
    currentRef->tqLim = (*currentRef->Vdc/currentRef->Vnom) * fminf(currentRef->Tmax, speed);
    currentRef->tqRefSat = sat(*currentRef->tqRef, -(currentRef->tqLim), currentRef->tqLim);

    // zero d-axis control (ZDAC)
    currentRef->idqRef.id = 0.0f;
    currentRef->idqRef.iq = 2.0f * (currentRef->tqRefSat) / (3 * currentRef->polePairs * currentRef->fluxLink);

    // TODO field weakening
}