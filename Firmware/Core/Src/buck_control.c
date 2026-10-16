#include "buck_control.h"
#include "buck_pwm.h"
#include "power_stage.h"
#include "measurement.h"
#include "adc_sample.h"
#include "FreeRTOS.h"
#include "task.h"
#include "pid.h"


#define BUCK_TARGET_VOLTAGE_MIN_MV           500u
#define BUCK_TARGET_VOLTAGE_MAX_MV         11500u
#define BUCK_TARGET_VOLTAGE_STEP_MV          100u
#define BUCK_TARGET_VOLTAGE_DEFAULT_MV      3300u


static volatile uint8_t ControlEnabled = 0u;
static volatile uint8_t FaultLatched = 0u;

static volatile uint16_t CurrentBuckPwmCompareValue = 0u;

static volatile uint16_t TargetVoltageMv = BUCK_TARGET_VOLTAGE_DEFAULT_MV;

static BuckPID_t BuckPID;



/*Initialization*/
void BuckControl_Init(void)
{
	ControlEnabled = 0u;
	FaultLatched = 0u;
	CurrentBuckPwmCompareValue = 0u;


	TargetVoltageMv = BUCK_TARGET_VOLTAGE_DEFAULT_MV;

	BuckPwm_OutputOff();
	BuckPID_Init(&BuckPID);
}
/****************************************/




/*State OFF*/
void BuckControl_OutputOff(void)
{
	ControlEnabled = 0u;
	CurrentBuckPwmCompareValue = 0u;


	BuckPwm_OutputOff();
	PowerStage_DriverOff();
	PowerStage_ProtectionOn();
}
/****************************************/



/*State Ready*/
void BuckControl_OutputReady(void)
{
	ControlEnabled = 0u;
	CurrentBuckPwmCompareValue = 0u;

	BuckPwm_OutputOff();
	PowerStage_DriverOff();
	PowerStage_ProtectionOff();
}
/****************************************/



/*State Run*/
uint8_t BuckControl_OutputRun(void)
{
    uint8_t RunStarted = 0u;

    ControlEnabled = 0u;

    CurrentBuckPwmCompareValue = 0u;
    BuckPwm_OutputOff();

    BuckPID_Reset(&BuckPID);

    taskENTER_CRITICAL();

    if(FaultLatched == 0u)
    {
        PowerStage_ProtectionOff();
        PowerStage_DriverOn();

        ControlEnabled = 1u;
        RunStarted = 1u;
    }

    taskEXIT_CRITICAL();

    return RunStarted;
}
/****************************************/




/*LatchFault*/
void BuckControl_LatchFault(void)
{
	FaultLatched = 1u;

	BuckControl_OutputOff();
}
/****************************************/



/*ClearFault*/
void BuckControl_ClearFault(void)
{
	FaultLatched = 0u;
}
/****************************************/



/*Get Fault State*/
uint8_t BuckControl_IsFaultLatched(void)
{
    return FaultLatched;
}
/****************************************/






/*Set Target Voltage (mv)*/
uint16_t BuckControl_GetTargetVoltageMv(void)
{
    return TargetVoltageMv;
}
/****************************************/



/*ADD or REDUCE Target Voltage(showed on the OLED)*/
void BuckControl_IncreaseTargetVoltage(void)
{
    if(TargetVoltageMv <=
       (BUCK_TARGET_VOLTAGE_MAX_MV -
        BUCK_TARGET_VOLTAGE_STEP_MV))
    {
        TargetVoltageMv += BUCK_TARGET_VOLTAGE_STEP_MV;
    }
    else
    {
        TargetVoltageMv = BUCK_TARGET_VOLTAGE_MAX_MV;
    }
}


void BuckControl_DecreaseTargetVoltage(void)
{
    if(TargetVoltageMv >=
       (BUCK_TARGET_VOLTAGE_MIN_MV +
        BUCK_TARGET_VOLTAGE_STEP_MV))
    {
        TargetVoltageMv -= BUCK_TARGET_VOLTAGE_STEP_MV;
    }
    else
    {
        TargetVoltageMv = BUCK_TARGET_VOLTAGE_MIN_MV;
    }
}
/****************************************/



/* Adjust Closed Loop PWM*/
void BuckControl_AdjustPwmClosedLoop(void)
{
    float TargetVoltage;

    uint16_t TargetVoltageADCRaw;
    uint16_t CurrentVoltageADCRaw;
    uint16_t NextPwmCompareValue;

    if((ControlEnabled == 0u) || (FaultLatched != 0u))
    {
        return;
    }

    TargetVoltage = (float)TargetVoltageMv / 1000.0f;

    TargetVoltageADCRaw = Measurement_VoltageToAdcRaw(TargetVoltage);

    CurrentVoltageADCRaw = ADC_Sample_GetVoltageRaw();

    NextPwmCompareValue = BuckPID_Update(&BuckPID,
    									 TargetVoltageADCRaw,
										 CurrentVoltageADCRaw,
										 BUCK_MAX_OUTPUT_STEP,
										 BUCK_PWM_MAX_COMPARE);

    taskENTER_CRITICAL();

    if((ControlEnabled != 0u) && (FaultLatched == 0u))
    {
        CurrentBuckPwmCompareValue = NextPwmCompareValue;

        BuckPwm_SetCompare(CurrentBuckPwmCompareValue);
    }

    taskEXIT_CRITICAL();
}
/****************************************/


