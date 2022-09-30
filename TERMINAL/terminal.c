/************************************************************************************/
/******************************Author: Mohammed Gaafar*******************************/
/******************************File: terminal.c**************************************/
/************************************************************************************/
#include "terminal.h"
#include<stdio.h>
#include <time.h>		// To get the time of the device
#include "../CARD/card.h"
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;




//We get Transaction Date from the system without asking the user (More practical)
EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
		printf("\t-==Getting Date==-\t\n");
		/*To get date from system*/
		time_t t = time(NULL);
		struct tm tm = *localtime(&t);
		
		/*Saving the date in variables in common date format*/
		int year = tm.tm_year + 1900;
		int month = tm.tm_mon + 1;
		int day = tm.tm_mday;
		
		/*Transfer date to ASCII and save it in transactionDate array*/
		termData->transactionDate[0] = day / 10 + '0';			//Get first char of day		x0/00/0000
		termData->transactionDate[1] = day % 10 + '0';			//Get Second char of day		0x/00/0000
		termData->transactionDate[2] = '/';						//put the / char
		termData->transactionDate[3] = month / 10 + '0';		//Get first char of Month		00/x0/0000
		termData->transactionDate[4] = month % 10 + '0';		//Get Second char of Month		00/0x/0000
		termData->transactionDate[5] = '/';						//put the / char
		termData->transactionDate[6] = '0' + year / 1000;			//Get first char of Year		00/00/x000
		termData->transactionDate[7] = '0' + (year % 1000) / 100;	//Get Second char of Year		00/00/0x00
		termData->transactionDate[8] = '0' + (year % 100) / 10;		//Get Third char of Year		00/00/00x0
		termData->transactionDate[9] = '0' + year % 10;				//Get fourth char of Year		00/00/000x
		termData->transactionDate[10] = '\0';						//Ending the array with null char to point to the end of the string
		printf("System date is :\t %s\n", termData->transactionDate);
		return OK_terminalError;									//Return the OK error
	
}



//Check if the card is expired

EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData)
{
	//Transfer all months data to decimal to compare with each other
	int Expiry_Month = (cardData.cardExpirationDate[0] - '0') * 10 + cardData.cardExpirationDate[1]-'0';	//Get the cardExpirationDate	Month
	int Expiry_Year = (cardData.cardExpirationDate[3] - '0') * 10 + cardData.cardExpirationDate[4] - '0'+2000;	//Get the cardExpirationDate	Year
	int Trans_Month = (termData.transactionDate[3] - '0') * 10 + termData.transactionDate[4] - '0';		//Get the cardtransactionDate	Month
	int Trans_Year = (termData.transactionDate[6] - '0') * 1000 + (termData.transactionDate[7] - '0') * 100 + (termData.transactionDate[8] - '0') * 10+ +termData.transactionDate[9] - '0';	//Get the cardtransactionDate	Year
	/*Compare between them*/
	if (Trans_Year > Expiry_Year) {			 //if transaction year after expiry data year
		return EXPIRED_CARD;		//Return card expired
	}
	if (Trans_Year == Expiry_Year && Trans_Month > Expiry_Month) {  //if transaction year equal expiry data year and transaction month after expiry month
		return EXPIRED_CARD;					//Return card expired
	}
	return OK_terminalError;			// if none of conditions are true return Ok error
}


EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
	float amount=0;
	/*Ask the user to enter transaction amount*/
	printf("Enter transaction amount: ");
	scanf("%f", &amount);		//Scan the amount
	
	if (amount <= 0) {							//If the transaction amount is less than or equal to 0 
		return INVALID_AMOUNT;					//return INVALID_AMOUNT
	}
	termData->transAmount = amount;				//if the conditin is not true save the amount in transAmount
	return OK_terminalError;					//return OK
}




EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
	if (termData->transAmount > termData->maxTransAmount) {		//If the transaction amount is larger than the terminal max amount
		return EXCEED_MAX_AMOUNT;								//return EXCEED_MAX_AMOUNT
	}
	return OK_terminalError;									// return OK
}


EN_terminalError_t setMaxAmount(ST_terminalData_t* termData)
{	
	float Max_Amount;
	/*Ask to enter max amount*/
	printf("Enter MAX Amount: ");				
	scanf("%f", &Max_Amount);
	
	if (Max_Amount <= 0) { 						//If transaction max amount less than or equal to 0
		return INVALID_MAX_AMOUNT;				//return INVALID_MAX_AMOUNT error
	}
	termData->maxTransAmount = Max_Amount;	 	//Store the max amount value in maxTransAmount
	return OK_terminalError;					// else return OK.
}
