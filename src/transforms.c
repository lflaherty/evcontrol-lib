/**
 * transforms.c
 * 
 *  Created on: Oct 24 2021
 *      Author: Liam Flaherty
 */

#include "transforms.h"

#include <math.h>
#include "constants.h"

void parkTransform(idq_T* idq, const iabc_T* iabc, const float theta_e)
{
    idq->id = sinf(theta_e)*iabc->ia 
                + sinf(theta_e - TWO_PI_3)*iabc->ib 
                + sinf(theta_e + TWO_PI_3)*iabc->ic;
    idq->id *= 0.666667F;

    idq->iq = cosf(theta_e)*iabc->ia 
                + cosf(theta_e - TWO_PI_3)*iabc->ib 
                + cosf(theta_e + TWO_PI_3)*iabc->ic;
    idq->iq *= 0.666667F;
}

void inverseParkTransform(Vabc_T* Vabc, const Vdq_T* vdq, const float theta_e)
{
    Vabc->va = sin(theta_e)*vdq->vd 
                + cos(theta_e)*vdq->vq;

    Vabc->vb = sin(theta_e - TWO_PI_3)*vdq->vd
                + cos(theta_e - TWO_PI_3)*vdq->vq;
    
    Vabc->vc = sin(theta_e + TWO_PI_3)*vdq->vd
                + cos(theta_e + TWO_PI_3)*vdq->vq;
}