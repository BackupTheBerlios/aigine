/* Game Engine Design */



#ifndef SCENEGRAPH_H

#define SCENEGRAPH_H

#include <GL/glut.h>

#include "../Model/Tree.h"
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

private:
    ObjectTree * lnkObjectTree;
};

#endif //SCENEGRAPH_H