#ifndef __APP_STATE_H
#define __APP_STATE_H


#include "key.h"

typedef enum
{
    APP_OFF = 0,
    APP_READY,
    APP_RUN,
    APP_FAULT
} AppState_t;

/* 初始化应用状态机 */
void AppState_Init(void);

/* 获取当前应用状态 */
AppState_t AppState_Get(void);

/* 状态机周期更新 */
void AppState_HandleKeyEvent(KeyCode_t KeyCode);

#endif /* __APP_STATE_H */

