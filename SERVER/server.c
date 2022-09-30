/************************************************************************************/
/******************************Author: Mohammed Gaafar*******************************/
/******************************File: server.c****************************************/
/************************************************************************************/
#include "server.h"
#include<stdio.h>
#include<string.h>
#include "../CARD/card.h"
#include "../TERMINAL/terminal.h"

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
int index = -1;
ST_accountsDB_t AccountDB[255];

/*Function to get info from DataBase*/

void readAccountDB(void) {
	//Pointer to database file and open the file
	FILE* ptr;
	//Note this is not relative path
	ptr = fopen("D:\\eclipse\\pay\\Database.txt", "r");
	//check if the file is missing
	if (!ptr) {
		printf("'Database.txt'Not Found.\n");
		return;
	}
	//array to store the pan which we search for
	uint8_t *pan;
	//variable to store the amount
	float amount;
	int i = 0,j;
	while (fscanf(ptr, "%f %s", &amount, pan)) {			//Scan all Data in DB
		if (i > 0 && AccountDB[i - 1].balance == amount && AccountDB[i - 1].primaryAccountNumber[6] == pan[6]) {
			break;
		}
		AccountDB[i].balance = amount;		//store the amount
		for (j = 0; j < 20; j++) {
			AccountDB[i].primaryAccountNumber[j] = pan[j];	//store the pan
		}
		i++;
	}
	fclose(ptr);		//close the file
}


void updateAccountDB(void)
{
	//Open database file
	FILE* ptr;
	int i;
	ptr = fopen("D:\\eclipse\\pay\\Database.txt", "w");
	if (!ptr){		//check if the file is missing
		printf("'Database.txt' Not Found.\n");
		return;
	}
	for (i = 0; i < 255; i++) {
		if (AccountDB[i].primaryAccountNumber[0] == '\0') {		//if the pan is clear break
			break;
		}
		fprintf(ptr, "%f %s\n", AccountDB[i].balance, AccountDB[i].primaryAccountNumber);	//update information
	}
}


EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	if (isValidAccount(&transData->cardHolderData)==ACCOUNT_NOT_FOUND) {//Check if the PAN is not found
		transData->transState = DECLINED_STOLEN_CARD;

		return DECLINED_STOLEN_CARD;
	}
	if (isAmountAvailable(&transData->terminalData)==LOW_BALANCE) {		//Check for the amount available
		transData->transState = DECLINED_INSUFFECIENT_FUND;

		return DECLINED_INSUFFECIENT_FUND;
	}
	transData->transState = APPROVED;				//else return approved
	AccountDB[index].balance -= transData->terminalData.transAmount;	//withdrawed amount minus from db
	if (saveTransaction(transData) == SAVING_FAILED) {	//if the file not found
		return INTERNAL_SERVER_ERROR;
	}
	return APPROVED;
}

EN_serverError_t isValidAccount(ST_cardData_t* cardData)
{
	uint8_t* PAN =cardData->primaryAccountNumber;		//store pan in array PAN
	//linear search through AccountsDB
	int found = 0;
	int i, j;
	for (i = 0; i < 255; i++) {
		if (AccountDB[i].primaryAccountNumber[0] != '\0') {//if not empty
			int equal = 1;		//initially found
			for (j = 0; j < 20; j++) {	//search for the pan in DB
				if (AccountDB[i].primaryAccountNumber[j] != PAN[j]) {//if not found -->not found
					equal = 0;
					break;
				}
			}
			if (equal) {		//if equal flag is high --> found with index i
				found = 1;
				index = i;
				break;			//break the loop
			}
		}
		else {					//if equal flag is low --> found =0 initially
			break;
		}
	}
	if (!found) {			//if not found
		return ACCOUNT_NOT_FOUND;
	}
	return OK_serverError;		//else
}





EN_serverError_t isAmountAvailable(ST_terminalData_t* termData)
{
	if (AccountDB[index].balance < termData->transAmount) {//if there is amount lower than transfered
		return LOW_BALANCE;		//low
	}
	return OK_serverError;		//else ok
}


EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	//pointer to file to save transaction data
	FILE* ptr;
	ptr = fopen("D:\\eclipse\\pay\\seq.txt", "r");
	if (!ptr) {			//if the file is missing
		return SAVING_FAILED;
	}
	int transnum;
	fscanf(ptr, "%d", &transnum);	//get transfer number from the file
	fclose(ptr);					//close the file
	ptr = fopen("D:\\eclipse\\pay\\seq.txt", "w");
	fprintf(ptr, "%d", transnum+1);	//write the next transfer number
	fclose(ptr);					//close the file
	transData->transactionSequenceNumber = transnum+1;	//save the next transfer number in transdata
	ptr = fopen("D:\\eclipse\\pay\\Transactions.txt", "a");				//open transaction DP file
	if (!ptr) {
		return SAVING_FAILED;		//if file not found
	}
	fprintf(ptr, "\tTransaction\t\n");			//-=Transaction=-
	fprintf(ptr, "\tCard Holder Name: %s\n", transData->cardHolderData.cardHolderName);	//	-Card Holder Name:
	fprintf(ptr, "\t-PAN: %s\n", transData->cardHolderData.primaryAccountNumber);	//	-PAN:
	fprintf(ptr, "\t-Expiry Date: %s\n", transData->cardHolderData.cardExpirationDate);	//	-Expiry Date:
	fprintf(ptr, "\t-Amount: %f\n", transData->terminalData.transAmount);	//	-Amount:
	fprintf(ptr, "\t-Max Amount: %f\n", transData->terminalData.maxTransAmount);	//	-Max Amount:
	fprintf(ptr, "\t-Transaction State: ");	//	-Transaction State:
	if (transData->transState == APPROVED) {//if the transaction is approved
		fprintf(ptr, "APPROVED\n");
	}
	else if (transData->transState == DECLINED_INSUFFECIENT_FUND) {
		fprintf(ptr, "DECLINED_INSUFFECIENT_FUND\n");
	}
	else if (transData->transState == DECLINED_STOLEN_CARD) {
		fprintf(ptr, "DECLINED_STOLEN_CARD\n");
	}
	fprintf(ptr, "\t-Transaction Sequence Number: %d\n\n", transData->transactionSequenceNumber);	//	-Transaction Sequence Number:
	fclose(ptr);
	return OK_serverError;
}

EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData)
{
	return OK_serverError;
}




