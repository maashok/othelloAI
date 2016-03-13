#include "player.h"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
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
	board->doMove(opponentsMove, opp);

	// First do the opponent's move
	if (opponentsMove != NULL) {
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
    // TO successfully win against SimplePlayer using the naive heuristic
    // and minimax, merely change testingMinimax to true in the constructor
    // and call board->getBest(5, 1, true, true).
	if (testingMinimax) {
		board->getBest(2, 1, true, true);

		Move *goodMove = new Move(board->moveToDo->getX(), board->moveToDo->getY());
		// After we got a move, we will reset the next move to be -1 for now
		board->moveToDo->setX(-1);
		board->moveToDo->setY(-1);
		if (goodMove->getX() == -1) return NULL;
		// We will also undo all moves done after the permanent ones before
		// pushing the new move on
		while (!Board::moves->empty() && Board::moves->top() != -5) {
			board->undoMove();
		}

		board->doMove(goodMove, me);
		// Push a marker move -5 to signify a permanent move has been done
		Board::moves->push(-5);

		return goodMove;
	}
	// Otherwise, this will be implemented better later to include more
	// advanced heuristic...
	else {
		//board->getBest(5, 1, false, true);
		board->printBoard();
		board->alphabeta(7, -100000000, 100000000, 1, true);
		if (msLeft > 100000) { // Change this to either 3/4 minutes
			if (board->countBlack() + board->countWhite() > 32) {
				board->alphabeta(12, -100000000, 100000000, 1, true);
			}
			else {
				board->alphabeta(8, -100000000, 100000000, 1, true);
			}
		}
		Move *goodMove = new Move(board->moveToDo->getX(), board->moveToDo->getY());
		// After we got a move, we will reset the next move to be -1 for now
		board->moveToDo->setX(-1);
		board->moveToDo->setY(-1);

		if (goodMove->getX() < 0 || goodMove->getY() < 0) return NULL;

		
		// We will also undo all moves done after the permanent ones before
		// pushing the new move on
		while (!Board::moves->empty() && Board::moves->top() != -5) {
			board->undoMove();
		}

		board->doMove(goodMove, me);
		// Push a marker move -5 to signify a permanent move has been done
		Board::moves->push(-5);

		return goodMove;
	}
	
	// If not testing minimax, can do search with depth of 5 using naive
	// heuristic for minimax
	/*else {
		board->getBest(5, 1, true, true);

		Move *goodMove = new Move(board->moveToDo->getX(), board->moveToDo->getY());
		// After we got a move, we will reset the next move to be -1 for now
		board->moveToDo->setX(-1);
		board->moveToDo->setY(-1);
		if (goodMove->getX() == -1) return NULL;
		// We will also undo all moves done after the permanent ones before
		// pushing the new move on
		while (!Board::moves->empty() && Board::moves->top() != -5) {
			board->undoMove();
		}

		board->doMove(goodMove, me);
		// Push a marker move -5 to signify a permanent move has been done
		Board::moves->push(-5);

		return goodMove;
	}*/
	
	
	// The Basic Heuristic, that doesn't look ahead into the future
	// but still chooses squares based on the position's score
    int bestCoord = board->bestMove(me);
	if (bestCoord == -1) return NULL;
    Move *best = new Move(bestCoord/8, bestCoord%8);
	board->doMove(best, me);
    return best; 
    
}
