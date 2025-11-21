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
void Update_DAC(void);
void spi_xmit(Uint16 data);


unsigned short cnt_delay;
float DA_Delay = 0.;
float* DA_Value[4] = {&DCmotor1.WrpmRef, &DCmotor1.Wrpm, &DCmotor1.Duty_a, &DCmotor1.Ia};
float DA_Ratio[4] = {0., 0., 0., 0.};
float DA_Offset[4] = {0., 0., 0., 0.};
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


    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 0; // GPIO19 as GPIO19 (but we will use this GPIO19 for SPISTEA)
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

    // WrpmRef: -3000rpm ~ 3000rpm(Wrpm_rated) => -3.3V ~ 3.3V
    DA_Ratio[0] = 3.3 / 3000.;
    DA_Offset[0] = 0.;

    // Wrpm: -3000rpm ~ 3000rpm(Wrpm_rated) => -3.3V ~ 3.3V
    DA_Ratio[1] = 3.3 / 3000.;
    DA_Offset[1] = 0.;

    // Duty_a: 0 ~ 1 => -3.3V ~ 3.3V
    DA_Ratio[2] = 6.6;
    DA_Offset[2] = -3.3;

    // Ia: -25A ~ 25A(Is_rated) => -3.3V ~ 3.3V
    DA_Ratio[3] = 3.3 / 25.;
    DA_Offset[3] = 0.;


    // CONTROL bit 매핑 (16비트 중 MSB 부터 LD1, LD0, Sel1, Sel0)
    DAC[0] = 0x8000; // channel A
    DAC[1] = 0x9000; // channel B
    DAC[2] = 0xA000; // channel C
    DAC[3] = 0xB000; // channel D

}


void Update_DAC(void)
{
    Uint16 ch_idx; // channel index
    float dac_analog_val; // -3.3V ~ 3.3V로 매핑된 DAC 아날로그 전압 값


    for(ch_idx = 0; ch_idx < 4; ch_idx++)
    {
        // 물리량 => -3.3V ~ 3.3V로 매핑
        dac_analog_val = SAT(DA_Ratio[ch_idx] * (*DA_Value[ch_idx]) + DA_Offset[ch_idx], -3.3, 3.3);

        // -3.3V ~ 3.3V => 0 ~ 4095로 매핑
        // 12비트 data bit 채우기
        DAC[ch_idx] |= ((Uint16)((dac_analog_val + 3.3) * (.5 * INV_3p3) * 4095.)) & 0x0FFF;

        spi_xmit(DAC[ch_idx]);

        DAC[ch_idx] &= 0xF000; // 다시 control bit만 남기기
    }


}


void spi_xmit(Uint16 data)
{
    GpioDataRegs.GPACLEAR.bit.GPIO19 = 1;
    while (SpiaRegs.SPISTS.bit.BUFFULL_FLAG == 1) {}
    SpiaRegs.SPITXBUF = data;

    while (SpiaRegs.SPISTS.bit.INT_FLAG == 0) {}
    (void) SpiaRegs.SPIRXBUF;
    GpioDataRegs.GPASET.bit.GPIO19 = 1;
}
