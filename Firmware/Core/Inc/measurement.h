#ifndef __MEASUREMENT_H
#define __MEASUREMENT_H

#include <stdint.h>



void Measurement_Init(void);

void Measurement_UpdateFromAdcRaw(uint16_t VoltageRawValue, uint16_t CurrentRawValue);

float Measurement_GetVoltage(void);
float Measurement_GetCurrent(void);


uint16_t Measurement_VoltageToAdcRaw(float VoltageValue);



#endif
