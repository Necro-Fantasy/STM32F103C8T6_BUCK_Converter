#include "app_state.h"
#include "buck_control.h"
#include "protection.h"
#include "adc_sample.h"
#include "key.h"



static volatile AppState_t CurrentState = APP_OFF;


void AppState_Init(void)
{
    CurrentState = APP_OFF;

    BuckControl_OutputOff();
}




AppState_t AppState_Get(void)
{
    if(BuckControl_IsFaultLatched() != 0u)
    {
        return APP_FAULT;
    }

    return CurrentState;
}





static void AppState_Set(AppState_t NewState)
{

	switch (NewState) {
	case APP_OFF:
		BuckControl_OutputOff();
		CurrentState = APP_OFF;
		break;

	case APP_READY:
		BuckControl_OutputReady();
		CurrentState = APP_READY;
		break;

	case APP_RUN:
		CurrentState = APP_RUN;

		if(BuckControl_OutputRun() == 0u)
		{
		    CurrentState = APP_READY;
		}
		break;


	default:
		BuckControl_OutputOff();
		CurrentState = APP_OFF;
		break;
	}

}






void AppState_HandleKeyEvent(KeyCode_t KeyCode)
{
	AppState_t State;

	State = AppState_Get();

    switch(State)
    {
    case APP_OFF:

        if(KeyCode == KEY_SET)
        {
            AppState_Set(APP_READY);
        }
        else if(KeyCode == KEY_ADD)
        {
            BuckControl_IncreaseTargetVoltage();
        }
        else if(KeyCode == KEY_REDUCE)
        {
            BuckControl_DecreaseTargetVoltage();
        }

        break;


    case APP_READY:

        if(KeyCode == KEY_SET)
        {
            AppState_Set(APP_RUN);
        }
        else if(KeyCode == KEY_MODE)
        {
            AppState_Set(APP_OFF);
        }
        else if(KeyCode == KEY_ADD)
        {
            BuckControl_IncreaseTargetVoltage();
        }
        else if(KeyCode == KEY_REDUCE)
        {
            BuckControl_DecreaseTargetVoltage();
        }

        break;


    case APP_RUN:

        if(KeyCode == KEY_SET)
        {
            AppState_Set(APP_READY);
        }
        else if(KeyCode == KEY_MODE)
        {
            AppState_Set(APP_OFF);
        }
        else if(KeyCode == KEY_ADD)
        {
            BuckControl_IncreaseTargetVoltage();
        }
        else if(KeyCode == KEY_REDUCE)
        {
            BuckControl_DecreaseTargetVoltage();
        }

        break;


    case APP_FAULT:

        /*
         * 这里暂时保留原来的 Fault recovery，
         * 下一步再讨论它是否应该继续放这里。
         */
        if(KeyCode == KEY_SET)
        {
            uint16_t ADCVoltageRawValue;
            uint16_t ADCCurrentRawValue;

            ADCVoltageRawValue = ADC_Sample_GetVoltageRaw();
            ADCCurrentRawValue = ADC_Sample_GetCurrentRaw();

            if(Protection_IsFaultDetected(
                    ADCVoltageRawValue,
                    ADCCurrentRawValue) == 0u)
            {
            	AppState_Set(APP_READY);
            	BuckControl_ClearFault();
            }
        }

        break;


    default:

        AppState_Set(APP_OFF);
        break;
    }
}



