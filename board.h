#ifndef __BOARD_H__
#define __BOARD_H__

#include <bitset>
#include "common.h"
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <stack>
using namespace std;
#include <stdint.h>

class Board {
   
private:
    bitset<64> black;
    bitset<64> taken;
    // (bottom right to top left)    
    //long unsigned int black1, black2;
    //long unsigned int taken1, taken2;
    std::vector<int> simpleScores;
       
    bool occupied(int x, int y);
    bool get(Side side, int x, int y);
    void set(Side side, int x, int y);
    bool onBoard(int x, int y);\
    int myFrontierSquares;
    int theirFrontierSquares;
      
public:
	static std::stack<int> *moves;
    Board(Side side);
    ~Board();
    Board *copy();
        
    Side mySelf;
    Side opp;
    Move *moveToDo;
	
    bool isDone();
    int hasMoves(Side side);
    int bestMove(Side side);
    bool checkMove(Move *m, Side side);
    void doMove(Move *m, Side side);
    void undoMove();
    int count(Side side);
    int countBlack();
    int countWhite();
	int basicHeuristic();
	int betterHeuristic();
	int getBest(int depth, int player, bool testing, bool topLevel);
	int alphabeta(int depth, int alpha, int beta, int player, bool topLevel);
    void setBoard(char data[]);
    int getMyNumMoves();
    int getOppNumMoves();
    void setCornerScore(int indices, Side me);
	void printBoard();
};

#endif
