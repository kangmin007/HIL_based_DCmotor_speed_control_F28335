/*
 * ADC.c
 *
 *  Created on: 2020. 11. 15.
 *      Author: HyeonSeong Kim
 */

#include <math.h>
#include <DSP28x_Project.h>
#include <ePWM.h>
#include <Variables.h>
#include <Control_Source.h>
#include <ADC.h>



interrupt void ADC_INT(void);
interrupt void ADC_Offset_INT(void);
void Init_ADC(void);
void Calculate_ADC(void);


unsigned int ADC_Offset_Cnt = 0;
float INV_Offset_Range = 0.;
float ADC_Offset[16] = {0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.};
float ADC_Offset_Sum[16] = {0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.};
float ADC_Scale[16] = {0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.};
float ADC_Init[16] = {0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.};
float ADC_Result[16] = {0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.};


void Init_ADC(void)
{
    EALLOW;

    SysCtrlRegs.HISPCP.all = 0x3; // HSPCLK = SYSCLKOUT/(2*HISPCP) = 25MHz
    AdcRegs.ADCTRL3.bit.ADCCLKPS = 1;
    AdcRegs.ADCTRL1.bit.CPS = 0;
    AdcRegs.ADCTRL1.bit.ACQ_PS = 14;



    AdcRegs.ADCTRL1.bit.SEQ_CASC = 1;
    AdcRegs.ADCMAXCONV.bit.MAX_CONV1 = 1;
    AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0; // step0 : A0 채널 변환
    AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 1; // step1 : A1 채널 변환

    AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = 1;
    AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;
    AdcRegs.ADCTRL2.bit.INT_MOD_SEQ1 = 0;

    AdcRegs.ADCTRL3.bit.ADCBGRFDN = 0x3;        // ADC band-gap 및 reference 회로의 전원 설정. {0 = 전원 차단, 3 = 전원 입력}
    AdcRegs.ADCTRL3.bit.ADCPWDN = 0x1;          // ADC Analog 회로의 전원 설정. {0 = band-gap과 reference 회로를 제외한 analog 회로의 전원 차단, 1 = 전원 입력}


    /*
    AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;
    */


    PieVectTable.ADCINT = &ADC_INT;


    EDIS;
}


interrupt void ADC_INT(void)
{
    //DINT;

    GpioDataRegs.GPADAT.bit.GPIO13 = 1;

    ADC_Result[0] = (float) (AdcRegs.ADCRESULT0 >> 4);
    ADC_Result[1] = (float) (AdcRegs.ADCRESULT1 >> 4);



    AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;

    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
    PieCtrlRegs.PIEIER3.bit.INTx1 = 1; // Enable ePWM Interrupt (Disable after the ePWM Interrupt)

    GpioDataRegs.GPADAT.bit.GPIO13 = 0;

    //EINT;
}


interrupt void ADC_Offset_INT(void) // Calculate average offset from 2 [sec.] to 4 [sec.] after ADC Start
{
}


void Calculate_ADC(void)
{
}
