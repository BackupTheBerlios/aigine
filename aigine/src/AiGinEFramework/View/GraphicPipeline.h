/* Game Engine Design */



#ifndef GRAPHICPIPELINE_H

#define GRAPHICPIPELINE_H

#include <GL/glut.h>
#include "Camera.h"
class SceneGraph;

class Renderer;

class Viewport;

class StateManager;

/**
 * Die "GraphicPipeline" ist verantwortlich für "Camera",
 * Projektion von 3D auf 2D, Viewport und das Zeichnen von Polygonen.
 * Beinhaltet weiterhin die Initialisierung des OpenGL Fensters und stellt
 * eine Abstraktionsebene für den Zugriff auf OpenGL dar.
 */
class GraphicPipeline
{

public:

    GraphicPipeline();

    virtual ~GraphicPipeline();

    /** Initialisiert OpenGL. */
    void initOpenGL(int argc, char * * argv);

    void reshape(int width, int height);

    /**
     * Initialisiert die Bildschirmausgabe. 
     */
    void initDisplay();

    Camera* getCamera();

private:



    /**
     * @supplierCardinality 1
     * @clientCardinality 1
     */

    StateManager * lnkStateManager;

    /**
     * @supplierCardinality 1..*
     * @clientCardinality 1*/
    Camera * lnkCamera;
};

#endif //GRAPHICPIPELINE_H