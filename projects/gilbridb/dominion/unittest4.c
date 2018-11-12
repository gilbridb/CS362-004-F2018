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
//			fullDeckCount()
//			
// For card count implementation it goes like this: deck_in/deck_tot
// Thus, for calls deck_tot must always be greater than deck_in
//
// This function should test mutliple things regarding the ScoreFor() function:
//
// Test that hand, deck, and discard card count add up correctly
// Test that the cards of other players are left unaffected 
// Test that the number of total players is not changed
//
// This should suffice to test a large part of the possible places for error.
******************************************************************/

void testfullDeckCount(int num_players, int search_card, int deck_in, int hand_in, int dis_in, int deck_tot, int hand_tot, int dis_tot, int score_in)
{
	printf("Tests for: %d players, %d card being searched. Card totals:\n%d/%d in deck, %d/%d in hand, %d/%d in discard\nExpected Result: %d\n", num_players, search_card, deck_in, deck_tot, hand_in, hand_tot, dis_in, dis_tot, score_in);
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

	//Manually set deck, discard, and hand for player
	state->deckCount[0] = 0;
	state->discardCount[0] = 0;
	state->handCount[0] = 0;

	int not_search = k[0];
	if(k[0] == search_card) {not_search = k[1];}

	//ORDERING, for good testing putting 1 searched card in front one in back and the rest in between as long as the amount allows
	holder = deck_in;
	for(i = 0; i < deck_tot; i++)
	{
		if((deck_in > 0) && (i == 0))
		{
			state->deck[0][state->deckCount[0]] = search_card;
			state->deckCount[0]++;
		}
		else if((deck_in > 1) && (i == (deck_tot -1)))
		{	
			state->deck[0][state->deckCount[0]]= search_card;
			state->deckCount[0]++;
		}
		else if(holder > 2)
		{
			state->deck[0][state->deckCount[0]] = search_card;
			state->deckCount[0]++;
			holder--;
		}
		else
		{
			state->deck[0][state->deckCount[0]] = not_search;
			state->deckCount[0]++;
		}
	}	

	holder = hand_in;
	for(i = 0; i < hand_tot; i++)
	{
		if((hand_in > 0) && (i == 0))
		{
			state->hand[0][state->handCount[0]] = search_card;
			state->handCount[0]++;
		}
		else if((hand_in > 1) && (i == (hand_tot -1)))
		{	
			state->hand[0][state->handCount[0]]= search_card;
			state->handCount[0]++;
		}
		else if(holder > 2)
		{
			state->hand[0][state->handCount[0]] = search_card;
			state->handCount[0]++;
			holder--;
		}
		else
		{
			state->hand[0][state->handCount[0]] = not_search;
			state->handCount[0]++;
		}
	}
	
	holder = dis_in;
	for(i = 0; i < dis_tot; i++)
	{
		if((dis_in > 0) && (i == 0))
		{
			state->discard[0][state->discardCount[0]] = search_card;
			state->discardCount[0]++;
		}
		else if((dis_in > 1) && (i == (dis_tot -1)))
		{	
			state->discard[0][state->discardCount[0]]= search_card;
			state->discardCount[0]++;
		}
		else if(holder > 2)
		{
			state->discard[0][state->discardCount[0]] = search_card;
			state->discardCount[0]++;
			holder--;
		}
		else
		{
			state->discard[0][state->discardCount[0]] = not_search;
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

	//Shuffle Player 1 deck
	holder = fullDeckCount(0, search_card, &G);
	
	//Checks Number of players remains unchanged
	if(player_holder == state->numPlayers)
		printf("FullDeckCount(): PASS when test number of players.\n");
	else
		printf("FullDeckCount(): FAIL when test number of players.\n");

	//printf("Score read: %d\n", holder);
	if(score_in == holder)
		printf("FullDeckCount(): PASS when test card count.\n");
	else
		printf("FullDeckCount(): FAIL when test card count.\n");

	//Test state of other players
	//Test Hand Count remains unchanged
	for(i=0; i < num_players; i++)
	{
		if(handPCount[i] != state->handCount[i])
		{
			printf("FullDeckCount(): FAIL when test hand Count changed for a player.\n");
			break;
		}
		if(i == (num_players - 1)) //Last Pass
			printf("FullDeckCount(): PASS when test hand Count didn't change for all players.\n");
			
			
	}


	//Test Discard Count remains unchanged
	for(i=0; i < num_players; i++)
	{
		if(disPCount[i] != state->discardCount[i])
		{
			printf("FullDeckCount(): FAIL when test discard Count changed for a player.\n");
			break;
		}
		if(i == (num_players - 1)) //Last Pass
			printf("FullDeckCount(): PASS when test discard Count didn't change for all players.\n");
			
			
	}

	//Test State of other player decks
	for(i=0; i < num_players; i++)
	{		
		player_holder = cmpDeck(deckP[i], state->deck[i], deckPpos[i], state->deckCount[i]);
			if(player_holder == -1)	
			{
				printf("FullDeckCount(): FAIL when testing deck Count changed for a player.\n");
				break;
			}
			if(player_holder == 0)	
			{
				printf("FullDeckCount(): FAIL when testing deck contents changed for a player.\n");
				break;
			}
			if(player_holder == 1)	
			{
				printf("FullDeckCount(): FAIL when testing deck order changed for a player.\n");
				break;
			}
			if((player_holder == 2) && (i == (num_players - 1)))
				printf("FullDeckCount(): PASS when testing decks were not altered for all players.\n");
	}			
		
}

int main(int argc, char *argv[])
{
//void testfullDeckCount(int num_players, int search_card, int deck_in, int hand_in, int dis_in, int deck_tot, int hand_tot, int dis_tot, score_in)
    testfullDeckCount(3, gardens, 3, 3, 3, 7, 8, 9, 9); // General test
    testfullDeckCount(4, feast, 0, 0, 0, 0, 0, 8, 0); // Testing Empty deck, discard, and no search cards
    testfullDeckCount(2, minion, 0, 8, 0, 15, 8, 15, 8); //Testing all in hand and entire hand
    testfullDeckCount(2, gardens, 6, 0, 0, 20, 0, 0, 6); // Testing all in deck, empty others
    testfullDeckCount(2, gardens, 0, 0, 8, 0, 0, 10, 8); // Testing all in discard, empty others
    return 0;
}



