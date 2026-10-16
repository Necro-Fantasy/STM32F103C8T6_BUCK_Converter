#ifndef __BUCK_PWM_H
#define __BUCK_PWM_H

#include <stdint.h>

#define BUCK_PWM_ARR                  			3599u
#define BUCK_PWM_MAX_DUTY_PERCENT     			95u

#define BUCK_PWM_MAX_COMPARE \
    ((uint16_t)(((BUCK_PWM_ARR + 1u) * BUCK_PWM_MAX_DUTY_PERCENT) / 100u))
#define BUCK_PWM_MIN_COMPARE 					0u

void BuckPwm_Init(void);
void BuckPwm_SetCompare(uint16_t CompareValue);
void BuckPwm_OutputOff(void);

#endif
