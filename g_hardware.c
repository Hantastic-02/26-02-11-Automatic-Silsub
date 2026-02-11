#include <g_hardware.h>

/** Variables of FND **/
uint8_t             FND_number[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xD8, 0x80, 0x90};

/** Functions **/
void HW_Setting()
{
    IRQ_Setting();
    FND_Setting();
}

void IRQ_Setting()
{
    R_ICU_ExternalIrqOpen(&g_external_irq11_ctrl, &g_external_irq11_cfg);
    R_ICU_ExternalIrqOpen(&g_external_irq12_ctrl, &g_external_irq12_cfg);
    R_ICU_ExternalIrqOpen(&g_external_irq13_ctrl, &g_external_irq13_cfg);
    R_ICU_ExternalIrqOpen(&g_external_irq14_ctrl, &g_external_irq14_cfg);

    R_ICU_ExternalIrqEnable(&g_external_irq11_ctrl);
    R_ICU_ExternalIrqEnable(&g_external_irq12_ctrl);
    R_ICU_ExternalIrqEnable(&g_external_irq13_ctrl);
    R_ICU_ExternalIrqEnable(&g_external_irq14_ctrl);
}

void FND_Setting()
{
    /* 7-Segment LED Pin Output Setting */
    R_PORT3->PCNTR1_b.PDR |= (uint32_t)PODR_DIGIT_MASK;
    R_PORT6->PCNTR1_b.PDR |= (uint32_t)PODR_PIN_MASK;

    R_FND_Reset();
}

void R_FND_Reset()
{
    /* 7-Segment LED Pin State Initialization */
    R_PORT3->PCNTR1_b.PODR &= ~PODR_DIGIT_MASK & 0xFFFF;
    R_PORT6->PCNTR1_b.PODR |= PODR_PIN_MASK;
}

void R_FND_Print_Data(uint8_t double_digits)
{
    uint8_t ten = 0, one = 0;

    ten = double_digits / 10;
    one = double_digits % 10;

    R_FND_Display_Data(0, FND_number[ten]);
    R_FND_Display_Data(1, FND_number[one]);
}

void R_FND_Display_Data(uint8_t digit, uint8_t data)
{
    uint16_t high_nibble = (uint16_t)((data << PODR_INDEX_HIGH) & PODR_HIGH_MASK);
    uint16_t low_nibble = (uint16_t)((data << PODR_INDEX_LOW) & PODR_LOW_MASK);

    R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MICROSECONDS);
    R_FND_Reset();

    /* 7-Segment Digit Selection */
    R_PORT3->PCNTR1_b.PODR = (uint16_t)((0x0010 << (1 + digit)) & PODR_DIGIT_MASK);

    /* 7-Segment LED Pin State Setting */
    R_PORT6->PCNTR1_b.PODR = high_nibble | low_nibble;
}


