#ifndef __COMMON_H__
#define __COMMON_H__

#include <iostream>


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

struct Node
{
	std::string key;
	int value;
	int popularity;
	Node *next;
	
	Node(std::string k, int v) {
		key = k;
		value = v;
		next = NULL;
		popularity = 0;
	}
	void insert(std::string k, int v) {
		if (key.compare(k) == 0) {
			popularity++;
			return;
		}
		if (next == NULL) {
			next = new Node(k, v);
		}
		else next->insert(k, v);
	}
	int find(std::string search) {
		if (key.compare(search) == 0) {
			return value;
		}
		if (next == NULL) return -1;
		return next->find(search);
	}
	
	void replace(std::string k, int val) {
		if (popularity == 0) {
			key = k;
			value = val;
		}
		else if (next == NULL) {
			return;
		}
		else next->replace(k, val);
	}
};

struct linkedList
{
	Node *first;
	int size;
	linkedList() {
		first = NULL;
		size = 0;
	}
	int find(std::string search) {
		if (first == NULL) return -1;
		int x = first->find(search);
		return x;
	}
	
	void add(std::string key, int val) {
		if (first == NULL) {
			first = new Node(key, val);
			size++;
		}
		else if (size > 10) {
			first->replace(key, val);
		}
		else {
			first->insert(key, val);
			size++;
		}
	}
};


#endif
