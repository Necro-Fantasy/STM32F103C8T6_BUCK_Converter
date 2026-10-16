#include "measurement.h"



#define MEASUREMENT_ADC_REFERENCE_VOLTAGE      3.3f
#define MEASUREMENT_ADC_FULL_SCALE             4096.0f
#define MEASUREMENT_ADC_MAX_COUNT              4095u

#define MEASUREMENT_VOLTAGE_SCALE              9.9786f
#define MEASUREMENT_CURRENT_SCALE              1.04f

#define MEASUREMENT_VOLTAGE_ZERO_RAW           56u
#define MEASUREMENT_CURRENT_ZERO_RAW           0u



static volatile float Measurement_Voltage = 0.0f;
static volatile float Measurement_Current = 0.0f;




void Measurement_Init(void)
{
    Measurement_Voltage = 0.0f;
    Measurement_Current = 0.0f;

}



static float Measurement_AdcRawToVoltage(uint16_t RawValue)
{
    uint16_t CorrectedRawValue = 0u;
    float PinVoltage = 0.0f;
    float VoltageValue = 0.0f;

    if (RawValue > MEASUREMENT_VOLTAGE_ZERO_RAW)
    {
        CorrectedRawValue = RawValue - MEASUREMENT_VOLTAGE_ZERO_RAW;
    }
    else
    {
        CorrectedRawValue = 0u;
    }

    PinVoltage = ((float)CorrectedRawValue / MEASUREMENT_ADC_FULL_SCALE)
               * MEASUREMENT_ADC_REFERENCE_VOLTAGE;

    VoltageValue = PinVoltage * MEASUREMENT_VOLTAGE_SCALE;

    return VoltageValue;
}



static float Measurement_AdcRawToCurrent(uint16_t RawValue)
{
    uint16_t CorrectedRawValue = 0u;
    float PinVoltage = 0.0f;
    float CurrentValue = 0.0f;

    if(RawValue > MEASUREMENT_CURRENT_ZERO_RAW)
    {
    	CorrectedRawValue = RawValue - MEASUREMENT_CURRENT_ZERO_RAW;
    }
    else
    {
    	CorrectedRawValue = 0u;
    }
    PinVoltage = ((float)CorrectedRawValue / MEASUREMENT_ADC_FULL_SCALE)
               * MEASUREMENT_ADC_REFERENCE_VOLTAGE;

    CurrentValue = PinVoltage * MEASUREMENT_CURRENT_SCALE;

    return CurrentValue;
}



uint16_t Measurement_VoltageToAdcRaw(float VoltageValue)
{
    uint32_t RawValue = 0u;

    if(VoltageValue <= 0.0f)
    {
    	RawValue = MEASUREMENT_VOLTAGE_ZERO_RAW;
    }

    else
    {
        RawValue =  VoltageValue / MEASUREMENT_VOLTAGE_SCALE
        		 * MEASUREMENT_ADC_FULL_SCALE
        		 / MEASUREMENT_ADC_REFERENCE_VOLTAGE
    			 + MEASUREMENT_VOLTAGE_ZERO_RAW  ;
    }

    if(RawValue > MEASUREMENT_ADC_MAX_COUNT)
    {
    	RawValue = MEASUREMENT_ADC_MAX_COUNT;
    }


    return (uint16_t)RawValue;
}





void Measurement_UpdateFromAdcRaw(uint16_t VoltageRawValue, uint16_t CurrentRawValue)
{
    Measurement_Voltage = Measurement_AdcRawToVoltage(VoltageRawValue);
    Measurement_Current = Measurement_AdcRawToCurrent(CurrentRawValue);

}



float Measurement_GetVoltage(void)
{
    return Measurement_Voltage;
}



float Measurement_GetCurrent(void)
{
    return Measurement_Current;
}


