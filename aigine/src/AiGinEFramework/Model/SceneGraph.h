/* Game Engine Design */



#ifndef SCENEGRAPH_H

#define SCENEGRAPH_H

#include <GL/glut.h>
#include "SceneManagement.h"

#include "../Model/Tree.h"
#include "GraphicPipeline.h"
class Camera;
class AiGinEObject;
class Renderer;
class ObjectTree;



/**
 * Unter "SceneGraph" versteht man die Abbildung einer gesammten Szene. Er
 * beinhaltet einen Baum, der die Hierachie der Objekte wiederspiegelt. Der
 * SceneGraph kann weitere SceneGraphs enthalten. So wird es möglich aus einem
 * Modell einen ScenenGraph zu parsen und dem eigentlichen SceneGraph
 * hinzuzufügen (z.B. aus vrml oder 3ds).
 */

class SceneGraph {
public:

    void addObject(AiGinEObject* object);

    void draw();

    void removeObject(AiGinEObject* object);

private:
    Tree* objects;
};

#endif //SCENEGRAPH_H