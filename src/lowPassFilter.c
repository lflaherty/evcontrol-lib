/**
 * lowPassFilter.c
 *
 *      Author: Liam Flaherty
 */

#include "lowPassFilter.h"

void lowPassFilter_Init(LowPassFilter_t *lpf,
                        const LowPassFilter_Params_t *params) {
  lpf->xPrev = 0.0f;
  lpf->yPrev = 0.0f;

  // pre-compute gain factors
  lpf->alpha = params->T / (params->timeConst + params->T);
  lpf->oneMinusAlpha = 1.0f - lpf->alpha;
}

void lowPassFilter_Step(LowPassFilter_t *lpf, const float x, float *y) {
  // calculate the 1st order low pass filter difference equation
  *y = lpf->alpha * x + lpf->oneMinusAlpha * lpf->yPrev;

  // store memory elements of filter
  lpf->xPrev = x;
  lpf->yPrev = *y;
}
