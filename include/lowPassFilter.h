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
    // Inputs
    float x;

    // Outputs
    float y;

    // Parameters
    float T; // sample time [s]
    float timeConst; // time constant

    // Internal variables
    float alpha;
    float oneMinusAlpha;
    float xPrev;
    float yPrev;
} LowPassFilter_T;

void lowPassFilter_Init(LowPassFilter_T* lpf);

void lowPassFilter_Step(LowPassFilter_T* lpf);


#endif 