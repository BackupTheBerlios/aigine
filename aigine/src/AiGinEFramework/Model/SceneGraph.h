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

class SceneGraph
{
public:

    void addObject(AiGinEObject * object);

    void draw();

    void removeObject(AiGinEObject * object);

    Object3DS* getTmpTestObject();

    void setTmpTestObject(Object3DS* tmpTestObject);

private:
    ObjectTree * lnkObjectTree;
    Object3DS* tmpTestObject;
};

#endif //SCENEGRAPH_H