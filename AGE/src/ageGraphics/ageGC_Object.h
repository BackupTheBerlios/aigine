/* Game Engine Design */

#ifndef AGEGC_OBJECT_H
#define AGEGC_OBJECT_H
#include "ageGM_Model.h"
#include "ageGM_Model.h"
#include "ageGraphics/ageGM_Knot.h"
#include "ageGraphics/ageGM_Model.h"
#include "ageGM_Knot.h"

class Shader;
class Renderer;

/**
 * @stereotype interface 
 */
class ageGC_Object : public ageGM_Model, public ageGM_Knot
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

	ageGC_Object();
	virtual void display();

};
#endif //AGEGC_OBJECT_H