#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.h"
#include "board.h"
using namespace std;

class Player {

public:
    Player(Side side);
    ~Player();
    
    Move *doMove(Move *opponentsMove, int msLeft);
    int heuristic(Move *move);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
    Side side;
    Side opponentsSide;
	Board * board;
};

#endif
