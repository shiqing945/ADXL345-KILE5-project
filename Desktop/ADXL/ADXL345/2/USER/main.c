#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"	
#include "OLED.h"
#include "string.h" 	
#include "adxl345.h"
#include "stm32f10x_rcc.h"

/*****************���絥Ƭ�����******************
											STM32
 * ��Ŀ			:	ADXL345���ٶȴ�����ʵ��                   
 * �汾			: V1.0
 * ����			: 2024.8.15
 * MCU			:	STM32F103C8T6
 * �ӿ�			:	��adxl345.h							
 * BILIBILI	:	���絥Ƭ�����
 * CSDN			:	���絥Ƭ�����
 * ����			:	����

**********************BEGIN***********************/
volatile uint32_t sys_tick_count=0;
void SysTick_Init(void) {  
    if (SysTick_Config(SystemCoreClock / 1000)) { // ���� SysTick ÿ 1ms �ж�һ��  
        while (1); // ��ʼ��ʧ�������  
    }  
}  

void Configure_System_Clock() {
    // ʹ�� HSE
    RCC_HSEConfig(RCC_HSE_ON);
    while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);
    // ���� PLL
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
    RCC_PLLCmd(ENABLE);
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
    // ѡ�� PLL ��Ϊϵͳʱ��
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
}
float x_angle,y_angle,z_angle;

int main(void) {
    unsigned char p[16] = " ";
	  Configure_System_Clock() ;
    delay_init(72);
    LED_Init();
    OLED_Init();
    delay_ms(50);
    OLED_Clear();

    // ��ʾ����������
    OLED_ShowChinese(0, 0, chinese_bu1616, 16, 1);
    OLED_ShowChinese(16, 0, chinese_shu1616, 16, 1);
    OLED_ShowChar(40, 0, ':', 16, 1);

    ADXL345_Init();
    delay_ms(1000);
    USART1_Config();

    while (1) {
        detect_steps();
        int steps = get_step_count();
        // �����������
        printf("Steps: %d\n", steps);
        // ��ʽ����������Ϊ�ַ���
        sprintf((char*)p, "%d  ", steps);
        // �� OLED ����ʾ����
        OLED_ShowString(0, 40, p, 16, 1);
        OLED_Refresh();
        delay_ms(100);
    }
}
void assert_failed(uint8_t *file, uint32_t line) {  
    while (1) {  
        // ���ڴ˴���ӵ��Դ��루��ͨ�����ڴ�ӡ�����ļ����кţ�  
        // Ŀǰ���ó�����𣬱���δ������Ŵ���  
    }  
}  