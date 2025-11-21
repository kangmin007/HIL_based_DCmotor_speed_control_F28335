/*
 * ADC.h
 *
 *  Created on: 2020. 11. 15.
 *      Author: HyeonSeong Kim
 */

#ifndef INCLUDE_HSK_ADC_H_
#define INCLUDE_HSK_ADC_H_



#define ADC_usDELAY  5000L

#define CNT_OFFSET_START    ((unsigned int) 2 * Fsw) // ADC offset calculation starts at 2s after ADC Calculation
#define CNT_OFFSET_END      ((unsigned int) 4 * Fsw) // ADC offset calculation ends at 4s after ADC Calculation
#define Typ_to_DSP_SCALE    ((float) 3.333333)


extern interrupt void ADC_INT(void);
extern interrupt void ADC_Offset_INT(void);
extern void Init_ADC(void);
extern void Calculate_ADC(void);


extern unsigned int ADC_Offset_Cnt;
extern float INV_Offset_Range;
extern float ADC_Offset[];
extern float ADC_Offset_Sum[];
extern float ADC_Scale[];
extern float ADC_Init[];
extern float ADC_Result[];



#endif /* INCLUDE_HSK_ADC_H_ */
