#include "oled.h"
#include "oledfont.h"


/************************************************
 * Software I2C Start
 ************************************************/
void IIC_Start(void)
{
    OLED_SCLK_Set();
    OLED_SDIN_Set();

    OLED_SDIN_Clr();
    OLED_SCLK_Clr();
}


/************************************************
 * Software I2C Stop
 ************************************************/
void IIC_Stop(void)
{
    OLED_SCLK_Set();

    OLED_SDIN_Clr();
    OLED_SDIN_Set();
}


/************************************************
 * Software I2C Wait ACK
 *
 * 原驱动没有真正读取 ACK，
 * 这里只保留原来的时钟动作。
 ************************************************/
void IIC_Wait_Ack(void)
{
    OLED_SCLK_Set();
    OLED_SCLK_Clr();
}


/************************************************
 * Software I2C Write Byte
 ************************************************/
void Write_IIC_Byte(unsigned char IIC_Byte)
{
    unsigned char i;
    unsigned char Data;

    Data = IIC_Byte;

    OLED_SCLK_Clr();

    for(i = 0u; i < 8u; i++)
    {
        if((Data & 0x80u) != 0u)
        {
            OLED_SDIN_Set();
        }
        else
        {
            OLED_SDIN_Clr();
        }

        Data <<= 1;

        OLED_SCLK_Set();
        OLED_SCLK_Clr();
    }
}


/************************************************
 * Write OLED Command
 ************************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
    IIC_Start();

    Write_IIC_Byte(0x78u);
    IIC_Wait_Ack();

    Write_IIC_Byte(0x00u);
    IIC_Wait_Ack();

    Write_IIC_Byte(IIC_Command);
    IIC_Wait_Ack();

    IIC_Stop();
}


/************************************************
 * Write OLED Data
 ************************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
    IIC_Start();

    Write_IIC_Byte(0x78u);
    IIC_Wait_Ack();

    Write_IIC_Byte(0x40u);
    IIC_Wait_Ack();

    Write_IIC_Byte(IIC_Data);
    IIC_Wait_Ack();

    IIC_Stop();
}


/************************************************
 * OLED Write Byte
 ************************************************/
void OLED_WR_Byte(unsigned dat, unsigned cmd)
{
    if(cmd != 0u)
    {
        Write_IIC_Data((unsigned char)dat);
    }
    else
    {
        Write_IIC_Command((unsigned char)dat);
    }
}


/************************************************
 * Fill Screen
 ************************************************/
void fill_picture(unsigned char fill_Data)
{
    unsigned char Page;
    unsigned char Column;

    for(Page = 0u; Page < 8u; Page++)
    {
        OLED_WR_Byte(0xB0u + Page, OLED_CMD);
        OLED_WR_Byte(0x00u, OLED_CMD);
        OLED_WR_Byte(0x10u, OLED_CMD);

        for(Column = 0u; Column < 128u; Column++)
        {
            OLED_WR_Byte(fill_Data, OLED_DATA);
        }
    }
}


/************************************************
 * Compatibility Delay Functions
 ************************************************/
void Delay_50ms(unsigned int Del_50ms)
{
    while(Del_50ms > 0u)
    {
        HAL_Delay(50u);
        Del_50ms--;
    }
}


void Delay_1ms(unsigned int Del_1ms)
{
    HAL_Delay(Del_1ms);
}


/************************************************
 * Set OLED Position
 *
 * x: 0~127
 * y: Page 0~7
 ************************************************/
void OLED_Set_Pos(unsigned char x, unsigned char y)
{
    OLED_WR_Byte(0xB0u + y, OLED_CMD);
    OLED_WR_Byte(((x & 0xF0u) >> 4u) | 0x10u, OLED_CMD);
    OLED_WR_Byte(x & 0x0Fu, OLED_CMD);
}


/************************************************
 * OLED Display ON
 ************************************************/
void OLED_Display_On(void)
{
    OLED_WR_Byte(0x8Du, OLED_CMD);
    OLED_WR_Byte(0x14u, OLED_CMD);
    OLED_WR_Byte(0xAFu, OLED_CMD);
}


/************************************************
 * OLED Display OFF
 ************************************************/
void OLED_Display_Off(void)
{
    OLED_WR_Byte(0x8Du, OLED_CMD);
    OLED_WR_Byte(0x10u, OLED_CMD);
    OLED_WR_Byte(0xAEu, OLED_CMD);
}


/************************************************
 * Clear OLED
 ************************************************/
void OLED_Clear(void)
{
    u8 Page;
    u8 Column;

    for(Page = 0u; Page < 8u; Page++)
    {
        OLED_WR_Byte(0xB0u + Page, OLED_CMD);
        OLED_WR_Byte(0x00u, OLED_CMD);
        OLED_WR_Byte(0x10u, OLED_CMD);

        for(Column = 0u; Column < 128u; Column++)
        {
            OLED_WR_Byte(0x00u, OLED_DATA);
        }
    }
}


/************************************************
 * Fill OLED With Pixels
 ************************************************/
void OLED_On(void)
{
    u8 Page;
    u8 Column;

    for(Page = 0u; Page < 8u; Page++)
    {
        OLED_WR_Byte(0xB0u + Page, OLED_CMD);
        OLED_WR_Byte(0x00u, OLED_CMD);
        OLED_WR_Byte(0x10u, OLED_CMD);

        for(Column = 0u; Column < 128u; Column++)
        {
            OLED_WR_Byte(0x01u, OLED_DATA);
        }
    }
}


/************************************************
 * Show One Character
 *
 * Char_Size:
 * 16 -> 8x16
 * other -> 6x8
 ************************************************/
void OLED_ShowChar(u8 x,
                   u8 y,
                   u8 chr,
                   u8 Char_Size)
{
    unsigned char CharacterIndex;
    unsigned char i;

    CharacterIndex = chr - ' ';

    if(x > (Max_Column - 1u))
    {
        x = 0u;
        y += 2u;
    }

    if(Char_Size == 16u)
    {
        OLED_Set_Pos(x, y);

        for(i = 0u; i < 8u; i++)
        {
            OLED_WR_Byte(
                F8X16[CharacterIndex * 16u + i],
                OLED_DATA);
        }

        OLED_Set_Pos(x, y + 1u);

        for(i = 0u; i < 8u; i++)
        {
            OLED_WR_Byte(
                F8X16[CharacterIndex * 16u + i + 8u],
                OLED_DATA);
        }
    }
    else
    {
        OLED_Set_Pos(x, y);

        for(i = 0u; i < 6u; i++)
        {
        	OLED_WR_Byte(
        	    F6x8[CharacterIndex * 6u + i],
        	    OLED_DATA);
        }
    }
}


/************************************************
 * Power Function For OLED_ShowNum
 ************************************************/
static u32 OLED_Pow(u8 Base, u8 Power)
{
    u32 Result = 1u;

    while(Power > 0u)
    {
        Result *= Base;
        Power--;
    }

    return Result;
}


/************************************************
 * Show Integer Number
 ************************************************/
void OLED_ShowNum(u8 x,
                  u8 y,
                  u32 num,
                  u8 len,
                  u8 size2)
{
    u8 Position;
    u8 Digit;
    u8 ShowStarted = 0u;

    for(Position = 0u;
        Position < len;
        Position++)
    {
        Digit =
            (num /
             OLED_Pow(10u,
                      len - Position - 1u))
            % 10u;

        if((ShowStarted == 0u) &&
           (Position < (len - 1u)))
        {
            if(Digit == 0u)
            {
                /*
                 * 保持原程序行为：
                 * 前导位置显示0，而不是空格。
                 */
                OLED_ShowChar(
                    x + (size2 / 2u) * Position,
                    y,
                    '0',
                    size2);

                continue;
            }
            else
            {
                ShowStarted = 1u;
            }
        }

        OLED_ShowChar(
            x + (size2 / 2u) * Position,
            y,
            Digit + '0',
            size2);
    }
}


/************************************************
 * Show String
 ************************************************/
void OLED_ShowString(u8 x,
                     u8 y,
                     const char *chr,
                     u8 Char_Size)
{
    unsigned char Index = 0u;

    while(chr[Index] != '\0')
    {
        OLED_ShowChar(
            x,
            y,
            (u8)chr[Index],
            Char_Size);

        x += 8u;

        if(x > 120u)
        {
            x = 0u;
            y += 2u;
        }

        Index++;
    }
}


/************************************************
 * Show Chinese Character
 ************************************************/
void OLED_ShowCHinese(u8 x,
                      u8 y,
                      u8 no)
{
    u8 i;

    OLED_Set_Pos(x, y);

    for(i = 0u; i < 16u; i++)
    {
        OLED_WR_Byte(
            Hzk[2u * no][i],
            OLED_DATA);
    }

    OLED_Set_Pos(x, y + 1u);

    for(i = 0u; i < 16u; i++)
    {
        OLED_WR_Byte(
            Hzk[2u * no + 1u][i],
            OLED_DATA);
    }
}


/************************************************
 * Draw BMP
 ************************************************/
void OLED_DrawBMP(unsigned char x0,
                  unsigned char y0,
                  unsigned char x1,
                  unsigned char y1,
                  unsigned char BMP[])
{
    unsigned int Index = 0u;

    unsigned char x;
    unsigned char y;

    for(y = y0; y < y1; y++)
    {
        OLED_Set_Pos(x0, y);

        for(x = x0; x < x1; x++)
        {
            OLED_WR_Byte(
                BMP[Index],
                OLED_DATA);

            Index++;
        }
    }
}


/************************************************
 * SSD1306 Initialization
 ************************************************/
void OLED_Init(void)
{
    /*
     * GPIO本身由CubeMX的MX_GPIO_Init()完成配置。
     *
     * PA5 -> OLED_SDA
     * PA6 -> OLED_SCL
     */

    OLED_SCLK_Set();
    OLED_SDIN_Set();

    HAL_Delay(800u);


    /* Display OFF */
    OLED_WR_Byte(0xAEu, OLED_CMD);


    /* Set Column Address */
    OLED_WR_Byte(0x00u, OLED_CMD);
    OLED_WR_Byte(0x10u, OLED_CMD);


    /* Set Start Line */
    OLED_WR_Byte(0x40u, OLED_CMD);


    /* Set Page Address */
    OLED_WR_Byte(0xB0u, OLED_CMD);


    /* Contrast Control */
    OLED_WR_Byte(0x81u, OLED_CMD);
    OLED_WR_Byte(0xFFu, OLED_CMD);


    /* Segment Remap */
    OLED_WR_Byte(0xA1u, OLED_CMD);


    /* Normal Display */
    OLED_WR_Byte(0xA6u, OLED_CMD);


    /* Multiplex Ratio */
    OLED_WR_Byte(0xA8u, OLED_CMD);
    OLED_WR_Byte(0x3Fu, OLED_CMD);


    /* COM Scan Direction */
    OLED_WR_Byte(0xC8u, OLED_CMD);


    /* Display Offset */
    OLED_WR_Byte(0xD3u, OLED_CMD);
    OLED_WR_Byte(0x00u, OLED_CMD);


    /* Oscillator Frequency */
    OLED_WR_Byte(0xD5u, OLED_CMD);
    OLED_WR_Byte(0x80u, OLED_CMD);


    /* Area Color Mode */
    OLED_WR_Byte(0xD8u, OLED_CMD);
    OLED_WR_Byte(0x05u, OLED_CMD);


    /* Pre-Charge Period */
    OLED_WR_Byte(0xD9u, OLED_CMD);
    OLED_WR_Byte(0xF1u, OLED_CMD);


    /* COM Pins Configuration */
    OLED_WR_Byte(0xDAu, OLED_CMD);
    OLED_WR_Byte(0x12u, OLED_CMD);


    /* VCOMH */
    OLED_WR_Byte(0xDBu, OLED_CMD);
    OLED_WR_Byte(0x30u, OLED_CMD);


    /* Charge Pump */
    OLED_WR_Byte(0x8Du, OLED_CMD);
    OLED_WR_Byte(0x14u, OLED_CMD);


    /* Display ON */
    OLED_WR_Byte(0xAFu, OLED_CMD);


    OLED_Clear();
}
