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
//			adventurerRandomTest()
// This function makes use of random testing to test the Smithy card function
//
******************************************************************/
void adventurerRandomTest()
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
	int card_in_test = adventurer;
	int card_pos = 0;
	k[0] = card_in_test; // Make sure adventurer is a part of the set
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
		num = (rand() % (MAX_HAND + 1));
		post->handCount[i] = num;
		for(j = 0; j < post->handCount[i]; j++)
		{
			num = (rand() % (17));// choices 0 - 16 for cards[17]
			post->hand[i][j] = cards[num];
		}
		
		
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

	
	//Copy current game into pre before enacting the function on the current
	memcpy(&gamepre, post, sizeof(struct gameState));


	//enact function for testing on post
	int bonus[2] = {0,0}; // *bonus satisfier, does nothing in the code as far as I can tell
	int holder = 0;
	holder = cardEffect(card_in_test, 0,0,0,&gamepost,card_pos, bonus);
	int path = 0;

	//Alter pre to be what is expected of post

	//3 Possible outcomes:
	//1. 2 treasures drawn from deck
	//2. 2 treasures drrawn after shuffling discard into deck
	//3. 1 or 0 treasures found in deck and discard
	
	//discard Card
		
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
	
	int tempHand[1000]; //Must be large enough to store MAX_DECK and MAX_DISCARD
	int tempCount = 0;
	int treasure_count = 0;
	for(i=0; i < pre->deckCount[chosen_player]; i++)
	{
		if((pre->deck[chosen_player][i] == copper)||(pre->deck[chosen_player][i] == silver)||(pre->deck[chosen_player][i] == gold))
		{
			pre->hand[chosen_player][pre->handCount[chosen_player]] = pre->deck[chosen_player][i];
			pre->handCount[chosen_player]++;
			treasure_count++;
			for( j = i; j < (pre->deckCount[chosen_player]-1); j++)
			{
				pre->deck[chosen_player][j] = pre->deck[chosen_player][j+1];
			}
			pre->deckCount[chosen_player]--;
			if(treasure_count == 2)
			{
				break;
			}
		}
		else
		{
			tempHand[tempCount] = pre->deck[chosen_player][i];
			tempCount++;		
			for( j = i; j < (pre->deckCount[chosen_player]-1); j++)
			{
				pre->deck[chosen_player][j] = pre->deck[chosen_player][j+1];
			}
			pre->deckCount[chosen_player]--;
		}
	}


	int dis_treasure_count = treasure_count;
	//int treasure_holder = treasure_count;

	if(treasure_count > 1)
	{
		while((tempCount - 1) >= 0)
		{
			pre->discard[chosen_player][pre->discardCount[chosen_player]] = tempHand[tempCount-1];
			pre->discardCount[chosen_player]++;
			tempCount--;
		}
	}
	else	//From this point on it gets tricky due to shuffle
		//Regardless of the amount of treasures in the discard, we'll be grabbing
		//the freshest cards in post hand, adding them to our own, 
		//and copying the discard and deck.
		//We can first look through the discard for treasures so as to make an
		//estimate of how many treasures we end up with, but we can't really
		//test anything else
	{
		path = 1;
		for(i=0; i < pre->discardCount[chosen_player]; i++)
		{
			if((pre->deck[chosen_player][i] == copper)||(pre->deck[chosen_player][i] == silver)||(pre->deck[chosen_player][i] == gold))
			{
				dis_treasure_count++;
				if(dis_treasure_count == 2)
				{
					break;
				}
			}
		}
		
		//fill rest of the hand with cards straight from post
		//Due to random shuffle, can't predict next drawn cards
		dis_treasure_count = dis_treasure_count - treasure_count;
		int initial = post->handCount[chosen_player];

		//dis_treasure_count and treasure_count will be compared at the end
		//to make sure to ensure that if two treasures were available across the
		//deck and discard, then the end hand contains 2 extra cards
		if(dis_treasure_count == 2)
		{
			pre->hand[chosen_player][pre->handCount[chosen_player]] = post->hand[chosen_player][post->handCount[chosen_player]-2];
			pre->handCount[chosen_player]++;
			pre->hand[chosen_player][pre->handCount[chosen_player]] = post->hand[chosen_player][post->handCount[chosen_player]-1];
			pre->handCount[chosen_player]++;
		}
		else if(dis_treasure_count == 1)
		{
			pre->hand[chosen_player][pre->handCount[chosen_player]] = post->hand[chosen_player][post->handCount[chosen_player]-1];
			pre->handCount[chosen_player]++;
		}

		//set deck and discard equal to that of post
		memcpy(gamepre.deck[chosen_player], post->deck[chosen_player], sizeof(int)*MAX_DECK);
		memcpy(gamepre.discard[chosen_player], post->discard[chosen_player], sizeof(int)*MAX_DECK);
		pre->deckCount[chosen_player] = post->deckCount[chosen_player];
		pre->discardCount[chosen_player] = post->discardCount[chosen_player];

	} 


	//assert(memcmp(&gamepre, post, sizeof(struct gameState)) == 0);
	if((memcmp(&gamepre, post, sizeof(struct gameState)) != 0) ||(dis_treasure_count != treasure_count))
	{

		printf("Test failed on #%i\nNumPlayers: %i, Chosen_Player: %i, hand_pos: %i\n", times_run, pre->numPlayers, chosen_player, card_pos);
		printf("Adventurer game state for player: Deck Count: %i, Discard Count: %i, Hand Count: %i,\n PlayedCardCount: %i, Coin: %i, Action: %i, Buy: %i\nLast two cards in hand (4 = copper, 5 = silver, 6 = gold): %i, %i\n", post->deckCount[chosen_player], post->discardCount[chosen_player], post->handCount[chosen_player], post->playedCardCount, post->coins, post->numActions, post->numBuys, post->hand[chosen_player][post->handCount[chosen_player]-2], post->hand[chosen_player][post->handCount[chosen_player]-1]);

		if(path == 0)
		{
		printf("What game state should be for player: Deck Count: %i, Discard Count: %i, Hand Count: %i,\n PlayedCardCount: %i, Coin: %i, Action: %i, Buy: %i\nLast two cards in hand (4 = copper, 5 = silver, 6 = gold): %i, %i\n", pre->deckCount[chosen_player], pre->discardCount[chosen_player], pre->handCount[chosen_player], pre->playedCardCount, pre->coins, pre->numActions, pre->numBuys, pre->hand[chosen_player][pre->handCount[chosen_player]-2], pre->hand[chosen_player][pre->handCount[chosen_player]-1]);
		}
		else
		{
		printf("What game state should be: Due to shuffle, cannot test deck or discard, Hand Count: %i,\n PlayedCardCount: %i, Coin: %i, Action: %i, Buy: %i\nLast two cards in hand (4 = copper, 5 = silver, 6 = gold): %i, %i\n Treasure found in deck: %i, can the rest be filled with discard?: %i\n", pre->handCount[chosen_player], pre->playedCardCount, pre->coins, pre->numActions, pre->numBuys, pre->hand[chosen_player][pre->handCount[chosen_player]-2], pre->hand[chosen_player][pre->handCount[chosen_player]-1], treasure_count, dis_treasure_count);
		}
		success--;

	}

}


int main(int argc, char *argv[])
{
	//int i=0;
	printf("Beginning random Test 3 for Adventurer().\n");
	for(times_run=0; times_run<ITERATIONS;times_run++)
	{
		adventurerRandomTest();
	printf("\n\n");
	}

	printf("Number of successful runs: %i/%i\n", success, ITERATIONS);
	printf("Finished random Test 3 for Adventurer().\n\n\n\n\n");
	return 0;
}



