/* Game Engine Design */

#ifndef TREE_H
#define TREE_H

/**
* Template für eine typunabhängige Baumrepräsentation (Speicherverwaltung).
* @stereotype utility 
*/

#include "windows.h"
#include "Liste.h"

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
	knot* initKnot(knot* k);
	void makeTest();
	int actualListElement;
	Liste<knot*>* knotListPreorder;
	Liste<knot*>* knotListInorder;
	Liste<knot*>* knotListPostorder;
	Liste<knot*>* knotListLevelorder;
	
public:
	
	Tree();
	bool addKnotLeft(knot* paren, knot* child);
	bool addKnotRight(knot* paren, knot* child);
	knot* getRootKnot();
	knot* getTemporaryKnot();
	void rewindLists();
	//Traversal-Funktionen
	knot* getKnotPreorder();
	knot* getKnotInorder();
	knot* getKnotPostorder();
	knot* getKnotLevelorder();
	//Traversal-Build-Funktionen	
	void buildPreorder(knot* k);
	void buildInorder(knot* k);
	void buildPostorder(knot* k);
	void buildLevelorder(knot* k);

};
#endif //TREE_H