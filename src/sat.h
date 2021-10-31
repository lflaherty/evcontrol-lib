/**
 * sat.h
 * 
 *  Created on: Oct 24 2021
 *      Author: Liam Flaherty
 */

#ifndef _CONTROLLIB_SAT_H_
#define _CONTROLLIB_SAT_H_

#include "Types.h"

static inline float sat(float value, const float lower, const float upper)
{
    float newValue = value;

    if (value > upper) {
        newValue = upper;
    } else if (value < lower) {
        newValue = lower;
    }

    return newValue;
}

#endif