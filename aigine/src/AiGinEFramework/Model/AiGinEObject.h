/* Game Engine Design */



#ifndef AIGINEOBJECT_H

#define AIGINEOBJECT_H

#include "../Model/Translation3D.h"
#include "../Model/Rotation3D.h"
#include "../Model/Scale3D.h"
class Renderer;
class MeshList;

class Shader;

class Mesh;



/**
 * Jedes <class>Object</class> hat genau einen
 * <class>Shader</class> und ein oder mehrere
 * <class>Meshes</class>. Das Objekt setzt die
 * Werte des Shaders, und rendert dann die
 * einzelnen Meshes (wenn benötigt auch öfter als einmal).
 * @stereotype interface
 * @author Tobias Harpering
 * @author Danny Graef
 * @author Frank Otto
 */

class AiGinEObject
{

public:

    Translation3D & getTranslation();

    void setTranslation(Translation3D translation);

    Rotation3D & getRotation();

    void setRotation(Rotation3D rotation);

    Scale3D& getScale();

    void setScale(Scale3D scale);

private:



    /**
     * @supplierCardinality 1
     * @clientCardinality 1
     * @label ObjectShader
     */

    Shader * lnkShader;



    /**
     * @supplierCardinality 1
     * @clientCardinality 1
     * @label ObjectMeshes
     */

    MeshList * lnkMeshes;

    Translation3D translation;
    Rotation3D rotation;

    /**
     * @supplierCardinality 1
     * @clientCardinality 1
     */
    Renderer * lnkRenderer;
    Scale3D scale;
};

#endif //AIGINEOBJECT_H