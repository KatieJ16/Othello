#include "player.h"
#include <unistd.h>

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
     if(opponentsMove != NULL){
		board->doMove(opponentsMove, opponentsSide);
	}
     
     //check if you can move
     if(board->isDone()){
		 return NULL;
	 }
	 if (!(board->hasMoves(side))){
		 return NULL;
	 }
	 
	 //when testing minimax
	 if(testingMinimax){
		 return minimax();
	 }
	 
	 //If corner avaiable, take it
	 Move * cornerMove = corner();
	 if(cornerMove != NULL){
		 board->doMove(cornerMove, side);
		 //std::cerr << "CORNER" << std::endl;
		 return cornerMove;
	 }
	 
	 Move *best = minimax();//Simple();//
	 //std::cerr << "Move = (" << best->getX() << ", " << best->getY() << ")" << std::endl;
		
	 board->doMove(best, side);
	// sleep(1);
	 return best;
		
		
	
}

Move * Player::Simple(){
	 Move *best = new Move(-1, -1);//will stay (-1, -1) if no possible move
	 int scoreBest = -100000;
	 for(int i = 0; i < 8; i ++){
		 for(int j = 0; j < 8; j ++){
			 Move *move = new Move(i, j);
			 if(board->checkMove(move, side)){
				 //find possible move with the highest score
				 int score = this->heuristic(move, board->copy());
				/* if(leadToCorner(move, board)){
					 score = -99999;
				 }*/
				 //std::cerr << " Simple score = " << score << std::endl;
				 if(scoreBest < score){
					 best = move;
					 scoreBest = score;
				 }else {
					 delete move;
				 }
			 }
			 
		}
	}
	return best;
		
}
bool Player::leadToCorner(Move * move, Board * board2){
	int x = move->getX();
	int y = move->getY();
	if((((x == 0 and y == 1) or (x == 1 and y == 0) or (x == 1 and y == 1)) and !(board2->occupied(0, 0)))
	or (((x == 0 and y == 6) or (x == 1 and y == 6) or (x == 1 and y == 7)) and !(board2->occupied(0, 7)))
	or (((x == 6 and y == 0) or (x == 6 and y == 1) or (x == 7 and y == 1)) and !(board2->occupied(7, 0)))
	or (((x == 6 and y == 6) or (x == 6 and y == 7) or (x == 7 and y == 6)) and !(board2->occupied(7, 7)))){
		Board * board3 = board2->copy();
		board3->doMove(move, side);
		if(board3->checkMove(move, opponentsSide)){
			delete board3;
			return true;
		}
		delete board3;
		return false;
	}
	return false;
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
				score -= 10;
				//std::cerr << "move = (" << x << ", " << y << ") almost corner"<< std::endl;
			} else{
				score += 5;
			}
		}
	}
	
	return score;
}

Move *Player::corner(){
	Move * move = new Move(0,0);
	if(board->checkMove(move, side)){
		return move;
	}
	delete move;
	
	move = new Move(7,0);
	if(board->checkMove(move, side)){
		return move;
	}
	delete move;
	
	move = new Move(0,7);
	if(board->checkMove(move, side)){
		return move;
	}
	delete move;
	
	move = new Move(7,7);
	if(board->checkMove(move, side)){
		return move;
	}
	delete move;
	
	return NULL;
}

Move *Player::minimax(){
	std::vector<Move *> moveList;
	int depth = board->numMovesLeft()  - 1;
	if(board->numMovesLeft() < 10){
		return Simple();
	}
	
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
	 
	 //vector<vector<Move *> > depth = depthMin(3, board->copy(), side);
	 //moveList = depth.back();
	 int minMaxIndex = 0;
	 int minMax = depthMin(depth, moveList[0], board->copy(), side);
	 //std::cerr << " for Move = (" << moveList[0]->getX() << ", " << moveList[0]->getY() << ") min score = " << minMax << std::endl;
	 //check lowest score for each possible move and finds max min score
	 
	 for(unsigned int i = 1; i < moveList.size(); i ++){
		 Board *board2 = board->copy();
		 int worst = depthMin(depth, moveList[i], board2, side);
		 if((worst > minMax) and (worst != 100000)){
			 minMaxIndex = i;
			 minMax = worst;
			 //std::cerr << "change Move to (" << moveList[i]->getX() << ", " << moveList[i]->getY() << ") minMax = " << minMax << std::endl;
			 
		 }
		 //std::cerr << " for Move = (" << moveList[i]->getX() << ", " << moveList[i]->getY() << ") min score = " << worst << std::endl;
		
	 }
	
	 
	 return moveList[minMaxIndex];
	
}

int Player:: depthMin(int depthInt, Move *move, Board * board2, Side side1){
	Side side2;
	if(side1 == BLACK){
		side2 = WHITE;
	} else{
		side2 = BLACK;
	}
	
	std::vector<Move *> moveList;
	std::vector<vector<Move *> > depth;
	for (int i = 0;  i < depthInt; i ++){
		moveList.clear();
		board2->doMove(move, side1);
		
		//int scoreWorst = 100000;//make worst score very high so any possible score will be lower
		for(int i = 0; i < 8; i ++){
			 for(int j = 0; j < 8; j ++){
				 Move *move1 = new Move(i, j);
				 if(board2->checkMove(move1, side2)){
					 moveList.push_back(move1);
				 }
				 
			}
		}
		
		//flip sides
		side1 = side2;
		if(side2 == BLACK){
			side2 = WHITE;
		} else{
			side2 = BLACK;
		}
		
		depth.push_back(moveList);
	}
	int minIndex = 0;
	 int min = findMin(moveList[0], board2->copy());
	 //std::cerr << " for Move = (" << moveList[0]->getX() << ", " << moveList[0]->getY() << ") min score = " << min << std::endl;
	 //check lowest score for each possible move and finds max min score
	 
	 for(unsigned int i = 1; i < moveList.size(); i ++){
		 int worst = findMin(moveList[i], board2);
		 if((worst > min) and (worst != 100000)){
			 minIndex = i;
			 min = worst;
		//	 std::cerr << "change Move to (" << moveList[i]->getX() << ", " << moveList[i]->getY() << ") minMax = " << min << std::endl;
			 
		 }
		 //std::cerr << " for Move = (" << moveList[i]->getX() << ", " << moveList[i]->getY() << ") min score = " << worst << std::endl;
		
	 }
	 
	 return min;
	
}

int Player::findMin(Move * move, Board * board2){
	board2->doMove(move, opponentsSide);
	
	int scoreWorst = 100000;//make worst score very high so any possible score will be lower
	for(int i = 0; i < 8; i ++){
		 for(int j = 0; j < 8; j ++){
			 Move *move1 = new Move(i, j);
			 if(board2->checkMove(move1, side)){
				 int score = this->heuristic(move1, board2);
				 //std::cerr << "Move = (" << move1->getX() << ", " << move1->getY() << ") score = " << score << std::endl;
				 if(scoreWorst > score){
					 scoreWorst = score;
				 }
				 delete move1;
			 }
			 
		}
	}
	//returns lowest possible score
	return scoreWorst;
	
}
