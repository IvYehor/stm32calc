
#include "calculator2.h"

void computeEquation(uint8_t *expression, uint8_t *filled, const uint8_t exprWidth) {
	uint8_t nextTokenBegin = 0, nextTokenEnd = 0;
	struct Node *tree = parseE(expression, *filled, &nextTokenBegin, &nextTokenEnd);
	if(tree == NULL) {
		pasteToExpression(expression, filled, exprWidth, 4);
		//pasteErrorToExpression(expression, filled, exprWidth, 0);
		return;
	}
	uint32_t res = 4;
	if(!evaluate(tree, &res) || 1) {
		pasteToExpression(expression, filled, exprWidth, res);
	}
	else {
		pasteErrorToExpression(expression, filled, exprWidth, 1);
	}
	freeTree(tree);
}

void freeTree(struct Node *eq) {
	if(eq->lNode != NULL)
			freeTree(eq->lNode);
	if(eq->rNode != NULL)
			freeTree(eq->rNode);
	free(eq);
}


uint8_t evaluate(struct Node *eq, uint32_t *result) {
	uint32_t a, b;
	switch (eq->type)
	{
	case NUMBER:
		*result=eq->value;
		break;
	
	case ADD:
		if(evaluate(eq->lNode, &a))
			return 1;
		if(evaluate(eq->rNode, &b))
			return 1;
		*result=a+b;
		break;
	
	case SUB:
		if(evaluate(eq->lNode, &a))
			return 1;
		if(evaluate(eq->rNode, &b))
			return 1;
		*result=a-b;
		break;
	
	case MULT:
		if(evaluate(eq->lNode, &a))
			return 1;
		if(evaluate(eq->rNode, &b))
			return 1;
		*result=a*b;
		break;
	
	case DIV:
		uint32_t a, b;
		if(evaluate(eq->lNode, &a))
			return 1;
		if(evaluate(eq->rNode, &b))
			return 1;
		if(b == 0)
			return 1;
		*result=a/b;
		break;
	
	case NEG:
		if(evaluate(eq->lNode, &b))
			return 1;
		*result=-a;
		break;
	
	default:
		break;
	}
	return 0;
}
void pasteToExpression(uint8_t *expression, uint8_t *filled, uint8_t exprWidth, uint32_t value) {
	uint8_t i = 0;
	if(value == 0) {
		expression[0] = 48;
		i=1;
	}
	while(value && i < exprWidth) {
		expression[i] = value%10+48;
		value/=10;
		++i;
	}
	for (uint8_t k = i; k < exprWidth; ++k) {
		expression[k] = ' ';
	}
	for(uint8_t j = 0; j < i/2; ++j) {
		uint8_t a = expression[j];
		expression[j] = expression[i-1-j];
		expression[i-1-j] = a;
	}
	*filled = i;
}

void pasteErrorToExpression(uint8_t *expression, uint8_t *filled, uint8_t exprWidth, uint8_t error) {
	if(error == 0) {
		expression[0] = 'P';
		expression[1] = 'A';
		expression[2] = 'R';
		expression[3] = 'S';
		expression[4] = 'E';
		expression[5] = 'E';
		expression[6] = 'R';
		expression[7] = 'R';
		for(uint8_t i = 8; i < exprWidth; ++i)
			expression[i] = ' ';
		*filled = 8;
	}
	else if(error == 1) {
		expression[0] = 'D';
		expression[1] = 'I';
		expression[2] = 'V';
		expression[3] = 'B';
		expression[4] = 'Y';
		expression[5] = '0';
		for(uint8_t i = 6; i < exprWidth; ++i)
			expression[i] = ' ';
		*filled = 6;
	}
}

struct Node *parseE(uint8_t *expression, uint8_t filled, uint8_t *nextTokenBegin, uint8_t *nextTokenEnd) {
	struct Node *f = parseT(expression, filled, nextTokenBegin, nextTokenEnd);
	if(f == NULL)
		return NULL;
		
	uint8_t tokentype;
	uint32_t tokenVal;

	uint8_t nextTokenBeginTemp = *nextTokenBegin, nextTokenEndTemp = *nextTokenEnd;
	//!lastToken(filled, *nextTokenEnd)
	while(!nextToken(expression, filled, &nextTokenBeginTemp, &nextTokenEndTemp)) {
		getToken(expression, nextTokenBeginTemp, nextTokenEndTemp, &tokentype, &tokenVal);
		if(tokentype == 0) {
			freeTree(f);
			return NULL;
		}
		if(tokenVal == '+' || tokenVal == '-') {
			*nextTokenBegin = nextTokenBeginTemp;
			*nextTokenEnd = nextTokenEndTemp;
			struct Node *f2 = parseT(expression, filled, nextTokenBegin, nextTokenEnd);
			nextTokenBeginTemp = *nextTokenBegin;
			nextTokenEndTemp = *nextTokenEnd;
			if(f2 == NULL) {
				freeTree(f);
				return NULL;
			}
			struct Node *ret = (struct Node *)malloc(sizeof(struct Node));
			ret->type=tokenVal=='+' ? ADD : SUB;
			ret->lNode = f;
			ret->rNode = f2;
			f = ret;
		}
		else { // Problem that it nexts the next token here
			//return NULL;
			break;
		}
	}
	return f;
}

// Assume there is T to parse in the expression
// If not, return NULL
//
// If there is one additional char, there is no error
struct Node *parseT(uint8_t *expression, uint8_t filled, uint8_t *nextTokenBegin, uint8_t *nextTokenEnd) {
	struct Node *f = parseF(expression, filled, nextTokenBegin, nextTokenEnd);
	if(f == NULL)
		return NULL;
	
	uint8_t tokentype;
	uint32_t tokenVal;

	uint8_t nextTokenBeginTemp = *nextTokenBegin, nextTokenEndTemp = *nextTokenEnd;
	//!lastToken(filled, *nextTokenEnd)
	while(!nextToken(expression, filled, &nextTokenBeginTemp, &nextTokenEndTemp)) {
		getToken(expression, nextTokenBeginTemp, nextTokenEndTemp, &tokentype, &tokenVal);
		if(tokentype == 0) {
			freeTree(f);
			return NULL;
		}
		if(tokenVal == '*' || tokenVal == '/') {
			*nextTokenBegin = nextTokenBeginTemp;
			*nextTokenEnd = nextTokenEndTemp;
			struct Node *f2 = parseF(expression, filled, nextTokenBegin, nextTokenEnd);
			nextTokenBeginTemp = *nextTokenBegin;
			nextTokenEndTemp = *nextTokenEnd;
			if(f2 == NULL) {
				freeTree(f);
				return NULL;
			}
			struct Node *ret = (struct Node *)malloc(sizeof(struct Node));
			ret->type=tokenVal=='*' ? MULT : DIV;
			ret->lNode = f;
			ret->rNode = f2;
			f = ret;
		}
		else {
			//return NULL;
			break;
		}
	}
	return f;


}

// It should parse factor as much as it can
// expression length >= 0
// filled is in [0, expression length]
// *nextTokenBegin is in [0, filled-1]
// *nextTokenBegin is in [0, filled]
struct Node *parseF(uint8_t *expression, uint8_t filled, uint8_t *nextTokenBegin, uint8_t *nextTokenEnd) {
	if(nextToken(expression, filled, nextTokenBegin, nextTokenEnd))// this should be called by parseT
		return NULL;
	uint8_t tokentype;
	uint32_t tokenVal;
	getToken(expression, *nextTokenBegin, *nextTokenEnd, &tokentype, &tokenVal);
	if(tokentype == 0) {
		struct Node *ret = (struct Node *)malloc(sizeof(struct Node));
		ret->type = NUMBER;
		ret->value = tokenVal;
		ret->lNode = NULL;
		ret->rNode = NULL;
		return ret;
	}
	else if(tokenVal == '(') {
		//nextToken(expression, filled, nextTokenBegin, nextTokenEnd);
		struct Node *r = parseE(expression, filled, nextTokenBegin, nextTokenEnd);
		if(nextToken(expression, filled, nextTokenBegin, nextTokenEnd))
			return NULL;
		getToken(expression, *nextTokenBegin, *nextTokenEnd, &tokentype, &tokenVal);
		if(tokentype == 1 && tokenVal == ')')
			return r;
		else 
			return NULL;
	}
	else if(tokenVal == '-') {
		struct Node *r = parseF(expression, filled, nextTokenBegin, nextTokenEnd);
		if(r == NULL)
			return NULL;
		struct Node *ret = (struct Node *)malloc(sizeof(struct Node));
		ret->type = NEG;
		ret->lNode = r;
		ret->rNode = NULL;
		return ret;
	}
	else {
		return NULL;
	}
}


// expression length >= 0
// filled is in [0, expression length]
// *nextTokenBegin is in [0, filled-1] - not important
// *nextTokenBegin is in [0, filled]
// returns 1 when no more tokens
// return 0 when got a token

uint8_t nextToken(uint8_t *expression, uint8_t filled, uint8_t *nextTokenBegin, uint8_t *nextTokenEnd) {
	if(*nextTokenEnd >= filled) 
		return 1;
	*nextTokenBegin=*nextTokenEnd;
	if(isNum(expression[*nextTokenBegin])) {
		while(++(*nextTokenEnd) < filled && isNum(expression[*nextTokenEnd]));
	}
	else {
		++(*nextTokenEnd);
	}
	return 0;
}

uint8_t lastToken(uint8_t filled, uint8_t nextTokenEnd) {
	return nextTokenEnd >= filled;
};

uint8_t getToken(uint8_t *expression, uint8_t nextTokenBegin, uint8_t nextTokenEnd, uint8_t *tokenType, uint32_t *tokenVal) {
	if(expression[nextTokenBegin] >= '0' && expression[nextTokenBegin] <= '9') {
		*tokenType = 0;
		*tokenVal = expression[nextTokenBegin]-48;
		for(uint8_t i = nextTokenBegin+1; i < nextTokenEnd; ++i) {
			*tokenVal *= 10;
			*tokenVal += expression[i]-48;
		}
	}
	else {
		*tokenType = 1;
		*tokenVal = expression[nextTokenBegin];
	}
}

uint8_t isNum(uint8_t ch) {
	return ch >= '0' && ch <= '9';
}
