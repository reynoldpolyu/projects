/*
 *  randomtestcard.c
 *
 *  Created on: May 2, 2015
 *      Author: Spencer Hubbard
 *
 *  This is a random test generator for the mine card. The input for each test
 *  is generated by picking two random treasure cards (copper, silver, or gold).
 *  The first random treasure card is added to the hand and trashed when the
 *  mine card is played. The second random treasure card is gained when the mine
 *  card is played. The output checked by each test is the number of coins,
 *  handCount, and playedCardCount. If the second random treasure card costs
 *  more than two more than the first random treasure card, then the mine card
 *  should not be played, otherwise the mine card should be played.
 *
 *  Note that there are only nine different test inputs corresponding to the
 *  nine different pairs of two treasure cards. So, it would make more sense to
 *  test these inputs exhaustively instead of randomly.
 */

#include <assert.h>  // for assert()
#include <stdbool.h> // for bool type (true, false)
#include <stdio.h>   // for NULL, printf()
#include <stdlib.h>  // for malloc(), free(), exit(), rand(), srand()
#include <time.h>    // for time()

#include "dominion.h"
#include "rngs.h"
#include "Verify362.h"

#define RANDOM_SEED 42
#define MAX_TESTS 100
#define MIN_PLAYERS 2
#define OFFSET 3

static int kingdom_cards[10] = {adventurer, council_room, feast, gardens, mine,
    remodel, smithy, village, baron, great_hall};

// TODO: comment
static void debug(struct gameState *state);

static void debug(struct gameState *state) {
  printf("\n*** debugging information ***\n");
  printf("whoseTurn      : %d\n", state->whoseTurn);
  printf("phase          : %d\n", state->phase);
  printf("numActions     : %d\n", state->numActions);
  printf("numBuys        : %d\n", state->numBuys);
  printf("coins          : %d\n", state->coins);
  printf("playedCardCount: %d\n", state->playedCardCount);
  printf("handCount      : %d\n", state->handCount[state->whoseTurn]);
  printf("deckCount      : %d\n", state->deckCount[state->whoseTurn]);
  printf("discardCount   : %d\n", state->discardCount[state->whoseTurn]);
}

int main(int argc, char **argv) {
  srand(RANDOM_SEED);
  
  for (int i = 0; i < MAX_TESTS; i++) {
    // initial setup
    struct gameState state;
    int randomSeed = rand();
    int numPlayers = rand() % (MAX_PLAYERS - MIN_PLAYERS + 1) + MIN_PLAYERS;
    initializeGame(numPlayers, kingdom_cards, randomSeed, &state);
    int whoseTurn = state.whoseTurn;

    // make sure phase = 0, numActions > 0, handCount > 1
    state.phase = 0;
    state.numActions = state.numActions > 0 ? state.numActions : 1;
    state.handCount[whoseTurn] = state.handCount[whoseTurn] > 1 ? state.handCount[whoseTurn] : 2;
    state.hand[whoseTurn][0] = mine;

    // pick random treasure cards and
    int coin1 = rand() % 3 + 1 + OFFSET;
    int coin2 = rand() % 3 + 1 + OFFSET;
    state.hand[whoseTurn][1] = coin1;
    
    // expected state after test
    int coins = state.coins;
    int handCount = state.handCount[whoseTurn];
    int playedCardCount = state.playedCardCount;
    if (coin2 - coin1 <= 1) {
      coins += coin2 * (coin2 - OFFSET) - coin1 * (coin1 - OFFSET);
      handCount--;
      playedCardCount++;
    }

    // should not be allowed to buy treasure card costing > 3 more
    playCard(0, 1, coin2, 0, &state);

    // test state after card effect
    Verify362(whoseTurn == state.whoseTurn);
    Verify362(coins == state.coins);
    Verify362(handCount == state.handCount[whoseTurn]);
    Verify362(playedCardCount == state.playedCardCount);
  }

  printf("random test for mine card passed\n");

  return EXIT_SUCCESS;
}
