/* Game Engine Design */

#ifndef AIGINEOBJECT_H
#define AIGINEOBJECT_H
#include "Model/Object3D.h"
class Shader;
class Renderer;

/**
 * @stereotype interface 
 */
class AiGinEObject : public Object3D
{
private:    

    /**
     * @supplierCardinality 1
     * @clientCardinality 1 
     */
    Renderer * lnkRenderer;

    /**
     * @supplierCardinality 1..*
     * @clientCardinality 1 
     */
    Shader * lnkShader;
};
#endif //AIGINEOBJECT_H