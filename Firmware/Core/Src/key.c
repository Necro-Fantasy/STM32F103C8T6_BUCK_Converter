#include "key.h"

#define KEY_DEBOUNCE_TIME_MS    20u


/* 等待 AppState 读取的按键事件 */
static KeyCode_t PendingKeyEvent = KEY_NONE;


/* ADD 按键状态 */
static uint8_t KeyAddStablePressed = 0u;
static uint8_t KeyAddDebounceCount = 0u;

/* REDUCE 按键状态 */
static uint8_t KeyReduceStablePressed = 0u;
static uint8_t KeyReduceDebounceCount = 0u;

/* SET 按键状态 */
static uint8_t KeySetStablePressed = 0u;
static uint8_t KeySetDebounceCount = 0u;

/* MODE 按键状态 */
static uint8_t KeyModeStablePressed = 0u;
static uint8_t KeyModeDebounceCount = 0u;



void Key_Init(void)
{
    PendingKeyEvent = KEY_NONE;

    KeyAddStablePressed = 0u;
    KeyAddDebounceCount = 0u;

    KeyReduceStablePressed = 0u;
    KeyReduceDebounceCount = 0u;

    KeySetStablePressed = 0u;
    KeySetDebounceCount = 0u;

    KeyModeStablePressed = 0u;
    KeyModeDebounceCount = 0u;
}



void Key_Scan1ms(void)
{
    uint8_t RawPressed;


    /* ==================== ADD ==================== */

    if(HAL_GPIO_ReadPin(KEY_ADD_GPIO_Port, KEY_ADD_Pin) == GPIO_PIN_RESET)
    {
        RawPressed = 1u;
    }
    else
    {
        RawPressed = 0u;
    }

    if(RawPressed == KeyAddStablePressed)
    {
        KeyAddDebounceCount = 0u;
    }
    else
    {
        KeyAddDebounceCount++;

        if(KeyAddDebounceCount >= KEY_DEBOUNCE_TIME_MS)
        {
            KeyAddDebounceCount = 0u;

            KeyAddStablePressed = RawPressed;

            if((RawPressed != 0u) &&
               (PendingKeyEvent == KEY_NONE))
            {
                PendingKeyEvent = KEY_ADD;
            }
        }
    }



    /* ==================== REDUCE ==================== */

    if(HAL_GPIO_ReadPin(KEY_REDUCE_GPIO_Port, KEY_REDUCE_Pin) == GPIO_PIN_RESET)
    {
        RawPressed = 1u;
    }
    else
    {
        RawPressed = 0u;
    }

    if(RawPressed == KeyReduceStablePressed)
    {
        KeyReduceDebounceCount = 0u;
    }
    else
    {
        KeyReduceDebounceCount++;

        if(KeyReduceDebounceCount >= KEY_DEBOUNCE_TIME_MS)
        {
            KeyReduceDebounceCount = 0u;

            KeyReduceStablePressed = RawPressed;

            if((RawPressed != 0u) &&
               (PendingKeyEvent == KEY_NONE))
            {
                PendingKeyEvent = KEY_REDUCE;
            }
        }
    }



    /* ==================== SET ==================== */

    if(HAL_GPIO_ReadPin(KEY_SET_GPIO_Port, KEY_SET_Pin) == GPIO_PIN_RESET)
    {
        RawPressed = 1u;
    }
    else
    {
        RawPressed = 0u;
    }

    if(RawPressed == KeySetStablePressed)
    {
        KeySetDebounceCount = 0u;
    }
    else
    {
        KeySetDebounceCount++;

        if(KeySetDebounceCount >= KEY_DEBOUNCE_TIME_MS)
        {
            KeySetDebounceCount = 0u;

            KeySetStablePressed = RawPressed;

            if((RawPressed != 0u) &&
               (PendingKeyEvent == KEY_NONE))
            {
                PendingKeyEvent = KEY_SET;
            }
        }
    }



    /* ==================== MODE ==================== */

    if(HAL_GPIO_ReadPin(KEY_MODE_GPIO_Port, KEY_MODE_Pin) == GPIO_PIN_RESET)
    {
        RawPressed = 1u;
    }
    else
    {
        RawPressed = 0u;
    }

    if(RawPressed == KeyModeStablePressed)
    {
        KeyModeDebounceCount = 0u;
    }
    else
    {
        KeyModeDebounceCount++;

        if(KeyModeDebounceCount >= KEY_DEBOUNCE_TIME_MS)
        {
            KeyModeDebounceCount = 0u;

            KeyModeStablePressed = RawPressed;

            if((RawPressed != 0u) &&
               (PendingKeyEvent == KEY_NONE))
            {
                PendingKeyEvent = KEY_MODE;
            }
        }
    }
}



KeyCode_t Key_GetEvent(void)
{
    KeyCode_t KeyEvent;

    KeyEvent = PendingKeyEvent;

    PendingKeyEvent = KEY_NONE;

    return KeyEvent;
}

