#include "buck_pwm.h"
#include "tim.h"
#include "main.h"



void BuckPwm_Init(void)
{
    BuckPwm_OutputOff();

    if (HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1) != HAL_OK)
    {
        Error_Handler();
    }

    BuckPwm_OutputOff();

}



void BuckPwm_SetCompare(uint16_t CompareValue)
{
    if (CompareValue > BUCK_PWM_MAX_COMPARE)
    {
        CompareValue = BUCK_PWM_MAX_COMPARE;
    }

    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, CompareValue);
}



void BuckPwm_OutputOff(void)
{
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0u);
}
