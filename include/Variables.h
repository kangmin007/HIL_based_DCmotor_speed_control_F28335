/*
 * Variables.h
 *
 *  Created on: 2020. 11. 15.
 *      Author: HyeonSeong Kim
 */

#ifndef INCLUDE_HSK_VARIABLES_H_
#define INCLUDE_HSK_VARIABLES_H_


#ifdef __cplusplus
extern "C" {
#endif



// Constants for Calculation
#define PI          ((float) 3.1415926535897932384626433832795)
#define SQRT2       ((float) 1.4142135623730950488016887242097)
#define SQRT3       ((float) 1.7320508075688772935274463415059)
#define INV_PI      ((float) 0.31830988618379069121644420192752)
#define INV_2PI     ((float) 0.15915494309189534560822210096376)
#define INV_3       ((float) 0.33333333333333333333333333333333)
#define INV_6       ((float) 0.16666666666666666666666666666667)
#define INV_3p3     ((float) 0.3030303030303030303030303030303)
#define INV_SQRT2   ((float) 0.70710678118654752440084436210485)
#define INV_SQRT3   ((float) 0.57735026918962576450914878050196)
#define INV_4096    ((float) 0.000244140625)
#define RmToRpm     ((float) 9.5492965855137210695602334453724)
#define RpmToRm     ((float) 0.10471975511965977461542144610932)
#define RadToDeg    ((float) 57.295779513082322864647721871734)
#define DegToRad    ((float) 0.017453292519943295769236907684886)


// Macro Functions
#define ABS(X)                  ((X > 0.) ? (X) : (-X))
#define SIGN(X)                 ((X > 0.) ? (1) : (-1))
#define MAX(X, Y)               ((X > Y) ? (X) : (Y))
#define MIN(X, Y)               ((X > Y) ? (Y) : (X))
#define SAT(X, LOWER, UPPER)    ((X < LOWER) ? (LOWER) : (X > UPPER) ? (UPPER) : (X))
#define Bound_PI(X)             ((X > 0.) ? ((X) - 2.*PI * (int)(INV_2PI * (X + PI))) : ((X) - 2.*PI * (int)(INV_2PI * (X - PI))))
#define Bound_2PI(X)            ((X > 0.) ? ((X) - 2.*PI * (int)(INV_2PI * (X + 2.*PI))) : ((X) - 2.*PI * (int)(INV_2PI * (X - 2.*PI))))


// Frequencies and Periods
#define Fsys        ((float) (150.e6))
#define Fsw         ((float) (10.e3))
#define Wsw         ((float) (2.*PI * Fsw))
#define Tsys        ((float) (6.6666666666666668061504055341898e-9))
#define Tsamp       ((float) (100.e-6))
#define Fsamp       ((float) (10.e3))
#define Tdead       ((float) (1.e-6))


// Inverter Parameters
#define PWM_PERIOD  ((unsigned int) (Tsamp * Fsys * 0.5))
#define DEAD_BAND   ((unsigned int) (Tdead * Fsys))



#ifdef __cplusplus
}
#endif


#endif /* INCLUDE_HSK_VARIABLES_H_ */
