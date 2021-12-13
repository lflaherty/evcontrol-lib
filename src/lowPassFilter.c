/**
 * lowPassFilter.c
 * 
 *  Created on: Dec 6 2021
 *      Author: Liam Flaherty
 */

#include "lowPassFilter.h"

void lowPassFilter_Init(LowPassFilter_T* lpf)
{
    lpf->x = 0.0f;
    lpf->y = 0.0f;
    lpf->xPrev = 0.0f;
    lpf->yPrev = 0.0f;

    // pre-compute gain factors
    lpf->alpha = lpf->T / (lpf->timeConst + lpf->T);
    lpf->oneMinusAlpha = 1.0f - lpf->alpha;
}

void lowPassFilter_Step(LowPassFilter_T* lpf)
{
    // calculate the 1st order low pass filter difference equation
    lpf->y = lpf->alpha * lpf->x + lpf->oneMinusAlpha * lpf->yPrev;

    // store memory elements of filter
    lpf->xPrev = lpf->x;
    lpf->yPrev = lpf->y;
}