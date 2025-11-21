/*
 * ePWM.c
 *
 *  Created on: 2020. 11. 15.
 *      Author: HyeonSeong Kim
 */

#include <DSP28x_Project.h>
#include <Variables.h>
#include <Control_Source.h>
#include <ePWM.h>



void Init_ePWM(void)
{
    Uint16 tbprd = 3750;    // f_pwm = 20kHz (up-down mode -> 7500 tick per cycle -> 100us per cycle in 75MHz)
    Uint16 db_tick = 23;    // 23tick * 13.333ns = 300ns

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;   // 타이머 정지
    EDIS;


    EALLOW;
    // ===== ePWM1 : left leg (ePWM1A - upper, ePWM1B - lower) =====
    // 1. TB 모듈
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
    EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV2;    // TBCLK = SYSCLKOUT / (HSPCLKDIV * CLKDIV) = 75MHz
    EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;   // TBCLK = SYSCLKOUT / (HSPCLKDIV * CLKDIV) = 75 MHz
    EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO; // CTR=0에서 SYNCOUT 발생
    EPwm1Regs.TBCTL.bit.FREE_SOFT = 2;

    EPwm1Regs.TBPRD = tbprd;
    EPwm1Regs.TBCTR = 0;

    // 2. CC 모듈
    EPwm1Regs.CMPA.half.CMPA = 0; // 초기 CMPA = 0
    EPwm1Regs.CMPCTL.bit.SHDWAMODE = 0; // CMPA : shadow 모드
    EPwm1Regs.CMPCTL.bit.LOADAMODE = 1; // CTR=PRD에서 CMPA 변경

    // 3. DB 모듈
    EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
    EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
    EPwm1Regs.DBCTL.bit.IN_MODE = DBA_ALL;
    EPwm1Regs.DBRED = db_tick;
    EPwm1Regs.DBFED = db_tick;

    // 4. ET 모듈 (TBPRD 마다 전류 샘플링 SOCA 발생)
    EPwm1Regs.ETSEL.bit.SOCAEN = 1;
    EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO;
    EPwm1Regs.ETPS.bit.SOCAPRD = 1; // 매회 발생
    EPwm1Regs.ETSEL.bit.INTEN = 1;
    EPwm1Regs.ETSEL.bit.INTSEL = 1;
    EPwm1Regs.ETPS.bit.INTPRD = 1;

    PieVectTable.EPWM1_INT = &ePWM_INT;

    // 5. AQ 모듈
    EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;
    EPwm1Regs.AQCTLA.bit.CAD = AQ_SET;


    // ===== ePWM2 : right leg (ePWM2A - upper, ePWM2B - lower)
    // 1. TB 모듈
    EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
    EPwm2Regs.TBCTL.bit.PHSEN = TB_ENABLE;
    EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV2;
    EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;

    EPwm2Regs.TBPHS.half.TBPHS = 0; // ePWM1과 위상 동일하게
    EPwm2Regs.TBPRD = tbprd;
    EPwm2Regs.TBCTR = 0;
    EPwm2Regs.TBCTL.bit.FREE_SOFT = 2;


    // 2. CC 모듈
    EPwm2Regs.CMPB = 0; // 초기 CMPB = 0
    EPwm2Regs.CMPCTL.bit.SHDWBMODE = 0; // CMPB : shadow 모드
    EPwm2Regs.CMPCTL.bit.LOADBMODE = 1; // CTR=PRD에서 CMPB 변경

    // 3. DB 모듈
    EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
    EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
    EPwm2Regs.DBCTL.bit.IN_MODE = DBA_ALL;
    EPwm2Regs.DBRED = db_tick;
    EPwm2Regs.DBFED = db_tick;

    // 4. AQ 모듈
    EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;
    EPwm2Regs.AQCTLA.bit.CAD = AQ_SET;

    EDIS;

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;
}


interrupt void ePWM_INT(void)
{
    //DINT;

    GpioDataRegs.GPADAT.bit.GPIO15 = 1;

    // Control Source
    Control_Source();

    EPwm1Regs.ETCLR.bit.INT = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
    PieCtrlRegs.PIEIER3.bit.INTx1 = 0; // Disable ePWM Interrupt (Enable after the ADC Interrupt)

    GpioDataRegs.GPADAT.bit.GPIO15 = 0;

    //EINT;
}

