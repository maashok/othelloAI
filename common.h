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

/**
 * Maintains a node for the linked lists in the tansposition table
 */
struct Node
{
	// Stores a key value, given as a string representation of the board,
	// with empty space for empty square, 1 for black, and 2 for white
	std::string key;
	// Stores the alpha value for the move done in the case that this board
	// was used
	int value;
	// Stores how popular this board is so it isn't removed
	int popularity;
	// Stores the next node of the linked list
	Node *next;
	
	Node(std::string k, int v) {
		key = k;
		value = v;
		next = NULL;
		popularity = 0;
	}
	// Inserts a new node
	void insert(std::string k, int v) {
		// If this new node matches up to this node, just increase popularity
		if (key.compare(k) == 0) {
			popularity++;
			return;
		}
		// If there is no next node, make this new one the next
		if (next == NULL) {
			next = new Node(k, v);
		}
		// Otherwise call insert on next node
		else next->insert(k, v);
	}
	
	// Find the value of a key
	int find(std::string search) {
		// If it is this one return it
		if (key.compare(search) == 0) {
			return value;
		}
		// If next is null, meaning it can't be found then return -1
		if (next == NULL) return -1;
		// Otherwise call find on the next node
		return next->find(search);
	}
	// Replace a node with a new one
	void replace(std::string k, int val) {
		// Try to replace a node that has popularity 0
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

/** Maintain linked list for hash table. Each linked list is similar
 * in that it has the same number of some stone.
 */
struct linkedList
{
	// Stores the first node of the linked list
	Node *first;
	// Store the size of the linked list to not let it grow beyond certain length
	int size;
	linkedList() {
		first = NULL;
		size = 0;
	}
	// To find, call find on the first node or make a first node
	int find(std::string search) {
		if (first == NULL) {
			return -1;
		}
		int x = first->find(search);
		return x;
	}
	// To add, call insert on the first node, or make a first node
	// or replace if necessary
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

// NOT USED
struct bitBoard
{
	uint64_t black;
	uint64_t taken;
	bitBoard(uint64_t b, uint64_t t) {
		black = b;
		taken = t;
	}
};
#endif
