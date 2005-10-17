/* Game Engine Design */



#ifndef AGEGC_SCENEGRAPH_H

#define AGEGC_SCENEGRAPH_H

#include <GL/glut.h>

#include "ageGraphics/ageGM_Tree.h"
#include "ageGraphics/ageGM_Knot.h"
#include "ageGraphics/ageGC_Object.h"
#include "ageGM_Tree.h"
#include "ageGC_Object.h"
#include "ageGM_Knot.h"

class ageGM_ObjectTree;

/**
 * Unter "SceneGraph" versteht man die Abbildung einer gesammten Szene. Er
 * beinhaltet einen Baum, der die Hierachie der Objekte wiederspiegelt. Der
 * SceneGraph kann weitere SceneGraphs enthalten. So wird es möglich aus einem
 * Modell einen ScenenGraph zu parsen und dem eigentlichen SceneGraph
 * hinzuzufügen (z.B. aus vrml oder 3ds).
 */
class ageGC_SceneGraph : public ageGM_Tree
{
public:

	ageGC_SceneGraph();
    void addObject(ageGC_Object*);

    void draw();

    void drawSceneGraph(ageGC_Object* obj);
	
	void printGraph();
	void printSceneGraph(ageGC_Object* obj, string tabs);

    //alle add- bzw. remove-Funktionen geben einen bool als Erfogsmeldung zurück
    //true -> Operation erfolgreich
    //false -> Fehler (könnte gegen int ausgetauscht werden; 0->OK x ->Fehler Nr. x)
	void addRoot(ageGC_Object* rootObject);

    bool addChild(ageGC_Object * parentObject, ageGC_Object * childObject);

    bool addNext(ageGC_Object * parentObject, ageGC_Object * nextObject);

    bool removeObject(ageGC_Object * object);

private:
    ageGM_ObjectTree * lnkObjectTree;
    ageGM_Knot * root;
    //sucht ihm Baum nach einem gegebenen Object
    ageGM_Knot * findObject(ageGC_Object * obj);
};

#endif //AGEGC_SCENEGRAPH_H