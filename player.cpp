#include "player.h"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
	board = new Board(side);
	
	me = side;
	opp = (side == WHITE) ? BLACK : WHITE;
	
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
     
     /* The random way
     board->doMove(opponentsMove, opp);
     int randCoord = board->hasMoves(me);
     if (randCoord == -1) return NULL;
     Move *randMove = new Move(randCoord/10, randCoord%10);
     board->doMove(randMove, me);
     return randMove;  
     */ 
     
     board->doMove(opponentsMove, opp);
     int bestCoord = board->bestMove(me);
     if (bestCoord == -1) return NULL;
     Move *best = new Move(bestCoord/8, bestCoord%8);
     board->doMove(best, me);
     return best; 
     
}
