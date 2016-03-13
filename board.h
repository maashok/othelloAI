#ifndef __BOARD_H__
#define __BOARD_H__

#include <bitset>
#include "common.h"
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <stack>
using namespace std;
#include <string>
#include <cstdint>

class Board {
   
private:
    //bitset<64> black;
    //bitset<64> taken;
    // (bottom right to top left)  
    
    uint64_t blackb, takenb;  
    uint64_t one = 1;

    std::vector<int> simpleScores;
	std::unordered_map<bitBoard, int> hashTable;
    bool occupied(int x, int y);
    bool get(Side side, int x, int y);
    void set(Side side, int x, int y);
    bool onBoard(int x, int y);\
    int myFrontierSquares;
    int theirFrontierSquares;
   	int numOpen;

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
    std::string boardRepresentation();
    int hash();
    void addToHashTable(int hashVal, std::string boardRep, int move, int alpha);
	void printBoard();
};

#endif
