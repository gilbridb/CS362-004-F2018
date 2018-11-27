#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#define ITERATIONS 1000

//global variable
int times_run = 0;
int success = ITERATIONS;


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
//			smithyRandomTest()
// This function makes use of random testing to test the Smithy card function
//
******************************************************************/
void smithyRandomTest()
{
	//Create a random game state within reason
	struct gameState gamepre;
	struct gameState gamepost;

	struct gameState *pre;
	pre = &gamepre;
	struct gameState *post;
	post = &gamepost;



	//Fill K with random cards and also the card being tested
	int k[10];
	int i;
	int j;
	int num;//general purpose int
	int card_in_test = smithy;
	int card_pos = 0;
	k[0] = card_in_test; // Make sure smithy is a part of the set
	for(i = 1; i < 10; i++)
	{
		//Card info: 26 is last card, 7 is first card that would be found in deck
		//rand % (upper + 1 - lower) -> (26 + 1 - 7) -> 20
		num = 7 + (rand() % 20);
		k[i] = num;
		for(j = 0; j < i; j++)
		{
			if (k[j] == num)
			{
				i--;	//this will force i to "redo" the num if it already exists in the list
				break;
			}
		}//End for loop j	
	}// End for loop i
	
	//Random player count
	//pick a random number between minimum player count(2) and max
	num = 2 + (rand() % (MAX_PLAYERS + 1));

	//radom seed
	i = 1 + rand()% 100; //random 1 - 100
	
	//initialize game, makes sure any values that should be filled in are filled in
	initializeGame(num, k, i, &gamepost);

	//Choose a player
	//This will be the player playing the card
	int chosen_player = rand() % (post->numPlayers);
	post->whoseTurn = chosen_player;

	//randomize all players hands, decks, and discards
	int cards[17] = {curse, estate, duchy, province, copper, silver, gold, k[0], k[1], k[2], k[3], k[4], k[5], k[6], k[7], k[8], k[9]}; //list of possible card to fill up deck, hand, or discard


	for(i = 0; i < post->numPlayers; i++)
	{
		//randomize hand
		//num = (rand() % (MAX_HAND + 1));
		//post->handCount[i] = num;
		post->handCount[i] = (rand() % (MAX_HAND + 1));
		
		if(post->handCount[i] < 0) 
		{
			post->handCount[i] += 10;
			printf("NEW post->handCount: %i", post->handCount[i]);
		}//Somehow getting negative values, this is a fix
		for(j = 0; j < post->handCount[i]; j++)
		{
			num = (rand() % (17));// choices 0 - 16 for cards[17]
			post->hand[i][j] = cards[num];
		}
		if(post->handCount[i] < 0) {printf("RED HOT! %i!\n", times_run);}	
		
		//randomize deck
		num = (rand() % (MAX_DECK + 1));
		post->deckCount[i] = num;
		for(j = 0; j < post->deckCount[i]; j++)
		{
			num = (rand() % (17));// choices 0 - 16 for cards[17]
			post->deck[i][j] = cards[num];
		}


		//randomize discard
		num = (rand() % (MAX_DECK + 1));
		post->discardCount[i] = num;
		for(j = 0; j < post->discardCount[i]; j++)
		{
			num = (rand() % (17));// choices 0 - 16 for cards[17]
			post->discard[i][j] = cards[num];
		}


	}
	if(post->handCount[chosen_player] < 0) {printf("It happened right away %i!\n", times_run);}	

	//Randomized playedCards
	post->playedCardCount = rand() % (MAX_DECK + 1);
	for(i = 0; i < post->playedCardCount; i++)
	{
		post->playedCards[i] = k[rand() % 10]; //Chose a random playable card
		if(post->playedCards[i] == outpost)
		{
			post->outpostPlayed = 1;
		}
	}
	


	
	//chose a random handslot for the card to be played from and place the card in that spot	
	if(post->handCount[chosen_player] != 0)
	{
		//card_pos chosen based on numPlayers in order to get around segfault caused by bug
		//...otherwise would make use of commented line below
		card_pos = rand() % (post->numPlayers + 1); // chose a random hand slot
		//card_pos = rand() % (post->handCount[chosen_player]); // chose a random hand slot
		post->hand[chosen_player][card_pos] = card_in_test;
	}
	else
	{
		post->handCount[chosen_player] = 1;
		card_pos = 0;
		post->hand[chosen_player][card_pos] = card_in_test;
	}

	if(post->handCount[chosen_player] < 0) {printf("It happened at handslot %i!\n", times_run);}	
	//Randomize OutpostTurn between 0-1
	post->outpostTurn = rand() % 2;

	
	//Randomize numActions, coins, numBuys up to 100
	post->numActions = rand() % 101;
	post->numBuys = rand() % 101;
	post->coins = rand() % 101;
	
	//Randomize Supply Counts
	i = post->supplyCount[curse];
	post->supplyCount[curse] = rand() % (i + 1);

	i = post->supplyCount[estate];
	post->supplyCount[estate] = rand() % (i + 1);
	
	i = post->supplyCount[duchy];
	post->supplyCount[duchy] = rand() % (i + 1);

	i = post->supplyCount[province];
	post->supplyCount[province] = rand() % (i + 1);

	i = post->supplyCount[copper];
	post->supplyCount[copper] = rand() % (i + 1);

	i = post->supplyCount[silver];
	post->supplyCount[silver] = rand() % (i + 1);

	i = post->supplyCount[gold];
	post->supplyCount[gold] = rand() % (i + 1);

	for(i=0; i<17; i++)
	{
		j = post->supplyCount[cards[i]];
		if(j >= 0)
		{
			post->supplyCount[cards[i]] = rand() % (j + 1);
		}
		else
		{
			post->supplyCount[cards[i]] = rand() % 11; //0-10
		}
	}

	if(post->handCount[chosen_player] < 0) {printf("It happened pretty late %i!\n", times_run);}	

	//Randomly Apply Embargo Tokens
	//Not completely random as it favors earlier stacks, but can technically create
	//...all possible combos
	j = 10;//amount of possible embargo tokens in a game
	for(i = 0; i < 17; i++)
	{
		if(j > 0)
		{
			num = rand() % (j+1); 
			post->embargoTokens[cards[i]] = num;
			j -= num;
		}
		else{ break;}
	}

	//Randomize phase
	//NOTE: The documentation and code seems to make little to no use of phase
	//...but from what I can tell, it varies from 0 to 1
	post->phase = rand() % 2; //random number from 0-1
	

	//All game variables randomized to reasonable values!

	//To Catch a Bug!
	/*
	if((post->whoseTurn > MAX_PLAYERS) || (post->numPlayers > MAX_PLAYERS))
	{
		printf("WHOA NELLY!, whose:%i, players:%i", post->whoseTurn, post->numPlayers);
	}
	else
	{
		printf("Hold it kelly!, whose:%i, players:%i", post->whoseTurn, post->numPlayers);
	}
	*/	

	//TESTING HERE
	if((post->handCount[chosen_player] < 0)||(post->discardCount[chosen_player] < 0)||
		(post->deckCount[chosen_player] < 0)||(post->playedCardCount < 0))
	{
		printf("\n\n\nWHAT A CONUNDRUM ON %i!\nHAND: %i\nDISCARD: %i\nDECK: %i\nPLAYED: %i\n\n\n", times_run, post->handCount[chosen_player], post->discardCount[chosen_player], post->deckCount[chosen_player], post->playedCardCount);
	}



	
	//Copy current game into pre before enacting the function on the current
	memcpy(&gamepre, post, sizeof(struct gameState));

	//enact function for testing on post
	int bonus[2] = {0,0}; // *bonus satisfier, does nothing in the code as far as I can tell
	int holder = 0;
	holder = cardEffect(card_in_test, 0,0,0,&gamepost,card_pos, bonus);


	//Alter pre to be what is expected of post


	if(pre->deckCount[chosen_player] >= 3) //Shuffling not needed
	{

		//draw three cards
		for(i=0; i<3; i++)
		{
			int hand_count = pre->handCount[chosen_player];
			int deck_count = pre->deckCount[chosen_player];
			pre->hand[chosen_player][hand_count] = pre->deck[chosen_player][deck_count - 1];
			pre->deckCount[chosen_player]--;
			pre->handCount[chosen_player]++;
		}

		//discard Smithy
		
		//Add card to played card pile
		pre->playedCards[pre->playedCardCount] = pre->hand[chosen_player][card_pos];
		pre->playedCardCount++;


		//set played card to -1
		pre->hand[chosen_player][card_pos] = -1;
		//remove card from player's hand
		if ( card_pos == (pre->handCount[chosen_player] - 1) )       //last card in hand array is played
          	{
//reduce number of cards in hand
			pre->handCount[chosen_player]--;
        	}
		else if ( pre->handCount[chosen_player] == 1 ) //only one card in hand
		{
			//reduce number of cards in hand
			pre->handCount[chosen_player]--;
		}
		else
		{
			//replace discarded card with last card in hand
			pre->hand[chosen_player][card_pos] = pre->hand[chosen_player][ (pre->handCount[chosen_player] - 1)];
			//set last card to -1
			pre->hand[chosen_player][pre->handCount[chosen_player] - 1] = -1;
			//reduce number of cards in hand
			pre->handCount[chosen_player]--;
		}

/*
		for(i=card_pos; i < pre->handCount[chosen_player]; i++)
		{
			pre->hand[chosen_player][i] = pre->hand[chosen_player][i + 1];	
		}
*/
	}
//	else if((pre->deckCount[chosen_player] + pre->discardCount[chosen_player]) >= 3) // Shuffling occurs, but discard large enough to fill
//	{

//	}
		//Turns out to be same as the middle case, moving both to the same
	else	//Shuffling occurs and there were not enough cards in discard for all 3 draws
	{

		//discard Smithy
		
		//Add card to played card pile
		pre->playedCards[pre->playedCardCount] = pre->hand[chosen_player][card_pos];
		pre->playedCardCount++;


		//set played card to -1
		pre->hand[chosen_player][card_pos] = -1;
		//remove card from player's hand
		if ( card_pos == (pre->handCount[chosen_player] - 1) )       //last card in hand array is played
          	{
//reduce number of cards in hand
			pre->handCount[chosen_player]--;
        	}
		else if ( pre->handCount[chosen_player] == 1 ) //only one card in hand
		{
			//reduce number of cards in hand
			pre->handCount[chosen_player]--;
		}
		else
		{

			//replace discarded card with last card in hand
			pre->hand[chosen_player][card_pos] = pre->hand[chosen_player][ (pre->handCount[chosen_player] - 1)];
			//set last card to -1
			pre->hand[chosen_player][pre->handCount[chosen_player] - 1] = -1;
			//reduce number of cards in hand
			pre->handCount[chosen_player]--;
		}

		//draw deck
		int initial = pre->deckCount[chosen_player];
		int hand_count = pre->handCount[chosen_player];
		for(i=0; i<initial; i++)
		{
			hand_count = pre->handCount[chosen_player];
			int deck_count = pre->deckCount[chosen_player];
			pre->hand[chosen_player][hand_count] = pre->deck[chosen_player][deck_count - 1];
			pre->deckCount[chosen_player]--;
			pre->handCount[chosen_player]++;
		}

		//fill rest of the hand with cards straight from post
		//Due to random shuffle, can't predict next drawn cards
		initial = post->handCount[chosen_player];
		if(initial > pre->handCount[chosen_player])
		{
			hand_count = pre->handCount[chosen_player]; 
			for(i= hand_count; i<initial; i++)
			{
				pre->hand[chosen_player][i] = post->hand[chosen_player][i];
				pre->handCount[chosen_player]++;
			}
		}
		
		//set deck and discard equal to that of post
		memcpy(gamepre.deck[chosen_player], post->deck[chosen_player], sizeof(int)*MAX_DECK);
		
		memcpy(gamepre.discard[chosen_player], post->discard[chosen_player], sizeof(int)*MAX_DECK);
		pre->deckCount[chosen_player] = post->deckCount[chosen_player];
		pre->discardCount[chosen_player] = post->discardCount[chosen_player];

	}


	//assert(memcmp(&gamepre, post, sizeof(struct gameState)) == 0);
	if(memcmp(&gamepre, post, sizeof(struct gameState)) != 0)
	{
		printf("Test failed on #%i\nNumPlayers: %i, Chosen_Player: %i, hand_pos: %i\n", times_run, pre->numPlayers, chosen_player, card_pos);
		printf("Smithy game state for player: Deck Count: %i, Discard Count: %i, Hand Count: %i,\n PlayedCardCount: %i, Coin: %i, Action: %i, Buy: %i\n", post->deckCount[chosen_player], post->discardCount[chosen_player], post->handCount[chosen_player], post->playedCardCount, post->coins, post->numActions, post->numBuys);
		printf("What game state should be for player: Deck Count: %i, Discard Count: %i, Hand Count: %i,\n PlayedCardCount: %i, Coin: %i, Action: %i, Buy: %i\n", pre->deckCount[chosen_player], pre->discardCount[chosen_player], pre->handCount[chosen_player], pre->playedCardCount, pre->coins, pre->numActions, pre->numBuys);
		success--;
	}

}


int main(int argc, char *argv[])
{
	//int i=0;
	printf("Beginning random Test 1 for Smithy().\n");
	for(times_run=0; times_run<ITERATIONS;times_run++)
	{
		smithyRandomTest();
	printf("\n\n");
	}

	printf("Number of successful runs: %i/%i\n", success, ITERATIONS);
	printf("Finished random Test 1 for Smithy().\n\n\n\n\n");
	return 0;
}



