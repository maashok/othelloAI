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
	// First do the opponent's move
	board->doMove(opponentsMove, opp);
	
	// Calculate some random valid move and return that move
     /* The random way
     int randCoord = board->hasMoves(me);
     if (randCoord == -1) return NULL;
     Move *randMove = new Move(randCoord/10, randCoord%10);
     board->doMove(randMove, me);
     return randMove;  
     */ 
    
    // If testing minimax, then do minimax with depth of 2 and basic
    // heuristic
	if (testingMinimax) {
		board->getBasicBest(2, 1, true);
		Move *goodMove = board->bestBasicMove;
		return goodMove;
	}
	// Otherwise, ...
	else {
		std::cerr << "Sorry, not implemented yet!" << std::endl;
		return NULL;
		/*if (msLeft > 300000)
			board->getBetterBest(10, 1);
		else 
			board->getBetterBest(4, 1);
		Move *goodMove = board->betterBestMove;
		return goodMove;*/
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
