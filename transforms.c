/**
 * transforms.c
 * 
 *  Created on: Oct 24 2021
 *      Author: Liam Flaherty
 */

#include "transforms.h"

void parkTransform(idq_T* idq, const iabc_T* iabc, const real_T theta_e)
{
    idq->id = 0;
    idq->iq = 0;
    // TODO implement
}

void inverseParkTransform(Vabc_T* Vabc, const Vdq_T* vdq, const real_T theta_e)
{
    Vabc->va = 0;
    Vabc->vb = 0;
    Vabc->vc = 0;
    // TODO implement
}