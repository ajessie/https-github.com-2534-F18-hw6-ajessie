#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "ButtonLED_HAL.h"


// This function initializes all the peripherals except graphics
void initialize();


//The number of counter cycles during 1ms based on a source clock of 3MHz and the divider of 1
#define ONE_MILLISEC_COUNT  3000

// This is based on a 3MHz source clock and a clock divider of 64
#define PWM_PERIOD_CYLCLES ONE_MILLISEC_COUNT

// Duty cycle as a fraction (between 0 and 1)
#define DUTY_CYCLE_FRACTION 0.001

// The number of on cycles (Timer_A counter cycles)
#define BLUE_ON_CYCLES (DUTY_CYCLE_FRACTION * PWM_PERIOD_CYLCLES)

// The number of off cycles (Timer_A counter cycles)
#define BLUE_OFF_CYCLES ((1-DUTY_CYCLE_FRACTION) * PWM_PERIOD_CYLCLES)

// Since we start at "off" cycle, the first change comes once the number of "off" cycles are over
// With a different starting point or a different timer_A mode, this value could be different to achieve the same duty cycle
#define BLU_COMPARE_CYCLES    BLUE_OFF_CYCLES

// The below set of choices are not programmer's.
// We need to study the Booster board and Launchpad board diagrams for this info.

// The BLUE LED is wired to TA2.1.
// As the programmer we cannot change this. It is decided by the board designer
#define BLU_TIMER    TIMER_A2_BASE
#define BLU_CHANNEL  TIMER_A_CAPTURECOMPARE_REGISTER_1

#define BLOCKING    0
#define BLOCK_COUNT 100000

void initPWM()
{
    // For the Blue LED, we first configure it as a GPIO and turn it off
    // Then, we configure it to work as output channel of a timer
    // The blue LED on booster is wried to Port 5, Pin 6, we cannot change this as the programmer
    // BLUE LED
    GPIO_setAsOutputPin    (GPIO_PORT_P5,    GPIO_PIN6);
    GPIO_setOutputLowOnPin (GPIO_PORT_P5,    GPIO_PIN6);
    GPIO_setAsPeripheralModuleFunctionOutputPin(
            GPIO_PORT_P5,
            GPIO_PIN6,
            GPIO_PRIMARY_MODULE_FUNCTION);
}


int main(void) {

  // Stop WDT
  WDT_A_holdTimer();
  Timer_A_PWMConfig pwmConfig_blu = {
          TIMER_A_CLOCKSOURCE_SMCLK,      // The source clock is the system clock (3MHz)
          TIMER_A_CLOCKSOURCE_DIVIDER_1,  // The clock divider is 1
          PWM_PERIOD_CYLCLES,             // The # of counter cycles in one PWM round (period of PWM in terms of counter cycle)
          BLU_CHANNEL,                    // The output register used for OC operation. The programmer does not choose this.
          TIMER_A_OUTPUTMODE_SET_RESET,   // The OC mode. We choose this such that we can achieve the waveform we are interested in.
          BLU_COMPARE_CYCLES              // The OC value. This is the value that when the counter hits it, something happens to the waveform
  };
  initPWM();

  // The below line starts generating the pulse
  Timer_A_generatePWM(BLU_TIMER, &pwmConfig_blu);

  int i;

  while (1)
  {
      // The blocking code.
      turnOn_LaunchpadLED1();
      for (i=0; i<(BLOCK_COUNT * BLOCKING); i++);
      turnOff_LaunchpadLED1();

      // Nothing needs to be done here for handling the pulse as the Timer_A automatically takes care of it and the processor is hands off.
  }

}


