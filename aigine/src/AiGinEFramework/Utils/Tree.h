/* Game Engine Design */

#ifndef TREE_H
#define TREE_H

/**
* Template für eine typunabhängige Baumrepräsentation (Speicherverwaltung).
* @stereotype utility 
*/

#include "List.h"
#include "Knot.h"
#include <iostream>
using namespace std;


class Tree {
		
private:
	Knot* rootKnot;
	Knot* temporaryKnot;
	Knot* initKnot(Knot* k);
	void makeTest();
	int actualListElement;
	List<Knot*>* knotListPreorder;
	List<Knot*>* knotListInorder;
	List<Knot*>* knotListPostorder;
	List<Knot*>* knotListLevelorder;

public:	
	Tree();
	void setRoot(Knot* k);
	bool addKnotLeft(Knot* paren, Knot* child);
	bool addKnotRight(Knot* paren, Knot* child);
	Knot* getRootKnot();
	Knot* getTemporaryKnot();
	void rewindLists();
	//Traversal-Funktionen
	Knot* getKnotPreorder();
	Knot* getKnotInorder();
	Knot* getKnotPostorder();
	Knot* getKnotLevelorder();
	//Traversal-Build-Funktionen	
	void buildPreorder(Knot* k);
	void buildInorder(Knot* k);
	void buildPostorder(Knot* k);
	void buildLevelorder(Knot* k);

};
#endif //TREE_H