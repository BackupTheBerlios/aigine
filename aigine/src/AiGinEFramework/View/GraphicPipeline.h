/* Game Engine Design */

#ifndef GRAPHICPIPELINE_H
#define GRAPHICPIPELINE_H
class Renderer;
class Viewport;
class StateManager;
class Camera;

/**
 * Die "GraphicPipeline" ist verantwortlich für "Camera",
 * Projektion von 3D auf 2D, "Viewport" und das Zeichnen von Polygonen. Beinhaltet weiterhin die Initialisierung des OpenGL Fensters und stellt eine Abstraktionsebene für den Zugriff auf OpenGL dar	.
 */
class GraphicPipeline
{
private:

    /**
     * @supplierCardinality 1
     * @clientCardinality 1
     */
    Camera * lnkCamera;

    /**
     * @supplierCardinality 1
     * @clientCardinality 1
     */
    StateManager * lnkStateManager;

    /**
     * @supplierCardinality 1
     * @clientCardinality 1 
     */
    Viewport * lnkViewport;

    /**
     * @clientCardinality 1
     * @supplierCardinality 1 
     */
    Renderer * lnkRenderer;
};
#endif //GRAPHICPIPELINE_H