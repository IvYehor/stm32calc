/*
 * gui.h
 *
 *  Created on: Apr 3, 2024
 *      Author: yehor
 */

#ifndef INC_GUI_H_
#define INC_GUI_H_

#include <stdint.h>


void drawExpression(uint8_t *expression, uint8_t cursor, uint8_t shift, uint8_t filled, uint8_t exprwidth, uint8_t screenwidth);
void drawMenu(uint8_t page, uint8_t screenwidth, uint8_t (*pages)[3], uint8_t numofpages, uint8_t cursorScreen);

void addChar(uint8_t *expression, uint8_t (*pages)[3], uint8_t page, uint8_t button, uint8_t *cursor, uint8_t *shift, uint8_t *filled, uint8_t exprwidth, uint8_t screenwidth);
void moveLeft(uint8_t filled, uint8_t *cursor, uint8_t *shift);
void moveRight(uint8_t filled, uint8_t *cursor, uint8_t *shift, uint8_t screenwidth);

void nextPage(uint8_t *page, uint8_t numofpages);

#endif /* INC_GUI_H_ */
