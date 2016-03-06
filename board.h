#ifndef __BOARD_H__
#define __BOARD_H__

#include <bitset>
#include "common.h"
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <stack>
using namespace std;

class Board {
   
private:
    bitset<64> black;
    bitset<64> taken;    
    std::vector<int> simpleScores;
       
    bool occupied(int x, int y);
    bool get(Side side, int x, int y);
    void set(Side side, int x, int y);
    bool onBoard(int x, int y);
      
public:
	static std::stack<Move> *moves;
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
    void setBoard(char data[]);
    void setCornerScore(int indices, Side me);
	void printBoard();
};

#endif
