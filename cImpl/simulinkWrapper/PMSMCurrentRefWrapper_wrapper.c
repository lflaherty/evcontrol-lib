
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
#include "../evcontrol-lib/src/PMSMCurrentRef.h"
#include "../evcontrol-lib/src/sat.h"
/* %%%-SFUNWIZ_wrapper_includes_Changes_END --- EDIT HERE TO _BEGIN */
#define u_width 1
#define y_width 1

/*
 * Create external references here.  
 *
 */
/* %%%-SFUNWIZ_wrapper_externs_Changes_BEGIN --- EDIT HERE TO _END */
/* extern double func(double a); */
static PMSMCurrentRef_T pmsmCurrentRef;
/* %%%-SFUNWIZ_wrapper_externs_Changes_END --- EDIT HERE TO _BEGIN */

/*
 * Start function
 *
 */
void PMSMCurrentRefWrapper_Start_wrapper(const real_T *T, const int_T p_width0,
			const real_T *Vnom, const int_T p_width1,
			const real_T *Pmax, const int_T p_width2,
			const real_T *Tmax, const int_T p_width3,
			const real_T *p, const int_T p_width4,
			const real_T *psim, const int_T p_width5)
{
/* %%%-SFUNWIZ_wrapper_Start_Changes_BEGIN --- EDIT HERE TO _END */
/*
 * Custom Start code goes here.
 */

pmsmCurrentRef.Vnom = Vnom[0];
pmsmCurrentRef.Pmax = Pmax[0];
pmsmCurrentRef.Tmax = Tmax[0];
pmsmCurrentRef.polePairs = p[0];
pmsmCurrentRef.fluxLink = psim[0];

/* %%%-SFUNWIZ_wrapper_Start_Changes_END --- EDIT HERE TO _BEGIN */
}
/*
 * Output function
 *
 */
void PMSMCurrentRefWrapper_Outputs_wrapper(const real_T *TqRef,
			const real_T *wMech,
			const real_T *Vdc,
			real_T *idqRef,
			real_T *TqRefSat,
			real_T *TqLim,
			const real_T *T, const int_T p_width0,
			const real_T *Vnom, const int_T p_width1,
			const real_T *Pmax, const int_T p_width2,
			const real_T *Tmax, const int_T p_width3,
			const real_T *p, const int_T p_width4,
			const real_T *psim, const int_T p_width5)
{
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_BEGIN --- EDIT HERE TO _END */
/* This sample sets the output equal to the input
      y0[0] = u0[0]; 
 For complex signals use: y0[0].re = u0[0].re; 
      y0[0].im = u0[0].im;
      y1[0].re = u1[0].re;
      y1[0].im = u1[0].im;
 */


// get output
idq_T idqRefI;
float tqRefSatI;
float tqRefLimI;
PMSMCurrentRefStep(&pmsmCurrentRef,
        &idqRefI,
        &tqRefSatI,
        &tqRefLimI,
        TqRef[0],
        wMech[0],
        Vdc[0]);

idqRef[0] = idqRefI.id;
idqRef[1] = idqRefI.iq;
TqRefSat[0] = tqRefSatI;
TqLim[0] = tqRefLimI;

/* %%%-SFUNWIZ_wrapper_Outputs_Changes_END --- EDIT HERE TO _BEGIN */
}


