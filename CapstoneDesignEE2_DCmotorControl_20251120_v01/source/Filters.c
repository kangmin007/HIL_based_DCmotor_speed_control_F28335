/*
 * Filters.c
 *
 *  Created on: 2020. 11. 15.
 *      Author: HyeonSeong Kim
 */

#include <DSP28x_Project.h>
#include <Filters.h>



Filt_1st Wrm_LPF, Wrm_HALL_LPF, Vs_LPF;


void Init_Filt_1st_BackwardEuler(Filt_1st *F, float W_In, float Tsamp_In);
float Upd_Filt_1st_BackwardEuler(Filt_1st *F, float In, int Mode);


void Init_Filt_1st_BackwardEuler(Filt_1st *F, float W_In, float Tsamp_In)
{
    F->In_Old = 0.;
    F->APF = 0., F->LPF = 0., F->HPF = 0.;
    F->G1 = 1. / (1. + W_In * Tsamp_In);    // 1 / (1 + W*T)
    F->G2 = W_In * Tsamp_In;                // W*T
    F->G3 = 1. - F->G2;                     // 1 - W*T
}

float Upd_Filt_1st_BackwardEuler(Filt_1st *F, float In, int MODE)
{
    switch(MODE) {
        case MODE_APF:
            F->APF = F->G1 * (F->APF + F->G3 * In - F->In_Old);
            F->In_Old = In;
            return(F->APF);
        case MODE_LPF:
            F->LPF = F->G1 * (F->LPF + F->G2 * In);
            return(F->LPF);
        case MODE_HPF:
            F->HPF = F->G1 * (F->HPF + In - F->In_Old);
            F->In_Old = In;
            return(F->HPF);
        default:
            return(In);
    }
}
