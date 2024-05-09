In the laboratory experiments, the STM32 B-L475E-IOT01A1 Discovery Kit, a comprehensive IoT development platform, was utilized alongside the STM32CubeIDE, an integrated development environment tailored for STM32 microcontrollers.
**Lab 0**
    1. Configure PB14, mode to **GPIO_Output**, with user label **LED**
    2. TIM17 to SysTick mode from Timebase Source
    3. TIM2: set the timer to count processor cloc edges at 80 MHz, set the prescaler to 40,000, and counter period to 2000, interrupt raised after 80, 000, 000 clock cycles. 
