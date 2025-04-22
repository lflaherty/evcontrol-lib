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
    // zero inputs
    currentRef->tqRef = 0.0f;
    currentRef->we = 0.0f;
    currentRef->Vdc = 0.0f;

    // zero ouptuts
    currentRef->idqRef.id = 0.0f;
    currentRef->idqRef.iq = 0.0f;
    currentRef->tqRefSat = 0.0f;
    currentRef->tqLim = 0.0f;
}

void PMSMCurrentRefStep(PMSMCurrentRef_T* currentRef)
{
    // Calculate torque limits
    float wMech = currentRef->we / currentRef->polePairs; 
    float speed = fmaxf(fabsf(wMech), currentRef->Pmax / currentRef->Tmax);
    currentRef->tqLim = (currentRef->Vdc / currentRef->Vnom) * fminf(currentRef->Tmax, currentRef->Pmax / speed);
    currentRef->tqRefSat = sat(currentRef->tqRef, -(currentRef->tqLim), currentRef->tqLim);

    // params needed for current ref generation
    float we_rpm = currentRef->we * (30.0f / M_PI) / currentRef->polePairs;
    float back_emf = currentRef->Ke * we_rpm;
    float VphMax = currentRef->Vdc * ONE_SQRT3;
    if (VphMax == 0.0f) {
        VphMax = 0.1f;
    }

    // voltage modulation index
    float M = back_emf / VphMax;

    float iq;
    float id;

    float I_nom = 2.0f * (currentRef->tqRefSat) / (3.0f * currentRef->polePairs * currentRef->fluxLink);
    if (M <= currentRef->Modulation_Index_Threshold) {
        // zero d-axis control (ZDAC)
        id = 0.0f;
        iq = sat(I_nom, -currentRef->Imax, currentRef->Imax);
    } else {
        // Field weakening
        id = -currentRef->Imax *
            (M - currentRef->Modulation_Index_Threshold) /
            (currentRef->Modulation_Index_FwMax - currentRef->Modulation_Index_Threshold);
        id = sat(id, -currentRef->Imax, 0);

        float iq_lim = sqrtf(powf(currentRef->Imax, 2.0f) - powf(id, 2.0f));
        iq = sat(I_nom, -iq_lim, iq_lim);
    }

    currentRef->idqRef.id = id;
    currentRef->idqRef.iq = iq;
}