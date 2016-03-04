#ifndef __BOARD_H__
#define __BOARD_H__

#include <bitset>
#include "common.h"
#include <vector>
#include <iostream>
#include <stdlib.h>
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
    Board(Side side);
    ~Board();
    Board *copy();
        
    Side mySelf;

    bool isDone();
    int hasMoves(Side side);
    int bestMove(Side side);
    bool checkMove(Move *m, Side side);
    void doMove(Move *m, Side side);
    int count(Side side);
    int countBlack();
    int countWhite();

    void setBoard(char data[]);
    void setCornerScore(int indices, Side me);

};

#endif
