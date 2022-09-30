#include "card.h"
#include <stdio.h>
void main (void){
	ST_cardData_t cardData;
	EN_cardError_t Card_Return;
	Card_Return = getCardExpiryDate(&cardData);
		while (Card_Return == WRONG_EXP_DATE) {
			printf("\n\tExpiry date is Wrong\n");
			printf("Please try again:\n");
			Card_Return = getCardExpiryDate(&cardData);
		}
}