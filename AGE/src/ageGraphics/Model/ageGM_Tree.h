/* Game Engine Design */

#ifndef AGEGM_TREE_H
#define AGEGM_TREE_H

/**
* Template für eine typunabhängige Baumrepräsentation (Speicherverwaltung).
* @stereotype utility 
*/

#include "ageGM_List.h"
#include "ageGM_Knot.h"
#include <iostream>
using namespace std;


class ageGM_Tree {
		
private:
	ageGM_Knot* rootKnot;
	ageGM_Knot* temporaryKnot;
	ageGM_Knot* initKnot(ageGM_Knot* k);
	void makeTest();
	int actualListElement;
	ageGM_List< Type >* knotListPreorder;
	ageGM_List< Type >* knotListInorder;
	ageGM_List< Type >* knotListPostorder;
	ageGM_List< Type >* knotListLevelorder;

public:	
	ageGM_Tree();
	void setRoot(ageGM_Knot* k);
	bool addKnotLeft(ageGM_Knot* paren, ageGM_Knot* child);
	bool addKnotRight(ageGM_Knot* paren, ageGM_Knot* child);
	ageGM_Knot* getRootKnot();
	ageGM_Knot* getTemporaryKnot();
	void rewindLists();
	//Traversal-Funktionen
	ageGM_Knot* getKnotPreorder();
	ageGM_Knot* getKnotInorder();
	ageGM_Knot* getKnotPostorder();
	ageGM_Knot* getKnotLevelorder();
	//Traversal-Build-Funktionen	
	void buildPreorder(ageGM_Knot* k);
	void buildInorder(ageGM_Knot* k);
	void buildPostorder(ageGM_Knot* k);
	void buildLevelorder(ageGM_Knot* k);

};
#endif //AGEGM_TREE_H