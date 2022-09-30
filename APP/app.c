/************************************************************************************/
/******************************Author: Mohammed Gaafar*******************************/
/*********************************File: app.c****************************************/
/************************************************************************************/
#include "app.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>


void StartApplication (void){
	printf("\t-==Starting Application==-\t\n");
		readAccountDB();
		ST_cardData_t cardData;
		ST_terminalData_t termData;
		ST_transaction_t transData;
		EN_terminalError_t Terminal_Return;
		EN_cardError_t Card_Return;
		/*================================================================*/
		/*==============================CARD==============================*/
		/*================================================================*/


		/*========================Card Holder Name=========================*/
		Card_Return = getCardHolderName(&cardData);
		while (Card_Return == WRONG_NAME) {
			printf("\n\tThe Name is Wrong\n");
			printf("Please try again:\n");
			Card_Return = getCardHolderName(&cardData);
		}

		/*========================Expiry Date=========================*/
		Card_Return = getCardExpiryDate(&cardData);
		while (Card_Return == WRONG_EXP_DATE) {
			printf("\n\tExpiry date is Wrong\n");
			printf("Please try again:\n");
			Card_Return = getCardExpiryDate(&cardData);
		}

		/*========================PAN=========================*/
		Card_Return = getCardPAN(&cardData);
		while (Card_Return == WRONG_PAN) {
			printf("\n\tPAN is Wrong\n");
			printf("Please try again:\n");
			Card_Return = getCardPAN(&cardData);
		}
		/*================================================================*/
		/*============================TERMINAL============================*/
		/*================================================================*/
		Terminal_Return = setMaxAmount(&termData);
		while (Terminal_Return == INVALID_AMOUNT) {
			printf("\n\tMax Amount is Invalid\n");
			printf("Please try again:\n");
			Terminal_Return = setMaxAmount(&termData);
		}
		Terminal_Return = getTransactionDate(&termData);

		Terminal_Return = isCardExpired(cardData, termData);
		if (Terminal_Return == EXPIRED_CARD) {
			printf("\n\n\tEXPIRED CARD\n");
			return;
		}
		Terminal_Return = getTransactionAmount(&termData);
		while (Terminal_Return == INVALID_AMOUNT) {
			printf("\n\tAmount is Invalid\n");
			printf("Please try again:\n");
			Terminal_Return = getTransactionAmount(&termData);
		}
		Terminal_Return = isBelowMaxAmount(&termData);
		if (Terminal_Return == EXCEED_MAX_AMOUNT){
			printf("\n\t      EXCEED_MAX_AMOUNT   \n");
			return;
		}
		transData.cardHolderData = cardData;
		transData.terminalData = termData;
		/*================================================================*/
		/*============================Server============================*/
		/*================================================================*/
		EN_transState_t transSt = recieveTransactionData(&transData);
		if (transSt == DECLINED_STOLEN_CARD) {
			printf("\n\tSTOLEN CARD/INVALID ACCOUNT\n");
		}
		else if (transSt== DECLINED_INSUFFECIENT_FUND) {
			printf("\n\tDECLINED_INSUFFECIENT_FUND\n");
		}
		else if (transSt == APPROVED) {
			printf("\n\tAPPROVED\n");
			printf("\tUPDATING ACCOUNTS DB...\n");
			updateAccountDB();
			printf("\tSAVING TRANSACTION...\n");

			printf("\t\t\t\t------Closing Program------");
		}

	}
