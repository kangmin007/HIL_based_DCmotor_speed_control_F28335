/*
 * eQEP.c
 *
 *  Created on: 2020. 11. 15.
 *      Author: HyeonSeong Kim
 */

#include <DSP28x_Project.h>
#include <Variables.h>
#include <eQEP.h>



void Init_eQEP(void);
void Encoder(void);


int EncCnt = 0;
float Theta_ENC = 0.;


void Init_eQEP(void)
{
    EALLOW;

    GpioCtrlRegs.GPAPUD.bit.GPIO20 = 0;     // Enable pull-up on GPIO20 (EQEP1A)
    GpioCtrlRegs.GPAPUD.bit.GPIO21 = 0;     // Enable pull-up on GPIO21 (EQEP1B)
    GpioCtrlRegs.GPAPUD.bit.GPIO23 = 0;     // Enable pull-up on GPIO23 (EQEP1I)

    GpioCtrlRegs.GPAQSEL2.bit.GPIO20 = 0;   // Sync to SYSCLKOUT GPIO20 (EQEP1A)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO21 = 0;   // Sync to SYSCLKOUT GPIO21 (EQEP1B)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO23 = 0;   // Sync to SYSCLKOUT GPIO23 (EQEP1I)

    GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 1;    // Configure GPIO20 as EQEP1A
    GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 1;    // Configure GPIO21 as EQEP1B
    GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 1;    // Configure GPIO23 as EQEP1I

    EDIS;

    EQep1Regs.QPOSCNT = 0x0000;             // Initialization of the QPOSCNT.
    EQep1Regs.QPOSINIT = 0x0000;            // Initialization value of the QPOSCNT when Index event occurs.
    EQep1Regs.QPOSMAX = 4 * PPR - 1;        // Maximum value of the QPOSCNT.
    EQep1Regs.QUPRD = EPRD;                 // eQEP Period

    EQep1Regs.QDECCTL.bit.QSRC = 0;         // Quadrature count mode
    EQep1Regs.QDECCTL.bit.SWAP = 1;         // CCW ¹æ½Ä.
    EQep1Regs.QDECCTL.bit.XCR = 0;          // 2x resolution: Count the rising/falling edge

    EQep1Regs.QEPCTL.bit.FREE_SOFT = 0x2;   // Position count is unaffected by emulation suspend.
    EQep1Regs.QEPCTL.bit.PCRM = 0;          // Reset the position count when the index event occurs.
    EQep1Regs.QEPCTL.bit.QPEN = 1;          // QEP position counter enable
}



void Encoder(void)
{
    EncCnt = EQep1Regs.QPOSCNT;
    Theta_ENC = ((float) EncCnt * (0.25 * INV_PPR)) * 2.*PI;
    Theta_ENC = Bound_PI(Theta_ENC);
}



