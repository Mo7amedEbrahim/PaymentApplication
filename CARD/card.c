/************************************************************************************/
/******************************Author: Mohammed Gaafar*******************************/
/******************************File: card.h******************************************/
/************************************************************************************/

#include <stdio.h>
#include "card.h"

#include <time.h>
#include <stdio.h>


EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	char Local_Name[30];		//To store the name
	int i;						//For iterations
	
	
	/*Initial value for all array elements with '\0'*/
	for (i = 0; i < 30; i++) Local_Name[i] = '\0';
	
	/*Ask the user to enter his name*/
	printf("\nEnter card holder name [20-24 characters]: ");
	fseek(stdin, 0, SEEK_END);					//Clear the stdin buffer
	
	
	scanf("%[^\n]s",Local_Name);				//We use this ^\n tells to take input until newline

	fseek(stdin, 0, SEEK_END);				//Clear the stdin buffer

	/*Check if the name meets the requirments and returns the error*/
	if (!Local_Name) {				//if the name = NULL
		return WRONG_NAME;			//return wrong name error
	}
	if (Local_Name[19] == '\0' || Local_Name[25] != '\0') {		//if it is less than 20 or larger than 25
		return WRONG_NAME;										//return wrong name error
	}
	for(i=0;i<25;i++){
		cardData->cardHolderName[i] = Local_Name[i];			//Store the entered name in cardHolderName array
	}
	printf("Name is:\t %s\n", cardData->cardHolderName);		//Print the name
	return (EN_cardError_t)OK_cardError;						//return OK error
}








EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
	char ExpDate[10];		//array to store expiry date
	int i;					//for iterations
	
	
	/*Initial value for all array elements with '\0'*/
	for (i = 0; i < 10; i++)ExpDate[i] = '\0';
	
	
	uint32_t month, year;		//variables to store month and year As decimal
	
	/*Ask the user to enter Expiry date*/
	printf("Enter Expiry date [MM/YY]: ");
	fseek(stdin, 0, SEEK_END);				//Clear the stdin buffer
	scanf("%s", ExpDate);		//we used scan if because we will not use spaces

	/*Check if the Date meets the requirments and returns the error*/
	
	if (!ExpDate) {														//if the Expiry date = NULL
		return WRONG_EXP_DATE;											//return WRONG_EXP_DATE error
	}
	if (ExpDate[6] != '\0' || ExpDate[3] == '\0') { 					//if it is less than 4 or larger than 5
		return WRONG_EXP_DATE; 											//return WRONG_EXP_DATE error
	}

	month = (ExpDate[0] - '0') * 10 + (ExpDate[1] - '0');				//Transfer to DECIMAL	to be able to compare
	year = (ExpDate[3] - '0') * 10 + (ExpDate[4] - '0');				//Transfer to DECIMAL   to be able to compare
	
	if (month < 1 || month>12 || ExpDate[2] != '/' || (year > 99 || year < 0)) {		//Check the date format MM/YY
		return WRONG_EXP_DATE;															//return WRONG_EXP_DATE error
	}
	for (i = 0; i < 6; i++) {
		cardData->cardExpirationDate[i] = ExpDate[i];					//Store the entered name in cardExpirationDate array
	}
	printf("Expiry date:\t %s\n", cardData->cardExpirationDate);
	return (EN_cardError_t)OK_cardError;								//return OK error
}






EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
	char CardPan[30];					//To store the Pan
		int i;							//for iterations


		/*Initial value for all array elements with '\0'*/
		for (i = 0; i < 30; i++)CardPan[i] = '\0';
		
		
		/*Ask the user to enter Expiry date*/
		printf("Enter PAN [20 : 25] characters]:");
		i = 0;
		fseek(stdin, 0, SEEK_END);									//Clear the stdin buffer
		scanf("%s", CardPan);
		if (!CardPan)	return WRONG_PAN;								//if PAN = NULL Return Wrong PAN Card ERROR
		if (CardPan[25] != '\0' || CardPan[19] == '\0')return WRONG_PAN;	//if pan <20 or >25 Return Wrong PAN Card ERROR
		
		for (int i = 0; i < 25; i++) {
			cardData->primaryAccountNumber[i] = CardPan[i];			//Store the entered name in primaryAccountNumber array
		}
		printf("PAN is:\t %s\n", cardData->primaryAccountNumber);
	
	return OK_cardError;											//Rertrn OK Card Error
}
