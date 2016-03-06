#ifndef __COMMON_H__
#define __COMMON_H__

enum Side { 
    WHITE, BLACK, EMPTY
};

class Move {
   
public:
    int x, y;
    Side oldSide;
    Move(int x, int y) {
        this->x = x;
        this->y = y;        
    }
    Move(int x, int y, Side side) {
		this->x = x;
		this->y = y;
		this->oldSide = side;
	}
    ~Move() {}

    int getX() { return x; }
    int getY() { return y; }

    void setX(int x) { this->x = x; }
    void setY(int y) { this->y = y; }
};

#endif
