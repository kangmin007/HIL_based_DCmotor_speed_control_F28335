/*
 * ePWM.h
 *
 *  Created on: 2020. 11. 15.
 *      Author: HyeonSeong Kim
 */

#ifndef INCLUDE_HSK_EPWM_H_
#define INCLUDE_HSK_EPWM_H_



interrupt void ePWM_INT(void);
void Init_ePWM(void);


extern float Tdead1, Tdead2, Tdead3, Tdead4, Tdead5, Tdead6;



#endif /* INCLUDE_HSK_EPWM_H_ */
