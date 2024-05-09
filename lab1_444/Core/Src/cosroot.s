/*
 * cos.s
 *
 *  Created on: Sep 17, 2023
 *      Author: 10200
 */
/*
 * armSquareRoot.s
 *
 *  Created on: Sep 14, 2023
 *      Author: alice xu
 */
.syntax unified


// export the label asmMax expected by lab1math.h
.global cosroot

// .section marks a new section in assembly. .text identifies it as source code;
// .rodata marks it as read-only, setting it to go in FLASH, not SRAM
.section .text.rodata

/**extern void cosroot(float *input_theta, float *w , float *Xguess, float *ouput );
	Ro--> pointer to input
	R1--> pointer to inputt
	R2--> pointer to Xguess
	R3--> pointer to output
*/

cosroot:
	PUSH	{R4,R5}
	//PUSH {lr}
	VLDR.f32	S1, [R0] 		//angle
	VLDR.f32	S2, [R1]		//w
	VLDR.f32	S3, [R2]		//Xguess
	//VLDR.f32	S4, [R3]		//output

//special cases


init:
//constant initialization (2 for division, and 0.001 for alleerrr
	MOV		R5 , #1
	LDR     R4, =0x3a83126f   // Load the IEEE 754 representation of 0.0001 as a 32-bit integer
	VMOV    S5, R4           // Convert to a floating-point value


//newton iteration
newton_loop:
    // Calculate h = (input - x0^2) / (-2 * x0)

    //VMOV.f32	S5, S2		// Copy xguess to s5 , imtermidate
    VMUL.f32    S6, S2, S3 //WX//wx
    VADD.f32	S6, S6, S1 //WX+theta
    //vcvt.f32.f64 S0, D0  // Convert the input angle from double to float
    //VPUSH.32	{S0}
    VMOV.f32	S0,S6 // s6 input
	push {lr,r0-r5}
    Bl arm_cos_f32 //s0 is cos()
    pop {lr,r0-r5}
    VMOV.f32	S7,S0 // s7 is cos
   // VPOP.32		{S0}

    //VCOS.f32 	S6, S5     // Compute the cosine USEING S5 INPUT
    VMUL.f32	S8, S3, S3	// Calculate x0^2 and store in S7 , TEMPLATE
    VSUB.f32	S7,	S8,	S7 //x^2-cos() IN S6

    //VPUSH.32	{S0}
    VMOV.f32	S0,S6 // s6 input
    push {lr,r0-r5}
    Bl arm_sin_f32
    pop {lr,r0-r5}
    VMOV.f32	S8,S0 // s6 is sin
    //VPOP.32		{S0}
	//vsin.f32 	S7, S5  //sin()
	VMUL.f32	S8, S2, S8 //w*sin
	VADD.f32	s9, s3, s3 //2x
	vadd.f32	s9, s8, s9 //2x+wsin
	VDIV.F32	s9, s7,	s9 //x^2-cos()/2x+wsin,h


    VSUB.f32	S3, S3, S9	// Update x1 = x0 - h

    // Check for convergence: fabs(h) < allerr
    VABS.f32	S10, S9		// Calculate the absolute value of h and store in S7
    VCMP.f32	S10, S5		// Compare with 'allerr'
    VMRS		APSR_nzvc, FPSCR	// load the FP PSR to branch using FP conditions


    BLT	done
    //BGE	done		// If |h| < allerr, exit the loop
	ADD	R5,	#1 // add iteration max
	//VMRS		APSR_nzvc, FPSCR	// load the FP PSR to branch using FP conditions

	CMP R5 , #100 // larger 100 iteration
	//VMRS		APSR_nzvc, FPSCR	// load the FP PSR to branch using FP conditions

	BGE exit_loop
    // Update x0 with x1


//back to iterations
	B	newton_loop
exit_loop:
	LDR     R5, =0x0   // Load the IEEE 754 representation of 0.0001 as a 32-bit integer

	VMOV.F32	S4 , R5 // if no solution return 0
	VSTR.f32	S4, [R3]
	POP	{R4, R5}
	BX	LR

done:
	//VCVT.f32.U32	S2,S2			 //convert S1 from 32-bit unsigned integer to 32-bit float
	//bx lr
	VMOV.f32	S4, S3
	VABS.f32	S4 , S4
	//VCVT.f32.U32	S2,S2
	//vcvt.F32 Fd, S2
	//vmov.f32 R4 , S2


	VSTR.f32	s4, [R3]		//store output into R17
	//LDR     R5, =0x0
	POP	{R4, R5}
	//pop {lr}
	BX	LR

