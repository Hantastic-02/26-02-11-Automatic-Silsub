#include <g_hardware.h>

/*----------------To do----------------*/
// 1. 변수 선언시 해당 위치에 변수 선언
volatile uint32_t Motor_Enable = 1;
volatile uint32_t Gear = 0;
volatile uint32_t Timer_Period = 0x249F00; // 20[ms] Duty Cycle (50[Hz])

// 2. 함수 호출 순서에 따라 선언이 필요할때 함수 선언(ex. void IRQ_Setting(void);)
void IRQ_Setting(void);
void GPT_Setting(void);
/*-------------------------------------*/

/** Variables of ADC **/
const float         ADC_CONST = (float)(3.3/4096);
uint16_t            potentiometer_mV = 0, potentiometer_Ra = 0, potentiometer_Rb = 0;
uint16_t            cds_data = 0;

/** Variables of Ethernet **/
volatile bool       g_ether_link_up = false;  // Global Link Status Flag
uint8_t             g_ether_tx_buffer[1514];
uint8_t             myMac[6] = {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};

/** Variables of CAN **/
uint8_t             canData[20];
can_frame_t         g_can_tx_frame;
can_frame_t         g_can_rx_frame;

volatile bool g_sw1_flag = false;
volatile bool g_sw2_flag = false;

/** Functions **/
void HW_Setting()
{
    ADC_Setting();
    ETH_Setting();
    CAN_Setting();
}

// [전체 초기화]: 인터럽트, FND 등 전체 초기화 함수
void Hardware_Init(void)
{
    // 인터럽트 및 타이머 설정
    IRQ_Setting();

    // GPT 초기화
    GPT_Setting();

    R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_09_PIN_00, Motor_Enable);
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
            g_sw1_flag = true; // 깃발 들기
            break;

        case 12:
            g_sw2_flag = true; // 깃발 들기
            break;

        case 13:
            break;

        case 14:
            break;
        /*-------------------------------------*/
    }
}

void ADC_Setting()
{
    R_ADC_Open(&g_adc0_ctrl, &g_adc0_cfg);
    R_ADC_ScanCfg(&g_adc0_ctrl, &g_adc0_channel_cfg);
}

void ADC_Read_and_Convert(void)
{
    adc_status_t status;

    R_ADC_ScanStart(&g_adc0_ctrl);
    status.state = ADC_STATE_SCAN_IN_PROGRESS;
    while (ADC_STATE_SCAN_IN_PROGRESS == status.state) {
        R_ADC_StatusGet(&g_adc0_ctrl, &status);
    }

    Potentiometer_Read();
    Cds_Sensor_Read();
}

void Potentiometer_Read()
{
    uint16_t ch0_adc_result;

    R_ADC_Read(&g_adc0_ctrl, ADC_CHANNEL_0, &ch0_adc_result);
    potentiometer_mV = (uint16_t)(ch0_adc_result * ADC_CONST * 1000);
    potentiometer_Rb = (uint16_t)(potentiometer_mV * 3.0303);
    potentiometer_Ra = (uint16_t)(10000 - potentiometer_Rb);
}

void Cds_Sensor_Read()
{
    uint16_t ch2_adc_result;

    R_ADC_Read(&g_adc0_ctrl, ADC_CHANNEL_2, &ch2_adc_result);
    cds_data = ch2_adc_result;
}

void ETH_Setting()
{
    R_ETHER_Open(&g_ether0_ctrl, &g_ether0_cfg);
}

void CAN_Setting()
{
    R_CAN_Open(&g_can0_ctrl, &g_can0_cfg);
}

void Send_CAN_Frame(uint32_t id, uint8_t dlc, uint8_t *data)
{
    g_can_tx_frame.id = id;
    g_can_tx_frame.id_mode = CAN_ID_MODE_STANDARD;
    g_can_tx_frame.type = CAN_FRAME_TYPE_DATA;

    if (dlc > 8)
        dlc = 8;
    g_can_tx_frame.data_length_code = dlc;

    if (data != NULL)
        memcpy(g_can_tx_frame.data, data, dlc);
    else
        memset(g_can_tx_frame.data, 0, 8);

    if (dlc < 8)
        memset(&g_can_tx_frame.data[dlc], 0, (8 - dlc));

    R_CAN_Write(&g_can0_ctrl, 0, &g_can_tx_frame);
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
    R_GPT_DutyCycleSet(&g_timer3_ctrl, 0, GPT_IO_PIN_GTIOCA);

    /*-------------------------------------*/
}
