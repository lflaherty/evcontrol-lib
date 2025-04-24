/**
 * torqueEst.c
 *
 *      Author: Liam Flaherty
 */

#include "torqueEst.h"

float TorqeEst(const TorqueEst_Params_t *params, const idq_T *idq) {
  return (3.0f * params->polePairs / 2.0f) *
         (params->fluxLink * idq->iq +
          (params->Ld - params->Lq) * idq->id * idq->iq);
}
