#ifndef __PID_H
#define __PID_H

#include <stdint.h>


typedef struct
{
    float Kp;
    float Ki;
    float Kd;

    float SampleTimeMs;
    float IntegralTimeMs;
    float DerivativeTimeMs;

    float CurrentADCError;
    float PreviousADCError;
    float PreviousADCError2;

    float POutput;
    float IOutput;
    float DOutput;

    float OutputStep;
    float Output;

} BuckPID_t;



void BuckPID_Init(BuckPID_t *PID);

void BuckPID_Reset(BuckPID_t *PID);

uint16_t BuckPID_Update(BuckPID_t *PID,
                        uint16_t TargetADCRaw,
                        uint16_t ActualADCRaw,
                        uint16_t MaxOutputStep,
						uint16_t MaxOutput);


#endif
