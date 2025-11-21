
#include <math.h>
#include <DSP28x_Project.h>
#include <Variables.h>
#include <ePWM.h>
#include <ADC.h>
#include <DAC.h>
#include <eQEP.h>
#include <Filters.h>
#include <Control_Source.h>



SynM DCmotor1;
SpdObs SpdObs1;


void Control_Source(void);
void Init_Para_SynM(SynM *M);
void Init_Cont_SynM(SynM *M, float Wsc_In, float Wcc_In, float Alpha_In);
void Init_Speed_Observer(SpdObs *Obs, float Wc_In, float Wz_In, float Tsamp_In);
void Upd_Speed_Observer(SpdObs *Obs, float Theta_In);
void PI_Speed_Control_SynM(SynM *M);
void PI_Current_Control_SynM(SynM *M);


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
    DCmotor1.Ia = ((ADC_Result[0] * INV_4096 * 3.) * (10. / 3.) - 5.) * 26.4;
    DCmotor1.Vdc = ((ADC_Result[1] * INV_4096 * 3.) * (10. / 3.) - 5.) * 73.535;

    // Speed Observer
    Upd_Speed_Observer(&SpdObs1, Theta_ENC);

    // Position
    DCmotor1.Thetarm = Bound_PI(SpdObs1.Theta_Est);
    DCmotor1.Thetar = DCmotor1.Thetarm;
    DCmotor1.Thetar = Bound_PI(DCmotor1.Thetar);

    // Speed
    DCmotor1.Wrm = Upd_Filt_1st_BackwardEuler(&Wrm_LPF, (SpdObs1.W_Est), MODE_LPF);
    DCmotor1.Wrpm = RmToRpm * DCmotor1.Wrm;
    DCmotor1.Wr = DCmotor1.Wrm;

    // 1 / Vdc
    DCmotor1.INV_Vdc = 1. / DCmotor1.Vdc;

    // Reset Detection
    if (Flag_Reset) {
        Flag_Reset = 0;
        DCmotor1.Flag_Reset = 1;
        Flag_Start = 0;
        t_DSP = 0.;
    }
    if (DCmotor1.Flag_Reset) {
        DCmotor1.Flag_Reset = 0;
        Init_Para_SynM(&DCmotor1);
        Init_Cont_SynM(&DCmotor1, (5. * 2.*PI), (200. * 2.*PI), 1.);  // Wsc = 5(2pi), Wcc = 200(2pi)
        Init_Filt_1st_BackwardEuler(&Wrm_LPF, (200. * 2.*PI), Tsamp);
    }

    // Control Commands
    if (t_DSP < 1.) {
        DCmotor1.Flag_SW = 0;
        DCmotor1.Flag_SC = 0;
        DCmotor1.Flag_CC = 0;
        DCmotor1.WrpmCmd = 0.;
    }
    else if ( (t_DSP >= 1.) && (t_DSP < 2.) ) {
        DCmotor1.Flag_SW = 1;
        DCmotor1.Flag_SC = 0;
        DCmotor1.Flag_CC = 0;
        DCmotor1.WrpmCmd = 0.;
    }
    else {
        DCmotor1.Flag_SW = 1;
        DCmotor1.Flag_SC = 1;
        DCmotor1.Flag_CC = 1;
    }


    if (DCmotor1.Flag_SW) {
        PI_Speed_Control_SynM(&DCmotor1);
        PI_Current_Control_SynM(&DCmotor1);

        duty = SAT(DCmotor1.VaCmd * DCmotor1.INV_Vdc, 0., 1.);

        DCmotor1.Duty_a = duty;
        DCmotor1.Duty_b = 1. - duty;

        EPwm1Regs.CMPA.half.CMPA = (Uint16)(DCmotor1.Duty_a * (float)tbprd);
        EPwm2Regs.CMPB = (Uint16)(DCmotor1.Duty_b * (float)tbprd);

    }
    else {
        DCmotor1.Duty_a = 0.5;
        DCmotor1.Duty_b = 0.5;

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
    M->La = 1.7e-3;                                // Stator Inductance [H]
    M->INV_Rs = 1. / M->Rs;                         // 1 / Rs
    M->INV_La = 1. / M->La;                       // 1 / La

    M->J = 0.00252;                                    // Inertia [kg*m^2]
    M->INV_J = 1. / M->J;                           // 1 / J
    M->B = 0.;                                      // Viscous Damping [N*m*s]

    M->Vdc = 0.;                                    // DC-Link Voltage [V]
    M->INV_Vdc = 0.;                                // 1 / Vdc
    M->Ia = 0.;


    M->Wr = 0.;                                     // Electrical Rotor Speed [rad/s]
    M->Wrm = 0.;                                    // Mechanical Rotor Speed [rad/s]
    M->Wrpm = 0.;                                   // Mechanical Rotor Speed [r/min]
    M->Thetar = 0.;                                 // Electrical Rotor Position [rad]
    M->Thetarm = 0.;                                // Mechanical Rotor Position [rad]
    M->Thetarc = 0.;                                // Electrical Rotor Positin with Delay Compensation [rad]


    M->Flag_Reset = 0;                              // Reset Flag
    M->Flag_SW = 0;                                 // Switching Flag
    M->Duty_a = 0., M->Duty_b = 0.; // PWM Duty

    // Dead-Time Compensation
    M->Flag_DT_Comp = 0;
    M->Ddead = 0.;//Tdead * Fsw;
    M->Duty_Comp = 0.;

    // PI Speed Controller
    M->Flag_SC = 0;
    M->WrpmRatio = 0.;
    M->Alpha = 0., M->zeta_sc = 0., M->Wcoff = 0.;
    M->Kps = 0., M->Kis = 0., M->Kas = 0.;
    M->WrpmRef = 0., M->WrmRef = 0.;
    M->WrpmCmd = 0., M->WrmCmd = 0., M->WrmErr = 0.;
    M->TeCmd_P = 0., M->TeCmd_I = 0., M->TeCmd_A = 0.;
    M->TeCmd = 0.;


    // PI Current Controller
    M->Flag_CC = 0;
    M->Kpc = 0., M->Kic = 0., M->Kac = 0.;


    // 추가한 부분
    M->IaCmd = 0., M->IaErr = 0.;
    M->VaCmd = 0.;
    M->Va_P = 0., M->Va_I = 0., M->Va_A = 0.;


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
    M->Kpc = M->La * Wcc_In;
    M->Kic = M->Rs * Wcc_In;
    M->Kac = 1. / M->Kpc;

}


void PI_Speed_Control_SynM(SynM *M)
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
    M->IaCmd = SAT(M->TeCmd * M->INV_Kt, - M->Is_rated, M->Is_rated);
}


void PI_Current_Control_SynM(SynM *M)
{
   // float before_limiter;
   // float after_limiter;

    if(M->Flag_CC == 0){
        M->IaErr = 0.;
        M->Va_P = 0.;
        M->Va_I = 0.;
        M->Va_A = 0.;
        M->VaCmd = 0.;
        return;
    }

    // Err 계산
    M->IaErr = M->IaCmd - M->Ia;

    // P, I 항
    M->Va_P = M->Kpc * M->IaErr;
    M->Va_I += Tsamp * M->Kic * (M->IaErr - M->Kac * (M->Va_A - M->VaCmd));

    M->Va_A = M->Va_P + M->Va_I;


    // 최종 전압 명령
    M->VaCmd = SAT(M->Va_A, 0, M->Vdc);



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
