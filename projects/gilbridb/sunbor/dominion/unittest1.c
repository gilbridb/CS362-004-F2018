#include<stdio.h>
#include<stdlib.h>
#include <math.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
//#include "dominion.c"



/********************************************************************
			Shuffle Function Code
int shuffle(int player, struct gameState *state) {


  int newDeck[MAX_DECK];
  int newDeckPos = 0;
  int card;
  int i;

  if (state->deckCount[player] < 1)
    return -1;
  qsort ((void*)(state->deck[player]), state->deckCount[player], sizeof(int), compare);
  // SORT CARDS IN DECK TO ENSURE DETERMINISM! 

  while (state->deckCount[player] > 0) {
    card = floor(Random() * state->deckCount[player]);
    newDeck[newDeckPos] = state->deck[player][card];
    newDeckPos++;
    for (i = card; i < state->deckCount[player]-1; i++) {
      state->deck[player][i] = state->deck[player][i+1];
    }
    state->deckCount[player]--;
  }
  for (i = 0; i < newDeckPos; i++) {
    state->deck[player][i] = newDeck[i];
    state->deckCount[player]++;
  }

  return 0;
}



*******************************************************************/

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
//			testShuffle()
//			
// This function should test mutliple things regarding the Shuffle function:
//
// Test that deck count of player being shuffled does not change
// Test that the decks of other players are not changed
// Test that the order of the deck is changed (shuffled) in most cases
// Check for a fail with too small deck size
// Test for deck unchanged when deck size 1.
// Test Player count does not change
// Contents of the deck must not change
//
// This should suffice to test a large part of the possible places for error.
******************************************************************/

void testShuffle(){
	int i;
	int j;
	int holder;
	int player_holder = 0;	

	struct gameState G;
	struct gameState * state = &G;
  	int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
           sea_hag, tribute, smithy};
	initializeGame(3, k, 10, &G); //number players, cards, randomSeed, gameState
	//Holder number of players before Shuffle()
	player_holder = state->numPlayers;




	//Initialize Deck holders for all players
	int deckP[3][MAX_DECK];
	int deckPpos[3];
	
	//Capture current decks in deck holders for all players
	for(j = 0; j < 3; j++){	
		holder = state->deckCount[j];
		for(i = 0; i < holder; i++)
		{
			//FOR TESTING printf("Deck %d card %d\n", j, state->deck[j][i]);
			deckP[j][deckPpos[j]] = state->deck[j][i];//current Player top card = current card of deck
			deckPpos[j]++;//increase top of deck
		}
	}//End for loop

	//Shuffle Player 1 deck
	holder = shuffle(0, &G);
	
	//Checks Number of players remains unchanged
	if(player_holder == state->numPlayers)
		printf("Shuffle(): PASS when test number of players.\n");
	else
		printf("Shuffle(): FAIL when test number of players.\n");

	//Test various things after the shuffle of player 1
	player_holder = cmpDeck(deckP[0], state->deck[0], deckPpos[0], state->deckCount[0]);
	if(player_holder == -1)	
		printf("Shuffle(): FAIL when testing p1 amount of cards after shuffle.\n");
	if(player_holder == 0)	
		printf("Shuffle(): FAIL when testing p1 contents of deck after shuffle.\n");
	if(player_holder == 2)	
		printf("Shuffle(): FAIL when testing p1 order of cards changed after shuffle.\n");
	if(player_holder == 1)	
		printf("Shuffle(): PASS when testing p1 cards are shuffled and deck content is the same after shuffle.\n");

	//Test other players after shuffle of player 1
	player_holder = cmpDeck(deckP[1], state->deck[1], deckPpos[1], state->deckCount[1]);
	if(player_holder == -1)	
		printf("Shuffle(): FAIL when testing p2 amount of cards after shuffle.\n");
	if(player_holder == 0)	
		printf("Shuffle(): FAIL when testing p2 contents of deck after shuffle.\n");
	if(player_holder == 1)	
		printf("Shuffle(): FAIL when testing p2 order of cards changed after shuffle.\n");
	if(player_holder == 2) // Passed only for player 2
		{
			player_holder = cmpDeck(deckP[2], state->deck[2], deckPpos[2], state->deckCount[2]);
			if(player_holder == -1)	
				printf("Shuffle(): FAIL when testing p3 amount of cards after shuffle.\n");
			if(player_holder == 0)	
				printf("Shuffle(): FAIL when testing p3 contents of deck after shuffle.\n");
			if(player_holder == 1)	
				printf("Shuffle(): FAIL when testing p3 order of cards changed after shuffle.\n");
			if(player_holder == 2)
				printf("Shuffle(): PASS when testing p2 and p3 were not altered.\n");
				
		}
	
	state->deckCount[0] = 0; //Set deck empty for p1 to test for empty deck shuffle()
	holder = shuffle(0, &G);
	if(holder != -1)
		printf("Shuffle(): FAIL when testing shuffle of deck 0, it was allowed.\n");
	else
		printf("Shuffle(): PASS when testing shuffle of deck 0, not allowed.\n");


	state->deckCount[0] = 1; //Set deck to one card for p1 to test for deck shuffle()
	
	//Set Deck Holder to new deck size
	deckP[0][0] = state->deck[0][0];
	deckPpos[0] = 1;


	holder = shuffle(0, &G);
	//Test various things after the shuffle of player 1
	player_holder = cmpDeck(deckP[0], state->deck[0], deckPpos[0], state->deckCount[0]);
	if(player_holder == -1)	
		printf("Shuffle(): FAIL when testing p1 amount of cards after 1 card shuffle.\n");
	if(player_holder == 0)	
		printf("Shuffle(): FAIL when testing p1 contents of deck after 1 card shuffle.\n");
	if(player_holder == 1)	
		printf("Shuffle(): FAIL when testing p1 order of cards changed after 1 card shuffle.\n");
	if(player_holder == 2)	
		printf("Shuffle(): PASS when testing p1 deck is same after 1 card shuffle.\n");


}

int main(int argc, char *argv[])
{
    testShuffle();
    return 0;
}



