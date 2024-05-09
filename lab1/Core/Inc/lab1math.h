/*
 * lab1math.h
 *
 *  Created on: Sep 18, 2023
 *      Author: alice
 */

#ifndef INC_LAB1MATH_H_
#define INC_LAB1MATH_H_


#include "main.h"

extern void asmMax(float *array, uint32_t size, float *max, uint32_t *maxIndex);
extern void armSquareRoot(float *input, float *output, float *Xguess);
extern void cosroot(float *input_theta, float *w , float *Xguess, float *ouput);


void cMax(float *array, uint32_t size, float *max, uint32_t *maxIndex);
void NR_Trig(float *input_omega, float *phi, float *Xguess, float *Xoutput);

#endif /* INC_LAB1MATH_H_ */
