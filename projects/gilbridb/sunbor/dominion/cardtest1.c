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
//			smithyTest()
//			
// This function should test mutliple things regarding the Smithy card implementation:
//
// Test that hand, deck, and discard score add up to the correct total
// Test that the cards of other players are left unaffected 
// Test that the number of total players is not changed
//
// This should suffice to test a large part of the possible places for error.
******************************************************************/

void testsmithy(int num_players, int hand_size_in, int smithy_pos)
{
	printf("Tests for: %d players, %d hand size, %d smithy card position\n", num_players, hand_size_in, smithy_pos);
	int i;
	int j;
	int holder;
	int player_holder = 0;	
	int bonus[2] = {0,0}; // *bonus satisfier

	struct gameState G;
	struct gameState * state = &G;
  	int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
           great_hall, tribute, smithy};
	initializeGame(num_players, k, 10, &G); //number players, cards, randomSeed, gameState

	//Holder number of players before card call
	player_holder = state->numPlayers;

	int not_smithy = k[0];
	if(k[0] == smithy){not_smithy = k[1];}


	//Manually set hand for player
	state->handCount[0] = 0;
	for(i = 0; i < hand_size_in; i++)
	{
		if(i == smithy_pos)
		{
			state->hand[0][state->handCount[0]] = smithy;
			state->handCount[0]++;
		}
		else
		{
			state->hand[0][state->handCount[0]] = not_smithy;
			state->handCount[0]++;
		}
	}

	
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
	holder = cardEffect(smithy, 0,0,0,&G,smithy_pos, bonus);
	
	//Checks Number of players remains unchanged
	if(player_holder == state->numPlayers)
		printf("Smithy(): PASS when test number of players.\n");
	else
		printf("Smithy(): FAIL when test number of players.\n");

	//printf("Score read: %d\n", holder);
	if(state->handCount[0] >= (handPCount[0] + 2))
	{
		if(state->handCount[0] > (handPCount[0] + 3))
			printf("Smithy(): FAIL when test added more than 3 cards.\n");
		else
			printf("Smithy(): PASS when test add 3 cards.\n");
	}
	else
		printf("Smithy(): FAIL when test add 3 cards.\n");

	//printf("Score read: %d\n", holder);
	if(state->handCount[0] == (handPCount[0] + 3))
		printf("Smithy(): FAIL when test no cards were discarded.\n");
	else
	{
		for(i=0; i < state->handCount[0]; i++) // Deck was set up to only have one smithy card in hand, test to see that card no longer is inside the hand
		{
			if(state->hand[0][i] == smithy)
			{
				printf("Smithy(): FAIL when test Smithy card was not discarded.\n");
				break;
			}
			else if(i == (state->handCount[0] - 1))//check for last pass
				printf("Smithy(): PASS when test Smithy card was discarded.\n");
		}
	}


	//Test that the deck has been changed
	//NOTE: it is possible that the deck will be the exact same afterwards, but the odds are very slim
	player_holder = cmpDeck(deckP[0], state->deck[0], deckPpos[0], state->deckCount[0]);
	if(player_holder == 2)	
	{
		printf("Smithy(): FAIL when testing deck wasn't changed after card played.\n");
	}
	else //player_holder is -1 or 0 or 1
		printf("Smithy(): PASS when testing deck was altered for p1.\n");
	



	//Test state of other players
	//Test Hand Count remains unchanged
	for(i=1; i < num_players; i++)
	{
		if(handPCount[i] != state->handCount[i])
		{
			printf("Smithy(): FAIL when test hand Count changed for a different player.\n");
			break;
		}
		if(i == (num_players - 1)) //Last Pass
			printf("Smithy(): PASS when test hand Count didn't change for all other players.\n");
			
			
	}

	





	//Test Discard Count remains unchanged for other players
	for(i=1; i < num_players; i++)
	{
		if(disPCount[i] != state->discardCount[i])
		{
			printf("Smithy(): FAIL when test discard Count changed for a different player.\n");
			break;
		}
		if(i == (num_players - 1)) //Last Pass
			printf("Smithy(): PASS when test discard Count didn't change for all other players.\n");
			
			
	}

	//Test State of other player decks
	for(i=1; i < num_players; i++)
	{		
		player_holder = cmpDeck(deckP[i], state->deck[i], deckPpos[i], state->deckCount[i]);
			if(player_holder == -1)	
			{
				printf("Smithy(): FAIL when testing deck Count changed for a different player\n");
				break;
			}
			if(player_holder == 0)	
			{
				printf("Smithy(): FAIL when testing deck contents changed for a different player.\n");
				break;
			}
			if(player_holder == 1)	
			{
				printf("Smithy(): FAIL when testing deck order changed for a different player.\n");
				break;
			}
			if((player_holder == 2) && (i == (num_players - 1)))
				printf("Smithy(): PASS when testing decks were not altered for all other players.\n");
	}
			
}

int main(int argc, char *argv[])
{
    testsmithy(3, 5, 0);
    testsmithy(4, 10, 2);
    testsmithy(2, 8, 7);
    testsmithy(2, 1, 0);
    return 0;
}



