/*
 * lcddisplay.c
 *
 *  Created on: Apr 2, 2024
 *      Author: yehor
 */

#include "stm32f1xx_hal.h"

#include "lcddisplay.h"

// Should add 100us delay right after control byte and right after data byte
void LCDsend_(uint8_t control, uint8_t data, uint8_t delay) {
	uint8_t toSend[2] = {control, data};
	HAL_I2C_Master_Transmit(hi2c1Ptr_, LCDADDR<<1, toSend, 2, HAL_MAX_DELAY);
	HAL_Delay(delay);
}

// Maybe rewrite using one function
void LCDBacklightsend_(uint8_t control, uint8_t data, uint8_t delay) {
	uint8_t toSend[2] = {control, data};
	HAL_I2C_Master_Transmit(hi2c1Ptr_, LCDBACKLIGHTADDR<<1, toSend, 2, HAL_MAX_DELAY);
	HAL_Delay(delay);
}

void LCDinit(I2C_HandleTypeDef *hi2c1Ptr__) {
	hi2c1Ptr_ = hi2c1Ptr__;

	LCDdisplayOn_ = 1;
	LCDcursorOn_ = 0;
	LCDblinkOn_ = 0;

	HAL_Delay(50);
	LCDsend_(COMMAND, 0x28, 5);
	LCDsend_(COMMAND, 0x0C, 1);
	LCDsend_(COMMAND, 0x01, 2);
	LCDsend_(COMMAND, 0x06, 1);

	LCDBacklightsend_(0x2F, 0x00, 0);
	LCDBacklightsend_(0x00, 0x20, 0);
	LCDBacklightsend_(0x01, 0x00, 0);
	LCDBacklightsend_(0x02, 0x01, 0);
	LCDBacklightsend_(0x03, 4, 0);

	LCDBacklightsend_(0x06, 255, 0);
	LCDBacklightsend_(0x05, 255, 0);
	LCDBacklightsend_(0x04, 255, 0);
	LCDBacklightsend_(0x07, 0xFF, 0);

}

void LCDwrite(uint8_t ch) {
	LCDsend_(DATA, ch, 1);
}

void LCDwriteText(uint8_t *text) {
	while(*text != '\0') LCDwrite(*text++);
}

void LCDsetCursorPos(uint8_t row, uint8_t col) {
	LCDsend_(COMMAND, 0x80 | col | (row==0 ? 0x00 : 0x40), 1);
}

void LCDsetDisplay(uint8_t isOn) {
	LCDdisplayOn_ = isOn;
	LCDsend_(COMMAND, (LCDdisplayOn_ << 2) | (LCDcursorOn_ << 1) | (LCDblinkOn_) | 0x08, 1);
}
void LCDsetCursor(uint8_t isOn) {
	LCDcursorOn_ = isOn;
	LCDsend_(COMMAND, (LCDdisplayOn_ << 2) | (LCDcursorOn_ << 1) | (LCDblinkOn_) | 0x08, 1);
}
void LCDsetBlink(uint8_t isOn) {
	LCDblinkOn_ = isOn;
	LCDsend_(COMMAND, (LCDdisplayOn_ << 2) | (LCDcursorOn_ << 1) | (LCDblinkOn_) | 0x08, 1);
}

void LCDclear() {
	LCDsend_(COMMAND, 0x01, 2);
}

//Rewrite
void LCDsetBacklight(uint8_t isOn) {
	LCDBacklightsend_(0x07, isOn ? 0xFF : 0x00, 0);
}

