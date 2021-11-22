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


void PMSMCurrentRefStep(PMSMCurrentRef_T* currentRef,
                        idq_T* idqRef,
                        float* tqRefSat,
                        float* tqLim,
                        const float tqRef,
                        const float wMech,
                        const float Vdc)
{
    float speed = sat(fabsf(wMech), currentRef->Pmax/currentRef->Tmax, 10e3f); // todo sensible large number
    *tqLim = (Vdc/currentRef->Vnom) * fminf(currentRef->Tmax, speed);
    *tqRefSat = sat(tqRef, -(*tqLim), *tqLim);

    // zero d-axis control (ZDAC)
    idqRef->id = 0.0f;
    idqRef->iq = 2.0f * (*tqRefSat) / (3 * currentRef->polePairs * currentRef->fluxLink);

    // TODO field weakening
}