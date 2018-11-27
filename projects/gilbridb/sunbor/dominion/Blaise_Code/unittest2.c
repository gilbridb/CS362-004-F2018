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
//			scoreFor()
//			
// This function should test mutliple things regarding the ScoreFor() function:
//
// Test that hand, deck, and discard score add up to the correct total
// Test that the cards of other players are left unaffected 
// Test that the number of total players is not changed
//
// This should suffice to test a large part of the possible places for error.
******************************************************************/

void testscoreFor(int num_players, int curse_in, int estate_in, int duchy_in, int province_in, int great_hall_in, int gardens_in, int score_in)
{
	printf("Tests for: %d players, %d curses, %d estates, %d duchys\n%d provinces, %d great halls, %d gardens\nExpected Score: %d\n", num_players, curse_in, estate_in, duchy_in, province_in, great_hall_in, gardens_in, score_in);
	int i;
	int j;
	int holder;
	int player_holder = 0;	

	struct gameState G;
	struct gameState * state = &G;
  	int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
           great_hall, tribute, smithy};
	initializeGame(num_players, k, 10, &G); //number players, cards, randomSeed, gameState
	//Holder number of players before Shuffle()
	player_holder = state->numPlayers;

	//Manually set deck for player
	state->deckCount[0] = 0;
	for(i = 0; i < curse_in; i++)
	{
		state->deck[0][state->deckCount[0]] = curse;
		state->deckCount[0]++;
	}	
	
	for(i = 0; i < estate_in; i++)
	{
		state->deck[0][state->deckCount[0]] = estate;
		state->deckCount[0]++;
	}	

	for(i = 0; i < duchy_in; i++)
	{
		state->deck[0][state->deckCount[0]] = duchy;
		state->deckCount[0]++;
	}	


	for(i = 0; i < province_in; i++)
	{
		state->deck[0][state->deckCount[0]] = province;
		state->deckCount[0]++;
	}	

	for(i = 0; i < great_hall_in; i++)
	{
		state->deck[0][state->deckCount[0]] = great_hall;
		state->deckCount[0]++;
	}

	for(i = 0; i < gardens_in; i++)
	{
		state->deck[0][state->deckCount[0]] = gardens;
		state->deckCount[0]++;
	}	

	//Set Discard and hand to zero
	state->discardCount[0] = 0;
	state->handCount[0] = 0;


	
	//Initialize Deck holders for all players
	int deckP[num_players][MAX_DECK];
	int deckPpos[num_players];
	int handPCount[num_players];
	int disPCount[num_players];
	
	//Capture current decks in deck holders for all players
	for(j = 0; j < num_players; j++){	
		holder = state->deckCount[j];
		handPCount[j] = state->handCount[j];
		disPCount[j] = state->discardCount[j];
		for(i = 0; i < holder; i++)
		{
			//FOR TESTING printf("Deck %d card %d\n", j, state->deck[j][i]);
			deckP[j][deckPpos[j]] = state->deck[j][i];//current Player top card = current card of deck
			deckPpos[j]++;//increase top of deck
		}
	}//End for loop

	//Shuffle Player 1 deck
	holder = scoreFor(0, &G);
	
	//Checks Number of players remains unchanged
	if(player_holder == state->numPlayers)
		printf("scoreFor(): PASS when test number of players.\n");
	else
		printf("scoreFor(): FAIL when test number of players.\n");

	//printf("Score read: %d\n", holder);
	if(score_in == holder)
		printf("scoreFor(): PASS when test score of deck.\n");
	else
		printf("scoreFor(): FAIL when test score of deck.\n");

	//Test state of other players
	//Test Hand Count remains unchanged
	for(i=0; i < num_players; i++)
	{
		if(handPCount[i] != state->handCount[i])
		{
			printf("scoreFor(): FAIL when test hand Count changed for a player.\n");
			break;
		}
		if(i == (num_players - 1)) //Last Pass
			printf("scoreFor(): PASS when test hand Count didn't change for all players.\n");
			
			
	}


	//Test Discard Count remains unchanged
	for(i=0; i < num_players; i++)
	{
		if(disPCount[i] != state->discardCount[i])
		{
			printf("scoreFor(): FAIL when test discard Count changed for a player.\n");
			break;
		}
		if(i == (num_players - 1)) //Last Pass
			printf("scoreFor(): PASS when test discard Count didn't change for all players.\n");
			
			
	}

	//Test State of other player decks
	for(i=0; i < num_players; i++)
	{		
		player_holder = cmpDeck(deckP[i], state->deck[i], deckPpos[i], state->deckCount[i]);
			if(player_holder == -1)	
			{
				printf("scoreFor(): FAIL when testing deck Count changed for a player.\n");
				break;
			}
			if(player_holder == 0)	
			{
				printf("scoreFor(): FAIL when testing deck contents changed for a player.\n");
				break;
			}
			if(player_holder == 1)	
			{
				printf("scoreFor(): FAIL when testing deck order changed for a player.\n");
				break;
			}
			if((player_holder == 2) && (i == (num_players - 1)))
				printf("scoreFor(): PASS when testing decks were not altered for all players.\n");
	}			
		
	//Manually set discard for player
	state->discardCount[0] = 0;
	for(i = 0; i < curse_in; i++)
	{
		state->discard[0][state->discardCount[0]] = curse;
		state->discardCount[0]++;

	}	
	
	for(i = 0; i < estate_in; i++)
	{
		state->discard[0][state->discardCount[0]] = estate;
		state->discardCount[0]++;
	}	

	for(i = 0; i < duchy_in; i++)
	{
		state->discard[0][state->discardCount[0]] = duchy;
		state->discardCount[0]++;
	}	


	for(i = 0; i < province_in; i++)
	{
		state->discard[0][state->discardCount[0]] = province;
		state->discardCount[0]++;
	}	

	for(i = 0; i < great_hall_in; i++)
	{
		state->discard[0][state->discardCount[0]] = great_hall;
		state->discardCount[0]++;
	}

	for(i = 0; i < gardens_in; i++)
	{
		state->discard[0][state->discardCount[0]] = gardens;
		state->discardCount[0]++;
	}	

	//Set Discard and hand to zero
	state->handCount[0] = 0;
	state->deckCount[0] = 0;

	//Shuffle Player 1 deck
	holder = scoreFor(0, &G);
	
	//printf("Score read: %d\n", holder);
	if(score_in == holder)
		printf("scoreFor(): PASS when test score of discard.\n");
	else
		printf("scoreFor(): FAIL when test score of discard.\n");

	//Manually set hand for player
	state->handCount[0] = 0;
	for(i = 0; i < curse_in; i++)
	{
		state->hand[0][state->handCount[0]] = curse;
		state->handCount[0]++;

	}	
	
	for(i = 0; i < estate_in; i++)
	{
		state->hand[0][state->handCount[0]] = estate;
		state->handCount[0]++;
	}	

	for(i = 0; i < duchy_in; i++)
	{
		state->hand[0][state->handCount[0]] = duchy;
		state->handCount[0]++;
	}	


	for(i = 0; i < province_in; i++)
	{
		state->hand[0][state->handCount[0]] = province;
		state->handCount[0]++;
	}	

	for(i = 0; i < great_hall_in; i++)
	{
		state->hand[0][state->handCount[0]] = great_hall;
		state->handCount[0]++;
	}

	for(i = 0; i < gardens_in; i++)
	{
		state->hand[0][state->handCount[0]] = gardens;
		state->handCount[0]++;
	}	

	//Set Discard and hand to zero
	state->discardCount[0] = 0;
	state->deckCount[0] = 0;

	//Shuffle Player 1 deck
	holder = scoreFor(0, &G);
	
	///printf("Score read: %d\n", holder);
	if(score_in == holder)
		printf("scoreFor(): PASS when test score of hand.\n");
	else
		printf("scoreFor(): FAIL when test score of hand.\n");


}

int main(int argc, char *argv[])
{
    testscoreFor(3, 1, 0, 3, 0, 0, 0, 8);
    testscoreFor(4, 0, 0, 0, 0, 0, 10, 10);
    testscoreFor(2, 1, 1, 1, 1, 1, 0, 10);
    testscoreFor(3, 0, 0, 0, 0, 0, 0, 0);
    return 0;
}



