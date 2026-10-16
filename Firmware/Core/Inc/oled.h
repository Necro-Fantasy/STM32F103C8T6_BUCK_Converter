#ifndef __OLED_H
#define __OLED_H

#include "main.h"
#include <stdint.h>

/* 兼容原OLED驱动里的旧类型 */
typedef uint8_t  u8;
typedef uint32_t u32;

#define OLED_MODE      0
#define SIZE           8
#define XLevelL        0x00
#define XLevelH        0x10
#define Max_Column     128
#define Max_Row        64
#define Brightness     0xFF
#define X_WIDTH        128
#define Y_WIDTH        64


/* OLED software I2C GPIO */
#define OLED_SCLK_Clr() \
    HAL_GPIO_WritePin(OLED_SCL_GPIO_Port, OLED_SCL_Pin, GPIO_PIN_RESET)

#define OLED_SCLK_Set() \
    HAL_GPIO_WritePin(OLED_SCL_GPIO_Port, OLED_SCL_Pin, GPIO_PIN_SET)

#define OLED_SDIN_Clr() \
    HAL_GPIO_WritePin(OLED_SDA_GPIO_Port, OLED_SDA_Pin, GPIO_PIN_RESET)

#define OLED_SDIN_Set() \
    HAL_GPIO_WritePin(OLED_SDA_GPIO_Port, OLED_SDA_Pin, GPIO_PIN_SET)



#define OLED_CMD       0u
#define OLED_DATA      1u


void OLED_WR_Byte(unsigned dat, unsigned cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Init(void);
void OLED_Clear(void);

void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 Char_Size);
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size);

void OLED_ShowString(u8 x,
                     u8 y,
                     const char *p,
                     u8 Char_Size);

void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x, u8 y, u8 no);

void OLED_DrawBMP(unsigned char x0,
                  unsigned char y0,
                  unsigned char x1,
                  unsigned char y1,
                  unsigned char BMP[]);

void fill_picture(unsigned char fill_Data);

void IIC_Start(void);
void IIC_Stop(void);
void IIC_Wait_Ack(void);
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);
void Write_IIC_Byte(unsigned char IIC_Byte);

#endif

