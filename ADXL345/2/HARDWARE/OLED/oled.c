#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"
#include "delay.h"
#include "stdint.h"
#include "stm32f10x_gpio.h"
typedef unsigned char u8;
/*****************辰哥单片机设计******************
											STM32
 * 文件			:	OLED显示屏c文件                     
 * 版本			: V1.0
 * 日期			: 2024.8.7
 * MCU			:	STM32F103C8T6
 * 接口			:	见代码							
 * BILIBILI	:	辰哥单片机设计
 * CSDN			:	辰哥单片机设计
 * 作者			:	辰哥 

**********************BEGIN***********************/




u8 OLED_GRAM[144][8];

// 反显函数
void OLED_ColorTurn(u8 i)
{
    if(i==0)
    {
        OLED_WR_Byte(0xA6,OLED_CMD);//正常显示
    }
    if(i==1)
    {
        OLED_WR_Byte(0xA7,OLED_CMD);//反色显示
    }
}

// 屏幕旋转 180 度
void OLED_DisplayTurn(u8 i)
{
    if(i==0)
    {
        OLED_WR_Byte(0xC8,OLED_CMD);//正常显示
        OLED_WR_Byte(0xA1,OLED_CMD);
    }
    if(i==1)
    {
        OLED_WR_Byte(0xC0,OLED_CMD);//反转显示
        OLED_WR_Byte(0xA0,OLED_CMD);
    }
}

// 延时
void IIC_delay(void)
{
    u8 t=3;
    while(t--);
}

// 起始信号
void OLED_I2C_Start(void)
{
    OLED_SDA_Set();
    OLED_SCL_Set();
    IIC_delay();
    OLED_SDA_Clr();
    IIC_delay();
    OLED_SCL_Clr();
    IIC_delay();
}

// 结束信号
void OLED_I2C_Stop(void)
{
    OLED_SDA_Clr();
    OLED_SCL_Set();
    IIC_delay();
    OLED_SDA_Set();
}

// 等待信号响应
void OLED_I2C_WaitAck(void) //测数据信号的电平
{
    OLED_SDA_Set();
    IIC_delay();
    OLED_SCL_Set();
    IIC_delay();
    OLED_SCL_Clr();
    IIC_delay();
}

// 写入一个字节
void OLED_Send_Byte(u8 dat)
{
    u8 i;
    for(i=0;i<8;i++)
    {
        if(dat&0x80)//将 dat 的 8 位从最高位依次写入
        {
            OLED_SDA_Set();
        }
        else
        {
            OLED_SDA_Clr();
        }
        IIC_delay();
        OLED_SCL_Set();
        IIC_delay();
        OLED_SCL_Clr();//将时钟信号设置为低电平
        dat<<=1;
    }
}

// 发送一个字节
// mode:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(u8 dat,u8 mode)
{
    OLED_I2C_Start();
    OLED_Send_Byte(0x78);
    OLED_I2C_WaitAck();
    if(mode){OLED_Send_Byte(0x40);}
    else{OLED_Send_Byte(0x00);}
    OLED_I2C_WaitAck();
    OLED_Send_Byte(dat);
    OLED_I2C_WaitAck();
    OLED_I2C_Stop();
}

// 开启 OLED 显示 
void OLED_DisPlay_On(void)
{
    OLED_WR_Byte(0x8D,OLED_CMD);//电荷泵使能
    OLED_WR_Byte(0x14,OLED_CMD);//开启电荷泵
    OLED_WR_Byte(0xAF,OLED_CMD);//点亮屏幕
}

// 关闭 OLED 显示 
void OLED_DisPlay_Off(void)
{
    OLED_WR_Byte(0x8D,OLED_CMD);//电荷泵使能
    OLED_WR_Byte(0x10,OLED_CMD);//关闭电荷泵
    OLED_WR_Byte(0xAE,OLED_CMD);//关闭屏幕
}

// 刷新 OLED 显示
void OLED_Refresh(void)
{
    u8 i,n;
    for(i=0;i<8;i++)
    {
        OLED_WR_Byte(0xb0+i,OLED_CMD);
        OLED_WR_Byte(0x00,OLED_CMD);
        OLED_WR_Byte(0x10,OLED_CMD);
        for(n=0;n<128;n++)
        {
            OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA);
        }
    }
}

// 清屏函数
void OLED_Clear(void)
{
    u8 i,n;
    for(i=0;i<8;i++)
    {
        for(n=0;n<128;n++)
        {
            OLED_GRAM[n][i]=0;
        }
    }
    OLED_Refresh();
}

// 画点函数
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
    u8 pos,bx,temp=0;
    if(x>127||y>63)return;
    pos = 7 - y / 8;
    bx = y % 8;
    temp = 1 << (7 - bx);
    if(t)OLED_GRAM[x][pos]|=temp;
    else OLED_GRAM[x][pos]&=~temp;
}

// 画线函数
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode)
{
    // 这里省略画线函数的具体实现，可根据需求补充
}

// 画圆函数
void OLED_DrawCircle(u8 x,u8 y,u8 r)
{
    // 这里省略画圆函数的具体实现，可根据需求补充
}

// 显示一个字符
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1,u8 mode)
{
    // 这里省略显示字符函数的具体实现，可根据需求补充
}

// 显示 6x8 字符
void OLED_ShowChar6x8(u8 x,u8 y,u8 chr,u8 mode)
{
    // 这里省略显示 6x8 字符函数的具体实现，可根据需求补充
}

// 显示字符串
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1,u8 mode)
{
    // 这里省略显示字符串函数的具体实现，可根据需求补充
}
#include <stdint.h>
// 计算 10 的 n 次方
u32 OLED_Pow(u8 n) {
    u32 result = 1;
    while(n--) result *= 10;
    return result;
}

// 显示数字
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1,u8 mode)
{  
    u8 t, temp;
    for(t = 0; t < len; t++)
    {
        temp = (num / OLED_Pow( len - t - 1)) % 10;
        if(temp == 0)
        {
            if(t == 0)
            {
                OLED_ShowChar(x + (size1 / 2) * t, y, '0', size1, mode);
            }
            else
            {
                OLED_ShowChar(x + (size1 / 2) * t, y, ' ', size1, mode);
            }
        }
        else
        {
            OLED_ShowChar(x + (size1 / 2) * t, y, temp + '0', size1, mode);
        }
    }
}

// 显示汉字
void OLED_ShowChinese(u8 x, u8 y, const unsigned char *chinese_data, u8 size1, u8 mode)
{
    u8 i, j, k;
    u8 x0 = x;
    for (i = 0; i < 16; i++)
    {
        if (size1 == 16)
        {
            for (j = 0; j < 8; j++)
            {
                k = chinese_data[i] & (0x01 << (7 - j));
                if (k) OLED_DrawPoint(x, y, mode);
                else OLED_DrawPoint(x, y, 1 - mode);
                x++;
            }
            x = x0;
            y++;
        }
    }
}

// 滚动显示
void OLED_ScrollDisplay(u8 num,u8 space,u8 mode)
{
    // 这里省略滚动显示函数的具体实现，可根据需求补充
}

// 显示图片
void OLED_ShowPicture(u8 x,u8 y,u8 sizex,u8 sizey,u8 BMP[],u8 mode)
{
    // 这里省略显示图片函数的具体实现，可根据需求补充
}

// OLED 初始化函数
void OLED_Init(void)
{
    // 初始化 GPIO 等相关设置
    // 假设已经有正确的代码来配置 OLED 相关的 GPIO 引脚
    // 例如使能时钟、设置引脚模式等

    OLED_WR_Byte(0xAE,OLED_CMD); //关闭显示
    OLED_WR_Byte(0x00,OLED_CMD); //设置低列地址
    OLED_WR_Byte(0x10,OLED_CMD); //设置高列地址
    OLED_WR_Byte(0x40,OLED_CMD); //设置起始行地址
    OLED_WR_Byte(0xB0,OLED_CMD); //设置页地址
    OLED_WR_Byte(0x81,OLED_CMD); //设置对比度控制寄存器
    OLED_WR_Byte(0xFF,OLED_CMD);
    OLED_WR_Byte(0xA1,OLED_CMD); //设置段重定义寄存器(0xa0 左右反置 0xa1 正常)
    OLED_WR_Byte(0xC8,OLED_CMD); //设置 COM 扫描方向(0xc0 上下反置 0xc8 正常)
    OLED_WR_Byte(0xA6,OLED_CMD); //正常显示
    OLED_WR_Byte(0xA8,OLED_CMD); //设置驱动路数
    OLED_WR_Byte(0x3F,OLED_CMD);
    OLED_WR_Byte(0xD3,OLED_CMD); //设置显示偏移
    OLED_WR_Byte(0x00,OLED_CMD);
    OLED_WR_Byte(0xD5,OLED_CMD); //设置显示时钟分频比/振荡器频率
    OLED_WR_Byte(0x80,OLED_CMD);
    OLED_WR_Byte(0xD9,OLED_CMD); //设置预充电周期
    OLED_WR_Byte(0xF1,OLED_CMD);
    OLED_WR_Byte(0xDA,OLED_CMD); //设置 COM 硬件引脚配置
    OLED_WR_Byte(0x12,OLED_CMD);
    OLED_WR_Byte(0xDB,OLED_CMD); //设置 VCOMH 电压倍率
    OLED_WR_Byte(0x30,OLED_CMD);
    OLED_WR_Byte(0x8D,OLED_CMD); //设置电荷泵
    OLED_WR_Byte(0x14,OLED_CMD);
    OLED_WR_Byte(0xAF,OLED_CMD); //开启显示
    OLED_Clear();
}

