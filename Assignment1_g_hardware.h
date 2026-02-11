#ifndef ASSIGNMENT1_G_HARDWARE_H_
#define ASSIGNMENT1_G_HARDWARE_H_

#include "hal_data.h"

/*----------------To do----------------*/
// 1. g_hardware.c에서 선언한 변수가 외부에서 사용되면 extern
extern uint8_t print_data[4];
extern const uint8_t number[10];
extern volatile uint32_t Motor_Enable;          // 모터의 동작/멈춤 변수
extern volatile uint8_t LED_light1;             // 1번 LED 값
extern volatile uint32_t Gear;                  // 기어 값
extern volatile uint32_t Timer_Period;

// 2. g_hardware.c에서 선언한 함수 선언
void Hardware_Init(void);
void IRQ_Setting(void);
void FND_Display_Data(uint8_t digit, uint8_t data);
void FND_Print_Data(uint8_t *string);
void FND_Reset(void);
void GPT_Setting(void);
/*-------------------------------------*/

#endif


