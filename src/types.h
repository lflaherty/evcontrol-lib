/**
 * Types.h
 * 
 * Defines common datatypes for control-lib
 * 
 *  Created on: Oct 24 2021
 *      Author: Liam Flaherty
 */

#ifndef _CONTROLLIB_TYPES_H
#define _CONTROLLIB_TYPES_H

/**
 * Use real_T for any data variable.
 * This typedef allows the underlying data type to be changed
 * (e.g. between single/double precision, or a fixed point integer) 
 */
typedef float real_T;

/**
 * Three phase duty cycles
 */
typedef struct
{
    real_T a;
    real_T b;
    real_T c;
} DutyCycle_T;

/**
 * Three phase voltage
 */
typedef struct
{
    real_T va;
    real_T vb;
    real_T vc;
} Vabc_T;


/**
 * Direct-Quadrature voltage
 */
typedef struct 
{
    real_T vd;
    real_T vq;
} Vdq_T;

/**
 * Three phase current
 */
typedef struct
{
    real_T ia;
    real_T ib;
    real_T ic;
} iabc_T;

/**
 * Direct-Quadrature current
 */
typedef struct
{
    real_T id;
    real_T iq;
} idq_T;




#endif