#include "adc_sample.h"
#include "adc.h"
#include "main.h"

#define ADC_SAMPLE_CHANNEL_COUNT    2u

#define ADC_SAMPLE_VOLTAGE_INDEX    0u
#define ADC_SAMPLE_CURRENT_INDEX    1u



static volatile uint16_t ADC_DmaBuffer[ADC_SAMPLE_CHANNEL_COUNT] = {0u, 0u};



void ADC_Sample_Init(void)
{

    if (hadc1.Instance != ADC1)
    {

        Error_Handler();
    }


    if (hadc1.DMA_Handle == NULL)
    {
        Error_Handler();
    }

    if (HAL_ADCEx_Calibration_Start(&hadc1) != HAL_OK)
    {
        Error_Handler();
    }


    if (HAL_ADC_Start_DMA(
            &hadc1,
            (uint32_t *)ADC_DmaBuffer,
            ADC_SAMPLE_CHANNEL_COUNT) != HAL_OK)
    {
        Error_Handler();
    }

    /* ADC DMA only updates the circular buffer.
       No DMA interrupt callbacks are required. */
    __HAL_DMA_DISABLE_IT(
        hadc1.DMA_Handle,
        DMA_IT_HT | DMA_IT_TC
    );

}



uint16_t ADC_Sample_GetVoltageRaw(void)
{
    return ADC_DmaBuffer[ADC_SAMPLE_VOLTAGE_INDEX];
}



uint16_t ADC_Sample_GetCurrentRaw(void)
{
    return ADC_DmaBuffer[ADC_SAMPLE_CURRENT_INDEX];
}


