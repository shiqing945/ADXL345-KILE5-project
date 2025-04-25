#include "adxl345.h"
#include "delay.h"
#include "math.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "core_cm3.h"
#include <stdint.h>
extern volatile uint32_t sys_tick_count;
/*****************���絥Ƭ�����******************
											STM32
 * �ļ�			:	ADXL345���ٶȴ�����c�ļ�                   
 * �汾			: V1.0
 * ����			: 2024.8.15
 * MCU			:	STM32F103C8T6
 * �ӿ�			:	��ADXL345.h�ļ�							
 * BILIBILI	:	���絥Ƭ�����
 * CSDN			:	���絥Ƭ�����
 * ����			:	����

**********************BEGIN***********************/			

//ADXL345�������ģʽ����
void ADXL345_IIC_SDA_OUT(void)//SDA�����������
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	GPIO_InitStructure.GPIO_Pin=ADXL345_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//SDA�������
	GPIO_Init(ADXL345_SDA_PORT,&GPIO_InitStructure); 						

}

void ADXL345_IIC_SDA_IN(void)//SDA���뷽������
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	GPIO_InitStructure.GPIO_Pin=ADXL345_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//SCL��������
	GPIO_Init(ADXL345_SDA_PORT,&GPIO_InitStructure);
	
}
//����Ϊģ��IIC���ߺ���
void ADXL345_IIC_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(ADXL345_SCL_GPIO_CLK, ENABLE);	 	//ʹ��SCL�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = ADXL345_SCL_PIN;					//����Ϊ�������,SCL
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 		//IO���ٶ�Ϊ50MHz
	GPIO_Init(ADXL345_SCL_PORT, &GPIO_InitStructure);				
	GPIO_SetBits(ADXL345_SCL_PORT,ADXL345_SCL_PIN); 
	
	RCC_APB2PeriphClockCmd(ADXL345_SDA_GPIO_CLK, ENABLE);	 	//ʹ��SDA�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = ADXL345_SDA_PIN;					//����Ϊ�������,SDA
	GPIO_SetBits(ADXL345_SDA_PORT,ADXL345_SDA_PIN); 
	
//	I2C_SCL_H;
//	I2C_SDA_H;//������
}
void ADXL345_IIC_Start()
{
	ADXL345_IIC_SDA_OUT();
	ADXL345_IIC_SDA=1;	  	  
	ADXL345_IIC_SCL=1;
	delay_us(5);
	ADXL345_IIC_SDA=0;
	delay_us(5);
	ADXL345_IIC_SCL=0;
}
void ADXL345_IIC_Stop()
{
	ADXL345_IIC_SDA_OUT();
	ADXL345_IIC_SCL=0;
	ADXL345_IIC_SDA=0;
	delay_us(5);
	ADXL345_IIC_SCL=1; 
	ADXL345_IIC_SDA=1;
	delay_us(5);
}
//��������һ��Ӧ���ź�
void ADXL345_IIC_Ack()
{
	ADXL345_IIC_SCL=0;
	ADXL345_IIC_SDA_OUT();
	ADXL345_IIC_SDA=0;
	
	delay_us(2);
	ADXL345_IIC_SCL=1;
	delay_us(5);
	ADXL345_IIC_SCL=0;	
}
//����������Ӧ���ź�
void ADXL345_IIC_NAck()
{
	ADXL345_IIC_SCL=0;
	ADXL345_IIC_SDA_OUT();
	ADXL345_IIC_SDA=1;
	delay_us(2);
	ADXL345_IIC_SCL=1;
	delay_us(2);
	ADXL345_IIC_SCL=0;
}
//�ȴ��ӻ�Ӧ���ź�
//����ֵ��1 ����Ӧ��ʧ��
//		  0 ����Ӧ��ɹ�
u8 ADXL345_IIC_Wait_Ack()
{
	u8 tempTime=0;
	ADXL345_IIC_SDA_IN();
	ADXL345_IIC_SDA=1;
	delay_us(1);
	ADXL345_IIC_SCL=1;
	delay_us(1);

	while(ADXL345_READ_SDA)
	{
		tempTime++;
		if(tempTime>250)
		{
			ADXL345_IIC_Stop();
			return 1;
		}	 
	}

	ADXL345_IIC_SCL=0;
	return 0;
}
void ADXL345_IIC_Send_Byte(u8 txd)
{
	u8 i=0;
	ADXL345_IIC_SDA_OUT();
	ADXL345_IIC_SCL=0;;//����ʱ�ӿ�ʼ���ݴ���
	for(i=0;i<8;i++)
	{
		ADXL345_IIC_SDA=(txd&0x80)>>7;//��ȡ�ֽ�
		txd<<=1;
		ADXL345_IIC_SCL=1;
		delay_us(2); //��������
		ADXL345_IIC_SCL=0;
		delay_us(2);
	}
}
//��ȡһ���ֽ�
u8 ADXL345_IIC_Read_Byte(u8 ack)
{
	u8 i=0,receive=0;
	ADXL345_IIC_SDA_IN();
   for(i=0;i<8;i++)
   {
			ADXL345_IIC_SCL=0;
			delay_us(2);
			ADXL345_IIC_SCL=1;
			receive<<=1;//����
			if(ADXL345_READ_SDA)
			receive++;//������ȡ��λ
			delay_us(1);	
   }

   	if(!ack)
	   	ADXL345_IIC_NAck();
	else
		ADXL345_IIC_Ack();

	return receive;//���ض�ȡ�����ֽ�
}



//��������ʼ��
void ADXL345_Init()
{
		ADXL345_IIC_Init();
	
		adxl345_write_reg(0X31,0X0B);		//�͵�ƽ�ж����,13λȫ�ֱ���,��������Ҷ���,16g���� 
		adxl345_write_reg(0x2C,0x0B);		//��������ٶ�Ϊ100Hz
		adxl345_write_reg(0x2D,0x08);		//����ʹ��,����ģʽ,ʡ������
		adxl345_write_reg(0X2E,0x80);		//��ʹ���ж�		 
	 	adxl345_write_reg(0X1E,0x00);
		adxl345_write_reg(0X1F,0x00);
		adxl345_write_reg(0X20,0x05);	
	
	
}
//д�Ĵ�������
void adxl345_write_reg(u8 addr,u8 val) 
{
	ADXL345_IIC_Start();  				 
	ADXL345_IIC_Send_Byte(slaveaddress);     	//����д����ָ��	 
	ADXL345_IIC_Wait_Ack();	   
  ADXL345_IIC_Send_Byte(addr);   			//���ͼĴ�����ַ
	ADXL345_IIC_Wait_Ack(); 	 										  		   
	ADXL345_IIC_Send_Byte(val);     		//����ֵ					   
	ADXL345_IIC_Wait_Ack();  		    	   
	ADXL345_IIC_Stop();						//����һ��ֹͣ���� 	   
}
//���Ĵ�������
u8 adxl345_read_reg(u8 addr)
{
	u8 temp=0;		 
	ADXL345_IIC_Start();  				 
	ADXL345_IIC_Send_Byte(slaveaddress);	//����д����ָ��	 
	temp=ADXL345_IIC_Wait_Ack();	   
    ADXL345_IIC_Send_Byte(addr);   		//���ͼĴ�����ַ
	temp=ADXL345_IIC_Wait_Ack(); 	 										  		   
	ADXL345_IIC_Start();  	 	   		//��������
	ADXL345_IIC_Send_Byte(regaddress);	//���Ͷ�����ָ��	 
	temp=ADXL345_IIC_Wait_Ack();	   
    temp=ADXL345_IIC_Read_Byte(0);		//��ȡһ���ֽ�,�������ٶ�,����NAK 	    	   
    ADXL345_IIC_Stop();					//����һ��ֹͣ���� 	    
	return temp;
}
//��ȡ���ݺ���
void adxl345_read_data(short *x,short *y,short *z)
{
	u8 buf[6];
	u8 i;
	ADXL345_IIC_Start();  				 
	ADXL345_IIC_Send_Byte(slaveaddress);	//����д����ָ��	 
	ADXL345_IIC_Wait_Ack();	   
    ADXL345_IIC_Send_Byte(0x32);   		//���ͼĴ�����ַ(���ݻ������ʼ��ַΪ0X32)
	ADXL345_IIC_Wait_Ack(); 	 										  		   
 
 	ADXL345_IIC_Start();  	 	   		//��������
	ADXL345_IIC_Send_Byte(regaddress);	//���Ͷ�����ָ��
	ADXL345_IIC_Wait_Ack();
	for(i=0;i<6;i++)
	{
		if(i==5)buf[i]=ADXL345_IIC_Read_Byte(0);//��ȡһ���ֽ�,�������ٶ�,����NACK  
		else buf[i]=ADXL345_IIC_Read_Byte(1);	//��ȡһ���ֽ�,������,����ACK 
 	}	        	   
    ADXL345_IIC_Stop();					//����һ��ֹͣ����
	*x=(short)(((u16)buf[1]<<8)+buf[0]); 	//�ϳ�����    
	*y=(short)(((u16)buf[3]<<8)+buf[2]); 	    
	*z=(short)(((u16)buf[5]<<8)+buf[4]); 
}
//������ȡ����ȡƽ��ֵ����
//times ȡƽ��ֵ�Ĵ���
void adxl345_read_average(float *x,float *y,float *z,u8 times)
{
	u8 i;
	short tx,ty,tz;
	*x=0;
	*y=0;
	*z=0;
	if(times)//��ȡ������Ϊ0
	{
		for(i=0;i<times;i++)//������ȡtimes��
		{
			adxl345_read_data(&tx,&ty,&tz);
			*x+=tx;
			*y+=ty;
			*z+=tz;
			delay_ms(5);
		}
		*x/=times;
		*y/=times;
		*z/=times;
	}
}
void get_angle(float *x_angle,float *y_angle,float *z_angle)
{
	float ax,ay,az;
	adxl345_read_average(&ax,&ay,&az,10);
	*x_angle=atan(ax/sqrt((az*az+ay*ay)))*180/3.14;
	*y_angle=atan(ay/sqrt((ax*ax+az*az)))*180/3.14;
	*z_angle=atan(sqrt((ax*ax+ay*ay)/az))*180/3.14;
//return x_angle;
}
// ������������¼����
static int step_count = 0;
// ������������¼��һ�μ�⵽������ʱ��
static uint32_t last_step_time = 0;





// ��ⲽ���ĺ���
void detect_steps(void) {
    short x, y, z;
    adxl345_read_data(&x, &y, &z);
    // ����ϼ��ٶ�
    float magnitude = sqrt((float)(x * x + y * y + z * z));
    // ���ݾ������ü��ٶ���ֵ��ʱ������ֵ�������ʵ�ʲ��Ե���
    const float acceleration_threshold = 1000; 
    const uint32_t time_threshold = 200; 
     uint32_t current_time = sys_tick_count; // ԭ HAL_GetTick() �滻Ϊ sys_tick_count  
    if (magnitude > acceleration_threshold && current_time - last_step_time > time_threshold) {  
        step_count++;  
        last_step_time = current_time;  
    }  
}  

// ��ȡ��ǰ�����ĺ���
int get_step_count(void) {
    return step_count;
}