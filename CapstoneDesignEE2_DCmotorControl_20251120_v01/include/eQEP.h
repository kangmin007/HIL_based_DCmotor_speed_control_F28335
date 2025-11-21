/*
 * eQEP.h
 *
 *  Created on: 2020. 11. 15.
 *      Author: HyeonSeong Kim
 */

#ifndef INCLUDE_HSK_EQEP_H_
#define INCLUDE_HSK_EQEP_H_



// ENCODER
#define PPR         ((int) 2000)        // Encoder pulses per revolution
#define INV_PPR     ((float) 0.0005)    // 1 / PPR
#define EPRD        ((int) 1500)        // Unit Timer for 100 kHz at 150 MHz SYSCLKOUT.


extern void Init_eQEP(void);
extern void Encoder(void);


extern int EncCnt;
extern float Theta_ENC;



#endif /* INCLUDE_HSK_EQEP_H_ */
