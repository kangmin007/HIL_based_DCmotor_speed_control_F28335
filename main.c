#include <DSP28x_Project.h>
#include <GPIO.h>
#include <ePWM.h>
#include <ADC.h>
#include <DAC.h>
#include <eQEP.h>
#include <Variables.h>
#include <Control_Source.h>
#include <Filters.h>



int main(void)
{
    InitSysCtrl();

    DINT;

    InitPieCtrl();
    InitXintf();

    IER = 0x0000;
    IFR = 0x0000;

    InitPieVectTable();

    EALLOW;
    EDIS;

    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;
    IER = M_INT1 | M_INT3;              // ADC INT | ePWM INT

    PieCtrlRegs.PIEIER1.bit.INTx6 = 1;  // Enable ADC Interrupt
    PieCtrlRegs.PIEIER3.bit.INTx1 = 0;  // Disable ePWM Interrupt (Enable after the ADC Interrupt / Disable after the ePWM Interrupt)

    EINT;
    ERTM;

    // Initializations for DSP
    Init_GPIO();
    Init_ePWM();
    Init_ADC();
    Init_DAC();
    Init_eQEP();

    // Initializations for Controller
    Init_Para_SynM(&DCmotor1);
    Init_Cont_SynM(&DCmotor1, (5. * 2.*PI), (200. * 2.*PI), 1.);
    Init_Speed_Observer(&SpdObs1, 1000., 100., Tsamp);
    Init_Filt_1st_BackwardEuler(&Wrm_LPF, (200. * 2.*PI), Tsamp);

    for(;;)    {}
}
