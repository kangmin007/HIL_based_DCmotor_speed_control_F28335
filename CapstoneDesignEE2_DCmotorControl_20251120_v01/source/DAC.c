/*
 * DAC.c
 *
 *  Created on: 2024. 01. 29.
 *      Author: HyeonSeong Kim
 */

#include <DSP28x_Project.h>
#include <eQEP.h>
#include <Variables.h>
#include <Control_Source.h>
#include <DAC.h>



void delaycc(float time);
void Init_DAC(void);
void Select_DA_variable(void);
void spi_xmit(Uint16 ch, Uint16 data);


unsigned short cnt_delay;
float DA_Delay = 0.;
float* DA_Value[4] = {&SPMSM1.Wrpm, &SPMSM1.Idsr, &SPMSM1.Iqsr, &SPMSM1.Thetar};
float DA_Ratio[4] = {0., 0., 0., 0.};
float DA_Offset[4] = {0., 0., 0., 0.};
float DA[4] = {0., 0., 0., 0.};
Uint16 DAC[4] = {0, 0, 0, 0};


void delaycc(float time) {
    cnt_delay = (unsigned short)(time*Fsys);
    asm(" RPT @_cnt_delay   || NOP ");
    asm(" NOP       ");
}


void Init_DAC(void)
{
    // SPI 설정
    // 1. GPIO 설정
    EALLOW;
    GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0; // pull-up (SPISIMO-A)
    GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0; // pull-up (SPISOMI-A)
    GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0; // pull-up (SPICLK-A)

    GpioCtrlRegs.GPAQSEL2.bit.GPIO16 = 3; // Asynch input (SPISIMO-A)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3; // Asynch input (SPISOMI-A)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 3; // Asynch input (SPICLK-A)
//    GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3; // Asynch input (SPISTEA)

    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 1; // GPIO16 as SPISIMO-A
    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 1; // GPIO17 as SPISOMI-A
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 1; // GPIO18 as SPICLK-A


    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 1; // GPIO19 as GPIO19 (but we will use this GPIO19 for SPISTEA)
    GpioCtrlRegs.GPADIR.bit.GPIO19 = 1; // output
    GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0; // pull-up (SPISTE-A)
    GpioDataRegs.GPASET.bit.GPIO19 = 1; // initial high


    EDIS;

    // 2. 기본 레지스터 설정
    EALLOW;
    SpiaRegs.SPICCR.bit.SPISWRESET = 0;

    SpiaRegs.SPICCR.bit.SPICHAR = 15; // 16bit frame
    SpiaRegs.SPICTL.bit.MASTER_SLAVE = 1; // Master
    SpiaRegs.SPICTL.bit.TALK = 1;
    SpiaRegs.SPIBRR = 14; // 통신속도 37.5MHz/(14+1) = 2.2MHz
    SpiaRegs.SPICCR.bit.CLKPOLARITY = 0;
    SpiaRegs.SPICTL.bit.CLK_PHASE = 1;
    SpiaRegs.SPIPRI.bit.FREE = 1;

    SpiaRegs.SPICCR.bit.SPISWRESET = 1;

    PieCtrlRegs.PIEIER6.bit.INTx1 = 1;
    IER |= M_INT6;
    EDIS;

}


void Select_DA_variable(void)
{
}


void spi_xmit(Uint16 ch, Uint16 data)
{
}
