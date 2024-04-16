/*
 * calculator.h
 *
 *  Created on: Apr 4, 2024
 *      Author: yehor
 */

#ifndef INC_CALCULATOR_H_
#define INC_CALCULATOR_H_

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

enum NodeType{ NUMBER, ADD, SUB, MULT, DIV, NEG };

struct Node {
	enum NodeType type;
	uint32_t value;
	struct Node *lNode;
	struct Node *rNode;
};

void computeEquation(uint8_t *expression, uint8_t *filled, const uint8_t exprWidth);
uint8_t evaluate(struct Node *eq, uint32_t *result);
void freeTree(struct Node *eq);
void pasteToExpression(uint8_t *expression, uint8_t *filled, uint8_t exprWidth, uint32_t value);
void pasteErrorToExpression(uint8_t *expression, uint8_t *filled, uint8_t exprWidth, uint8_t error);

uint8_t nextToken(uint8_t *expression, uint8_t filled, uint8_t *nextTokenBegin, uint8_t *nextTokenEnd);

// These cannot change expression
// Can change nextTokenBegin and nextTokenEnd through nextToken
struct Node *parseE(uint8_t *expression, uint8_t filled, uint8_t *nextTokenBegin, uint8_t *nextTokenEnd);
struct Node *parseT(uint8_t *expression, uint8_t filled, uint8_t *nextTokenBegin, uint8_t *nextTokenEnd);
struct Node *parseF(uint8_t *expression, uint8_t filled, uint8_t *nextTokenBegin, uint8_t *nextTokenEnd);

uint8_t isNum(uint8_t ch);

// Assume nextTokenBegin and nextTokenEnd are valid
uint8_t getToken(uint8_t *expression, uint8_t nextTokenBegin, uint8_t nextTokenEnd, uint8_t *tokenType, uint32_t *tokenVal);

uint8_t lastToken(uint8_t filled, uint8_t nextTokenEnd);



#endif /* INC_CALCULATOR_H_ */
