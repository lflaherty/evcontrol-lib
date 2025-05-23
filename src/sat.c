/**
 * sat.c
 *
 *      Author: Liam Flaherty
 */

#include "types.h"

float sat(float value, const float lower, const float upper) {
  float newValue = value;

  if (value > upper) {
    newValue = upper;
  } else if (value < lower) {
    newValue = lower;
  }

  return newValue;
}
