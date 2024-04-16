/*
 * lcddisplay.h
 *
 *  Created on: Apr 2, 2024
 *      Author: yehor
 */

#ifndef INC_LCDDISPLAY_H_
#define INC_LCDDISPLAY_H_

#include "stm32f1xx_hal.h"

// Address should be shifted 1 bit to the left when used in HAL_I2C_Master_Transmit (<< 1)
#define LCDADDR (0x7C>>1)
#define LCDBACKLIGHTADDR 0x6B

#define COMMAND 0x80
#define DATA 0x40

static I2C_HandleTypeDef *hi2c1Ptr_;

static uint8_t LCDdisplayOn_;
static uint8_t LCDcursorOn_;
static uint8_t LCDblinkOn_;

// INIT i2c ptr!
void LCDinit(I2C_HandleTypeDef *hi2c1Ptr__);

void LCDwrite(uint8_t ch);
void LCDwriteText(uint8_t *text);

void LCDsetCursorPos(uint8_t row, uint8_t col);

void LCDsetDisplay(uint8_t isOn);
void LCDsetCursor(uint8_t isOn);
void LCDsetBlink(uint8_t isOn);

void LCDclear();

void LCDsend_(uint8_t control, uint8_t data, uint8_t delay);
void LCDBacklightsend_(uint8_t control, uint8_t data, uint8_t delay);

void LCDsetBacklight(uint8_t isOn);

#endif /* INC_LCDDISPLAY_H_ */
