/* Game Engine Design */

#ifndef TREE_H
#define TREE_H

/**
* Template für eine typunabhängige Baumrepräsentation (Speicherverwaltung).
* @stereotype utility 
* @author Sebastian Blaum
*/

#include "windows.h"
#include "Liste.h"



// habe das struct mal ausgelagert inne eigene Klasse (daraus ´n template. 
// lass ich auch mit mir diskutieren ob das toll ist..:-)
// nee! ich lasses doch erstmal drinne....:-(
typedef struct knot {
	knot* left;
	knot* right;
	void* data;
} knot;/**/





/**
* Class Tree
* @date 14-01-2004
* @author Sebastian Blaum
* @modified by Tobias Harpering
*/


class Tree {
	
	
private:

	Knot knot * list;
	Knot knot * rootKnot;
	Knot knot * temporaryKnot;
	 
	/**
	* @description 
	* @param 
	* @return	
	* note: 
	*/
	Type * initKnot(knot * k);
	
	/**
	* @description 
	* @param 
	* @return
	* note:
	*/
	void makeTest();
	
	}
	
	/**
	* @description 
	* @param 
	* @return
	* note:
	*/
	int actualListElement;
	
	/**
	* @description 
	* @param 
	* @return
	* note:
	*/
	Liste<knot*>* knotListPreorder;
	
	/**
	* @description 
	* @param 
	* @return
	* note:
	*/
	Liste<knot*>* knotListInorder;
	
	/**
	* @description 
	* @param 
	* @return
	* note:
	*/
	Liste<knot*>* knotListPostorder;
	
	/**
	* @description 
	* @param 
	* @return
	* note:
	*/
	Liste<knot*>* knotListLevelorder;

	
	
public:
	
	Tree() ;	
	/**
	* @description 
	* @param 
	* @return
	* note:
	*/
	bool addKnotLeft(knot* paren, knot* child);
	
	/**
	* @description 
	* @param 
	* @return
	* note:
	*/
	bool addKnotRight(knot* paren, knot* child);
	
	/**
	* @description 
	* @param 
	* @return
	* note:
	*/
	knot* getRootKnot();
	
	/**
	* @description 
	* @param 
	* @return
	* note:
	*/
	knot* getTemporaryKnot();
	
	/**
	* @description 
	* @param 
	* @return
	* note:
	*/
	void rewindLists();
	
	
//#######Traversal-Funktionen######################################
	/**
	* @description 
	* @param 
	* @return
	* note:
	*/	
	knot* getKnotPreorder();
	
	/**
	* @description 
	* @param 
	* @return
	* note:
	*/
	knot* getKnotInorder();
	
	/**
	* @description 
	* @param 
	* @return
	* note:
	*/
	knot* getKnotPostorder();
	
	/**
	* @description 
	* @param 
	* @return
	* note:
	*/
	knot* getKnotLevelorder();

	//############Traversal-Build-Funktionen#######################################
	
	void buildPreorder(knot* k);
	
	/**
	* @description 
	* @param 
	* @return
	* note:
	*/
	void buildInorder(knot* k);
	
	/**
	* @description 
	* @param 
	* @return
	* note:
	*/
	void buildPostorder(knot* k);
	
	/**
	* @description 
	* @param 
	* @return
	* note:
	*/
	void buildLevelorder(knot* k);
	
};
#endif //TREE_H