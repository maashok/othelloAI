#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.h"
#include <limits>
#include "board.h"
using namespace std;

class Player {
	Side me;
	Side opp;
	Board *board;
public:
    Player(Side side);
    ~Player();
    bool haveTime;
	int depth;
    void setBoard(Board *newBoard);
    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

#endif
