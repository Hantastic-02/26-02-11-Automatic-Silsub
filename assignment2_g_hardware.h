#ifndef G_HARDWARE_H_
#define G_HARDWARE_H_

#include "hal_data.h"

/** Variables of ADC **/
extern uint16_t             potentiometer_mV;
extern uint16_t             cds_data;

/** Variables of Ethernet **/
extern volatile bool        g_ether_link_up;
extern uint8_t              g_ether_tx_buffer[1514];
extern uint8_t              myMac[6];

/********************** ADC *************************/
void ADC_Read_and_Convert(void);
void Potentiometer_Read();
void Cds_Sensor_Read();

/********************** ETH *************************/
#define MY_ETHER_TYPE           0x9000  // loopback

/********************** CAN *************************/
#define STANDARD_CAN_FRAME    8

void Send_CAN_Frame(uint32_t id, uint8_t dlc, uint8_t *data);

/** Declaration of Setting Functions **/
void HW_Setting();
void ADC_Setting();
void ETH_Setting();
void CAN_Setting();

/*----------------To do----------------*/
// 1. g_hardware.c에서 선언한 변수가 외부에서 사용되면 extern
extern volatile uint32_t Motor_Enable;          // 모터의 동작/멈춤 변수
extern volatile uint32_t Timer_Period;
extern volatile bool g_sw1_flag; // CAN 송신용 플래그
extern volatile bool g_sw2_flag; // 이더넷 송신용 플래그

// 2. g_hardware.c에서 선언한 함수 선언
void Hardware_Init(void);
void IRQ_Setting(void);
void GPT_Setting(void);
/*-------------------------------------*/

#endif /* G_HARDWARE_H_ */

