#include "player.h"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = true;
    // Creates a new board for this side
	board = new Board(side);
	// Save what side we are and what side the opponent is on
	me = side;
	opp = (side == WHITE) ? BLACK : WHITE;
}

/*
 * Destructor for the player.
 */
Player::~Player() {
	delete board;
}

void Player::setBoard(Board *newBoard) {
	board = newBoard;
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
	// First do the opponent's move
	if (opponentsMove != NULL) {
		board->doMove(opponentsMove, opp);
		Board::moves->push(-5);
	}
	
	// Calculate some random valid move and return that move
     /* The random way
     int randCoord = board->hasMoves(me);
     if (randCoord == -1) return NULL;
     Move *randMove = new Move(randCoord/8, randCoord%8);
     board->doMove(randMove, me);
     return randMove;  
     */ 
	
    // If testing minimax, then do minimax with depth of 2 and basic
    // heuristic
	if (testingMinimax) {
		board->getBest(5, 1, true, true);

		Move *goodMove = new Move(board->moveToDo->getX(), board->moveToDo->getY());
		board->moveToDo->setX(-1);
		board->moveToDo->setY(-1);
		if (goodMove->getX() == -1) return NULL;
		while (!Board::moves->empty() && Board::moves->top() != -5) {
			board->undoMove();
		}

		board->doMove(goodMove, me);
		Board::moves->push(-5);

		return goodMove;
	}
	// Otherwise, ...
	else {
		if (msLeft > 300000)
			board->getBest(5, 1, false, true);
		else 
			board->getBest(3, 1, false, true);
		Move *goodMove = new Move(board->moveToDo->getX(), board->moveToDo->getY());
		if (goodMove->getX() == -1) return NULL;
		board->doMove(goodMove, me);
		return goodMove;
	}
	
	
	// The Basic Heuristic, that doesn't look ahead into the future
	// but still chooses squares based on the position's score
    // Basic 
    int bestCoord = board->bestMove(me);
	if (bestCoord == -1) return NULL;
    Move *best = new Move(bestCoord/8, bestCoord%8);
	board->doMove(best, me);
    return best; 
    
}
