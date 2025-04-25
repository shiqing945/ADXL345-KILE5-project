#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"	
#include "OLED.h"
#include "string.h" 	
#include "adxl345.h"
#include "stm32f10x_rcc.h"

/*****************辰哥单片机设计******************
											STM32
 * 项目			:	ADXL345加速度传感器实验                   
 * 版本			: V1.0
 * 日期			: 2024.8.15
 * MCU			:	STM32F103C8T6
 * 接口			:	见adxl345.h							
 * BILIBILI	:	辰哥单片机设计
 * CSDN			:	辰哥单片机设计
 * 作者			:	辰哥

**********************BEGIN***********************/
volatile uint32_t sys_tick_count=0;
void SysTick_Init(void) {  
    if (SysTick_Config(SystemCoreClock / 1000)) { // 配置 SysTick 每 1ms 中断一次  
        while (1); // 初始化失败则挂起  
    }  
}  

void Configure_System_Clock() {
    // 使能 HSE
    RCC_HSEConfig(RCC_HSE_ON);
    while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);
    // 配置 PLL
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
    RCC_PLLCmd(ENABLE);
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
    // 选择 PLL 作为系统时钟
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

    // 显示“步数：”
    OLED_ShowChinese(0, 0, chinese_bu1616, 16, 1);
    OLED_ShowChinese(16, 0, chinese_shu1616, 16, 1);
    OLED_ShowChar(40, 0, ':', 16, 1);

    ADXL345_Init();
    delay_ms(1000);
    USART1_Config();

    while (1) {
        detect_steps();
        int steps = get_step_count();
        // 串口输出步数
        printf("Steps: %d\n", steps);
        // 格式化步数数据为字符串
        sprintf((char*)p, "%d  ", steps);
        // 在 OLED 上显示步数
        OLED_ShowString(0, 40, p, 16, 1);
        OLED_Refresh();
        delay_ms(100);
    }
}
void assert_failed(uint8_t *file, uint32_t line) {  
    while (1) {  
        // 可在此处添加调试代码（如通过串口打印错误文件和行号）  
        // 目前先让程序挂起，避免未定义符号错误  
    }  
}  