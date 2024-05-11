# ECSE 444 Labs

In the laboratory experiments, the **STM32 B-L475E-IOT01A1 Discovery Kit**, a comprehensive IoT development platform, was utilized alongside the **STM32CubeIDE**, an integrated development environment tailored for STM32 microcontrollers.

## Lab 0

- **Configure PB14**
  - Mode: `GPIO_Output`
  - User label: `LED`
- **TIM17**
  - Configuration: SysTick mode from Timebase Source
- **TIM2**
  - Timer setting: Count processor clock edges at 80 MHz
  - Prescaler: 40,000
  - Counter period: 2,000
  - Interrupt: Raised after 80,000,000 clock cycles

## Lab 1

Perform the execution of square root calculation in FPU, CMSIS-DSP, and C and execute the timing analysis. Solve the transcendental function in C and assembly and perform timing analysis.

- **Board Configuration**
  - Set `PB3` to `SYS_JTDO-SWO`
  - Include a header file `lab1math.h` in the Includes section. Note: `CMAX` function in C is provided by the professor.
  - Access ITM: `ITM_Port32(n)` is a memory location, set it to generate a trace packet in `main.c`. Under **Serial Wire Viewer (SWV)**:
    - Enable: Tick **"Enable"**
    - Core clock: Set to 80.0 MHz
    - Enable port 31
  - Functions:
    - Square Root: `arm_sqrt_f32`, `NR_root`, `armSquareRoot`
    - Transcendental Function: `NR_trig`, `cosroot`

## Lab 2

Deliverables include C implementations for various functionalities:

- **LED Lighting on Button Press**
  - When the blue pushbutton is pressed (`GPIO_PIN_RESET`), the LED light is on.
  - LED `PB14` is set as `GPIO_Output`.
  - Blue Pushbutton `PC13` is set as `GPIO_Input`.

- **Triangle, Saw, and Sine Signals**
  - DAC converts digital values to analog signals.
  - Configuration under DAC 1 (Pinout & Configuration -> Analog):
    - Enable OUT1 and OUT2 in "Connected to external pin only" mode.
    - Output Buffer: Enable
    - Trigger: None
    - User Trimming: Factory trimming
    - Sample and Hold: Disable
  - Functionality verification via ITM and SWV. Use an Oscilloscope or a speaker for output.

- **Scaling ADC Outputs to Calculate CPU Temperature**
  - ADC converts voltage to digital representation.
  - Configure the board, read ADC output then scale the ADC output.
  - Configuration:
    - ADC1: Read Internal temperature sensor
    - ADC2: Read internal voltage reference
    - Ensure channels `IN1` to `INX` are disabled.
    - Enable **Temperature Sensor Channel** on ADC 1 and **Vrefint Channel** on another ADC.
    - Select sampling time and clock frequency based on [STM32L47x Manual 18.4.16](https://www.st.com/resource/en/reference_manual/dm00083560-stm32l47xxx-stm32l48xxx-stm32l49xxx-and-stm32l4axxx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf).
  - Reading ADC Output: Refer to [Driver User Manual Section 7.2](https://www.st.com/resource/en/user_manual/dm00173145-description-of-stm32l4l4-hal-and-lowlayer-drivers-stmicroelectronics.pdf).
  - Scaling: Refer to the manual Section 18.4.32.

- **Final Application Integration**
  - By default, the application plays a fixed sound of your choice (triangle, saw, sine).
  - On button press:
    - The LED should turn on.
    - The sound should change based on the temperature sensor.
  - On subsequent button press:
    - The LED should turn off.
    - The sound should revert to the fixed sound, rotating among triangle, saw, and sine.

