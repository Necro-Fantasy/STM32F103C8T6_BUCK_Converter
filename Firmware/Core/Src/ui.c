#include "ui.h"

#include "oled.h"
#include "app_state.h"
#include "buck_control.h"
#include "measurement.h"



/****************************************
 * Show App State
 ****************************************/
static void UI_ShowState(void)
{
    AppState_t State;

    State = AppState_Get();

    switch(State)
    {
        case APP_OFF:
            OLED_ShowString(48, 0, "OFF   ", 16);
            break;

        case APP_READY:
            OLED_ShowString(48, 0, "READY ", 16);
            break;

        case APP_RUN:
            OLED_ShowString(48, 0, "RUN   ", 16);
            break;

        case APP_FAULT:
            OLED_ShowString(48, 0, "FAULT ", 16);
            break;

        default:
            OLED_ShowString(48, 0, "ERROR ", 16);
            break;
    }
}


/****************************************
 * Show Voltage
 *
 * Example:
 * 3300mV  -> 03.30V
 * 11500mV -> 11.50V
 ****************************************/
static void UI_ShowVoltageMv(uint8_t x,
                             uint8_t y,
                             uint16_t VoltageMv)
{
    uint16_t IntegerPart;
    uint16_t DecimalPart;

    IntegerPart = VoltageMv / 1000u;

    DecimalPart =
            (VoltageMv % 1000u) / 10u;

    OLED_ShowNum(x,
                 y,
                 IntegerPart,
                 2,
                 16);

    OLED_ShowString(x + 16u,
                    y,
                    ".",
                    16);

    OLED_ShowNum(x + 24u,
                 y,
                 DecimalPart,
                 2,
                 16);

    OLED_ShowString(x + 40u,
                    y,
                    "V",
                    16);
}


/****************************************
 * Show Current
 *
 * Example:
 * 1250mA -> 01.25A
 ****************************************/
static void UI_ShowCurrentMa(uint8_t x,
                             uint8_t y,
                             uint16_t CurrentMa)
{
    uint16_t IntegerPart;
    uint16_t DecimalPart;

    IntegerPart = CurrentMa / 1000u;

    DecimalPart =
            (CurrentMa % 1000u) / 10u;

    OLED_ShowNum(x,
                 y,
                 IntegerPart,
                 2,
                 16);

    OLED_ShowString(x + 16u,
                    y,
                    ".",
                    16);

    OLED_ShowNum(x + 24u,
                 y,
                 DecimalPart,
                 2,
                 16);

    OLED_ShowString(x + 40u,
                    y,
                    "A",
                    16);
}


/****************************************
 * UI Initialization
 ****************************************/
void UI_Init(void)
{
    OLED_Init();
    OLED_Clear();


    /* Fixed Labels */

    OLED_ShowString(0, 0,
                    "State:",
                    16);

    OLED_ShowString(0, 2,
                    "Target:",
                    16);

    OLED_ShowString(0, 4,
                    "Voltage:",
                    16);

    OLED_ShowString(0, 6,
                    "Current:",
                    16);



    /* Initial State */

    UI_ShowState();


    /* Initial Target Voltage */
    UI_ShowVoltageMv(
            56,
            2,
            BuckControl_GetTargetVoltageMv());


    /* Initial Output Voltage */

    UI_ShowVoltageMv(
            64,
            4,
            0u);


    /* Initial Output Current */

    UI_ShowCurrentMa(
            64,
            6,
            0u);
}


/****************************************
 * UI Periodic Update
 *
 * Called periodically by uiTask.
 * Actual refresh period is limited to 200ms.
 ****************************************/
void UI_Update(void)
{

    float Voltage;
    float Current;

    uint32_t VoltageMv;
    uint32_t CurrentMa;


    /************************************
     * State
     ************************************/
    UI_ShowState();


    /************************************
     * Target Voltage
     ************************************/
    UI_ShowVoltageMv(
            56,
            2,
            BuckControl_GetTargetVoltageMv());


    /************************************
     * Measured Output Voltage
     ************************************/
    Voltage = Measurement_GetVoltage();

    if(Voltage < 0.0f)
    {
        Voltage = 0.0f;
    }

    if(Voltage > 99.99f)
    {
        Voltage = 99.99f;
    }


    VoltageMv =
            (uint16_t)(Voltage * 1000.0f + 0.5f);


    UI_ShowVoltageMv(
            64,
            4,
            VoltageMv);


    /************************************
     * Measured Output Current
     ************************************/
    Current = Measurement_GetCurrent();

    if(Current < 0.0f)
    {
        Current = 0.0f;
    }

    if(Current > 99.99f)
    {
        Current = 99.99f;
    }


    CurrentMa =
            (uint16_t)(Current * 1000.0f + 0.5f);


    UI_ShowCurrentMa(
            64,
            6,
            CurrentMa);
}
