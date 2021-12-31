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
    // Inputs
    float tqRef; // Reference torque [Nm]
    float wMech; // Mechanical speed [rad/s]
    float wElec; // Electrical speed [rad/s]
    float Vdc; // DC bus voltage [V]

    // Outputs
    idq_T idqRef; // Output idq reference current [A]
    float tqRefSat; // Troque reference (saturated) [Nm]
    float tqLim; // Torque limit [Nm]

    // Parameters
    float Vnom; // Nominal voltage [V]
    float Pmax; // Max power [W]
    float Tmax; // Max torque [Nm]
    float Imax; // Max current [A]
    float wBase; // Base speed [rad/s]
    uint16_t polePairs; // Pole pairs
    float fluxLink; // Flux linkage [Webers]
    float Ld; // D axis inductance [H]

    // Internal variables
    float weBase; // Electrical base speed
} PMSMCurrentRef_T;

/**
 * Initializes the internal fields.
 */
void PMSMCurrentRefInit(PMSMCurrentRef_T* currentRef);


/**
 * Performs an update of the PMSM Current Reference generator
 * @param currentRef Current reference data
 */
void PMSMCurrentRefStep(PMSMCurrentRef_T* currentRef);

#endif