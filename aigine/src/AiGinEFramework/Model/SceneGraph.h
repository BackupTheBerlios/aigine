/* Game Engine Design */



#ifndef SCENEGRAPH_H

#define SCENEGRAPH_H

#include <GL/glut.h>

#include "../Model/Tree.h"
#include "../Util/loader3DS/Object3DS.h"
class AiGinEObject;
class ObjectTree;



/**
 * Unter "SceneGraph" versteht man die Abbildung einer gesammten Szene. Er
 * beinhaltet einen Baum, der die Hierachie der Objekte wiederspiegelt. Der
 * SceneGraph kann weitere SceneGraphs enthalten. So wird es möglich aus einem
 * Modell einen ScenenGraph zu parsen und dem eigentlichen SceneGraph
 * hinzuzufügen (z.B. aus vrml oder 3ds).
 */


class SceneGraph : public Tree
{
public:

	SceneGraph();
    void addObject(AiGinEObject*);

    void draw();

    Object3DS * getTmpTestObject();

 //   void setTmpTestObject(Object3DS * tmpTestObject);

    void drawSceneGraph(AiGinEObject*);

    //alle add- bzw. remove-Funktionen geben einen bool als Erfogsmeldung zurück
    //true -> Operation erfolgreich
    //false -> Fehler (könnte gegen int ausgetauscht werden; 0->OK x ->Fehler Nr. x)
	void addRoot(AiGinEObject* rootObject);

    bool addChild(AiGinEObject * parentObject, AiGinEObject * childObject);

    bool addNext(AiGinEObject * parentObject, AiGinEObject * nextObject);

    bool removeObject(AiGinEObject * object);

private:
    ObjectTree * lnkObjectTree;
    Object3DS * tmpTestObject;
    Knot * root;
    //sucht ihm Baum nach einem gegebenen Object
    Knot * findObject(AiGinEObject * obj);
};

#endif //SCENEGRAPH_H