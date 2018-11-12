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
//			embargoTest()
//			
// This function should test mutliple things regarding the Embargo card implementation:
//
// Test that coins = coins +2 for p1
// Test that the cards of other players are left unaffected 
// Test that the number of total players is not changed
// Test that card is discarded AND trashed
// Test that embargo token was added to chosen pile, not others
// Test that choosing a supply count at -1 returns -1
//
// This should suffice to test a large part of the possible places for error.
******************************************************************/

void testembargo(int num_players, int hand_size_in, int embargo_pos, int choice1, int exp_return)
{
	printf("Tests for: %d players, %d hand size, %d embargo card position, %d choice1, expected return: %d\n", num_players, hand_size_in, embargo_pos, choice1, exp_return);
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

	//Set suppy Count 3 to -1 for -1 test
	state->supplyCount[3] = -1;

	int not_embargo = k[0];
	if(k[0] == embargo){not_embargo = k[1];}

	//Manually set hand for player
	state->handCount[0] = 0;
	for(i = 0; i < hand_size_in; i++)
	{
		if(i == embargo_pos)
		{
			state->hand[0][state->handCount[0]] = embargo;
			state->handCount[0]++;
		}
		else
		{
			state->hand[0][state->handCount[0]] = not_embargo;
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

	int coin_holder = state->coins;
	int embargo_tester = state->embargoTokens[choice1];
	
	int embargo_holder[10];
	for(i=0; i < 10; i++)
	{
		embargo_holder[i] = state->embargoTokens[i];
	}


	//Shuffle Player 1 deck
	holder = cardEffect(embargo, choice1,0,0,&G,embargo_pos, bonus);
	
	//Checks Number of players remains unchanged
	if(holder == exp_return)
		printf("Embargo(): PASS when test return value.\n");
	else
		printf("Embargo(): FAIL when test return value.\n");
	
	//Checks Number of players remains unchanged
	if(player_holder == state->numPlayers)
		printf("Embargo(): PASS when test number of players.\n");
	else
		printf("Embargo(): FAIL when test number of players.\n");

	//Checks coins + 2
	if(state->coins == (coin_holder + 2))
		printf("Embargo(): PASS when test coins + 2.\n");
	else
		printf("Embargo(): FAIL when test coins + 2.\n");
	
	//Checks Embargo token added
	if(state->embargoTokens[choice1] == (embargo_tester + 1))
		{
			for(i=0; i<10; i++)
			{
				if((i != choice1) && (embargo_holder[i] != state->embargoTokens[i]))
				{
					printf("Embargo(): FAIL when test embargo tokens were changed on piles other than choice1.\n");
					break;
				}
				else if( i == 9) // check for last pass
					printf("Embargo(): PASS when test embargo token was added to pile choice1 only.\n");

			}

		}
	else
		printf("Embargo(): FAIL when test embargo token was not added to pile choice1.\n");

	//Test Embargo discarded
	for(i=0; i < state->handCount[0]; i++) // Deck was set up to only have one smithy card in hand, test to see that card no longer is inside the hand
	{
		if(state->hand[0][i] == embargo)
		{
			printf("Embargo(): FAIL when test Embargo card was not discarded.\n");
			break;
		}
		else if(i == (state->handCount[0] - 1))//check for last pass
			printf("Embargo(): PASS when test Embargo card was discarded.\n");
	}


	//Test Embargo trash
	for(i=0; i < state->discardCount[0]; i++) // Deck was set up to only have one smithy card in hand, test to see that card no longer is inside the hand
	{
		if(state->discard[0][i] == embargo)
		{
			printf("Embargo(): FAIL when test Embargo card was not trashed.\n");
			break;
		}
		else if(i == (state->discardCount[0] - 1))//check for last pass
			printf("Embargo(): PASS when test Embargo card was trashed (Not in discard).\n");
	}



	//Test state of other players
	//Test Hand Count remains unchanged
	for(i=1; i < num_players; i++)
	{
		if(handPCount[i] != state->handCount[i])
		{
			printf("Embargo(): FAIL when test hand Count changed for a different player.\n");
			break;
		}
		if(i == (num_players - 1)) //Last Pass
			printf("Embargo(): PASS when test hand Count didn't change for all other players.\n");
			
			
	}

	//Test Discard Count remains unchanged
	for(i=0; i < num_players; i++)
	{
		if(disPCount[i] != state->discardCount[i])
		{
			printf("Embargo(): FAIL when test discard Count changed for a player.\n");
			break;
		}
		if(i == (num_players - 1)) //Last Pass
			printf("Embargo(): PASS when test discard Count didn't change for all players.\n");
			
			
	}


	//Test State of other player decks
	for(i=1; i < num_players; i++)
	{		
		player_holder = cmpDeck(deckP[i], state->deck[i], deckPpos[i], state->deckCount[i]);
			if(player_holder == -1)	
			{
				printf("Embargo(): FAIL when testing deck Count changed for a different player\n");
				break;
			}
			if(player_holder == 0)	
			{
				printf("Embargo(): FAIL when testing deck contents changed for a different player.\n");
				break;
			}
			if(player_holder == 1)	
			{
				printf("Embargo(): FAIL when testing deck order changed for a different player.\n");
				break;
			}
			if((player_holder == 2) && (i == (num_players - 1)))
				printf("Embargo(): PASS when testing decks were not altered for all other players.\n");
	}
			
}

int main(int argc, char *argv[])
{
    testembargo(3, 5, 0, 1, 0);//Normal function
    testembargo(4, 8, 6, 7, 0);//Normal function with different player size, hand size, slot choice, pile selection, etc.
    testembargo(2, 9, 8, 3, -1); //Supply Count set to -1 at 3 for testing the -1 return
    testembargo(4, 4, 1, 11, 0); // Outside pile boundary call

    return 0;
}



