/**
 * lowPassFilter.h
 * 
 * Implements a first order low pass filter.
 * 
 *  Created on: Dec 6 2021
 *      Author: Liam Flaherty
 */

#ifndef _CONTROL_LOWPASSFILTER_H_
#define _CONTROL_LOWPASSFILTER_H_

typedef struct
{
    float T; // sample time [s]
    float timeConst; // time constant
} LowPassFilter_Params_t;

typedef struct {
    float alpha;
    float oneMinusAlpha;
    float xPrev;
    float yPrev;
} LowPassFilter_t;

void lowPassFilter_Init(LowPassFilter_t* lpf, const LowPassFilter_Params_t* params);

void lowPassFilter_Step(LowPassFilter_t* lpf, const float x, float *y);

#endif
