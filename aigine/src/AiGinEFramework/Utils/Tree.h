/* Game Engine Design */

#ifndef TREE_H
#define TREE_H

/**
* Template für eine typunabhängige Baumrepräsentation (Speicherverwaltung).
* @stereotype utility 
*/

typedef struct knot {
	knot* left;
	knot* right;
	void* data;
} knot;



class Tree {
	
	
private:
	knot* list;
	knot* rootKnot;
	knot* temporaryKnot;
	void addToList(knot* k);
	
public:
	
	bool addKnotLeft(knot* paren, knot* child);
	bool addKnotRight(knot* paren, knot* child);
	knot* getRootKnot();
	knot* getTemporaryKnot();
	knot* getKnotPreorder();
	knot* getKnotInorder();
	knot* getKnotPostorder();
	knot* getKnotLevelorder();
	
};
#endif //TREE_H