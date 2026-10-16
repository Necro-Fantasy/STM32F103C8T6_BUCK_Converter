#include "power_stage.h"



void PowerStage_DriverOn(void)
{
    HAL_GPIO_WritePin(PWM_SD_GPIO_Port, PWM_SD_Pin, GPIO_PIN_SET);
}



void PowerStage_DriverOff(void)
{
    HAL_GPIO_WritePin(PWM_SD_GPIO_Port, PWM_SD_Pin, GPIO_PIN_RESET);
}



void PowerStage_ProtectionOn(void)
{
    /*
     * PA10 low level enables protection.
     */
    HAL_GPIO_WritePin(PROTECT_GPIO_Port, PROTECT_Pin, GPIO_PIN_RESET);
}



void PowerStage_ProtectionOff(void)
{
    /*
     * PA10 high level releases protection.
     */
    HAL_GPIO_WritePin(PROTECT_GPIO_Port, PROTECT_Pin, GPIO_PIN_SET);
}



static void PowerStage_OutputSafeState(void)
{
    PowerStage_DriverOff();
    PowerStage_ProtectionOn();
}



void PowerStage_Init(void)
{
    PowerStage_OutputSafeState();
}

