/**
 * sat.h
 * 
 *  Created on: Oct 24 2021
 *      Author: Liam Flaherty
 */

#ifndef _CONTROLLIB_SAT_H_
#define _CONTROLLIB_SAT_H_

#include "Types.h"

static inline real_T sat(real_T value, const real_T lower, const real_T upper)
{
    real_T newValue = value;

    if (value > upper) {
        newValue = upper;
    } else if (value < lower) {
        newValue = lower;
    }

    return newValue;
}

#endif