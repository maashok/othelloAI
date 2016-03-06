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
	
	moveToDo = new Move(-1, -1);
	
	simpleScores[9] *= 3;
	simpleScores[14] *= 3;
	simpleScores[49] *= 3;
	simpleScores[54] *= 3;
	
	// Print out the scores given to boxes for error checking purposes
	/*(for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			std::cerr << simpleScores[i*8 + j] << "  " << std::endl;
		}
		std::cerr << std::endl;
	}
	*/
    taken.set(3 + 8 * 3);
    taken.set(3 + 8 * 4);
    taken.set(4 + 8 * 3);
    taken.set(4 + 8 * 4);
    black.set(4 + 8 * 3);
    black.set(3 + 8 * 4);
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
    newBoard->black = black;
    newBoard->taken = taken;
    return newBoard;
}

bool Board::occupied(int x, int y) {
    return taken[x + 8*y];
}

bool Board::get(Side side, int x, int y) {
    return occupied(x, y) && (black[x + 8*y] == (side == BLACK));
}

void Board::set(Side side, int x, int y) {
    taken.set(x + 8*y);
    black.set(x + 8*y, side == BLACK);
}

bool Board::onBoard(int x, int y) {
    return(0 <= x && x < 8 && 0 <= y && y < 8);
}

 
/*
 * Returns true if the game is finished; false otherwise. The game is finished 
 * if neither side has a legal move.
 */
bool Board::isDone() {
    return !(hasMoves(BLACK) || hasMoves(WHITE));
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
			// Check if a move is valid and if it is, create a board
			// for it, and find the score of doing minimax on that
			// board for the opposite player
			if (checkMove(possMove, side)) {
				//std::cerr << "Move " << i << j << "Being tried by " << player << std::endl;
				doMove(possMove, side);
				int score = -1*getBest(depth - 1, -player, testing, false);
				if (score > bestScore) {
					bestScore = score;
					if (topLevel) {
						moveToDo->setX(possMove->getX());
						moveToDo->setY(possMove->getY());
					}
					changed = true;
				}
				// Delete memory allocated for variables we don't need
				// anymore
				if (possMove != NULL) delete possMove;
				undoMove();
			}
		}
	}
	if (!changed) {
		moveToDo->setX(-1);
	}
	return bestScore;
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
    Board::moves->push(Move(-1, -1, EMPTY));
    Board::moves->push(Move(X, Y, EMPTY));
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
					if (taken[x+8*y]) {
						if (black[x+8*y]) Board::moves->push(Move(x, y, BLACK));
						else Board::moves->push(Move(x, y, WHITE));
					}
					else Board::moves->push(Move(x, y, EMPTY));
                    set(side, x, y);
                    //std::cerr << "Acc move" << x << y << std::endl;
                    x += dx;
                    y += dy;
                }
            }
        }
    }
    if ((X == 0 && (Y == 0 || Y == 7)) || (X == 7 && (Y == 0 || Y == 7)))
		setCornerScore(X + Y*8, side);
    set(side, X, Y);
    //std::cerr << "Move" << X << Y << std::endl;

	Board::moves->push(Move(-1, -1, EMPTY));
}

/*
 * Undo a whole move, including stones that are turned over as part of
 * the move
 */
void Board::undoMove() {
	while (!Board::moves->empty() && moves->top().getX() == -1) {
		Board::moves->pop();
	}
	while (!Board::moves->empty()) {
		Move move = Board::moves->top();
		if (move.getX() == -1) {
			return;
		}
		if (move.getX() == -2) return;
		//std::cerr << "Undo move" << move.x << move.y << std::endl;
		if (move.oldSide == EMPTY) {
			taken[move.x + move.y*8] = 0;
			black[move.x + move.y*8] = 0;
		}
		else {
			taken[move.x + move.y*8] = 1;
			if (move.oldSide == BLACK)
				black[move.x + move.y*8] = 1;
			else black[move.x + move.y*8] = 0;
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
    return black.count();
}

/*
 * Current count of white stones.
 */
int Board::countWhite() {
    return taken.count() - black.count();
}

int Board::basicHeuristic() {
	if (mySelf == BLACK) return countBlack() - countWhite();
	return countWhite() - countBlack();
}

int Board::betterHeuristic() {
	int numMyCorners = 0, numMyEdges = 0;
	int numTheirCorners = 0, numTheirEdges = 0;
	for (int i = 1; i < 7; i++) {
		if (get(mySelf, i, 0)) numMyEdges ++;
		else if (get(opp, i, 0)) numTheirEdges ++;
		if (get(mySelf, i, 7)) numMyEdges ++;
		else if (get(opp, i, 7)) numTheirEdges ++;
		if (get(mySelf, 0, i)) numMyEdges ++;
		else if (get(opp, 0, i)) numTheirEdges ++;
		if (get(mySelf, 7, i)) numMyEdges ++;
		else if (get(opp, 7, i)) numTheirEdges ++;
	}
	if (get(mySelf, 0, 0)) numMyCorners ++;
	else if (get(opp, 0, 0)) numTheirCorners ++;
	if (get(mySelf, 0, 7)) numMyCorners ++;
	else if (get(opp, 0, 7)) numTheirCorners ++;
	if (get(mySelf, 7, 0)) numMyCorners ++;
	else if (get(opp, 7, 0)) numTheirCorners ++;
	if (get(mySelf, 7, 7)) numMyCorners ++;
	else if (get(opp, 7, 7)) numTheirCorners ++;
	
	numMyCorners *= 150;
	numMyEdges *= -50;
	numTheirCorners *= -150;
	numTheirEdges *= -50;
	int total = numMyCorners + numMyEdges + numTheirCorners + numTheirEdges;
	return total * basicHeuristic();

}

/*
 * Sets the board state given an 8x8 char array where 'w' indicates a white
 * piece and 'b' indicates a black piece. Mainly for testing purposes.
 */
void Board::setBoard(char data[]) {
    taken.reset();
    black.reset();
    for (int i = 0; i < 64; i++) {
        if (data[i] == 'b') {
            taken.set(i);
            black.set(i);
        } if (data[i] == 'w') {
            taken.set(i);
        }
    }
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

void Board::printBoard() {
	std::cerr << "Taken" << std::endl;
	for (int i  = 0; i < 64; i++) {
		std::cerr << taken[i];
	}
	std::cerr << std::endl;
}
