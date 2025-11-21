/*
 * DAC.h
 *
 *  Created on: 2020. 11. 24.
 *      Author: Hyeon-Seong Kim
 */

#ifndef INCLUDE_HSK_DAC_H_
#define INCLUDE_HSK_DAC_H_



#define CH1 ((int) 0)
#define CH2 ((int) 1)
#define CH3 ((int) 2)
#define CH4 ((int) 3)


extern void InitXintf(void);
extern void delaycc(float time);
extern void Init_DAC(void);
extern void Update_DAC(void);
extern void spi_xmit(Uint16 data);


extern unsigned short cnt_delay;
extern float DA_Delay;
extern float* DA_Value[];
extern float DA_Ratio[];
extern float DA_Offset[];
extern Uint16 DAC[];



#endif /* INCLUDE_HSK_DAC_H_ */
