#ifndef __BUCK_CONTROL_H
#define __BUCK_CONTROL_H

#include <stdint.h>

#define BUCK_MAX_OUTPUT_STEP				    5u

uint16_t BuckControl_GetTargetVoltageMv(void);

void BuckControl_IncreaseTargetVoltage(void);
void BuckControl_DecreaseTargetVoltage(void);


/* 初始化 */
void BuckControl_Init(void);

/* 三种输出状态 */
void BuckControl_OutputOff(void);
void BuckControl_OutputReady(void);
uint8_t BuckControl_OutputRun(void);

/* 故障处理 */
void BuckControl_LatchFault(void);
void BuckControl_ClearFault(void);
uint8_t BuckControl_IsFaultLatched(void);


void BuckControl_AdjustPwmClosedLoop(void);

#endif


