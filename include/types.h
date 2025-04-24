/**
 * Types.h
 *
 * Defines common datatypes for control-lib
 *
 *      Author: Liam Flaherty
 */

#ifndef _CONTROLLIB_TYPES_H
#define _CONTROLLIB_TYPES_H

/**
 * Three phase duty cycles
 */
typedef struct {
  float a;
  float b;
  float c;
} DutyCycle_T;

/**
 * Three phase voltage
 */
typedef struct {
  float va;
  float vb;
  float vc;
} Vabc_T;

/**
 * Direct-Quadrature voltage
 */
typedef struct {
  float vd;
  float vq;
} Vdq_T;

/**
 * Three phase current
 */
typedef struct {
  float ia;
  float ib;
  float ic;
} iabc_T;

/**
 * Direct-Quadrature current
 */
typedef struct {
  float id;
  float iq;
} idq_T;

#endif