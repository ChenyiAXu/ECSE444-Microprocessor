# ECSE 444 labs
In the laboratory experiments, the STM32 B-L475E-IOT01A1 Discovery Kit, a comprehensive IoT development platform, was utilized alongside the STM32CubeIDE, an integrated development environment tailored for STM32 microcontrollers.
## Lab 0
   <br> 1. Configure PB14, mode to **GPIO_Output**, with user label **LED**
   <br> 2. TIM17 to SysTick mode from Timebase Source
   <br> 3. TIM2: set the timer to count processor cloc edges at 80 MHz, set the prescaler to 40,000, and counter period to 2000, interrupt raised after 80, 000, 000 clock cycles. 
## Lab 1 
Perform the execution of square root calculation in FPU, CMSIS-DSP, and C and execute the timing analysis. Solve the transcendental function in C and assembly and perform timing analysis. 
   <br> **Board configuration**
         <br> 1. Set PB3 to SYS_JTDO-SWO
         <br> 2. A header file named lab1math.h is addded in the Includes section. Note: CMAX function in C is provided by the professor. 
         <br> 3. access ITM: ITM_Port32(n) is a location in memory, we would set it to a value to generate trace packet in main.c. Uner **Serial Wire Viewer(SWV)**, tick **"Enable"**, set core clock to 80.0 MHz, enable port 31,
         <br> Square Root --> arm_sqrt_f32, NR_root, armSquareRoot
         <br> Transcendental function --> NR_trig, cosroot
## Lab 2
Deliverables:
<br> **C implementation of LED lighting on button press.**
<br>       When the blue pushbutton is pressed(=GPIO_PIN_RESRT), the LED light is on. Otherwise, the LED is set off. 
<br>       LED which is PB14 is set as GPIO_Output, Blue Pushbutton (PC13) is set as GPIO_Input. 
<br>
<br> **C implementation of triangle, saw, and sine signals.**
<br>       DAC converts digital register values into analog values. Under DAC 1(Pinout & Configuration -->Analog section), enable OUT1 and OUT2 in Connected to external pin only mode. Then verify the following setting: Output Buffer (Enable), Trigger(None), User Trimming(factory trimming), sample and Hold (Disable). 
<br>      ITM can be used to verify the functionality. Enable Serial Wire Viewer. Enable Comparator 0 and Comparator 1, write names of signal under Var/Addr. 
<br>     An Oscilloscope or a speaker can be connected to check out the signal. 
<br>
<br> **C implementation scaling ADC outputs to calculate CPU temperature in degree C.**
<br> **C implementation of temperature-dependent output to the speaker.**
<br> **C implementation of a final application that integrates the above deliverables.**


