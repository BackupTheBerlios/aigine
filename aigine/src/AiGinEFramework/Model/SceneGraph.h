/* Game Engine Design */

#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H
#include "SceneManagement.h"
#include "Model/Tree.h"
class Renderer;
class ObjectList;

/**
 * Unter "SceneGraph" versteht man die Abbildung einer gesammten Szene. Er beinhaltet einen Baum, der die Hierachie der Objekte wiederspiegelt. Der SceneGraph kann weitere SceneGraphs entahlten. So wird es möglich aus einem Modell einen ScenenGraph zu parsen und dem eigentlichen SceneGraph hinzuzufügen (z.B. aus vrml oder 3ds).*/
class SceneGraph : public Tree {
private:    

    /**
     * @supplierCardinality 0..* 
     */
    SceneGraph * lnkSceneGraph;
};
#endif //SCENEGRAPH_H