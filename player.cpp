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
		 std::cerr << "NULL1" << std::endl;
		 return NULL;
	 }
	 if (!(board->hasMoves(side))){
		 std::cerr << "NULL2" << std::endl;
		 return NULL;
	 }
	 
	 //If corner avaiable, take it
	 Move * cornerMove = corner();
	 if(cornerMove != NULL){
		 board->doMove(cornerMove, side);
		 //std::cerr << "CORNER" << std::endl;
		 return cornerMove;
	 }
	 
	 Move *best = negamaxStart(2);//minimax();//Simple();//
	 //std::cerr << "Move = (" << best->getX() << ", " << best->getY() << ")" << std::endl;
		
	 board->doMove(best, side);
	 //sleep(2);
	 std::cerr << "return best" << std::endl;
	 return best;
		
		
	
}
/**
 * Initial call for Player A's root node
rootNegamaxValue := negamax( rootNode, depth, 1)
rootMinimaxValue := rootNegamaxValue
 * **/

Move * Player::negamaxStart(int depth){
	if(board->numMovesLeft() < (depth)){
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
	for(int i = 0; i < moveList.size(); i ++){
		Board * b = board->copy();
		b->doMove(moveList[i], side);
		int score = negamax(moveList[i], depth, 1, b);
		//std::cerr << "	Best Move = (" << moveList[i]->getX() << ", " << moveList[i]->getY() << ") score = " << score << std::endl;
		if(score > bestscore){
			bestscore = score;
			bestIndex = i;
			//std::cerr << "bestScore = " << bestscore << std::endl;
		}
	}
	//std::cerr << " 		picked Move = (" << moveList[bestIndex]->getX() << ", " << moveList[bestIndex]->getY() << ")" << std::endl;
	return moveList[bestIndex];
}
/** function negamax(node, depth, color)
02     if depth = 0 or node is a terminal node
03         return color * the heuristic value of node

04     bestValue := −∞
05     foreach child of node
06         v := −negamax(child, depth − 1, −color)
07         bestValue := max( bestValue, v )
08     return bestValue**/
int Player::negamax(Move * move, int depth, int color, Board * b){
	if(depth == 0){
		//std::cerr << "heuristic(move, b) = " << heuristic(move, b) << std::endl;
		return color * heuristic(b);
	}
	Side currentSide;
	//make currentSide
	if(color == 1){
		currentSide = side;
	}else{
		currentSide = opponentsSide;
	}
	//find all possible moves
	std::vector<Move *> moveList;
	for(int i = 0; i < 8; i ++){
		for(int j = 0; j < 8; j ++){
			Move * move1 = new Move(i, j);
			//std::cerr << " added to be Move = (" << move1->getX() << ", " << move1->getY() << ")" << std::endl;
			if(b->checkMove(move1, currentSide)){
				moveList.push_back(move1);
				//std::cerr << " added to list Move = (" << move1->getX() << ", " << move1->getY() << ")" << std::endl;
			}else{
				delete move1;
			}
		}	
	}
	int bestValue = -100000;
	for(int i = 0; i < moveList.size(); i ++){
		Board * b2 = b->copy();
		b2->doMove(moveList[i], currentSide);
		if(IsCorner(moveList[i])){
			return 100;	
		}
		int value = -1 * negamax(moveList[i], depth - 1, -1 * color, b2);
		delete b2;
		//std::cerr << "Value =  " << value << std::endl;
		if(value > bestValue){
			bestValue = value;
			//std::cerr << "Best Value changed to " << bestValue << std::endl;
			
		}
	}
	//std::cerr << "Move = (" << move->getX() << ", " << move->getY() << ") score = " << bestValue << std::endl;
	//std::cerr << "bestValue = " << bestValue << std::endl;
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

bool Player::IsCorner(Move * move){
	int x = move->getX();
	int y = move->getY();
	if((x == 0 and (y == 0 or y == 7)) or (x == 7 and (y == 0 or y == 7))){
		return true;
	}
	return false;
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

/**Move *Player::minimax(){
	std::vector<std::vector<Move *> > moveList;
	std::vector< std::vector<std::vector<Move *> > > moveList1;
	std::vector< std::vector< std::vector<std::vector<Move *> > > > moveList2;
	//std::vector< std::vector<Board *> > boardList;
	//std::vector< std::vector< std::vector<Board *> > > boardList2;
	int depth = 1;//board->numMovesLeft()  - 1;
	if(board->numMovesLeft() < 2){
		return Simple();
	}
	std::cerr << "cehck1" << std::endl;
	
	//find all possible moves
	for(int i = 0; i < 8; i ++){
		 for(int j = 0; j < 8; j ++){
			 Move *move = new Move(i, j);
			 if(board->checkMove(move, side)){
				 std::vector<Move *> move1;
				 move1.push_back(move);
				 moveList.push_back(move1);
			 }else{
				 delete move;
			 }
		}
	 }
	 
	 std::cerr << "cehck2" << std::endl;
	 std::cerr << "moveList.size() = " << moveList.size() << std::endl;
	 //find all possible second moves
	 for(unsigned int i = 0; i < moveList.size(); i ++){
		 moveList1.push_back(allPossibleMoves(board, opponentsSide, moveList[i]));
		 std::cerr << "moveList1 = " << moveList1[i].size() << std::endl;
	 }
	 std::cerr << "cehck3" << std::endl;
	 
	 //find all possible thrid moves
	 for(unsigned int i = 0; i < moveList1.size(); i ++){
		 for(int j = 0; j < moveList1[i].size(); j ++){
			 moveList2[i][j].push_back(allPossibleMoves(board, side, moveList1[i][j]));
			 //std::cerr << "moveList2 = " << moveList2[i][j].size() << std::endl;
		 }
	 }
	 
	 std::cerr << "cehck4" << std::endl;
	 
	 int minMax = -100000;
	 int minMaxIndex = 0;
	 for(int i = 0; i < moveList2.size(); i ++){
		 for(int j = 0; j < moveList2[i].size(); j ++){
			 for(int k = 0; k < moveList2.size(); k ++){
				 int score = findMin(board, moveList2[i][j][k]);
				 if(minMax < score){
					 minMax = score;
					 minMaxIndex = i;
				 }
			 }
		 }
	 }
	 ///fill boardList2 with next depth
	 for(unsigned int i = 0; i < boardList.size(); i ++){
		 std::vector< std::vector<Board *> > boardListTemp;
		 for(unsigned int j = 0; j < boardList[i].size(); j ++){
			boardListTemp.push_back(allPossibleMoves(boardList[i][j], side));
		}
		boardList2.push_back(boardListTemp);
	 }
	 
	 //find lowest from each part of list2
	 std::vector< std::vector<int> > scoreList2;
	 for(unsigned int i = 0; i < boardList.size(); i ++){
		 std::vector<int> scoreListTemp;
		 for(unsigned int j = 0; j < boardList[i].size(); j ++){
			scoreListTemp.push_back(findMin(boardList[i][j]));
		}
		scoreList2.push_back(scoreListTemp);
	 }
	 
	 //find max from list1
	 std::vector<int> scoreList;
	 for(unsigned int i = 0; i < boardList.size(); i ++){
		int max = -100000;
		 for(unsigned int j = 0; j < boardList[i].size(); j ++){
			 if(scoreList2[i][j] > max){
				 max = scoreList2[i][j];
			 }
		}
		scoreList.push_back(max);
	 }
	 
	 //find max of all
	 int minMax = -100000;
	 int minMaxIndex = 0;
	 for(unsigned int i = 0; i < scoreList.size(); i ++){
		 if(scoreList[i] > minMax){
			 minMax = scoreList[i];
			 minMaxIndex = i;
		 }
	 }
	 
	 ///vector<vector<Move *> > depth = depthMin(3, board->copy(), side);
	 //moveList = depth.back();
	 int minMaxIndex = 0;
	 int minMax = depthMin(depth, moveList[0], board->copy(), side);
	 std::cerr << " for Move = (" << moveList[0]->getX() << ", " << moveList[0]->getY() << ") min score = " << minMax << std::endl;
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
	
	 
	 std::cerr << "move = " << moveList[minMaxIndex][0]->getX() << ", " << moveList[minMaxIndex][0]->getY() << std::endl;
	 return moveList[minMaxIndex][0];
	
}

/*int Player:: depthMin(int depthInt, Move *move, Board * board2, Side side1){
	Side side2;
	if(side1 == BLACK){
		side2 = WHITE;
	} else{
		side2 = BLACK;
	}
	
	std::vector<Board *> boardList;
	std::vector<vector<Board *> > depth;
	int last = -1;
	for (int i = 0;  i < depthInt; i ++){
	    boardList.clear();
	    last = -1;
		board2->doMove(move, side1);
		
		//int scoreWorst = 100000;//make worst score very high so any possible score will be lower
		for(int i = 0; i < 8; i ++){
			 for(int j = 0; j < 8; j ++){
				 Move *move1 = new Move(i, j);
				 if(board2->checkMove(move1, side2)){
					 boardList.push_back(board2->copy());
					 last ++;
					 boardList[last]->doMove(move, side2);
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
		
		depth.push_back(boardList);
		std::cerr << "board added"<< std::endl;
	}
	int minIndex = 0;
	 int min = findMin(boardList[0]);
	 //std::cerr << " for Move = (" << moveList[0]->getX() << ", " << moveList[0]->getY() << ") min score = " << min << std::endl;
	 //check lowest score for each possible move and finds max min score
	 
	 for(unsigned int i = 1; i < boardList.size(); i ++){
		 int worst = findMin(boardList[i]);
		 if((worst > min) and (worst != 100000)){
			 minIndex = i;
			 min = worst;
		//	 std::cerr << "change Move to (" << moveList[i]->getX() << ", " << moveList[i]->getY() << ") minMax = " << min << std::endl;
			 
		 }
		 //std::cerr << " for Move = (" << moveList[i]->getX() << ", " << moveList[i]->getY() << ") min score = " << worst << std::endl;
		
	 }
	 
	 return min;
	
}*/

/*std::vector<Board *> Player::allPossibleMoves(Board * b, Side s){
	std::vector<Board *> boardList;
	int last = -1;
	for(int i = 0; i < 8; i ++){
		 for(int j = 0; j < 8; j ++){
			 Move *move = new Move(i, j);
			 if(b->checkMove(move, s)){
				 boardList.push_back(b->copy());
				 last ++;
				 boardList[last]->doMove(move, s);
			 }
			 
		}
	}
	return boardList;
}*/

/**std::vector< std::vector<Move *> > Player::allPossibleMoves(Board * b, Side s, std::vector<Move *> moveList){
	std::cerr <<"AllPossibleMoves" << std::endl;
	std::vector< std::vector<Move *> > moveListLarge;
	for(int i = 0; i < moveList.size(); i ++){
		b->doMove(moveList[i], s);
	}
	std::cerr <<"AllPossibleMoves2" << std::endl;
	for(int i = 0; i < 8; i ++){
		 for(int j = 0; j < 8; j ++){
			 Move *move = new Move(i, j);
			 if(b->checkMove(move, s)){
				 std::vector<Move *> moveListTemp = moveList;
				 moveListTemp.push_back(move);
				 moveListLarge.push_back(moveListTemp);
			 }
		}
	}
	std::cerr <<"AllPossibleMovesend = " << moveListLarge.size() << std::endl;
	return moveListLarge;
}

int Player::findMin(Board * board2, std::vector<Move *> moveList){
	Side currentSide = side;
	for(int i = 0; i < moveList.size() - 1; i ++){
		board2->doMove(moveList[i], currentSide);
		//swtich sides
		if(currentSide == BLACK){
			currentSide = WHITE;
		}else{
			currentSide = BLACK;
		}
	}
	return heuristic(board2);
	/*
	 * //board2->doMove(move, opponentsSide);
	
	int scoreWorst = 100000;//make worst score very high so any possible score will be lower
	for(int i = 0; i < 8; i ++){
		 for(int j = 0; j < 8; j ++){
			 Move *move1 = new Move(i, j);
			 if(board2->checkMove(move1, side)){
				 int score = this->heuristic(move1, board2);
				 //std::cerr << "Move = (" << move1->getX() << ", " << move1->getY() << ") score = " << score << std::endl;
				 if(scoreWorst < score){
					 scoreWorst = score;
				 }
				 delete move1;
			 }
			 
		}
	}
	//returns lowest possible score
	return scoreWorst;*/
	
//}
