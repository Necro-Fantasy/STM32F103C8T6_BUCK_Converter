#ifndef __ADC_SAMPLE_H
#define __ADC_SAMPLE_H

#include <stdint.h>

void ADC_Sample_Init(void);

uint16_t ADC_Sample_GetVoltageRaw(void);
uint16_t ADC_Sample_GetCurrentRaw(void);


#endif
