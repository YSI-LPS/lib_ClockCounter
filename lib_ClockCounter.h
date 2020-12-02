/** Mbed Library Clock Counter
* Hardware pulse counter with TIMER2 (CAP2.0 or CAP2.1) on Mbed LPC1768
*
* Counts signal transitions on p30(CAP2.0) or p29(CAP2.1).
* Can detecte rising, falling or both signal edge.
* Return the signal edge count during a periode in seconds.
* Shannon's theorem say with an input signal frequency up to 48 MHz with 96 MHz CCLK.
* Only tested with frequencys up to 20 MHz and it work.
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

#ifndef DEF_LIB_CLOCKCOUNTER_H
#define DEF_LIB_CLOCKCOUNTER_H

#include "mbed.h"
#ifndef TARGET_LPC1768
#error unsupported target
#else

enum edgeDetection  { RISING = 1, FALLING = 2, BOTH = 3 };

/** ClockCounter class
*   Counts signal transitions on p30(CAP2.0) or p29(CAP2.1) for LPC1768 target.
*   Can detecte rising, falling or both signal edge.
*   Return the signal edge count during a period in seconds.
*   In theory (Shannon's theorem) input signal frequency can up to 48 MHz with 96 MHz CCLK.
*   But only tested with frequencys up to 20 MHz and it work.
 */
class ClockCounter
{
public:
    /** Create an ClockCounter instance.
    *
    * Configure LPC1768 TIMER2 with capture input PIN_CAP2 and detecte transition EDGE.
    *
    * @param PIN_CAP2 can be p30(CAP2.0) or p29(CAP2.1), default is p30(CAP2.0).
    * @param EDGE can be RISING, FALLING, BOTH, default is RISING.
    */
    ClockCounter(PinName PIN_CAP2 = p30, edgeDetection EDGE = RISING);

    /** Select an ClockCounter instance
    *
    * Configure LPC1768 TIMER2 with capture input PIN_CAP2 and detecte transition EDGE.
    *
    * @param PIN_CAP2 can be p30(CAP2.0) or p29(CAP2.1).
    * @param EDGE can be RISING, FALLING, BOTH, default is RISING.
    */
    void setPin(PinName PIN_CAP2, edgeDetection EDGE = RISING);

    /** Start the signal transition count
    *
    */
    void startCount(void);

    /** Stop and Get the signal transition count
    *
     */
    int stopCount(void);

    /** Get the signal transition count during period
    *
    * @param period default is 1000000 microsecond.
     */
    int getCount(int period = 1000000);

    /** Get the selected input PIN_CAP2 on started signal transition count
    *
     */
    PinName getPin(void);
private:
    int _count;
    PinName _selectPin;
};
#endif
#endif