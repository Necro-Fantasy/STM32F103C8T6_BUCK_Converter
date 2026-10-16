#ifndef __KEY_H
#define __KEY_H

#include "main.h"

typedef enum
{
    KEY_NONE = 0,
    KEY_ADD,
    KEY_REDUCE,
    KEY_SET,
    KEY_MODE
} KeyCode_t;


void Key_Init(void);
void Key_Scan1ms(void);

KeyCode_t Key_GetEvent(void);

#endif /* __KEY_H */


