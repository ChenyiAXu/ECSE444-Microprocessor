/*
 * NRTrig.c
 *
 *  Created on: Sep 18, 2023
 *      Author: alice
 */
#include "main.h"
void NR_Trig(float *input_omega, float *phi, float *Xguess, float *Xoutput) {
    int itr, maxmitr;
    float h, x0, x1, allerr, omega, angle, fx, dfx;

    maxmitr = 1234567;
    x0 = *Xguess; // Initialize x0 to the value pointed to by Xguess
    omega = *input_omega;

    allerr = 0.0001;


    // Newton iterations
    for (itr = 1; itr <= maxmitr; itr++) {
        angle = omega * x0 + *phi; // Calculate wx + phi

        fx = pow(x0, 2) - cosf(angle); // Calculate f(x) = x^2 - cos(wx + phi)
        dfx = 2 * x0 + omega * sinf(angle); // Calculate f'(x) = 2x + wsin(wx + phi)
        h = fx / dfx;
        x1 = x0 - h;

        if (fabs(h) < allerr) {
        	 if(cosf(angle) < 0 || itr == maxmitr){
        		 break;
        	 }
        	 if(cosf(angle) == 0){
        		 *Xoutput = 0;
        	     return;
        	 }
            *Xoutput = x0;
            return;
        }
        x0 = x1;
    }
}
