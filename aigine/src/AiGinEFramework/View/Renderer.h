/* Game Engine Design */



#ifndef RENDERER_H

#define RENDERER_H
#include "GraphicPipeline.h"
class SceneGraph;

class SceneManagement;

/**
 * @stereotype interface 
 */
class Renderer {


    
private:    

    /**
     * @supplierCardinality 1
     * @clientCardinality 1 
     */
    GraphicPipeline * lnkGraphicPipeline;

    /**
     * @supplierCardinality 1
     * @clientCardinality 1 
     */
    SceneManagement * lnkSceneGraph;
};

#endif //RENDERER_H