#include "player.h"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side1) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
    board = new Board();
    side = side1;
    if(side == BLACK){
		opponentsSide = WHITE;
	} else{
		opponentsSide = BLACK;
	}

    /* 
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
}

/*
 * Destructor for the player.
 */
Player::~Player() {
	delete board;
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be NULL.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return NULL.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    /* 
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */ 
     
     //do opponents move
     board->doMove(opponentsMove, opponentsSide);
     std::cerr << "side = " << side << std::endl;
     
     if(board->isDone()){
		 std::cerr <<"Done!" << std::endl;
		 return NULL;
	 }
	 if (!(board->hasMoves(side))){
		 std::cerr << "no moves" << std::endl;
		 return NULL;
	 }
	 
	 Move *best = new Move(-1, -1);
	 std::cerr << "best = (" << best->getX() << ", " << best->getY() << ")" << std::endl;
	 int scoreBest;
	 for(int i = 0; i < 8; i ++){
		 for(int j = 0; j < 8; j ++){
			 std::cerr << "best = (" << best->getX() << ", " << best->getY() << ")" << std::endl;
			 Move *move = new Move(i, j);
			 std::cerr << "checking (" << i << ", " << j << ")" << std::endl;
			 if(board->checkMove(move, side)){
				 int score = this->heuristic(move);
				 std::cerr << "score = " << score << std::endl;
				 if(scoreBest < score){
					 best = move;
					 scoreBest = score;
					 std::cerr << "best = (" << best->getX() << ", " << best->getY() << ")" << std::endl;
				 }else {
					 delete move;
				 }
			 }
			 
		}
	 }
	 std::cerr << "best = (" << best->getX() << ", " << best->getY() << ")" << std::endl;
	if(best->getX() == -1){
		return NULL;
	}
	board->doMove(best, side);
	return best;
}

int Player::heuristic(Move *move){
	//clone board
	Board *board2 = board->copy();
	int x = move->getX();
	int y = move->getY();
	//move
	board2->doMove(move, side);
	//find score
	int score = board2->count(side) - board2->count(opponentsSide);
	
	//corners
	if((x == 0 and (y == 0 or y == 7)) or (x == 7 and (y == 0 or y == 7))){
		score *= 5;
	} else if (x == 0 or x == 7 or y == 0 or y == 7){ //edges
		score *= 2;
	}
	
	delete board2;
	return score;
}
