/** Mbed Library Clock Counter
* Hardware pulse counter with TIMER2 (CAP2.0 or CAP2.1) on Mbed LPC1768
*
* Counts signal transitions on p30(CAP2.0) or p29(CAP2.1) for LPC1768 target.
* Can detecte rising, falling or both signal edge.
* Return the signal edge count during a period in seconds.
* In theory (Shannon's theorem) input signal frequency can up to 48 MHz with 96 MHz CCLK.
* But only tested with frequencys up to 20 MHz and it work.
*
* Example :
* @code
* #include "mbed.h"
* #include "lib_ClockCounter.h"
*
* Serial pc(USBTX, USBRX);
* ClockCounter Frequency;
* 
* int main()
* {
*     while(1) pc.printf("Frequency = %d Hz\r\n", Frequency.getCount());
* }
* @endcode
*/

#include "lib_ClockCounter.h"

ClockCounter::ClockCounter(PinName PIN_CAP2, edgeDetection EDGE)
{
    switch(PIN_CAP2)
    {
        case p30: case p29:
                                                                    // PCONP => Power Mode Control register
            LPC_SC->PCONP |= (0x1<<22);                             // Bit(22) 1 => Timer2 power on
                                                                    // PCLKSEL1 => Peripheral Clock Selection register 1
            LPC_SC->PCLKSEL1 |= (0x1<<12);                          // Bits(13,12) 01 => PCLK_TIMER2 = CCLK(96 MHz)
                                                                    // TCR => Timer Control Register
            LPC_TIM2->TCR = 0x0;                                    // Bits(1,0) 00 => Timer2 disabled
                                                                    // PINSEL0 => Pin Function Select register 0
            LPC_PINCON->PINSEL0 |= (0x3<<((PIN_CAP2==p30)?8:10));   // Bits(9,8) 11 => pin function CAP2.0 (p30), Bits(11,10) 11 => pin function CAP2.1 (p29)
                                                                    // CTCR => Count Control Register
            LPC_TIM2->CTCR = ((PIN_CAP2==p30)?0:4)+EDGE;            // Bits(3,2) 00 => CAP2.0 (p30 signal is counter clock) or 11 => CAP2.1 (p29 signal is counter clock), Bits(1,0) XX => timer is incremented on edge
                                                                    // CCR => Capture Control Register
            LPC_TIM2->CCR = 0x0;                                    // Bits(5,4,3,2,1,0) 000000 => capture and interrupt on event disabled
        break;
    }
}

int ClockCounter::getCount(int period)
{
                                                                    // TCR => Timer Control Register
    LPC_TIM2->TCR = 0x2;                                            // Bits(1,0) 10 => Timer2 count reset
    LPC_TIM2->TCR = 0x1;                                            // Bits(1,0) 01 => Timer2 enabled
    wait_us(period);
    LPC_TIM2->TCR = 0x0;                                            // Bits(1,0) 00 => Timer2 disabled
    return LPC_TIM2->TC;                                            // TC => Timer Counter
}