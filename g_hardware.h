#ifndef G_HARDWARE_H_
#define G_HARDWARE_H_

#include "hal_data.h"

/********************** FND *************************/
#define SEGMENT_INDEX         8
#define DIGIT_INDEX           4

#define CLOCK_INDEX           4
#define CLOCK_COLUMN          0x7F

#define PODR_INDEX_HIGH       7
#define PODR_INDEX_LOW        4
#define PODR_DIGIT_MASK       0x03E0
#define PODR_HIGH_MASK        0x7800
#define PODR_LOW_MASK         0x00F0
#define PODR_PIN_MASK         PODR_HIGH_MASK | PODR_LOW_MASK

void R_FND_Reset();
void R_FND_Print_Data(uint8_t double_digits);
void R_FND_Display_Data(uint8_t digit, uint8_t data);

/** Declaration of Setting Functions **/
void HW_Setting();
void IRQ_Setting();
void FND_Setting();

#endif /* G_HARDWARE_H_ */
