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
	 
	 //If corner avaiable, take it
	 Move * cornerMove = corner();
	 if(cornerMove != NULL){
		 board->doMove(cornerMove, side);
		 return cornerMove;
	 }
	 
	 Move *best = negamaxStart(4);
		
	 board->doMove(best, side);
	 return best;
		
		
	
}

Move * Player::negamaxStart(int depth){
	//last move
	if(board->numMovesLeft() < (depth + 1)){
		return Simple();
	}
	//find all moves
	std::vector<Move *> moveList;
	for(int i = 0; i < 8; i ++){
		for(int j = 0; j < 8; j ++){
			Move * move = new Move(i, j);
			if(board->checkMove(move, side)){
				moveList.push_back(move);
			} else{
				delete move;
			}
		}	
	}
	int bestscore = -10000;
	int bestIndex = 0;
	//find negamax score for each possible move
	for(unsigned int i = 0; i < moveList.size(); i ++){
		Board * b = board->copy();
		b->doMove(moveList[i], side);
		int score = negamax(moveList[i], depth, -1, b);
		//find largest to return
		if(score > bestscore){
			bestscore = score;
			bestIndex = i;
		}
	}
	return moveList[bestIndex];
}
//resursive part of negamax
int Player::negamax(Move * move, int depth, int color, Board * b){
	if(depth == 0){//base case
		return color * heuristic(b);
	}
	Side currentSide;
	//make currentSide
	if(color == -1){
		currentSide = side;
	}else{
		currentSide = opponentsSide;
	}
	//find all possible moves
	std::vector<Move *> moveList;
	for(int i = 0; i < 8; i ++){
		for(int j = 0; j < 8; j ++){
			Move * move1 = new Move(i, j);
			if(b->checkMove(move1, currentSide)){
				moveList.push_back(move1);
			}else{
				delete move1;
			}
		}	
	}
	int bestValue = -100000;
	for(unsigned int i = 0; i < moveList.size(); i ++){
		Board * b2 = b->copy();
		b2->doMove(moveList[i], currentSide);
		//for corners
		if(IsCorner(moveList[i])){
			return 100;	
		}
		//recursive part
		int value = -1 * negamax(moveList[i], depth - 1, -1 * color, b2);
		delete b2;
		//find max to return
		if(value > bestValue){
			bestValue = value;
			
		}
	}
	return bestValue;
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
			score += 100;
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

int Player::heuristic(Board * board2){
	return board2->count(side) - board2->count(opponentsSide);
}

//checks if a move is a corner
bool Player::IsCorner(Move * move){
	int x = move->getX();
	int y = move->getY();
	if((x == 0 and (y == 0 or y == 7)) or (x == 7 and (y == 0 or y == 7))){
		return true;
	}
	return false;
}

//returns corner if corner is avaiable
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

