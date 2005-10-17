/* Game Engine Design */

#ifndef AGEG_OBJECT_H
#define AGEG_OBJECT_H
#include "ageG_Model.h"
#include "ageG_Model.h"
#include "ageG_Knot.h"
#include "ageG_Model.h"
#include "ageG_Knot.h"

class Shader;
class Renderer;

/**
 * @stereotype interface 
 */
class ageG_Object : public ageG_Model, public ageG_Knot
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

	ageG_Object();
	virtual void display();

};
#endif //AGEG_OBJECT_H