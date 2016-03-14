#include "board.h"

/*
 * Make a standard 8x8 othello board and initialize it to the standard setup.
 */
Board::Board(Side side) {

	// Stores what side this board is for
	mySelf = side;

	if (mySelf == BLACK) opp = WHITE;
	else opp = BLACK;

	// Store a basic method of assigning scores to different squares
	simpleScores = vector<int>(64, 1);

	// Corners have the highest score, edges also have high score,
	// Boxes next to edges and corners have lower scores.
	for (unsigned int i = 0; i < simpleScores.size(); i++) {
		if (i % 8 == 0 || i % 8 == 7) simpleScores[i] *= 3;
		if (i < 8 || i > 55) simpleScores[i] *= 3;
		if ((i/8 == 2 || i/8 == 5) && !(i == 16 || i == 23 || i == 40 || i == 47)) simpleScores[i] = -2*abs(simpleScores[i]);
		if ((i%8 == 2 || i%8 == 5) && !(i == 2 || i == 58 || i == 61 || i == 5)) simpleScores[i] = -2*abs(simpleScores[i]);
		if (i/8 == 1 || i/8 == 6) simpleScores[i] = -5*abs(simpleScores[i]);
		if (i % 8 == 1 || i % 8 == 6) simpleScores[i] = -5*abs(simpleScores[i]);
	}

	simpleScores[9] *= 3;
	simpleScores[14] *= 3;
	simpleScores[49] *= 3;
	simpleScores[54] *= 3;
	numOpen = 0;
		
	myFrontierSquares = 0;
	theirFrontierSquares = 0;
	
	for (int i = 0; i < 64; i++) {
		hashTable[i] = new linkedList();
	}

	moveToDo = new Move(-1, -1);
	
	blackb = 0b000000000000000000000000000100000010000000000000000000000000000;
	takenb = 0b000000000000000000000000001100000011000000000000000000000000000;
	            
	//blackb = 0b0000000000000000000000000000100000010000000000000000000000000000;
    //takenb = 0b0000000000000000000000000001100000011000000000000000000000000000;
   /* taken.set(3 + 8 * 3);
    taken.set(3 + 8 * 4);
    taken.set(4 + 8 * 3);
    taken.set(4 + 8 * 4);
    black.set(4 + 8 * 3);
    black.set(3 + 8 * 4);    */	
}

/*
 * Destructor for the board.
 */
Board::~Board() {
}

/*
 * Returns a copy of this board.
 */
Board *Board::copy() {
    Board *newBoard = new Board(this->mySelf);
    /*newBoard->black1 = black1;
    newBoard->black2 = black2;
    newBoard->taken1 = taken1;
    newBoard->taken2 = taken2;*/
    
   // newBoard->black = black;
   // newBoard->taken = taken;
    
    newBoard->blackb = blackb;
    newBoard->takenb = takenb;
    return newBoard;
}

bool Board::occupied(int x, int y) {
   // return taken[x + 8*y];
    
    return (takenb >> (x+y*8)) & 1;
}

bool Board::get(Side side, int x, int y) {
   // return occupied(x, y) && (black[x + 8*y] == (side == BLACK));
    
    return occupied(x, y) && (((blackb >> (x+y*8)) & 1) == (side == BLACK));
}

void Board::set(Side side, int x, int y) {
   // taken.set(x + 8*y);
   // black.set(x + 8*y, side == BLACK);
    takenb = takenb | (one << (x+y*8));
    if (side == BLACK) blackb = blackb | (one << (x+y*8));
    else blackb = blackb & ~(one << (x+y*8));
}

bool Board::onBoard(int x, int y) {
    return(0 <= x && x < 8 && 0 <= y && y < 8);
}

 
/*
 * Returns true if the game is finished; false otherwise. The game is finished 
 * if neither side has a legal move.
 */
bool Board::isDone() {
    return (hasMoves(BLACK) == -1) || (hasMoves(WHITE) == -1);
}

/*
 * Returns random indices if there are legal moves for the given side.
 */
int Board::hasMoves(Side side) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move move(i, j);
            if (checkMove(&move, side)) return i*8 + j;
        }
    }
    return -1;
}

/*
 * Returns the best move if there are legal moves for the given side.
 */
int Board::bestMove(Side side) {
	int best = -200;
	int indices = -1;
	int maybe;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move move(i, j);
            maybe = i*8 + j;
            // Picks the move with the highest simple score
            if ((simpleScores[maybe] > best) && checkMove(&move, side)) {
				best = simpleScores[maybe];
				indices = maybe;
			}
        }
    }
    return indices;
}

/*
 * Returns true if a move is legal for the given side; false otherwise.
 */
bool Board::checkMove(Move *m, Side side) {
    // Passing is only legal if you have no moves.
    if (m == NULL) return hasMoves(side) == -1;

    int X = m->getX();
    int Y = m->getY();
    // Make sure the square hasn't already been taken.
    if (occupied(X, Y)) return false;

    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            // Is there a capture in that direction?
            int x = X + dx;
            int y = Y + dy;
            if (onBoard(x, y) && get(other, x, y)) {
                do {
                    x += dx;
                    y += dy;
                } while (onBoard(x, y) && get(other, x, y));
                if (onBoard(x, y) && get(side, x, y)) return true;
            }
        }
    }
    return false;
}

/*
 * Does the minimax to find the best move to make and its score.
 * Returns the score of the board after making the move, and changes
 * the state of the best move for the board.
 * Uses DFS
 */

int Board::getBest(int depth, int player, bool testing, bool topLevel) {
	// Figures out which color the current move is for
	Side side;
	if (player == 1)
		side = mySelf;
	else side = opp;
	// If there are no valid moves for this player or we have reached
	// maximum depth, reutrn the score of the board right now
	if((hasMoves(side) == -1) || depth <= 0) {
		if (testing) return basicHeuristic() * player;
		else return betterHeuristic() * player;
	}
	// Find the best move and score
	bool changed = false;
	int bestScore = -1000000000;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8;j++) {
			Move *possMove = new Move(i, j);
			// Check if a move is valid and if it is, do the move, and 
			// find the score of doing minimax on that
			// board for the opposite player
			if (checkMove(possMove, side)) {
				doMove(possMove, side);
				int score = -1*getBest(depth - 1, -player, testing, false);
				// If this move yields a higher score than so far, do
				// it and if we are in the top level of recursion, change
				// the move we must to do to this move
				if (score > bestScore) {
					bestScore = score;
					if (topLevel) {
						moveToDo->setX(possMove->getX());
						moveToDo->setY(possMove->getY());
					}
					changed = true;
				}
				// Delete memory allocated for variables we don't need
				// anymore and undo the move we have done
				undoMove();
			}
			if (possMove != NULL) delete possMove;
		}
	}
	// If we couldn't find any best move, just set the move we need to
	// do to -1, which is a flag to the player that there are no moves left
	if (!changed) {
		moveToDo->setX(-1);
	}
	return bestScore;
} 

/*
 * Does the minimax to find the best move to make and its score using alpha
 * beta pruning to avoid looking at branches that neither the opponent nor
 * you will want to pick
 */

int Board::alphabeta(int depth, int alpha, int beta, int player, bool topLevel, double timeTaken) {
	if (timeTaken > 240) {
		std::cerr << "RUN OUT OF TIME" << std::endl;
		moveToDo->setX(-3);
		return 65;
	}
	time_t startTime, endTime;
	time(&startTime);
	// Figures out which color the current move is for
	Side side;
	if (player == 1)
		side = mySelf;
	else side = opp;
	// If there are no valid moves for this player or we have reached
	// maximum depth, return the score of the board right now
	if((hasMoves(side) == -1) || depth <= 0) {
		return betterHeuristic()*player;
	}

	int hashVal = hashFind();
	std::string rep = boardRepresentation();
	int val = hashTable[hashVal]->find(rep);
	if (val != -1) {
		int alpha = val/100;
		int moveDid = abs(val)%100;
		if (moveDid < 64) {
			doMove(moveToDo, side);
			time (&endTime);
			int score = -1*alphabeta(depth - 1, -beta, -alpha, -player, false, difftime(endTime, startTime) + timeTaken);
			if (abs(score) == 65 && moveToDo->x == -3) {
				undoMove();
				return 65;
			}
			if (score > alpha) {
				alpha = score;
				moveToDo->setX(moveDid%8);
				moveToDo->setY(moveDid/8);
			}
			undoMove();
		}
	}

	// Find the best move and score
	bool leave = false;
	
	if (topLevel && moveToDo->x != -1 && moveToDo->y != -1) {
		doMove(moveToDo, side);
		time (&endTime);
		int score = -1*alphabeta(depth - 1, -beta, -alpha, -player, false, difftime(endTime, startTime) + timeTaken);
		if (abs(score) == 65 && moveToDo->x == -3) {
			undoMove();
			return 65;
		}
		// If this move yields a higher score than so far, do
		// it and if we are in the top level of recursion, change
		// the move we must to do to this move
		if (score > alpha) {
			alpha = score;
		}
		else {
			moveToDo->setX(-1);
			moveToDo->setY(-1);
		}
		undoMove();
	}

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8;j++) {
			Move *possMove = new Move(i, j);
			// Check if a move is valid and if it is, do the move, and 
			// find the score of doing minimax on that
			// board for the opposite player
			if (checkMove(possMove, side)) {
				doMove(possMove, side);
				time (&endTime);
				int score = -1*alphabeta(depth - 1, -beta, -alpha, -player, false, difftime(endTime, startTime) + timeTaken);
				if (abs(score) == 65 && moveToDo->x == -3) {
					undoMove();
					return 65;
				}
				// If this move yields a higher score than so far, do
				// it and if we are in the top level of recursion, change
				// the move we must to do to this move
				if (score > alpha) {
					alpha = score;
					if (topLevel) {
						moveToDo->setX(possMove->getX());
						moveToDo->setY(possMove->getY());
					}
				}
				if (score >= beta) leave = true;
				undoMove();
				// Delete memory allocated for variables we don't need
				// anymore and undo the move we have done
			}
			if (possMove != NULL) delete possMove;
			if (leave) {
				addToHashTable(hashVal, rep, 66, alpha);
				return alpha;
			}
		}
	}
	if (topLevel)
		addToHashTable(hashVal, rep, moveToDo->x + moveToDo->y*8, alpha);
	else
		addToHashTable(hashVal, rep, 66, alpha);
	return alpha;
} 

int Board::negascout(int depth, int alpha, int beta, int player, bool topLevel, bool firstChild, double timeTaken) {
	if (timeTaken > 300) {
		moveToDo->setX(-3);
		return 65;
	}
	time_t startTime, endTime;
	time(&startTime);
	// Figures out which color the current move is for
	Side side;
	if (player == 1)
		side = mySelf;
	else side = opp;
	// If there are no valid moves for this player or we have reached
	// maximum depth, return the score of the board right now
	if((hasMoves(side) == -1) || depth <= 0) {
		return betterHeuristic()*player;
	}

	int hashVal = hashFind();
	std::string rep = boardRepresentation();
	int val = hashTable[hashVal]->find(rep);
	if (val != -1) {
		int alpha = val/100;
		int moveDid = abs(val)%100;
		if (moveDid < 64) {
			doMove(moveToDo, side);
			time (&endTime);
			int score = -1*negascout(depth - 1, -beta, -alpha, -player, false, true, difftime(endTime, startTime) + timeTaken);
			if (abs(score) == 65 && moveToDo->x == -3) {
				undoMove();
				return 65;
			}
			if (score > alpha) {
				alpha = score;
				moveToDo->setX(moveDid%8);
				moveToDo->setY(moveDid/8);
			}
			undoMove();
		}
	}

	// Find the best move and score
	bool leave = false;
	bool first = true;
	
	if (topLevel && moveToDo->x != -1 && moveToDo->y != -1) {
		doMove(moveToDo, side);
		time (&endTime);
		int score = -1*alphabeta(depth - 1, -beta, -alpha, -player, false, difftime(endTime, startTime) + timeTaken);
		if (abs(score) == 65 && moveToDo->x == -3) {
			undoMove();
			return 65;
		}
		// If this move yields a higher score than so far, do
		// it and if we are in the top level of recursion, change
		// the move we must to do to this move
		if (score > alpha) {
			alpha = score;
		}
		else {
			moveToDo->setX(-1);
			moveToDo->setY(-1);
		}
		undoMove();
	}

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8;j++) {
			Move *possMove = new Move(i, j);
			// Check if a move is valid and if it is, do the move, and 
			// find the score of doing minimax on that
			// board for the opposite player
			if (checkMove(possMove, side)) {
				doMove(possMove, side);
				time (&endTime);
				int score;
				if (!first) {
					score = -1*negascout(depth - 1, -alpha - 1, -alpha, -player, false, first, difftime(endTime, startTime) + timeTaken);
					if (score < beta && score > alpha)
						score = -1*negascout(depth - 1, -beta, -score, -player, false, first, difftime(endTime, startTime) + timeTaken);
				}
				else
					score = -1*negascout(depth - 1, -beta, -alpha, -player, false, first, difftime(endTime, startTime) + timeTaken);
				first = false;
				if (abs(score) == 65 && moveToDo->x == -3) {
					undoMove();
					return 65;
				}
				// If this move yields a higher score than so far, do
				// it and if we are in the top level of recursion, change
				// the move we must to do to this move
				if (score > alpha) {
					alpha = score;
					if (topLevel) {
						moveToDo->setX(possMove->getX());
						moveToDo->setY(possMove->getY());
					}
				}
				if (alpha >= beta) leave = true;
				undoMove();
				// Delete memory allocated for variables we don't need
				// anymore and undo the move we have done
			}
			if (possMove != NULL) delete possMove;
			if (leave) {
			//	addToHashTable(hashVal, rep, 66, alpha);
				return alpha;
			}
		}
	}
	//if (topLevel)
	//	addToHashTable(hashVal, rep, moveToDo->x + moveToDo->y*8, alpha);
	//else
	//	addToHashTable(hashVal, rep, 66, alpha);
	return alpha;
} 


void Board::addToHashTable(int hashVal, std::string rep, int move, int alpha) {
	if (alpha < 0) 
		hashTable[hashVal]->add(rep, -(move + 100*abs(alpha)));
	else hashTable[hashVal]->add(rep, move + 100*alpha);
}

/*
 * Modifies the board to reflect the specified move.
 */
void Board::doMove(Move *m, Side side) {

    // A NULL move means pass.
    if (m == NULL) return;
    // Ignore if move is invalid.
    if (!checkMove(m, side)) return;
	
    int X = m->getX();
    int Y = m->getY();
    // Add the move -1 as a marker between moves and then push the current
    // move that we want to do
    Board::moves->push(-1);
    Board::moves->push(X + Y*8);
    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            int x = X;
            int y = Y;
            do {
                x += dx;
                y += dy;
            } while (onBoard(x, y) && get(other, x, y));

            if (onBoard(x, y) && get(side, x, y)) {
                x = X;
                y = Y;
                x += dx;
                y += dy;
                while (onBoard(x, y) && get(other, x, y)) {
					// Add the stones we are turning as part of the move
					// to our stack of moves done
					/*if (taken[x+8*y]) {	
						if (black[x+8*y]) Board::moves->push(x + y*8 + 100);
						else Board::moves->push(x + y*8 + 200);
					}
					else Board::moves->push(x+y*8);*/
					
					if (takenb & (one << (x+8*y))) {
						if (blackb & (one << (x+8*y))) Board::moves->push(x+y*8 + 100);
						else Board::moves->push(x+y*8 + 200);
					}
					else Board::moves->push(x+y*8);
					
                    set(side, x, y);
                    x += dx;
                    y += dy;

                }
            }
        }
    }
    // If we got a corner square, the ones next to it become more stable
    if ((X == 0 && (Y == 0 || Y == 7)) || (X == 7 && (Y == 0 || Y == 7)))
		setCornerScore(X*8 + Y, side);
    set(side, X, Y);
	// As a marker between moves, push a move -1
	Board::moves->push(-1);
}

/*
 * Undo a whole move, including stones that are turned over as part of
 * the move
 */
void Board::undoMove() {
	// If there are -1 markers at the top of our stack, pop them off
	while (!Board::moves->empty() && moves->top() == -1) {
		Board::moves->pop();
	}
	// Make sure we don't go past capacity of stack
	while (!Board::moves->empty()) {
		int top = Board::moves->top();
		// If our top value is -1, that means we have reached a marker
		// between moves we are trying out, and we are done undoing this move
		if (top == -1) {
			return;
		}
		// If our top value is -5, we have reached the point where the moves
		// before are actual moves already done
		if (top == -5) return;
		// The moves are encoded as move = x + y*8 + 100*side, where side is
		// 0 if the square was empty before, 1 if it was black before, 
		// and 2 if it was white before
		/*if (top < 100) {
			taken[top] = 0;
			black[top] = 0;
		}
		else if (top < 200) {
			taken[top%100] = 1;
			black[top%100] = 1;
		}
		else {
			taken[top%200] = 1;
			black[top%200] = 0;
		}
		*/
		if (top < 100) {
			takenb &= ~(one << top);
			blackb &= ~(one << top);
		}
		else if (top < 200) {
			takenb |= (one << top%100);
			blackb |= (one << top%100);
		}
		else {
			takenb |= (one << top%200);
			blackb &= ~(one << top%200);
		}
		Board::moves->pop();
	}
}

/*
 * Current count of given side's stones.
 */
int Board::count(Side side) {
    return (side == BLACK) ? countBlack() : countWhite();
}

/*
 * Current count of black stones.
 */
int Board::countBlack() {
   // return black.count();
    uint64_t j = blackb;
	int index = 0, countBlack = 0;
	while (index < 64) {
		if (j & 1) countBlack ++;
		j = j >> 1; index ++;
	}
	return countBlack;
}

/*
 * Current count of white stones.
 */
int Board::countWhite() {
	uint64_t i = takenb;
	int index = 0, countTaken = 0;
	while (index < 64) {
		if (i & 1) countTaken ++;
		i = i >> 1; index ++;
	}
	return countTaken - countBlack();
   // return taken.count() - black.count();
}

/* 
 * Calculates the score using a basic heuristic, number of our stones
 * minus the number of the opponent's stones.
 */
int Board::basicHeuristic() {
	if (mySelf == BLACK) return countBlack() - countWhite();
	return countWhite() - countBlack();
}

/*
 * Calculates the score using a better heuristic - Is not working correctly
 */
int Board::betterHeuristic() {
	int stoneDiff = basicHeuristic();
	int yourStable = 0;
	int theirStable = 0;
	if (get(mySelf, 0, 0)) {
		yourStable ++;
		if (get(mySelf, 1, 0)) yourStable++;
		if (get(mySelf, 1, 1)) yourStable++;
		if (get(mySelf, 0, 1)) yourStable++;
	}
	else if (get(opp, 0, 0)) {
		theirStable ++;
		if (get(opp, 1, 0)) theirStable++;
		if (get(opp, 1, 1)) theirStable++;
		if (get(opp, 0, 1)) theirStable++;
	}

	if (get(mySelf, 7, 0)) {
		yourStable ++;
		if (get(mySelf, 6, 0)) yourStable++;
		if (get(mySelf, 6, 1)) yourStable++;
		if (get(mySelf, 7, 1)) yourStable++;
	}
	else if (get(opp, 7, 0)) {
		theirStable ++;
		if (get(opp, 6, 0)) theirStable++;
		if (get(opp, 6, 1)) theirStable++;
		if (get(opp, 7, 1)) theirStable++;
	}

	if (get(mySelf, 0, 7)) {
		yourStable ++;
		if (get(mySelf, 1, 7)) yourStable++;
		if (get(mySelf, 1, 6)) yourStable++;
		if (get(mySelf, 0, 6)) yourStable++;
	}
	else if (get(opp, 0, 7)) {
		theirStable ++;
		if (get(opp, 1, 7)) theirStable++;
		if (get(opp, 1, 6)) theirStable++;
		if (get(opp, 0, 6)) theirStable++;
	}

	if (get(mySelf, 7, 7)) {
		yourStable ++;
		if (get(mySelf, 6, 7)) yourStable++;
		if (get(mySelf, 6, 6)) yourStable++;
		if (get(mySelf, 7, 6)) yourStable++;
	}
	else if (get(opp, 7, 7)) {
		theirStable ++;
		if (get(opp, 6, 7)) theirStable++;
		if (get(opp, 6, 6)) theirStable++;
		if (get(opp, 7, 6)) theirStable++;
	}

	int myEdges = 0, theirEdges = 0;
	
	for (int i = 2; i < 5; i++) {
		if (get(mySelf, i, 0)) myEdges++;
		else if(get(opp, i, 0)) theirEdges++;
		if (get(mySelf, 0, i)) myEdges++;
		else if(get(opp, 0, i)) theirEdges++;
		if (get(mySelf, i, 7)) myEdges++;
		else if(get(opp, i, 7)) theirEdges++;
		if (get(mySelf, 7, i)) myEdges++;
		else if(get(opp, 7, i)) theirEdges++;
	}
	
	int myMoves = getMyNumMoves();
	int theirMoves = getOppNumMoves();
	if (numOpen < 5) {
		return stoneDiff*40 + (yourStable - theirStable) * 20;
	}
	return (stoneDiff + (yourStable - theirStable) * 30 + 
	(myEdges - theirEdges)*15 + (myMoves-theirMoves)*20 + 
	(theirFrontierSquares - myFrontierSquares)*5);
}

/*
 * Sets the board state given an 8x8 char array where 'w' indicates a white
 * piece and 'b' indicates a black piece. Mainly for testing purposes.
 */
void Board::setBoard(char data[]) {
    /*taken.reset();
    black.reset();
    
    for (int i = 1; i < 63; i++) {
		if (data[i] == 'b') {
			taken[i] = 1;
			black[i] = 1;
		}
		else if (data[i] == 'w')
			taken[i] = 1;
	}*/
	
	takenb = 0;
	blackb = 0;
	
	for (int i = 0; i < 64; i++) {
		takenb = takenb << 1;
		blackb = blackb << 1;
		if (data[i] == 'b') {
			takenb |= (one << i);
			blackb |= (one << i);
		}
		else if (data[i] == 'w') {
			takenb |= (one << i);
		}
	} 
}

int Board::getMyNumMoves(){
	int count = 0;
	myFrontierSquares = 0;
	numOpen = 0;
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 7; j++) {
			if (!(takenb & (one << (i+j*8)))) numOpen++;
			Move move(i, j);
			if (checkMove(&move, mySelf)) count++;
			if (get(mySelf, i, j)) {
				for(int dx = -1; dx <=1; dx++) {
					for (int dy = -1; dy <=1; dy++) {
						if (onBoard(i+dx, j+dy) && !(takenb & one << ((i+dx)+(j+dy)*8))) {
							myFrontierSquares ++;
						}
					}
				}
			}
		}
	}
	return count;
}

int Board::getOppNumMoves() {
	int count = 0;
	theirFrontierSquares = 0;
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 7; j++) {
			Move move(i, j);
			if (checkMove(&move, opp)) count++;
			if (get(opp, i, j)) {
				for(int dx = -1; dx <=1; dx++) {
					for (int dy = -1; dy <=1; dy++) {
						if (onBoard(i+dx, j+dy) && !(takenb & one << ((i+dx)+(j+dy)*8))) {
							theirFrontierSquares ++;
						}
					}
				}
			}
		}
	}
	return count;
}

/* Once we get a corner square, then the squares next to it become stable
 * rather than undesirable. If the opponent gets it, then the squares
 * become neutral since we don't want it to be stable for the opponent
 * but there are other more important squares we could be looking at.
 * Changes the squares accordingly.
 */

void Board::setCornerScore(int indices, Side me) {
	
	// If I got the corner, then the squares next to it, become -10 times
	// the current score. (Since they are currently negative, will become
	// large positive number)
	// If opponent got it, they become 1/5 of the current score (still 
	// negative but much smaller)
	double mult = 0.1;
	if (me == mySelf)
		mult = -10;
	if (indices == 0) {
		simpleScores[1] *= mult;
		simpleScores[8] *= mult; 
		simpleScores[9] *= mult;
	}
	else if (indices == 7) {
		simpleScores[6] *= mult;
		simpleScores[14] *= mult;
		simpleScores[15] *= mult;
	}
	else if (indices == 56) {
		simpleScores[48] *= mult;
		simpleScores[49] *= mult;
		simpleScores[57] *= mult;
	}
	else if (indices == 63) {
		simpleScores[54] *= mult;
		simpleScores[55] *= mult;
		simpleScores[62] *= mult;
	}
}		

/*
 * Print out which of the board squares are taken for error-checking
 * purposes.
 */
void Board::printBoard() {
	/*for (int i  = 0; i < 64; i++) {
		std::cerr << taken[i];
	}
	std::cerr << std::endl;*/
	
	for (int k = 0; k < 64; k++) {
		if ((takenb >> k) & 1) {
			if ((blackb >> k) & 1) {
				std::cerr << "b|";
			}
			else std::cerr << "w|";
		}
		else std::cerr << " |";
		if (k%8 == 7) std::cerr << std::endl;
	}
	std::cerr << std::endl;
}

std::string Board::boardRepresentation() {
	std::string h = "";
	for (int i = 1; i < 64; i++) {
		if (takenb & (one << i)) {
			if (blackb & (one << i)) h += "1";
			else h += "2";
		}
		else h += " ";
	}
	return h;
}


int Board::hashFind() {
	if (mySelf == BLACK) return countWhite();
	else return countBlack();
}
