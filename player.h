#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include <vector>
#include "common.h"
#include "board.h"
using namespace std;

class Player {

public:
    Player(Side side);
    ~Player();
    
	void setBoard(char boardData[64]);
    Move *doMove(Move *opponentsMove, int msLeft);
    int heuristic(Move *move, Board * board2);
    Move *corner();
    Move *minimax();
    int depthMin(int depth, Move *move, Board * board2, Side side1);
    int findMin(Move * move, Board * board2, Side thisSide);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
    Side side;
    Side opponentsSide;
	Board * board;
};

#endif
