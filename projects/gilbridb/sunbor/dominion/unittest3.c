#include<stdio.h>
#include<stdlib.h>
#include <math.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"



/****************************************************************
//			cmpDeck()
//
// Compare contents of the decks 1 and 2 against each other
//  returns a 2 if the decks are a perfect match
//  returns a 1 if the two decks have the same contents in different order
//  returns a 0 if the two decks do NOT have the same contents
//  returns a -1 if deck sizes are not equal.
***************************************************************/
int cmpDeck(int *deck1, int *deck2, int deck1size, int deck2size)
{
	if(deck1size != deck2size) {return -1;}
	//Copy decks to decks that can be altered
	int deckP1[MAX_DECK];
	int deckP2[MAX_DECK];
	int deckPpos1 = 0;
	int deckPpos2 = 0;
	int i;
	int j;

	//Capture current decks in deck holders for all players
	for(i = 0; i < deck1size; i++)
	{
		deckP1[deckPpos1] = deck1[i];//current Player top card = current card of deck
		deckP2[deckPpos2] = deck2[i];//current Player top card = current card of deck
		deckPpos1++;//increase top of deck
		deckPpos2++;//increase top of deck
	}


	//Check if perfect match
	for(i = 0; i < deck1size; i++)
	{
		//printf("FOR TESTING %d vs %d\n", deckP1[i], deckP2[i]);
		if(deckP1[i] != deckP2[i])
		{
			break; //Not perfect match
		}
		else if(i == (deck1size -1)) // if this is last iteration
		{
			return 2; // Decks are a perfect match
		}
	}

	for(i =0; i < deck1size; i++)
	{
		int conclusion = 0;
		for(j = 0; j < deck1size; j++)
		{
			if(deckP1[i] == deckP2[j])
			{
				deckP2[j] = -1; // Removes it as an option
				conclusion = 1;
				break;
			}
		}
		if(conclusion == 0){return 0;}// happens when inner if statement was not true
	}
	return 1;
}




/******************************************************************
//			kingdomCards()
//			
// This function should test mutliple things regarding the ScoreFor() function:
//
// Tests That each unique value is assigned to the correct position
// Tests that implementing the same value for all is correct
// Tests ability to hold negative values
//
// This should suffice to test a large part of the possible places for error.
******************************************************************/

void testkingdomCards(int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8, int i9, int i10)
{
	printf("Testing Values:\n%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", i1, i2, i3, i4, i5, i6, i7, i8, i9, i10);
	int* k = kingdomCards(i1,i2,i3,i4,i5,i6,i7,i8,i9,i10);
	int fail = 0;
	
	if(k[0] != i1)
	{
		printf("testkingdomCards(): FAIL when test value 1 of kingdom Cards.\n");
		fail = 1;
	} 
	if(k[1] != i2)
	{
		printf("testkingdomCards(): FAIL when test value 2 of kingdom Cards.\n");
		fail = 1;
	} 
	if(k[2] != i3)
	{
		printf("testkingdomCards(): FAIL when test value 3 of kingdom Cards.\n");
		fail = 1;
	} 
	if(k[3] != i4)
	{
		printf("testkingdomCards(): FAIL when test value 4 of kingdom Cards.\n");
		fail = 1;
	} 
	if(k[4] != i5)
	{
		printf("testkingdomCards(): FAIL when test value 5 of kingdom Cards.\n");
		fail = 1;
	} 
	if(k[5] != i6)
	{
		printf("testkingdomCards(): FAIL when test value 6 of kingdom Cards.\n");
		fail = 1;
	} 
	if(k[6] != i7)
	{
		printf("testkingdomCards(): FAIL when test value 7 of kingdom Cards.\n");
		fail = 1;
	} 
	if(k[7] != i8)
	{
		printf("testkingdomCards(): FAIL when test value 8 of kingdom Cards.\n");
		fail = 1;
	} 
	if(k[8] != i9)
	{
		printf("testkingdomCards(): FAIL when test value 9 of kingdom Cards.\n");
		fail = 1;
	} 
	if(k[9] != i10)
	{
		printf("testkingdomCards(): FAIL when test value 10 of kingdom Cards.\n");
		fail = 1;
	} 
	if(fail == 0)
	{
		printf("testkingdomCards(): PASS when test values of kingdom Cards.\n");
	}

}

int main(int argc, char *argv[])
{
    testkingdomCards(0,1,2,3,4,5,6,7,8,9);
    testkingdomCards(5,5,5,5,5,5,5,5,5,5);
    testkingdomCards(-10,2,-22,0,-4,2,16,-7,12,0);
    return 0;
}



