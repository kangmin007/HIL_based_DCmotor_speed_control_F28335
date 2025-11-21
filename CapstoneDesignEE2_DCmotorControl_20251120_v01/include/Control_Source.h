/*
 * Control_Source.h
 *
 *  Created on: 2020. 11. 15.
 *      Author: HyeonSeong Kim
 */

#ifndef INCLUDE_HSK_CONTROL_SOURCE_H_
#define INCLUDE_HSK_CONTROL_SOURCE_H_



typedef struct {
    float Pe_rated, Wrpm_rated, Te_rated, Is_rated, Kt, INV_Kt;
    float Rs, Lds, Lqs, LAMpm;
    float INV_Rs, INV_Lds, INV_Lqs, INV_LAMpm;
    float J, INV_J, B;
    float Pole, PolePairs;
    float INV_PolePairs;

    float Vdc, INV_Vdc;
    float Vas, Vbs, Vcs;
    float Vdss, Vqss;
    float Vdsr, Vqsr;
    float Vsn, MAX_Vabcs, MIN_Vabcs;
    float Van, Vbn, Vcn;

    float Ias, Ibs, Ics;
    float Idss, Iqss;
    float Idsr, Iqsr;

    float Wr, Wrm, Wrpm;
    float Thetar, Thetarm;
    float Thetarc;

    float CosThetar, SinThetar;
    float CosThetarc, SinThetarc;

    int Flag_Reset, Flag_SW;
    float Duty_a, Duty_b, Duty_c;
    float VasSat, VbsSat, VcsSat;
    float VdssSat, VqssSat;
    float VdsrSat, VqsrSat;

    // Dead-Time Compensation
    int Flag_DT_Comp;
    float Ddead;
    float SIGN_Ias, SIGN_Ibs, SIGN_Ics;
    float K_Ias, K_Ibs, K_Ics;
    float Ddead_a, Ddead_b, Ddead_c;
    float Duty_a_Comp, Duty_b_Comp, Duty_c_Comp;

    // PI-IP Speed Controller
    int Flag_SC;
    float WrpmRatio;
    float Alpha, zeta_sc, Wcoff;
    float Kps, Kis, Kas;
    float WrpmRef, WrmRef;
    float WrpmCmd, WrmCmd, WrmErr;
    float TeCmd_P, TeCmd_I, TeCmd_A;
    float TeCmd;

    // Torque Controller
    int Flag_TC;
    float TeCmdSat;
    float IsCmd, IsCmdSat;

    // PI Current Controller
    int Flag_CC;
    float Kpcd, Kicd, Kacd;
    float Kpcq, Kicq, Kacq;
    float IdsrCmd, IdsrErr;
    float IqsrCmd, IqsrErr;
    float Vdsr_ff, Vqsr_ff;
    float Vdsr_P, Vdsr_I, Vdsr_A;
    float Vqsr_P, Vqsr_I, Vqsr_A;

    // Flux-Weakening Controller
    float Vs, Vs_LPF;
    float VsMAX, VsErr;
    float Kpfw, Kifw, Kafw;
    float Delta_IdsrCmd_P, Delta_IdsrCmd_I, Delta_IdsrCmd_A;
    float Delta_IdsrCmd, Delta_IdsrCmdSat;
} SynM; // Synchronous Machine


typedef struct {
    float Tsamp_so;
    float Wc_so, Wz_so;
    float Kd_so, Kp_so, Ki_so;

    float Theta, Theta_Est, ThetaErr, ThetaErr_Old;
    float Accel_Est_I, Accel_Est_P, Accel_Est;
    float W_Est_D, W_Est_I, W_Est;
} SpdObs; // Speed Observer


extern SynM SPMSM1;
extern SpdObs SpdObs1;


extern void Control_Source(void);
extern void Init_Para_SynM(SynM *M);
extern void Init_Cont_SynM(SynM *M, float Wsc_In, float Wcc_In, float Alpha_In);
extern void Init_Speed_Observer(SpdObs *Obs, float Wc_In, float Wz_In, float Tsamp_In);
extern void Upd_Speed_Observer(SpdObs *Obs, float Theta_In);
extern void PI_IP_Speed_Control_SynM(SynM *M);
extern void Torque_Control_SynM(SynM *M);
extern void PI_Current_Control_SynM(SynM *M);
extern void Flux_Weakening_Control(SynM *M);


extern int Flag_Reset;
extern int Flag_Start;
extern float t_DSP;



#endif /* INCLUDE_HSK_CONTROL_SOURCE_H_ */
