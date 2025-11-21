/*
 * Filters.h
 *
 *  Created on: 2020. 11. 15.
 *      Author: HyeonSeong Kim
 */

#ifndef INCLUDE_HSK_FILTERS_H_
#define INCLUDE_HSK_FILTERS_H_



#define MODE_APF    ((int) 0)
#define MODE_LPF    ((int) 1)
#define MODE_HPF    ((int) 2)
#define MODE_BPF    ((int) 3)
#define MODE_BSF    ((int) 4)


typedef struct {
    float In_Old;
    float APF, LPF, HPF;
    float G1, G2, G3;
} Filt_1st; // 1st-order Filter


extern Filt_1st Wrm_LPF, Wrm_HALL_LPF, Vs_LPF;


extern void Init_Filt_1st_BackwardEuler(Filt_1st *F, float W_In, float Tsamp_In);
extern float Upd_Filt_1st_BackwardEuler(Filt_1st *F, float In, int Mode);



#endif /* INCLUDE_FILTERS_H_ */
