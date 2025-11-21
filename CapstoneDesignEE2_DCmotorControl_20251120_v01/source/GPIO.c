/*
 * GPIO.c
 *
 *  Created on: 2021. 2. 3.
 *      Author: HyeonSeong Kim
 */

#include <DSP28x_Project.h>
#include <Variables.h>
#include <GPIO.h>



void Init_GPIO(void);


void Init_GPIO(void)
{
    EALLOW;

    // -- GPIO12 to send the end of the ADC offset calculation -- //
    GpioCtrlRegs.GPAPUD.bit.GPIO12 = 0;     // Enable pull-up on GPIO12
    GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0;    // GPIO12 = GPIO12
    GpioCtrlRegs.GPADIR.bit.GPIO12 = 1;     // GPIO12 = output
    GpioDataRegs.GPADAT.bit.GPIO12 = 0;     // GPIO12 outputs Low

    // -- GPIO15 to observe the operating range of the ePWM_INT -- //
    GpioCtrlRegs.GPAPUD.bit.GPIO15 = 0;     // Enable pull-up on GPIO15
    GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 0;    // GPIO15 = GPIO15
    GpioCtrlRegs.GPADIR.bit.GPIO15 = 1;     // GPIO15 = output
    GpioDataRegs.GPADAT.bit.GPIO15 = 0;     // GPIO15 outputs Low

    // -- GPIO13 to observe the operating range of the ADC_INT -- //
    GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;     // Enable pull-up on GPIO13
    GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 0;    // GPIO13 = GPIO13
    GpioCtrlRegs.GPADIR.bit.GPIO13 = 1;     // GPIO13 = output
    GpioDataRegs.GPADAT.bit.GPIO13 = 0;     // GPIO13 outputs Low

    // -- H-bridge switching signal --
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1; // EPWM1A -> GPIO0 out
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1; // EPWM1B -> GPIO1 out
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1; // EPWM2A -> GPIO2 out
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1; // EPWM2B -> GPIO3 out



    EDIS;
    // -------------------------------
}
