/* Game Engine Design */



#ifndef RENDERER_H

#define RENDERER_H
#include "GraphicPipeline.h"
class SceneGraph;

class SceneManagement;

/** @stereotype interface */
class Renderer
{
private:

    /**
     * @supplierCardinality 1
     * @clientCardinality 1
     */
    GraphicPipeline * lnkGraphicPipeline;
};

#endif //RENDERER_H