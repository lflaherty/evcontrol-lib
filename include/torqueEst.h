/**
 * torqueEst.h
 *
 * Calculates esimated torque
 *
 *      Author: Liam Flaherty
 */

#ifndef _CONTROLLIB_TORQUEEST_H_
#define _CONTROLLIB_TORQUEEST_H_

#include "types.h"
#include <stdint.h>

typedef struct {
  uint16_t polePairs; // Pole pairs
  float fluxLink;     // Flux linkage [Webers]
  float Ld;           // D axis inductance [H]
  float Lq;           // Q axis inducatnce [H]
} TorqueEst_Params_t;

/// Estatime torque based on dq axis currents
float TorqeEst(const TorqueEst_Params_t *params, const idq_T *idq);

#endif // _CONTROLLIB_TORQUEEST_H_
