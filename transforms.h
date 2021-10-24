/**
 * transforms.h
 * 
 *  Created on: Oct 24 2021
 *      Author: Liam Flaherty
 */

#ifndef _CONTROLLIB_TRANSFORMS_H_
#define _CONTROLLIB_TRANSFORMS_H_

#include "types.h"

void parkTransform(idq_T* idq, const iabc_T* iabc, const real_T theta_e);
void inverseParkTransform(Vabc_T* Vabc, const Vdq_T* vdq, const real_T theta_e);

#endif
