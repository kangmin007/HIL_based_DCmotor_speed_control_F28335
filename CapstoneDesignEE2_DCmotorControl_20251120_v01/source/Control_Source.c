/*
 * Control_Source.c
 *
 *  Created on: 2020. 11. 15.
 *      Author: HyeonSeong Kim
 */

#include <math.h>
#include <DSP28x_Project.h>
#include <Variables.h>
#include <ePWM.h>
#include <ADC.h>
#include <DAC.h>
#include <eQEP.h>
#include <Filters.h>
#include <Control_Source.h>



SynM SPMSM1;
SpdObs SpdObs1;


void Control_Source(void);
void Init_Para_SynM(SynM *M);
void Init_Cont_SynM(SynM *M, float Wsc_In, float Wcc_In, float Alpha_In);
void Init_Speed_Observer(SpdObs *Obs, float Wc_In, float Wz_In, float Tsamp_In);
void Upd_Speed_Observer(SpdObs *Obs, float Theta_In);
void PI_IP_Speed_Control_SynM(SynM *M);
void Torque_Control_SynM(SynM *M);
void PI_Current_Control_SynM(SynM *M);
void Flux_Weakening_Control(SynM *M);


int Flag_Reset = 0;
int Flag_Start = 0;
float t_DSP = 0.;


void Control_Source(void)
{

    float duty;
    Uint16 tbprd = EPwm1Regs.TBPRD;

    // ENCODER
    Encoder();

    // ADC
    SPMSM1.Ias = ((ADC_Result[0] * INV_4096 * 3.) * (10. / 3.) - 5.) * 26.4;
    SPMSM1.Vdc = ((ADC_Result[1] * INV_4096 * 3.) * (10. / 3.) - 5.) * 73.535;

    // Speed Observer
    Upd_Speed_Observer(&SpdObs1, Theta_ENC);

    // Position
    SPMSM1.Thetarm = Bound_PI(SpdObs1.Theta_Est);
    SPMSM1.Thetar = SPMSM1.PolePairs * SPMSM1.Thetarm;
    SPMSM1.Thetar = Bound_PI(SPMSM1.Thetar);

    // Speed
    SPMSM1.Wrm = Upd_Filt_1st_BackwardEuler(&Wrm_LPF, (SpdObs1.W_Est), MODE_LPF);
    SPMSM1.Wrpm = RmToRpm * SPMSM1.Wrm;
    SPMSM1.Wr = SPMSM1.PolePairs * SPMSM1.Wrm;

    // 1 / Vdc
    SPMSM1.INV_Vdc = 1. / SPMSM1.Vdc;

    // Reset Detection
    if (Flag_Reset) {
        Flag_Reset = 0;
        SPMSM1.Flag_Reset = 1;
        Flag_Start = 0;
        t_DSP = 0.;
    }
    if (SPMSM1.Flag_Reset) {
        SPMSM1.Flag_Reset = 0;
        Init_Para_SynM(&SPMSM1);
        Init_Cont_SynM(&SPMSM1, (5. * 2.*PI), (200. * 2.*PI), 1.);  // Wsc = 5(2pi), Wcc = 200(2pi)
        Init_Filt_1st_BackwardEuler(&Wrm_LPF, (200. * 2.*PI), Tsamp);
    }

    // Control Commands
    if (t_DSP < 1.) {
        SPMSM1.Flag_SW = 0;
        SPMSM1.Flag_SC = 0;
        SPMSM1.Flag_CC = 0;
        SPMSM1.WrpmCmd = 0.;
    }
    else if ( (t_DSP >= 1.) && (t_DSP < 2.) ) {
        SPMSM1.Flag_SW = 1;
        SPMSM1.Flag_SC = 0;
        SPMSM1.Flag_CC = 0;
        SPMSM1.WrpmCmd = 0.;
    }
    else {
        SPMSM1.Flag_SW = 1;
        SPMSM1.Flag_SC = 1;
        SPMSM1.Flag_CC = 1;
    }


    if (SPMSM1.Flag_SW) {
        SPMSM1.Iqsr = SPMSM1.Ias;

        PI_IP_Speed_Control_SynM(&SPMSM1);
        PI_Current_Control_SynM(&SPMSM1);

        /*
        if(SPMSM1.Vdc > 1e-3f){
            duty = 0.5 + 0.5 * (SPMSM1.Vqsr * SPMSM1.INV_Vdc);
        }
        else {
            duty = 0.5;
        }
        */
        duty = SAT(SPMSM1.Vqsr * SPMSM1.INV_Vdc, 0., 1.);

        SPMSM1.Duty_a = duty;
        SPMSM1.Duty_b = 1. - duty;

        EPwm1Regs.CMPA.half.CMPA = (Uint16)(SPMSM1.Duty_a * (float)tbprd);
        EPwm2Regs.CMPB = (Uint16)(SPMSM1.Duty_b * (float)tbprd);

    }
    else {
        SPMSM1.Duty_a = 0.5;
        SPMSM1.Duty_b = 0.5;

        EPwm1Regs.CMPA.half.CMPA = (Uint16)(0.5 * (float)tbprd);
        EPwm2Regs.CMPB = (Uint16)(0.5 * (float)tbprd);
    }
    // ----------------------------------------------------------------------------------- //

    // Clock
    if (Flag_Start) {
        t_DSP += Tsamp;
    }
    else {
        t_DSP = 0.;
    }
}


void Init_Para_SynM(SynM *M)
{
    M->Pe_rated = 3336.;                             // Rated Power [W]
    M->Wrpm_rated = 3000.;                          // Rated Speed [r/min]
    M->Te_rated = M->Pe_rated / (M->Wrpm_rated / 60. * 2.*PI);                              // Rated Torque [N*m]
    M->Is_rated = 25.;                      // Rated Stator Current [A]
    M->Kt = M->Te_rated / M->Is_rated;              // Torque Constant [N*m/A]
    M->INV_Kt = 1. / M->Kt;                         // 1 / Kt

    M->Rs = 0.26;                                  // Stator Resistance [Ohms]
    M->Lds = 1.7e-3;                                // d-axis Stator Inductance [H]
    M->Lqs = 1.7e-3;                                // q-axis Stator Inductance [H]
    M->LAMpm = 0.264;                               // Rotor Magnet Flux Linkage [V*s]
    M->INV_Rs = 1. / M->Rs;                         // 1 / Rs
    M->INV_Lds = 1. / M->Lds;                       // 1 / Lds
    M->INV_Lqs = 1. / M->Lqs;                       // 1 / Lqs
    M->INV_LAMpm = 1. / M->LAMpm;                   // 1 / LAMpm

    M->J = 0.00252;                                    // Inertia [kg*m^2]
    M->INV_J = 1. / M->J;                           // 1 / J
    M->B = 0.;                                      // Viscous Damping [N*m*s]
    M->Pole = 2.;                                   // Pole
    M->PolePairs = M->Pole / 2.;                    // Pole Pairs
    M->INV_PolePairs = 1 / M->PolePairs;            // 1 / (Pole Pairs)

    M->Vdc = 0.;                                    // DC-Link Voltage [V]
    M->INV_Vdc = 0.;                                // 1 / Vdc
    M->Vas = 0., M->Vbs = 0., M->Vcs = 0.;          // Phase Voltages [V]
    M->Vdss = 0., M->Vqss = 0.;                     // dq-axis Stator Voltages in the Stationary Reference Frame [V]
    M->Vdsr = 0., M->Vqsr = 0.;                     // dq-axis Stator Voltages in the Rotor Reference Frame [V]
    M->Vsn = 0.;                                    // Offset Voltage for SVPWM [V]
    M->MAX_Vabcs = 0., M->MIN_Vabcs;                // Maximum & Minimum Values of Vabcs [V]
    M->Van = 0., M->Vbn = 0., M->Vcn = 0.;          // Pole Voltages [V]

    M->Ias = 0., M->Ibs = 0., M->Ics = 0.;          // Phase Currents [A]
    M->Idss = 0., M->Iqss = 0.;                     // dq-axis Stator Currents in the Stationary Reference Frame [A]
    M->Idsr = 0., M->Iqsr = 0.;                     // dq-axis Stator Currents in the Rotor Reference Frame [A]

    M->Wr = 0.;                                     // Electrical Rotor Speed [rad/s]
    M->Wrm = 0.;                                    // Mechanical Rotor Speed [rad/s]
    M->Wrpm = 0.;                                   // Mechanical Rotor Speed [r/min]
    M->Thetar = 0.;                                 // Electrical Rotor Position [rad]
    M->Thetarm = 0.;                                // Mechanical Rotor Position [rad]
    M->Thetarc = 0.;                                // Electrical Rotor Positin with Delay Compensation [rad]

    M->CosThetar = 0., M->SinThetar = 0.;           // cos(Thetar) & sin(Thetar)
    M->CosThetarc = 0., M->SinThetarc = 0.;         // cos(Thetarc) & sin(Thetarc)

    M->Flag_Reset = 0;                              // Reset Flag
    M->Flag_SW = 0;                                 // Switching Flag
    M->Duty_a = 0., M->Duty_b = 0., M->Duty_c = 0.; // PWM Duty
    M->VasSat = 0., M->VasSat = 0., M->VasSat = 0.; // Vabcs Saturation
    M->VdssSat = 0., M->VqssSat = 0.;               // Vdqsr Saturation
    M->VdsrSat = 0., M->VqsrSat = 0.;               // Vdqsr Saturation

    // Dead-Time Compensation
    M->Flag_DT_Comp = 0;
    M->Ddead = 0.;//Tdead * Fsw;
    M->SIGN_Ias = 0., M->SIGN_Ibs = 0., M->SIGN_Ics = 0.;
    M->K_Ias = 0., M->K_Ibs = 0., M->K_Ics = 0.;
    M->Ddead_a = 0., M->Ddead_b = 0., M->Ddead_c = 0.;
    M->Duty_a_Comp = 0., M->Duty_b_Comp = 0., M->Duty_c_Comp = 0.;

    // PI-IP Speed Controller
    M->Flag_SC = 0;
    M->WrpmRatio = 0.;
    M->Alpha = 0., M->zeta_sc = 0., M->Wcoff = 0.;
    M->Kps = 0., M->Kis = 0., M->Kas = 0.;
    M->WrpmRef = 0., M->WrmRef = 0.;
    M->WrpmCmd = 0., M->WrmCmd = 0., M->WrmErr = 0.;
    M->TeCmd_P = 0., M->TeCmd_I = 0., M->TeCmd_A = 0.;
    M->TeCmd = 0.;

    // Torque Controller
    M->Flag_TC = 0;
    M->TeCmdSat = 0.;
    M->IsCmd = 0., M->IsCmdSat = 0.;

    // PI Current Controller
    M->Flag_CC = 0;
    M->Kpcd = 0., M->Kicd = 0., M->Kacd = 0.;
    M->Kpcq = 0., M->Kicq = 0., M->Kacq = 0.;
    M->IdsrCmd = 0., M->IdsrErr = 0.;
    M->IqsrCmd = 0., M->IqsrErr = 0.;
    M->Vdsr_ff = 0., M->Vqsr_ff = 0.;
    M->Vdsr_P = 0., M->Vdsr_I = 0., M->Vdsr_A = 0.;
    M->Vqsr_P = 0., M->Vqsr_I = 0., M->Vqsr_A = 0.;

    // Flux-Weakening Controller
    M->Vs = 0., M->Vs_LPF = 0.;
    M->VsMAX = 0., M->VsErr = 0.;
    M->Kpfw = 0., M->Kifw = 0., M->Kafw = 0.;
    M->Delta_IdsrCmd_P = 0., M->Delta_IdsrCmd_I = 0., M->Delta_IdsrCmd_A = 0.;
    M->Delta_IdsrCmd = 0., M->Delta_IdsrCmdSat = 0.;
}


void Init_Cont_SynM(SynM *M, float Wsc_In, float Wcc_In, float Alpha_In)
{
    // PI-IP Speed Controller
    M->WrpmRatio = M->Wrpm_rated;
    M->Alpha = SAT(Alpha_In, 0., 1.);
    M->zeta_sc = 1.;
    M->Wcoff = Wsc_In / (4. + M->Alpha);
    M->Kps = M->zeta_sc * Wsc_In * M->J;
    M->Kis = Wsc_In * M->Wcoff * M->J;
    M->Kas = 1. / M->Kps;

    // PI Current Controller
    M->Kpcd = M->Lds * Wcc_In;
    M->Kicd = M->Rs * Wcc_In;
    M->Kacd = 1. / M->Kpcd;

    M->Kpcq = M->Lqs * Wcc_In;
    M->Kicq = M->Rs * Wcc_In;
    M->Kacq = 1. / M->Kpcq;

}


void PI_IP_Speed_Control_SynM(SynM *M)
{


    if(M->Flag_SC == 0){
        M->WrmErr = 0.;
        M->TeCmd_P = 0.; M->TeCmd_I = 0.; M->TeCmd_A = 0.;
        M->TeCmd = 0.;
        return;
    }

    M->WrpmCmd = M->WrpmRef;
    M->WrmCmd = M->WrpmCmd * RpmToRm; // speed ref [rad/s]

    M->Wrm = M->Wrpm * RpmToRm; // speed feedback [rad/s]

    M->WrmErr = M->WrmCmd - M->Wrm;

    // P, I 항
    M->TeCmd_P = M->Kps * M->WrmErr;
    M->TeCmd_I += Tsamp * M->Kis * (M->WrmErr - M->Kas * (M->TeCmd_A - M->TeCmd));

    M->TeCmd_A = M->TeCmd_P + M->TeCmd_I;


    // 최종 토크 명령
    M->TeCmd = SAT(M->TeCmd_A, - M->Te_rated, M->Te_rated);


    // 최종 전류 명령
    M->IqsrCmd = SAT(M->TeCmd * M->INV_Kt, - M->Is_rated, M->Is_rated);
}

/*
void Torque_Control_SynM(SynM *M)
{
    float before_limiter;
    float after_limiter;

    if(M->Flag_TC == 0){
        M->IsCmd = 0.;

        M->IqsrCmd = 0.;
        return;
    }


    before

}
*/


void PI_Current_Control_SynM(SynM *M)
{
   // float before_limiter;
   // float after_limiter;

    if(M->Flag_CC == 0){
        M->IqsrErr = 0.;
        M->Vqsr_P = 0.; M->Vqsr_I = 0.; M->Vqsr_A = 0.;
        M->Vqsr = 0.;
        return;
    }




    // Err 계산
    M->IqsrErr = M->IqsrCmd - M->Iqsr;

    // P, I 항
    M->Vqsr_P = M->Kpcq * M->IqsrErr;
    M->Vqsr_I += Tsamp * M->Kicq * (M->IqsrErr - M->Kacq * (M->Vqsr_A - M->Vqsr));

    M->Vqsr_A = M->Vqsr_P + M->Vqsr_I;


    // 최종 전압 명령
    M->Vqsr = SAT(M->Vqsr_A, 0, M->Vdc);



}


void Init_Speed_Observer(SpdObs *Obs, float Wc_In, float Wz_In, float Tsamp_In)
{
    Obs->Tsamp_so = Tsamp_In;

    Obs->Wc_so = Wc_In;
    Obs->Wz_so = Wz_In;

    Obs->Kd_so = Obs->Wc_so;
    Obs->Kp_so = 2. * Obs->Wc_so * Obs->Wz_so;
    Obs->Ki_so = Obs->Wc_so * Obs->Wz_so*Obs->Wz_so;

    Obs->Theta = 0., Obs->Theta_Est = 0., Obs->ThetaErr = 0., Obs->ThetaErr_Old = 0.;
    Obs->Accel_Est_I = 0., Obs->Accel_Est_P = 0., Obs->Accel_Est = 0.;
    Obs->W_Est_D = 0., Obs->W_Est_I = 0., Obs->W_Est = 0.;
}


void Upd_Speed_Observer(SpdObs *Obs, float Theta_In)
{
    Obs->Theta = Bound_PI(Theta_In);

    Obs->Accel_Est_P = Obs->Kp_so * Obs->ThetaErr;
    Obs->Accel_Est_I += Obs->Ki_so * Obs->ThetaErr * Obs->Tsamp_so;
    Obs->Accel_Est = Obs->Accel_Est_P + Obs->Accel_Est_I;

    Obs->W_Est_D = Obs->Kd_so * Obs->ThetaErr;
    Obs->W_Est_I += Obs->Accel_Est * Obs->Tsamp_so;
    Obs->W_Est = Obs->W_Est_D + Obs->W_Est_I;

    Obs->Theta_Est += Obs->W_Est * Obs->Tsamp_so;
    Obs->Theta_Est = Bound_PI(Obs->Theta_Est);

    Obs->ThetaErr = Bound_PI(Obs->Theta - Obs->Theta_Est);
}


void Flux_Weakening_Control(SynM *M)
{
}

