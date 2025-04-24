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


void PMSMCurrentRefInit(void)
{
    // empty
}

void PMSMCurrentRefStep(const PMSMCurrentRef_Params_t* params,
                        const PMSMCurrentRef_Input_t* in,
                        PMSMCurrentRef_Output_t* out)
{
    // Calculate torque limits
    float wMech = in->we / params->polePairs; 
    float speed = fmaxf(fabsf(wMech), params->Pmax / params->Tmax);
    out->tqLim = (in->Vdc / params->Vnom) * fminf(params->Tmax, params->Pmax / speed);
    out->tqRefSat = sat(in->tqRef, -(out->tqLim), out->tqLim);

    // params needed for current ref generation
    float we_rpm = in->we * (30.0f / M_PI) / params->polePairs;
    float back_emf = params->Ke * we_rpm;
    float VphMax = in->Vdc * ONE_SQRT3;
    if (VphMax == 0.0f) {
        VphMax = 0.1f;
    }

    // voltage modulation index
    float M = back_emf / VphMax;

    float iq;
    float id;

    float I_nom = 2.0f * (out->tqRefSat) / (3.0f * params->polePairs * params->fluxLink);
    if (M <= params->Modulation_Index_Threshold) {
        // zero d-axis control (ZDAC)
        id = 0.0f;
        iq = sat(I_nom, -(params->Imax), params->Imax);
    } else {
        // Field weakening
        id = -params->Imax *
            (M - params->Modulation_Index_Threshold) /
            (params->Modulation_Index_FwMax - params->Modulation_Index_Threshold);
        id = sat(id, -params->Imax, 0);

        float iq_lim = sqrtf(powf(params->Imax, 2.0f) - powf(id, 2.0f));
        iq = sat(I_nom, -iq_lim, iq_lim);
    }

    out->idqRef = (idq_T){
        .id = id,
        .iq = iq,
    };
}