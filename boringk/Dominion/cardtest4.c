#include "assert.h"
#include "dominion.h"
#include <stdio.h>
#include "rngs.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

int checkState(struct gameState *prevState, struct gameState *nextState, int coin_bonus){
		//printf("# of handCount = %d and prev %d\n",nextState->handCount[nextState->whoseTurn],prevState->handCount[prevState->whoseTurn]);
		//printf("# of actions = %d and prev %d\n",nextState->numActions,prevState->numActions);
		//printf("# of buys = %d and prev %d\n",nextState->numBuys,prevState->numBuys);
		assert((nextState->handCount[nextState->whoseTurn] - 2 ) == prevState->handCount[prevState->whoseTurn]);
		assert((nextState->numActions )== prevState->numActions);
		assert((nextState->numBuys )== prevState->numBuys);
		assert((nextState->coins )== prevState->coins);
		assert((nextState->playedCardCount - 1) == prevState->playedCardCount);
		assert(coin_bonus == 0);
		//does not increment discardCount
		//assert((nextState->discardCount[nextState->whoseTurn] - 1) == prevState->discardCount[prevState->whoseTurn] );
		assert((nextState->deckCount[nextState->whoseTurn] + 3) == prevState->deckCount[prevState->whoseTurn]);		

		return 0;
}


int main (int argc, char** argv) {
  struct gameState G;
  struct gameState *p = &G;
  
  int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, 
	       sea_hag, tribute, smithy};

  printf ("Starting test for smithy\n");
  
  int r = initializeGame(2, k, atoi(argv[1]), p);

  assert (r == 0);
    //Test card smithy
	//action card that gives
	//	+3 Card
	int coin_bonus = 0; 		//tracks coins gain from actions
	struct gameState prevState;
	
	memcpy(&prevState, p, sizeof(struct gameState));
	if(cardEffect(smithy, -1, -1, -1, p, 0, &coin_bonus) == 0){
		checkState(&prevState,p,coin_bonus);
	}
	
	//test for a full hand with random cards fails 
	//with a full deck
	srand(time(0));

	p->handCount[p->whoseTurn] = 0;
	for (int i=0; i < MAX_HAND; i++){
		p->hand[p->whoseTurn][i] = rand() %(treasure_map);
		p->handCount[p->whoseTurn]++;
	}
	for (int i=0; i < MAX_DECK; i++){
		p->deck[p->whoseTurn][i] = rand() %(treasure_map);
		p->deckCount[p->whoseTurn]++;
	}
	
	
	for(int x =0; x < 100; x++){
		memcpy(&prevState, p, sizeof(struct gameState));
		if(cardEffect(smithy, -1, -1, -1, p, 0, &coin_bonus) == 0){
			checkState(&prevState,p,coin_bonus);
		}
	}
	
	//check for only 1 card in hand
	p->hand[p->whoseTurn][0] = smithy;
	p->handCount[p->whoseTurn] = 1;
	memcpy(&prevState, p, sizeof(struct gameState));
	if(cardEffect(smithy, -1, -1, -1, p, 0, &coin_bonus) == 0){
			checkState(&prevState,p,coin_bonus);
	}
	

	printf("passed test for smithy\n");
  return 0;
}
