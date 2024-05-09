/*
 * armSquareRoot.s
 *
 *  Created on: Sep 18, 2023
 *      Author: alice
 */
.syntax unified

// export the label asmMax expected by lab1math.h
.global armSquareRoot

// .section marks a new section in assembly. .text identifies it as source code;
// .rodata marks it as read-only, setting it to go in FLASH, not SRAM
.section .text.rodata

/**extern void armSqrt(float *input, float *output, float *Xguess);
	Ro--> pointer to input
	R1--> pointer to output
	R2--> pointer to Xguess
*/

armSquareRoot:
	PUSH	{R4, R5}
	VLDR.f32	S0, [R0] 		//input
	VLDR.f32	S1, [R1]		//output
	VLDR.f32	S2, [R2]		//Xguess

//special cases
input_less_than_0:
	VCMP.f32 S0, #0.0
	VMRS		APSR_nzvc, FPSCR	// load the FP PSR to branch using FP conditions
	BLT	 done
	BEQ	 zero_result

input_is_1:
	VMOV.f32 S3, #1.0		//S3<-temp value = 1.0
	VCMP.f32 S0, S3
	VMRS		APSR_nzvc, FPSCR	// load the FP PSR to branch using FP conditions
	BEQ	one_result

sqrt:
//constant initialization (2 for division, and 0.001 for alleerrr

	LDR     R4, =0x3c23d70a   // Load the IEEE 754 representation of 0.0001 as a 32-bit integer
	VMOV    S3, R4           // Convert to a floating-point value
	VMOV.f32 	S4, #0x40000000	//S3<---temp variable = 2

//newton iteration
newton_loop:
    // Calculate h = (input - x0^2) / (-2 * x0)
    VMOV.f32	S6, S2		// Copy x0 to S6
    VMUL.f32	S6, S6, S6	// Calculate x0^2 and store in S6
    VSUB.f32	S5, S0, S6	// Calculate (input - x0^2) and store in S5  f(x)
    VMUL.f32	S6,	S6, S4	//2 * x0

	MOV			R5, #0		//temp variable R5<--=0
    VMOV.f32	S7, R5		//0 in float
    VSUB.f32	S6, S6, S7	//-2x df(x)
    VDIV.f32	S5, S5, S6	//h <- S5

    VSUB.f32	S1, S2, S5	// Update x1 = x0 - h


    // Check for convergence: fabs(h) < allerr
    VABS.f32	S7, S5		// Calculate the absolute value of h and store in S7
    VCMP.f32	S7, S3		// Compare with 'allerr'
    VMRS		APSR_nzvc, FPSCR	// load the FP PSR to branch using FP conditions
    BLE	done			// If |h| < allerr, exit the loop

    // Update x0 with x1
    VMOV.f32	S2, S1

//back to iterations
	B	newton_loop
done:
	VABS.f32	S1, S1
	VSTR.f32	S1, [R1]		//store output into R1
	POP	{R4, R5}
	BX	LR

zero_result:
//input zero--> output zero
	MOV		R4, #0			// R4 <- temperary value = 0
	VMOV	S1, R4			//S1<--output = R4 = 0
	B		done

one_result:
	MOV		R4, #1			// R4 <- temperary value = 1
	VMOV	S1, R4			//S1<--output = R4 = 1
	B		done
