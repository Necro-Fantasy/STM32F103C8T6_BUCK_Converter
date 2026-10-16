#include "pid.h"


void BuckPID_Init(BuckPID_t * PID)
{
	PID -> Kp = 0.0f;
	PID -> Ki = 1.0f;
	PID -> Kd = 0.0f;

	PID -> SampleTimeMs = 1.0f;
	PID -> IntegralTimeMs = 150.0f;
	PID -> DerivativeTimeMs = 2.5f;

    BuckPID_Reset(PID);
}



void BuckPID_Reset(BuckPID_t * PID)
{
	PID -> CurrentADCError = 0.0f;
	PID -> PreviousADCError = 0.0f;
	PID -> PreviousADCError2 = 0.0f;

	PID -> POutput = 0.0f;
	PID -> IOutput = 0.0f;
	PID -> DOutput = 0.0f;

	PID -> OutputStep = 0.0f;
	PID -> Output = 0.0f;
}



uint16_t BuckPID_Update(BuckPID_t *PID,
                        uint16_t TargetADCRaw,
                        uint16_t ActualADCRaw,
                        uint16_t MaxOutputStep,
						uint16_t MaxOutput)
{

	PID -> CurrentADCError = (float)TargetADCRaw - (float)ActualADCRaw;

	PID -> POutput = PID -> Kp * (PID -> CurrentADCError - PID -> PreviousADCError);
	PID -> IOutput = PID -> Ki * PID -> CurrentADCError * PID -> SampleTimeMs
								/ PID -> IntegralTimeMs;
	PID -> DOutput = PID -> Kd * PID -> DerivativeTimeMs / PID -> SampleTimeMs
						 * (PID -> CurrentADCError
						 -	2.0f * PID -> PreviousADCError
						 +	PID -> PreviousADCError2);

	PID -> OutputStep = PID -> POutput + PID -> IOutput + PID -> DOutput;

	if (PID -> OutputStep > (float)MaxOutputStep)
	{
		PID -> OutputStep = (float)MaxOutputStep;
	}
	else if (PID -> OutputStep < -(float)MaxOutputStep)
	{
		PID -> OutputStep = -(float)MaxOutputStep;
	}

	PID -> Output = PID -> Output + PID -> OutputStep;

	if (PID -> Output > (float)MaxOutput)
	{
		PID -> Output = (float)MaxOutput;
	}
	else if (PID -> Output < 0.0f)
	{
		PID -> Output = 0.0f;
	}

	PID -> PreviousADCError2 = PID -> PreviousADCError;
	PID -> PreviousADCError = PID -> CurrentADCError;

	return PID -> Output;
}
