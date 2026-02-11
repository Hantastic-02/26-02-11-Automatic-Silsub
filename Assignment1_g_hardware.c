#include <Assignment1_g_hardware.h>

/*----------------To do----------------*/
// 1. 변수 선언시 해당 위치에 변수 선언(ex. uint8_t LED1 = 1;)
volatile uint8_t LED_light1 = 1;
volatile uint32_t Motor_Enable = 0;
volatile uint32_t Gear = 0;
volatile uint32_t Timer_Period = 0x249F00; // 20[ms] Duty Cycle (50[Hz])

// 2. 함수 호출 순서에 따라 선언이 필요할때 함수 선언(ex. void IRQ_Setting(void);)
void IRQ_Setting(void);
void GPT_Setting(void);
/*-------------------------------------*/


// [FND 변수]: FND에서 사용하는 각종 변수
#define SEGMENT_INDEX         8
#define DIGIT_INDEX           4
const uint8_t number[10] = {0x81, 0xCF, 0x92, 0x86, 0xCC, 0xA4, 0xA0, 0x8D, 0x80, 0X84};
uint8_t print_data[4] = {0xFF, 0xFF, 0xFF, 0xFF};
static const bsp_io_port_pin_t segment_digit[4] = {
                                      BSP_IO_PORT_03_PIN_05,
                                      BSP_IO_PORT_03_PIN_06,
                                      BSP_IO_PORT_03_PIN_07,
                                      BSP_IO_PORT_03_PIN_08
};
static const bsp_io_port_pin_t segment_led_pin[8] = {
                                           BSP_IO_PORT_06_PIN_14,
                                           BSP_IO_PORT_06_PIN_04,
                                           BSP_IO_PORT_06_PIN_05,
                                           BSP_IO_PORT_06_PIN_06,
                                           BSP_IO_PORT_06_PIN_07,
                                           BSP_IO_PORT_06_PIN_11,
                                           BSP_IO_PORT_06_PIN_12,
                                           BSP_IO_PORT_06_PIN_13
};




// [전체 초기화]: 인터럽트, FND 등 전체 초기화 함수
void Hardware_Init(void)
{
    // 인터럽트 및 타이머 설정
    IRQ_Setting();

    // FND 초기화
    FND_Reset();

    // GPT 초기화
    GPT_Setting();
}

// [인터럽트]: 인터럽트 생성 및 활성화
void IRQ_Setting(void)
{
    /*----------------To do----------------*/
    // 1. Interrupt 동작 준비(Open)
    R_ICU_ExternalIrqOpen(&g_external_irq11_ctrl, &g_external_irq11_cfg);
    R_ICU_ExternalIrqOpen(&g_external_irq12_ctrl, &g_external_irq12_cfg);
    R_ICU_ExternalIrqOpen(&g_external_irq13_ctrl, &g_external_irq13_cfg);
    R_ICU_ExternalIrqOpen(&g_external_irq14_ctrl, &g_external_irq14_cfg);

    // 2. Interrupt 활성화(Enable)
    R_ICU_ExternalIrqEnable(&g_external_irq11_ctrl);
    R_ICU_ExternalIrqEnable(&g_external_irq12_ctrl);
    R_ICU_ExternalIrqEnable(&g_external_irq13_ctrl);
    R_ICU_ExternalIrqEnable(&g_external_irq14_ctrl);
    /*-------------------------------------*/
}

// [인터럽트 콜백 함수]: 인터럽트 입력시 버튼에 따른 출력이나 동작 실행
void R_IRQ_Interrupt(external_irq_callback_args_t *p_args)
{
    switch(p_args->channel)
    {
        /*----------------To do----------------*/
        // 1. 각 Case에 따라 각각의 위치의 FND의 값이 바뀌도록 변수 활용
        case 11:
            Motor_Enable ^= 1;
            LED_light1 ^= 1;
            R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_09_PIN_00, Motor_Enable);
            break;

        case 12:
            break;

        case 13:
            if(Gear <= 3)
                Gear += 1;
            break;

        case 14:
            if(Gear >= 1)
                Gear -= 1;
            break;
        /*-------------------------------------*/
    }
}

// [FND 초기화 및 설정 함수]: FND에 잘못된 숫자가 나오지 않도록 초기화
void FND_Reset(void)
{
    uint8_t idx = 0;

    /* 7-Segment Initialization */
    for (idx = 0; idx < SEGMENT_INDEX; idx++)
    {
        if (idx < DIGIT_INDEX)
            R_IOPORT_PinWrite(&g_ioport_ctrl, segment_digit[idx], BSP_IO_LEVEL_LOW);
        R_IOPORT_PinWrite(&g_ioport_ctrl, segment_led_pin[idx], BSP_IO_LEVEL_HIGH);
    }
}

// [7개의 FND LED 핀 제어함수]: FND의 각 Digit의 7-Segment LED Pin 제어
void FND_Display_Data(uint8_t digit, uint8_t data)
{
    uint8_t key = 0, idx = 0;

    R_BSP_SoftwareDelay(20, BSP_DELAY_UNITS_MICROSECONDS);
    FND_Reset();

    /* 7-Segment Digit Selection */
    R_IOPORT_PinWrite(&g_ioport_ctrl, segment_digit[digit], BSP_IO_LEVEL_HIGH);

    /* 7-Segment LED Pin Setting */
    for(idx = 0; idx < SEGMENT_INDEX; idx++)
    {
        key = (uint8_t)(data >> (7 - idx) & 0x01);
        R_IOPORT_PinWrite(&g_ioport_ctrl, segment_led_pin[idx], key);
    }
}

// [FND 출력 함수]: 원하는 문자열(string)을 FND에 출력하는 함수
void FND_Print_Data(uint8_t *string)
{
    uint8_t idx = 0;

    for (idx = 0; idx < DIGIT_INDEX; idx++)
        FND_Display_Data(idx, string[idx]);
}

// [GPT 초기화 및 설정 함수]: GPT를 초기화 및 설정하고, 동작시키는 함수
void GPT_Setting(void)
{
    /*----------------To do----------------*/
    // 1. GPT 설정하기(Register)
//    R_MSTP->MSTPCRD_b.MSTPD5 = 0U; // GPT32EH3 Module Stop State Cancel
//
//    R_GPT3->GTCR_b.MD = 0U; // GPT32EH3 Count Mode Setting (Saw-wave PWM Mode)
//    R_GPT3->GTCR_b.TPCS = 0U; // GPT32EH3 Clock Source Pre-scale Setting (PCLKD/1)
//
//    R_GPT3->GTPR = Timer_Period - 1; // GPT32EH3 Counting Maximum Cycle Setting
//    R_GPT3->GTCNT = 0; // GPT32EH3 Counter Initial Value Setting
//
//    R_GPT3->GTIOR_b.GTIOA = 9U; // Compare Matching Output Control Setting
//    R_GPT3->GTIOR_b.OAE = 1U; // GPIOCA Output Pin Enable
//
//    R_GPT3->GTCCR[0] = Timer_Period / 2; // 50% Duty Rate (Compare Matching Value Setting)
//
//    R_GPT3->GTCR_b.CST = 1U; // GPT32EH3 Count Start

    // 2. GPT 설정하기(Function)
    R_GPT_Open(&g_timer3_ctrl, &g_timer3_cfg);
    R_GPT_Start(&g_timer3_ctrl);
    R_GPT_DutyCycleSet(&g_timer3_ctrl, Timer_Period / 2, GPT_IO_PIN_GTIOCA);

    /*-------------------------------------*/
}
