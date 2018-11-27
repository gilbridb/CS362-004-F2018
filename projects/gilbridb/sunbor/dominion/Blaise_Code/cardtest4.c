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
//			adventurerTest()
//			
// This function should test mutliple things regarding the Adventurer card implementation:
//
// Test that adventurer card is discarded
// Test different types of treasures(copper, silver, gold)
// Test different locations for two treasures (discard and deck piles)
// Test that both cards are added to hand and removed from deck/discard
// Test that the cards of other players are left unaffected 
// Test that the number of total players is not changed
//
// This should suffice to test a large part of the possible places for error.
******************************************************************/

void testadventurer(int num_players, int hand_size_in, int adventurer_pos, int deck_size_in, int discard_size_in, int loc1, int loc2, int type1, int type2, int pos1, int pos2)
{
	printf("Tests for: %d players, %d hand size, %d adventurer card position, %d deck size, %d discard size\nTREASURE 1: %d type(4 = copper, 5 = silver, 6 = gold), %d location(0 = deck, 1 = discard), %d position in the pile (0 is top)\nTREASURE 2: %d type, %d location, %d position\n", num_players, hand_size_in, adventurer_pos, deck_size_in, discard_size_in, type1, loc1, pos1, type2, loc2, pos2);
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

	int not_adventurer = k[1];
	if(k[1] == adventurer){not_adventurer = k[2];}


	//Manually set hand, discard, and deck for player
	state->handCount[0] = 0;
	for(i = 0; i < hand_size_in; i++)
	{
		if(i == adventurer_pos)
		{
			state->hand[0][state->handCount[0]] = adventurer;
			state->handCount[0]++;
		}
		else
		{
			state->hand[0][state->handCount[0]] = not_adventurer;
			state->handCount[0]++;
		}
	}

	state->deckCount[0] = 0;
	for(i = 0; i < deck_size_in; i++)
	{
		if((loc1 == 1) && (i == pos1))
		{
			state->deck[0][state->deckCount[0]] = type1;
			state->deckCount[0]++;
		}
		if((loc2 == 1) && (i == pos2))
		{
			state->deck[0][state->deckCount[0]] = type2;
			state->deckCount[0]++;
		}
		else
		{
			state->deck[0][state->deckCount[0]] = not_adventurer;
			state->deckCount[0]++;
		}
	}
	state->discardCount[0] = 0;
	for(i = 0; i < discard_size_in; i++)
	{
		if((loc1 == 0) && (i == pos1))
		{
			state->discard[0][state->discardCount[0]] = type1;
			state->discardCount[0]++;
		}
		if((loc2 == 0) && (i == pos2))
		{
			state->discard[0][state->discardCount[0]] = type2;
			state->discardCount[0]++;
		}
		else
		{
			state->discard[0][state->discardCount[0]] = not_adventurer;
			state->discardCount[0]++;
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

	//Run Function
	holder = cardEffect(adventurer, 0,0,0,&G,adventurer_pos, bonus);
	
	//Checks Number of players remains unchanged
	if(player_holder == state->numPlayers)
		printf("Adventurer(): PASS when test number of players.\n");
	else
		printf("Adventurer(): FAIL when test number of players.\n");
	
	//Test if both treasures are in hand
	int tester = 0;
	for(i = 0; i < state->handCount[0]; i++)
	{
		if(state->hand[0][i] == type1)
		{
			tester++;
		}		

		if(state->hand[0][i] == type2)
		{
			tester++;
		}
		if(tester == 2)
		{
			printf("Adventurer(): PASS when test both treasures in hand.\n");
			break;
		}
		if(i == (state->handCount[0] - 1))
		{
			printf("Adventurer(): FAIL when test treasures missing from hand.\n");
		}
	}


	//Checks Number of cards in hand
	//printf("FOR TESTING %d vs %d\n",state->handCount[0], (handPCount[0] + 1));
	if(state->handCount[0] == (handPCount[0] + 1))
		printf("Adventurer(): PASS when test correct number of cards in hand.\n");
	else
		printf("Adventurer(): FAIL when test incorrect number of cards in hand.\n");




	//Check Adventurer was discarded
	for(i=0; i < state->handCount[0]; i++) // Deck was set up to only have one smithy card in hand, test to see that card no longer is inside the hand
	{
		if(state->hand[0][i] == adventurer)
		{
			printf("Adventurer(): FAIL when test Adventurer card was not discarded.\n");
			break;
		}
		else if(i == (state->handCount[0] - 1))//check for last pass
			printf("Adventurer(): PASS when test Adventurer card was discarded.\n");
	}

	tester = -1;
	//Check that treasures were removed from deck and discard
	for(i=0; i < state->deckCount[0]; i++)
	{
		tester = 0;
		if((state->deck[0][i] == type1) || (state->deck[0][i] == type2))
		{
			printf("Adventurer(): FAIL when test Treasures still in deck.\n");
			break;
		}
		else if(i == (state->deckCount[0] - 1))
			tester = -1; //So far so good
	}
	if(tester == -1)
	{
		for(i=0; i < state->discardCount[0]; i++)
		{
			if((state->discard[0][i] == type1) || (state->discard[0][i] == type2))
			{
				printf("Adventurer(): FAIL when test Treasures still in discard.\n");
				break;
			}
			else if(i == (state->discardCount[0] - 1))
				printf("Adventurer(): PASS when test Treasures no longer in discard or deck.\n");
		}
	
	}

	//Test correct amount of card in deck and discard after function
	//Should be -2 for treasures, +1 for advneturer card = -1 total
	//printf("FOR TESTING %d vs %d\n",(state->deckCount[0] + state->discardCount[0]), (deckPpos[0] + disPCount[0] - 1));
	if((state->deckCount[0] + state->discardCount[0]) == (deckPpos[0] + disPCount[0] - 1))
		printf("Adventurer(): PASS when test discard + deck count.\n");
	else
		printf("Adventurer(): FAIL when test discard + deck count is off.\n");



	//Test state of other players
	//Test Hand Count remains unchanged
	for(i=1; i < num_players; i++)
	{
		if(handPCount[i] != state->handCount[i])
		{
			printf("Adventurer(): FAIL when test hand Count changed for a different player.\n");
			break;
		}
		if(i == (num_players - 1)) //Last Pass
			printf("Adventurer(): PASS when test hand Count didn't change for all other players.\n");
			
			
	}

	//Test Discard Count remains unchanged for other players
	for(i=1; i < num_players; i++)
	{
		if(disPCount[i] != state->discardCount[i])
		{
			printf("Advneturer(): FAIL when test discard Count changed for a different player.\n");
			break;
		}
		if(i == (num_players - 1)) //Last Pass
			printf("Adventurer(): PASS when test discard Count didn't change for all other players.\n");
			
			
	}

	//Test State of other player decks
	for(i=1; i < num_players; i++)
	{		
		player_holder = cmpDeck(deckP[i], state->deck[i], deckPpos[i], state->deckCount[i]);
			if(player_holder == -1)	
			{
				printf("Adventurer(): FAIL when testing deck Count changed for a different player\n");
				break;
			}
			if(player_holder == 0)	
			{
				printf("Adventurer(): FAIL when testing deck contents changed for a different player.\n");
				break;
			}
			if(player_holder == 1)	
			{
				printf("Adventurer(): FAIL when testing deck order changed for a different player.\n");
				break;
			}
			if((player_holder == 2) && (i == (num_players - 1)))
				printf("Adventurer(): PASS when testing decks were not altered for all other players.\n");
	}
			
}

int main(int argc, char *argv[])
{
	//void testadventurer(int num_players, int hand_size_in, int adventurer_pos, int deck_size_in, int discard_size_in, int loc1, int loc2, int type1, int type2, int pos1, int pos2)

    testadventurer(3, 5, 0, 15, 10, 0, 1, copper, copper, 0, 9);//Functionality with 2 copper, one in each pile, bookend location
    testadventurer(2, 5, 2, 2, 10, 0, 0, silver, silver, 0, 1);//Functionality with 2 silver, both in deck, make up entire deck
    testadventurer(4, 5, 3, 15, 2, 1, 1, copper, silver, 1, 0);//Functionality with 1 copper 1 silver, both in discard, make up entire pile
    testadventurer(3, 2, 0, 6, 6, 0, 1, gold, gold, 3, 3);//Functionality with 2 gold, one in each pile, center of pile


    return 0;
}



