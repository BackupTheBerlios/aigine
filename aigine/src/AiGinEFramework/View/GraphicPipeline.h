/* Game Engine Design 
 * GraphicPipeline.h  
 */

#ifndef GRAPHICPIPELINE_H

#define GRAPHICPIPELINE_H

#include <GL/glut.h>
#include "Camera.h"

class SceneGraph;

class Renderer;

class Viewport;

class StateManager;

/**
 * Die "GraphicPipeline" ist verantwortlich f�r die Steuerung der "Camera",
 * die Initialisierung des OpenGL Fensters und stellt
 * eine Abstraktionsebene f�r den Zugriff auf OpenGL dar.
 *
 * TODO:
 * - Verwaltung von mehreren Kameras
 * - Projektion von 3D auf 2D
 * - Viewport
 * - StateManager
 * - Parameter f�r initOpenGL() und reshape() aus einer 
 *   Config-Datei lesen und in einem Hash speichern
 *
 * @author Danny Graef, Tobias Harpering
 * @date 2004-01-06
 */
class GraphicPipeline
{

public:


	/**
	 * Konstruktor	 
	 */
    GraphicPipeline();

    /**
     * Destruktor
     */
    virtual ~GraphicPipeline();

    /**
     * Initialisiert die OpenGL Umgebung und erzeugt eine
     * neue Kamera.
     */
    void initOpenGL(int argc, char ** argv);

    /**
     * Funktion f�r OpenGL reshape() Pointer.
     * Setzt Viewport, Frustum, etc.
     */
    void reshape(int width, int height);

    /**
     * Initialisiert die Bildschirmausgabe f�r
     * die verwendete Kamera und die eingestellten
     * OpenGl States.
     */
    void initDisplay();

    /**
     * Gibt einen Pointer auf die aktuelle
     * Kamera zur�ck.
     */
    Camera* getCamera();

    /**
     * Gibt die aktuelle Fenstergr��e zur�ck.
     */
	int * getWindowSize();

private:
	/**
     * Gr��e des Fensters.
     */
	int * windowSize;

    /**
     * @supplierCardinality 1
     * @clientCardinality 1
     */
    StateManager * lnkStateManager;

    /**
     * Verwendete Kamera
     * @supplierCardinality 1..*
     * @clientCardinality 1*/
    Camera * lnkCamera;
};

#endif //GRAPHICPIPELINE_H