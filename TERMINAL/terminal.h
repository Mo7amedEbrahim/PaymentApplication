/************************************************************************************/
/******************************Author: Mohammed Gaafar*******************************/
/******************************File: terminal.h**************************************/
/************************************************************************************/

#ifndef _TERMINAL_H
#define _TERMINAL_H
#include "../CARD/card.h"




typedef struct ST_terminalData_t
{
float transAmount;

float maxTransAmount;

uint8_t transactionDate[11];

}ST_terminalData_t;

typedef enum EN_terminalError_t
{
	
	OK_terminalError, WRONG_DATE, EXPIRED_CARD, INVALID_CARD, INVALID_AMOUNT, EXCEED_MAX_AMOUNT, INVALID_MAX_AMOUNT

}EN_terminalError_t ;




EN_terminalError_t getTransactionDate(ST_terminalData_t *);		

EN_terminalError_t isCardExpired(ST_cardData_t , ST_terminalData_t );	

EN_terminalError_t getTransactionAmount(ST_terminalData_t *);	

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *);	//

EN_terminalError_t setMaxAmount(ST_terminalData_t *);	








#endif
