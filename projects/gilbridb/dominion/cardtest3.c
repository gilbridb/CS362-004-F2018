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
//			treasureMapTest()
//			
// This function should test mutliple things regarding the Treasure Map card implementation:
//
// Test that 4 Gold are placed on top of deck
// Test that both treasure map cards are discarded AND trashed
// Test return if only one treasure map is in hand
// Test that the cards of other players are left unaffected 
// Test that the number of total players is not changed
//
// This should suffice to test a large part of the possible places for error.
******************************************************************/

void testtreasureMap(int num_players, int hand_size_in, int num_t, int t1_pos, int t2_pos, int exp_return)
{
	printf("Tests for: %d players, %d hand size, %d number of treasure map cards, %d %d Treasure map card positions, %d expected return\n", num_players, hand_size_in, num_t, t1_pos, t2_pos, exp_return);
	int i;
	int j;
	int holder;
	int player_holder = 0;	
	int bonus[2] = {0,0}; // *bonus satisfier

	struct gameState G;
	struct gameState * state = &G;
  	int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
           great_hall, treasure_map, smithy};
	initializeGame(num_players, k, 10, &G); //number players, cards, randomSeed, gameState

	//Holder number of players before card call
	player_holder = state->numPlayers;

	int not_treasure_map = k[0];
	if(k[0] == treasure_map){not_treasure_map = k[1];}


	//Manually set hand for player
	state->handCount[0] = 0;
	for(i = 0; i < hand_size_in; i++)
	{
		if(((i == t1_pos) && (num_t > 0)) || ((i == t2_pos) && (num_t > 1)))
		{
			state->hand[0][state->handCount[0]] = treasure_map;
			state->handCount[0]++;
		}
		else
		{
			state->hand[0][state->handCount[0]] = not_treasure_map;
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
	holder = cardEffect(treasure_map, 0,0,0,&G,t1_pos, bonus);
	
	//Checks Number of players remains unchanged
	if(player_holder == state->numPlayers)
		printf("Treasure_Map(): PASS when test number of players.\n");
	else
		printf("Treasure_Map(): FAIL when test number of players.\n");

	//Checks Return Value is what was expected
	if(holder == exp_return)
		printf("Treasure_Map(): PASS when test return value.\n");
	else
		printf("Treasure_Map(): FAIL when test return value.\n");

	//SPLIT TESTS BASED ON RETURN VALUE
	if(holder != -1)
	{
		//Test 2 cards were discarded
		for(i=0; i < state->handCount[0]; i++)
		{
			if(state->hand[0][i] == treasure_map)
			{
				printf("Treasure_Map(): FAIL when test treasure map cards still present in hand.\n");
				break;
			}
			else if(i == (state->handCount[0] - 1))//Check if last pass
			{
				if(state->handCount[0] == (handPCount[0] - 2))
					printf("Treasure_Map(): PASS when test treasure maps discarded.\n");
				else
					printf("Treasure_Map(): FAIL when test hand size not what it should be after discard of 2 treasure maps.\n");
			}
		}
		//Test cards were trashed
		for(i=0; i < state->discardCount[0]; i++)
		{
			if(state->discard[0][i] == treasure_map)
			{
				printf("Treasure_Map(): FAIL when test treasure maps not trashed.\n");
				break;
			}
			else if(i == (state->discardCount[0] - 1))
				printf("Treasure_Map(): PASS when test treasure maps trashed.\n");
		}

		//Test 4 Gold were added to top of deck
		int gold_count = 0;
		for(i=0; i < state->deckCount[0]; i++)
		{
			if(state->deck[0][i] == gold)
			{
				gold_count++;
				if((gold_count == 4) && (i == 3))
					{
					printf("Treasure_Map(): PASS when test 4 gold on top of deck.\n");
					break;
					}
			}
			else if(gold_count >= 4)
			{
				printf("Treasure_Map(): FAIL when test 4 gold in deck, not on top.\n");
				break;
			}
			else if(i == (state->deckCount[0] - 1))//Check if last pass
			{
				printf("Treasure_Map(): FAIL when test 4 gold not in deck.\n");
			}
		}

	}
	else
	{
		if(state->handCount[0] == handPCount[0])
			printf("Treasure_Map(): PASS when test didn't change hand for -1 return.\n");
		else
		printf("Treasure_Map(): FAIL when test hand size changed for -1 return.\n");
	}

	//Test state of other players
	//Test Hand Count remains unchanged
	for(i=1; i < num_players; i++)
	{
		if(handPCount[i] != state->handCount[i])
		{
			printf("Treasure_Map(): FAIL when test hand Count changed for a different player.\n");
			break;
		}
		if(i == (num_players - 1)) //Last Pass
			printf("Treasure_Map(): PASS when test hand Count didn't change for all other players.\n");
			
			
	}

	





	//Test Discard Count remains unchanged for other players
	for(i=1; i < num_players; i++)
	{
		if(disPCount[i] != state->discardCount[i])
		{
			printf("Treasure_Map(): FAIL when test discard Count changed for a different player.\n");
			break;
		}
		if(i == (num_players - 1)) //Last Pass
			printf("Treasure_Map(): PASS when test discard Count didn't change for all other players.\n");
			
			
	}

	//Test State of other player decks
	for(i=1; i < num_players; i++)
	{		
		player_holder = cmpDeck(deckP[i], state->deck[i], deckPpos[i], state->deckCount[i]);
			if(player_holder == -1)	
			{
				printf("Treasure_Map(): FAIL when testing deck Count changed for a different player\n");
				break;
			}
			if(player_holder == 0)	
			{
				printf("Treasure_Map(): FAIL when testing deck contents changed for a different player.\n");
				break;
			}
			if(player_holder == 1)	
			{
				printf("Treasure_Map(): FAIL when testing deck order changed for a different player.\n");
				break;
			}
			if((player_holder == 2) && (i == (num_players - 1)))
				printf("Treasure_Map(): PASS when testing decks were not altered for all other players.\n");
	}
			
}

int main(int argc, char *argv[])
{
	//void testtreasureMap(int num_players, int hand_size_in, int num_t, int t1_pos, int t2_pos, int exp_return)
    testtreasureMap(3, 5, 2, 0, 1, 1); //Normal Operation
    testtreasureMap(4, 8, 2, 7, 0, 1); //Bookend map placement
    testtreasureMap(4, 2, 2, 1, 0, 1); //Hand Only Maps
    testtreasureMap(2, 10, 1, 5, 7, -1); //Only one map
    return 0;
}



