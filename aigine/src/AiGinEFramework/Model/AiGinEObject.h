/* Game Engine Design */

#ifndef AIGINEOBJECT_H
#define AIGINEOBJECT_H
#include "../Model/Object3D.h"
#include "../Model/Knot.h"

class Shader;
class Renderer;

/**
 * @stereotype interface 
 */
class AiGinEObject : public Object3D, public Knot
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

public:

	AiGinEObject();
	virtual void display();

};
#endif //AIGINEOBJECT_H