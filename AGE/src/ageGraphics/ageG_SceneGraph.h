/* Game Engine Design */



#ifndef AGEG_SCENEGRAPH_H

#define AGEG_SCENEGRAPH_H

#include <GL/glut.h>

#include "ageG_Tree.h"
#include "ageG_Knot.h"
#include "ageG_Object.h"
#include "ageG_Tree.h"
#include "ageG_Object.h"
#include "ageG_Knot.h"

class ageG_ObjectTree;

/**
 * Unter "SceneGraph" versteht man die Abbildung einer gesammten Szene. Er
 * beinhaltet einen Baum, der die Hierachie der Objekte wiederspiegelt. Der
 * SceneGraph kann weitere SceneGraphs enthalten. So wird es möglich aus einem
 * Modell einen ScenenGraph zu parsen und dem eigentlichen SceneGraph
 * hinzuzufügen (z.B. aus vrml oder 3ds).
 */
class ageG_SceneGraph : public ageG_Tree
{
public:

	ageG_SceneGraph();
    void addObject(ageG_Object*);

    void draw();

    void drawSceneGraph(ageG_Object* obj);
	
	void printGraph();
	void printSceneGraph(ageG_Object* obj, string tabs);

    //alle add- bzw. remove-Funktionen geben einen bool als Erfogsmeldung zurück
    //true -> Operation erfolgreich
    //false -> Fehler (könnte gegen int ausgetauscht werden; 0->OK x ->Fehler Nr. x)
	void addRoot(ageG_Object* rootObject);

    bool addChild(ageG_Object * parentObject, ageG_Object * childObject);

    bool addNext(ageG_Object * parentObject, ageG_Object * nextObject);

    bool removeObject(ageG_Object * object);

private:
    ageG_ObjectTree * lnkObjectTree;
    ageG_Knot * root;
    //sucht ihm Baum nach einem gegebenen Object
    ageG_Knot * findObject(ageG_Object * obj);
};

#endif //AGEG_SCENEGRAPH_H