/* Game Engine Design */

#ifndef AGEG_TREE_H
#define AGEG_TREE_H

/**
* Template für eine typunabhängige Baumrepräsentation (Speicherverwaltung).
* @stereotype utility 
*/

#include "ageG_List.h"
#include "ageG_Knot.h"
#include <iostream>
#include "ageGraphics/Model/ageGM_Knot.h"
using namespace std;


class ageG_Tree {
		
private:
	ageG_Knot* rootKnot;
	ageG_Knot* temporaryKnot;
	ageG_Knot* initKnot(ageG_Knot* k);
	void makeTest();
	int actualListElement;
	ageG_List< Type >* knotListPreorder;
	ageG_List< Type >* knotListInorder;
	ageG_List< Type >* knotListPostorder;
	ageG_List< Type >* knotListLevelorder;

public:	
	ageG_Tree();
	void setRoot(ageG_Knot* k);
	bool addKnotLeft(ageG_Knot* paren, ageG_Knot* child);
	bool addKnotRight(ageG_Knot* paren, ageG_Knot* child);
	ageG_Knot* getRootKnot();
	ageG_Knot* getTemporaryKnot();
	void rewindLists();
	//Traversal-Funktionen
	ageG_Knot* getKnotPreorder();
	ageG_Knot* getKnotInorder();
	ageG_Knot* getKnotPostorder();
	ageG_Knot* getKnotLevelorder();
	//Traversal-Build-Funktionen	
	void buildPreorder(ageG_Knot* k);
	void buildInorder(ageG_Knot* k);
	void buildPostorder(ageG_Knot* k);
	void buildLevelorder(ageG_Knot* k);

};
#endif //AGEG_TREE_H