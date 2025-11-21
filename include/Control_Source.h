#ifndef INCLUDE_HSK_CONTROL_SOURCE_H_
#define INCLUDE_HSK_CONTROL_SOURCE_H_



typedef struct {
    float Pe_rated, Wrpm_rated, Te_rated, Is_rated, Kt, INV_Kt; // Is_rated´Â ¹¹Áö?
    float Rs, La;
    float INV_Rs, INV_La;
    float J, INV_J, B;

    float Vdc, INV_Vdc;

    float Ia;

    float Wr, Wrm, Wrpm;
    float Thetar, Thetarm;
    float Thetarc;

    int Flag_Reset, Flag_SW;
    float Duty_a, Duty_b;


    // Dead-Time Compensation
    int Flag_DT_Comp;
    float Ddead;
    float Duty_Comp;

    // PI Speed Controller
    int Flag_SC;
    float WrpmRatio;
    float Alpha, zeta_sc, Wcoff;
    float Kps, Kis, Kas;
    float WrpmRef, WrmRef;
    float WrpmCmd, WrmCmd, WrmErr;
    float TeCmd_P, TeCmd_I, TeCmd_A;
    float TeCmd;

    // PI Current Controller
    int Flag_CC;
    float Kpc, Kic, Kac;
    float IaCmd, IaErr;
    float VaCmd;
    float Va_P, Va_I, Va_A;



} SynM; // Synchronous Machine


typedef struct {
    float Tsamp_so;
    float Wc_so, Wz_so;
    float Kd_so, Kp_so, Ki_so;

    float Theta, Theta_Est, ThetaErr, ThetaErr_Old;
    float Accel_Est_I, Accel_Est_P, Accel_Est;
    float W_Est_D, W_Est_I, W_Est;
} SpdObs; // Speed Observer


extern SynM DCmotor1;
extern SpdObs SpdObs1;


extern void Control_Source(void);
extern void Init_Para_SynM(SynM *M);
extern void Init_Cont_SynM(SynM *M, float Wsc_In, float Wcc_In, float Alpha_In);
extern void Init_Speed_Observer(SpdObs *Obs, float Wc_In, float Wz_In, float Tsamp_In);
extern void Upd_Speed_Observer(SpdObs *Obs, float Theta_In);
extern void PI_Speed_Control_SynM(SynM *M);
extern void PI_Current_Control_SynM(SynM *M);


extern int Flag_Reset;
extern int Flag_Start;
extern float t_DSP;



#endif /* INCLUDE_HSK_CONTROL_SOURCE_H_ */
