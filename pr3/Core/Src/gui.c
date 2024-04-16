/*
 * gui.c
 *
 *  Created on: Apr 3, 2024
 *      Author: yehor
 */

#include "gui.h"

#include "lcddisplay.h"

void drawExpression(uint8_t *expression, uint8_t cursor, uint8_t shift, uint8_t filled, uint8_t exprwidth, uint8_t screenwidth) {
	LCDsetCursorPos(0, 0);
	for(uint8_t i = shift; i < shift + screenwidth; ++i) {
	  if(i < exprwidth)
		  LCDwrite(expression[i]);
	  else
		  LCDwrite(' ');
	}
	LCDsetCursorPos(0, cursor-shift);
}

void drawMenu(uint8_t page, uint8_t screenwidth, uint8_t (*pages)[3], uint8_t numofpages, uint8_t cursorScreen) {
	LCDsetCursorPos(1, 0);
	//uint8_t *t = "Page ";
	//LCDwriteText(t);
	LCDwrite(page + 48);
	LCDwrite(' ');
	LCDwrite(pages[page][0]);
	LCDwrite(' ');
	LCDwrite(pages[page][1]);
	LCDwrite(' ');
	LCDwrite(pages[page][2]);
	LCDsetCursorPos(0, cursorScreen);
}

void addChar(uint8_t *expression, uint8_t (*pages)[3], uint8_t page, uint8_t button, uint8_t *cursor, uint8_t *shift, uint8_t *filled, uint8_t exprwidth, uint8_t screenwidth) {
	if(*filled >= exprwidth)
		return;
	for(uint8_t i = *filled; i > *cursor; --i) {
		expression[i] = expression[i-1];
	}
	expression[*cursor] = pages[page][button];
	++(*filled);
	++(*cursor);
	if(*shift+screenwidth-1 < *cursor)
		++(*shift);
}

void moveLeft(uint8_t filled, uint8_t *cursor, uint8_t *shift) {
	if(*cursor > 0)
		--(*cursor);
	if(*shift > *cursor)
		--(*shift);
}

void moveRight(uint8_t filled, uint8_t *cursor, uint8_t *shift, uint8_t screenwidth) {
	if(*cursor < filled)
		++(*cursor);
	if(*shift+screenwidth-1 < *cursor)
		++(*shift);
}

void nextPage(uint8_t *page, uint8_t numofpages) {
	++(*page);
	(*page)%=numofpages;
}
