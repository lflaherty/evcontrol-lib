/**
 * PMSMCurrentRef.h
 * 
 * Implements the reference current generation for FOC.
 * 
 *  Created on: Nov 22 2021
 *      Author: Liam Flaherty
 */

#ifndef _CONTROLLIB_PMSMCURRENTREF_H_
#define _CONTROLLIB_PMSMCURRENTREF_H_

#include "types.h"

#include <stdint.h>

typedef struct
{
    float Vnom; // Nominal voltage [V]
    float Pmax; // Max power [W]
    float Tmax; // Max torque [Nm]
    uint16_t polePairs; // Pole pairs
    float fluxLink; // Flux linkage [Webers]
} PMSMCurrentRef_T;


/**
 * Performs an update of the PMSM Current Reference generator
 * @param currentRef Current reference data
 * @param idqRef output idq reference current (out) [A]
 * @param tqRefSat Torque reference (saturated) (out) [Nm]
 * @param tqLim Torque limits (out) [Nm]
 * @param tqRef Reference torque [Nm]
 * @param wMech Mechanical speed [rad/s]
 * @param Vdc DC bus voltage [V]
 */
void PMSMCurrentRefStep(PMSMCurrentRef_T* currentRef,
                        idq_T* idqRef,
                        float* tqRefSat,
                        float* tqLim,
                        const float tqRef,
                        const float wMech,
                        const float Vdc);

#endif