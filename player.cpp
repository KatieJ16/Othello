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

void Player::setBoard(char boardData[64]){
	board->setBoard(boardData);
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
     
     //do opponents move
     board->doMove(opponentsMove, opponentsSide);
     
     if(board->isDone()){
		 return NULL;
	 }
	 if (!(board->hasMoves(side))){
		 return NULL;
	 }
	 
	 if(testingMinimax){
		 return minimax();
	 }else{
	 
		 Move *best = new Move(-1, -1);
		 int scoreBest;
		 for(int i = 0; i < 8; i ++){
			 for(int j = 0; j < 8; j ++){
				 Move *move = new Move(i, j);
				 if(board->checkMove(move, side)){
					 int score = this->heuristic(move, board->copy());
					 if(scoreBest < score){
						 best = move;
						 scoreBest = score;
					 }else {
						 delete move;
					 }
				 }
				 
			}
		 }
		
		board->doMove(best, side);
		return best;
	}
}

int Player::heuristic(Move *move, Board * board2){
	int x = move->getX();
	int y = move->getY();
	//move
	if(!testingMinimax){
	board2->doMove(move, side);
}
	else{
		board2->doMove(move, opponentsSide);
	}
	//find score
	int score = board2->count(side) - board2->count(opponentsSide);
	
	if(!testingMinimax){
		//corners
		if((x == 0 and (y == 0 or y == 7)) or (x == 7 and (y == 0 or y == 7))){
			score += 10;
		} else if (x == 0 or x == 7 or y == 0 or y == 7){ //edges
			//spaces that lead to corners without corner taken 
			if((((x == 0 and y == 1) or (x == 1 and y == 0) or (x == 1 and y == 1)) and !(board2->occupied(0, 0)))
			or (((x == 0 and y == 6) or (x == 1 and y == 6) or (x == 1 and y == 7)) and !(board2->occupied(0, 7)))
			or (((x == 6 and y == 0) or (x == 6 and y == 1) or (x == 7 and y == 1)) and !(board2->occupied(7, 0)))
			or (((x == 6 and y == 6) or (x == 6 and y == 7) or (x == 7 and y == 6)) and !(board2->occupied(7, 7)))){
				score -= 7;
			} else{
				score += 4;
			}
		}
	}
	
	return score;
}

Move *Player::minimax(){
	std::vector<Move *> moveList;
	
	
	//find all possible moves
	for(int i = 0; i < 8; i ++){
		 for(int j = 0; j < 8; j ++){
			 Move *move = new Move(i, j);
			 if(board->checkMove(move, side)){
				 moveList.push_back(move);
			 }else{
				 delete move;
			 }
		}
	 }
	 
	 int minMaxIndex = 0;
	 int minMax = findMin(moveList[0], board->copy());
	 //check lowest score for each possible move
	 
	 for(unsigned int i = 1; i < moveList.size(); i ++){
		 Board *board2 = board->copy();
		 int worst = findMin(moveList[i], board2);
		 if(worst > minMax){
			 minMaxIndex = i;
		 }
		
	 }
	 
	 return moveList[minMaxIndex];
	
}

int Player::findMin(Move * move, Board * board2){
	board2->doMove(move, side);
	
	int scoreWorst = 100000;
	for(int i = 0; i < 8; i ++){
		 for(int j = 0; j < 8; j ++){
			 Move *move1 = new Move(i, j);
			 if(board2->checkMove(move1, opponentsSide)){
				 int score = this->heuristic(move1, board2);
				 if(scoreWorst > score){
					 scoreWorst = score;
				 }
				 delete move1;
			 }
			 
		}
	}
	return scoreWorst;
	
}
