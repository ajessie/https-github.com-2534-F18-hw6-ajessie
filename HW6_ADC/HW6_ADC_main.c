#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include "ButtonLED_HAL.h"
#include "ADC_HAL.h"
#include "graphics_HAL.h"


#define LEFT_THRESHOLD  0x1000
#define RIGHT_THRESHOLD 0x3000


// This function initializes all the peripherals except graphics
void initialize();
void ModifyLEDColor(bool leftButtonWasPushed, bool rightButtonWasPushed);

int main(void)
{

    button_t LauchpadLeftButton = {GPIO_PORT_P1, GPIO_PIN1, Stable_R, RELEASED_STATE, TIMER32_0_BASE};
    button_t LauchpadRightButton = {GPIO_PORT_P1, GPIO_PIN4, Stable_R, RELEASED_STATE, TIMER32_1_BASE};
    Graphics_Context g_sContext;

    initialize();
    InitGraphics(&g_sContext);
    draw_Base(&g_sContext);

    unsigned vx, vy;

    while (1)
    {

        bool leftButtonPushed = ButtonPushed(&LauchpadLeftButton);
        bool rightButtonPushed = ButtonPushed(&LauchpadRightButton);

        ModifyLEDColor(leftButtonPushed,rightButtonPushed);
        MoveCircle(&g_sContext, leftButtonPushed,rightButtonPushed);


        getSampleJoyStick(&vx, &vy);
        bool joyStickPushedtoRight = false;
        bool joyStickPushedtoLeft = false;

        if (vx > RIGHT_THRESHOLD)
        {
            joyStickPushedtoRight = true;
        }
        else if (vx < LEFT_THRESHOLD)
        {
            joyStickPushedtoLeft = true;
        }

        ModifyLEDColor(joyStickPushedtoLeft,joyStickPushedtoRight);
        MoveCircle(&g_sContext, joyStickPushedtoLeft,joyStickPushedtoRight);

     }
}




void initialize()
{
    // stop the watchdog timer
    WDT_A_hold(WDT_A_BASE);

    // initialize the boosterPack LEDs and turn them off except for red LED
    initialize_BoosterpackLED_red();
    initialize_BoosterpackLED_green();
    initialize_BoosterpackLED_blue();
    turnOn_BoosterpackLED_red();
    turnOff_BoosterpackLED_green();
    turnOff_BoosterpackLED_blue();

    // initialize the Launchpad buttons
    initialize_LaunchpadLeftButton();
    initialize_LaunchpadRightButton();


    // Initialize the timers needed for debouncing
    Timer32_initModule(TIMER32_0_BASE, // There are two timers, we are using the one with the index 0
                       TIMER32_PRESCALER_1, // The prescaler value is 1; The clock is not divided before feeding the counter
                       TIMER32_32BIT, // The counter is used in 32-bit mode; the alternative is 16-bit mode
                       TIMER32_PERIODIC_MODE); //This options is irrelevant for a one-shot timer

    Timer32_initModule(TIMER32_1_BASE, // There are two timers, we are using the one with the index 1
                       TIMER32_PRESCALER_1, // The prescaler value is 1; The clock is not divided before feeding the counter
                       TIMER32_32BIT, // The counter is used in 32-bit mode; the alternative is 16-bit mode
                       TIMER32_PERIODIC_MODE); //This options is irrelevant for a one-shot timer


    initADC();
    initJoyStick();
    startADC();
}

// This FSM has two inputs each of them the FSM if a button has been pushed or not
// The FSM has three states: Red, Green, Blue. The initial state is Red
// The FSM has three outputs, each output is a boolean that decides if an LED should be on or off
// When the left button is pressed, the FSM goes
void ModifyLEDColor(bool leftButtonWasPushed, bool rightButtonWasPushed)
{
    typedef enum {red, green, blue} LED_state_t;

    static LED_state_t BoosterLED = red;

    // outputs of the FSM and their default
    bool toggleGreen = false;
    bool toggleBlue = false;
    bool toggleRed = false;

    switch(BoosterLED)
    {
    case red:
        if (leftButtonWasPushed)
        {
            // next state
            BoosterLED = green;

            //outputs
            // This turns green on
            toggleGreen = true;

            // This turns red off
            toggleRed = true;
        }
        else if (rightButtonWasPushed)
        {
            BoosterLED = blue;

            //outputs
            toggleBlue = true;
            toggleRed = true;
        }
        break;
    case green:
        if (leftButtonWasPushed)
        {
            // next state
            BoosterLED = blue;

            //outputs
            toggleBlue = true;
            toggleGreen = true;
        }
        else if (rightButtonWasPushed)
        {
            BoosterLED = red;

            //outputs
            toggleRed = true;
            toggleGreen = true;
        }
        break;
    case blue:
        if (leftButtonWasPushed)
        {
            // next state
            BoosterLED = red;

            //outputs
            toggleRed = true;
            toggleBlue = true;
        }
        else if (rightButtonWasPushed)
        {
            BoosterLED = green;

            //outputs
            toggleGreen = true;
            toggleBlue = true;
        }
    }

    if (toggleRed)
        toggle_BoosterpackLED_red();

    if (toggleGreen)
        toggle_BoosterpackLED_green();

    if (toggleBlue)
        toggle_BoosterpackLED_blue();

}


