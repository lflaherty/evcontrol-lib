
/*
 * Include Files
 *
 */
#if defined(MATLAB_MEX_FILE)
#include "tmwtypes.h"
#include "simstruc_types.h"
#else
#include "rtwtypes.h"
#endif



/* %%%-SFUNWIZ_wrapper_includes_Changes_BEGIN --- EDIT HERE TO _END */
#include <math.h>
#include <stdio.h>
#include "../evcontrol-lib/src/types.h"
#include "../evcontrol-lib/src/constants.h"
#include "../evcontrol-lib/src/PMSMCurrentController.h"
#include "../evcontrol-lib/src/pid.h"
#include "../evcontrol-lib/src/sat.h"
/* %%%-SFUNWIZ_wrapper_includes_Changes_END --- EDIT HERE TO _BEGIN */
#define u_width 2
#define y_width 1

/*
 * Create external references here.  
 *
 */
/* %%%-SFUNWIZ_wrapper_externs_Changes_BEGIN --- EDIT HERE TO _END */
/* extern double func(double a); */
static PMSMCurrentController_T pmsmCurrentController;
/* %%%-SFUNWIZ_wrapper_externs_Changes_END --- EDIT HERE TO _BEGIN */

/*
 * Start function
 *
 */
void PMSMCurrentControllerWrapper_Start_wrapper(const real_T *T, const int_T p_width0,
			const real_T *Kp_id, const int_T p_width1,
			const real_T *Ki_id, const int_T p_width2,
			const real_T *Kp_iq, const int_T p_width3,
			const real_T *Ki_iq, const int_T p_width4)
{
/* %%%-SFUNWIZ_wrapper_Start_Changes_BEGIN --- EDIT HERE TO _END */
/*
 * Custom Start code goes here.
 */

pmsmCurrentController.T = *T;

pmsmCurrentController.pi_id.upperLimit = 10e3f;
pmsmCurrentController.pi_id.lowerLimit = -10e3f;
pmsmCurrentController.pi_id.upperLimitInt = 10e3f;
pmsmCurrentController.pi_id.lowerLimitInt = -10e3f;
pmsmCurrentController.pi_id.Kp = Kp_id[0];
pmsmCurrentController.pi_id.Ki = Ki_id[0];

pmsmCurrentController.pi_iq.upperLimit = 10e3f;
pmsmCurrentController.pi_iq.lowerLimit = -10e3f;
pmsmCurrentController.pi_iq.upperLimitInt = 10e3f;
pmsmCurrentController.pi_iq.lowerLimitInt = -10e3f;
pmsmCurrentController.pi_iq.Kp = Kp_iq[0];
pmsmCurrentController.pi_iq.Ki = Ki_iq[0];

PMSMCurrentControllerInit(&pmsmCurrentController);
/* %%%-SFUNWIZ_wrapper_Start_Changes_END --- EDIT HERE TO _BEGIN */
}
/*
 * Output function
 *
 */
void PMSMCurrentControllerWrapper_Outputs_wrapper(const real_T *idqRef,
			const real_T *iabc,
			const real_T *theta,
			const real_T *we,
			const real_T *Vdc,
			real_T *vdq,
			real_T *idq,
			const real_T *T, const int_T p_width0,
			const real_T *Kp_id, const int_T p_width1,
			const real_T *Ki_id, const int_T p_width2,
			const real_T *Kp_iq, const int_T p_width3,
			const real_T *Ki_iq, const int_T p_width4)
{
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_BEGIN --- EDIT HERE TO _END */
/* This sample sets the output equal to the input
      y0[0] = u0[0]; 
 For complex signals use: y0[0].re = u0[0].re; 
      y0[0].im = u0[0].im;
      y1[0].re = u1[0].re;
      y1[0].im = u1[0].im;
 */

idq_T idqI;
idqI.id = idqRef[0];
idqI.iq = idqRef[1];

iabc_T iabcI;
iabcI.ia = iabc[0];
iabcI.ib = iabc[1];
iabcI.ic = iabc[2];

// get output
Vdq_T vdqOut;
PMSMCurrentControllerStep(&pmsmCurrentController,
        &vdqOut,
        &idqI,
        &iabcI,
        theta[0],
        we[0],
        Vdc[0]);

vdq[0] = vdqOut.vd;
vdq[1] = vdqOut.vq;
idq[0] = 0.0f;
idq[1] = 0.0f;
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_END --- EDIT HERE TO _BEGIN */
}


