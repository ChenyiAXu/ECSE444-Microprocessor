/*
 * NR_root.c
 *
 *  Created on: Sep 18, 2023
 *      Author: alice
 */
#include "main.h"
void NR_root(float *input,  float *out,float *Xguess) {
	 	int itr, maxmitr;
	    float h, x0, x1, allerr;
	    maxmitr=1000;
	    x0=*Xguess;
	    allerr=0.0001;

	    if(*input == 0){
	    	*out = 0;
	    	return 0;
	    }
	    for (itr=1; itr<=maxmitr; itr++)
	    {
	        h=(*input-pow((x0),2))/(-2*x0);
	        x1=x0-h;

	        if (fabs(h) < allerr)
	        {
	        	*out=x0;
	            return 0;
	        }
	        x0=x1;
	    }
}
