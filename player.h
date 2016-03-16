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
    int negamax(Move * move, int depth, int color, Board * b);
    Move * negamaxStart(int depth);
    Move *Simple();
    bool leadToCorner(Move * move,  Board * board2);
    bool IsCorner(Move * move);
    int heuristic(Move *move, Board * board2);
    int heuristic(Board * board2);
    Move *corner();
    Move *minimax();
    int depthMin(int depth, Move *move, Board * board2, Side side1);
    //std::vector<Board *> allPossibleMoves(Board * b, Side s);
    std::vector< std::vector<Move *> > allPossibleMoves(Board * b, Side s, std::vector<Move *> moveList);
    int findMin(Board * board2, std::vector<Move *> moveList);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
    Side side;
    Side opponentsSide;
	Board * board;
};

#endif
