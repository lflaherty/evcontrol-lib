/**
 * PMSMCurrentRef.h
 *
 * Implements the reference current generation for FOC.
 *
 *      Author: Liam Flaherty
 */

#ifndef _CONTROLLIB_PMSMCURRENTREF_H_
#define _CONTROLLIB_PMSMCURRENTREF_H_

#include "types.h"

#include <stdint.h>

typedef struct {
  float tqRef; // Reference torque [Nm]
  float we;    // Electrical speed [rad/s]
  float Vdc;   // DC bus voltage [V]
} PMSMCurrentRef_Input_t;

typedef struct {
  idq_T idqRef;   // Output idq reference current [A]
  float tqRefSat; // Troque reference (saturated) [Nm]
  float tqLim;    // Torque limit [Nm]
} PMSMCurrentRef_Output_t;

typedef struct {
  float Vnom;                       // Nominal voltage [V]
  float Pmax;                       // Max power [W]
  float Tmax;                       // Max torque [Nm]
  float Imax;                       // Max current [A]
  uint16_t polePairs;               // Pole pairs
  float fluxLink;                   // Flux linkage [Webers]
  float Ld;                         // D axis inductance [H]
  float Ke;                         // back emf constant [V/rpm]
  float Modulation_Index_Threshold; // Field weakening M threshold
  float Modulation_Index_FwMax;     // Field weakening M saturation
} PMSMCurrentRef_Params_t;

/**
 * Initializes the internal fields.
 */
void PMSMCurrentRefInit(void);

/**
 * Performs an update of the PMSM Current Reference generator
 * @param currentRef Current reference data
 */
void PMSMCurrentRefStep(const PMSMCurrentRef_Params_t *params,
                        const PMSMCurrentRef_Input_t *in,
                        PMSMCurrentRef_Output_t *out);

#endif