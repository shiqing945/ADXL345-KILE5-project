#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"
#include "delay.h"
#include "stdint.h"
#include "stm32f10x_gpio.h"
typedef unsigned char u8;
/*****************���絥Ƭ�����******************
											STM32
 * �ļ�			:	OLED��ʾ��c�ļ�                     
 * �汾			: V1.0
 * ����			: 2024.8.7
 * MCU			:	STM32F103C8T6
 * �ӿ�			:	������							
 * BILIBILI	:	���絥Ƭ�����
 * CSDN			:	���絥Ƭ�����
 * ����			:	���� 

**********************BEGIN***********************/




u8 OLED_GRAM[144][8];

// ���Ժ���
void OLED_ColorTurn(u8 i)
{
    if(i==0)
    {
        OLED_WR_Byte(0xA6,OLED_CMD);//������ʾ
    }
    if(i==1)
    {
        OLED_WR_Byte(0xA7,OLED_CMD);//��ɫ��ʾ
    }
}

// ��Ļ��ת 180 ��
void OLED_DisplayTurn(u8 i)
{
    if(i==0)
    {
        OLED_WR_Byte(0xC8,OLED_CMD);//������ʾ
        OLED_WR_Byte(0xA1,OLED_CMD);
    }
    if(i==1)
    {
        OLED_WR_Byte(0xC0,OLED_CMD);//��ת��ʾ
        OLED_WR_Byte(0xA0,OLED_CMD);
    }
}

// ��ʱ
void IIC_delay(void)
{
    u8 t=3;
    while(t--);
}

// ��ʼ�ź�
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

// �����ź�
void OLED_I2C_Stop(void)
{
    OLED_SDA_Clr();
    OLED_SCL_Set();
    IIC_delay();
    OLED_SDA_Set();
}

// �ȴ��ź���Ӧ
void OLED_I2C_WaitAck(void) //�������źŵĵ�ƽ
{
    OLED_SDA_Set();
    IIC_delay();
    OLED_SCL_Set();
    IIC_delay();
    OLED_SCL_Clr();
    IIC_delay();
}

// д��һ���ֽ�
void OLED_Send_Byte(u8 dat)
{
    u8 i;
    for(i=0;i<8;i++)
    {
        if(dat&0x80)//�� dat �� 8 λ�����λ����д��
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
        OLED_SCL_Clr();//��ʱ���ź�����Ϊ�͵�ƽ
        dat<<=1;
    }
}

// ����һ���ֽ�
// mode:����/�����־ 0,��ʾ����;1,��ʾ����;
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

// ���� OLED ��ʾ 
void OLED_DisPlay_On(void)
{
    OLED_WR_Byte(0x8D,OLED_CMD);//��ɱ�ʹ��
    OLED_WR_Byte(0x14,OLED_CMD);//������ɱ�
    OLED_WR_Byte(0xAF,OLED_CMD);//������Ļ
}

// �ر� OLED ��ʾ 
void OLED_DisPlay_Off(void)
{
    OLED_WR_Byte(0x8D,OLED_CMD);//��ɱ�ʹ��
    OLED_WR_Byte(0x10,OLED_CMD);//�رյ�ɱ�
    OLED_WR_Byte(0xAE,OLED_CMD);//�ر���Ļ
}

// ˢ�� OLED ��ʾ
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

// ��������
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

// ���㺯��
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

// ���ߺ���
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode)
{
    // ����ʡ�Ի��ߺ����ľ���ʵ�֣��ɸ������󲹳�
}

// ��Բ����
void OLED_DrawCircle(u8 x,u8 y,u8 r)
{
    // ����ʡ�Ի�Բ�����ľ���ʵ�֣��ɸ������󲹳�
}

// ��ʾһ���ַ�
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1,u8 mode)
{
    // ����ʡ����ʾ�ַ������ľ���ʵ�֣��ɸ������󲹳�
}

// ��ʾ 6x8 �ַ�
void OLED_ShowChar6x8(u8 x,u8 y,u8 chr,u8 mode)
{
    // ����ʡ����ʾ 6x8 �ַ������ľ���ʵ�֣��ɸ������󲹳�
}

// ��ʾ�ַ���
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1,u8 mode)
{
    // ����ʡ����ʾ�ַ��������ľ���ʵ�֣��ɸ������󲹳�
}
#include <stdint.h>
// ���� 10 �� n �η�
u32 OLED_Pow(u8 n) {
    u32 result = 1;
    while(n--) result *= 10;
    return result;
}

// ��ʾ����
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

// ��ʾ����
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

// ������ʾ
void OLED_ScrollDisplay(u8 num,u8 space,u8 mode)
{
    // ����ʡ�Թ�����ʾ�����ľ���ʵ�֣��ɸ������󲹳�
}

// ��ʾͼƬ
void OLED_ShowPicture(u8 x,u8 y,u8 sizex,u8 sizey,u8 BMP[],u8 mode)
{
    // ����ʡ����ʾͼƬ�����ľ���ʵ�֣��ɸ������󲹳�
}

// OLED ��ʼ������
void OLED_Init(void)
{
    // ��ʼ�� GPIO ���������
    // �����Ѿ�����ȷ�Ĵ��������� OLED ��ص� GPIO ����
    // ����ʹ��ʱ�ӡ���������ģʽ��

    OLED_WR_Byte(0xAE,OLED_CMD); //�ر���ʾ
    OLED_WR_Byte(0x00,OLED_CMD); //���õ��е�ַ
    OLED_WR_Byte(0x10,OLED_CMD); //���ø��е�ַ
    OLED_WR_Byte(0x40,OLED_CMD); //������ʼ�е�ַ
    OLED_WR_Byte(0xB0,OLED_CMD); //����ҳ��ַ
    OLED_WR_Byte(0x81,OLED_CMD); //���öԱȶȿ��ƼĴ���
    OLED_WR_Byte(0xFF,OLED_CMD);
    OLED_WR_Byte(0xA1,OLED_CMD); //���ö��ض���Ĵ���(0xa0 ���ҷ��� 0xa1 ����)
    OLED_WR_Byte(0xC8,OLED_CMD); //���� COM ɨ�跽��(0xc0 ���·��� 0xc8 ����)
    OLED_WR_Byte(0xA6,OLED_CMD); //������ʾ
    OLED_WR_Byte(0xA8,OLED_CMD); //��������·��
    OLED_WR_Byte(0x3F,OLED_CMD);
    OLED_WR_Byte(0xD3,OLED_CMD); //������ʾƫ��
    OLED_WR_Byte(0x00,OLED_CMD);
    OLED_WR_Byte(0xD5,OLED_CMD); //������ʾʱ�ӷ�Ƶ��/����Ƶ��
    OLED_WR_Byte(0x80,OLED_CMD);
    OLED_WR_Byte(0xD9,OLED_CMD); //����Ԥ�������
    OLED_WR_Byte(0xF1,OLED_CMD);
    OLED_WR_Byte(0xDA,OLED_CMD); //���� COM Ӳ����������
    OLED_WR_Byte(0x12,OLED_CMD);
    OLED_WR_Byte(0xDB,OLED_CMD); //���� VCOMH ��ѹ����
    OLED_WR_Byte(0x30,OLED_CMD);
    OLED_WR_Byte(0x8D,OLED_CMD); //���õ�ɱ�
    OLED_WR_Byte(0x14,OLED_CMD);
    OLED_WR_Byte(0xAF,OLED_CMD); //������ʾ
    OLED_Clear();
}

