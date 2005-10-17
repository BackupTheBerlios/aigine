/* Game Engine Design
 * GraphicPipeline.h
 */

#ifndef AGEG_GRAPHICPIPELINE_H

#define AGEG_GRAPHICPIPELINE_H

#define map_nb 2

#include <GL/glut.h>
//#include <gl/glaux.h>// ich weiss...
#include "ageGraphics/Model/ageGM_Camera.h"
#include "ageG_Camera.h"
#include "ageG_Vector3.h"
#include "ageG_Light.h"
#include "ageG_Camera.h"
#include "ageG_Vector3.h"
#include "ageG_Light.h"



class ageG_SceneGraph;

class Renderer;

class ageG_Viewport;

class StateManager;

/**
 * Die "GraphicPipeline" ist verantwortlich für die Steuerung der "Camera",
 * die Initialisierung des OpenGL Fensters und stellt
 * eine Abstraktionsebene für den Zugriff auf OpenGL dar. TODO:
 * - Verwaltung von mehreren Kameras - Projektion von 3D auf 2D - Viewport
 * - StateManager - Parameter für initOpenGL() und reshape() aus einer
 * Config-Datei lesen und in einem Hash speichern
 * @author Danny Graef, Tobias Harpering ,Frank Otto
 * @date 2004-01-06
 */
class ageG_GraphicPipeline
{

public:

      /** Konstruktor */
    ageG_GraphicPipeline();

    /** 
     * Konstruktor für Übergabe der Programmparameter und Aufruf der
     * initOpenGL Funktion.
     * @param argc Anzahl der Parameter.
     * @param argv Liste mit Anzahl Parametern.
     */
    ageG_GraphicPipeline(int argc, char * * argv);

    /** Destruktor */
    virtual ~ageG_GraphicPipeline();

    /** Funktion für OpenGL reshape() Pointer. Setzt Viewport, Frustum, etc. */
    void reshape(int width, int height);

    /** Initialisiert die OpenGL Umgebung und erzeugt eine neue Kamera. */
    void initOpenGL(int argc, char * * argv);
    
    /**
     * Initialisiert die Bildschirmausgabe für
     * die verwendete Kamera und die eingestellten OpenGl States.
     */
    void initDisplay();

    /** Gibt einen Pointer auf die aktuelle Kamera zurück. */
    ageG_Camera * getCamera();

    /**Fühgt ein std. Licht an die angegebenen Stelle mit der angegebenen Blickrichtung*/
	  void addLightAt(ageG_Vector3 *position, ageG_Vector3 * lookAt);
    

    /** Gibt die aktuelle Fenstergröße zurück. */
    int * getWindowSize();

    /** Ruft die Kamera Rotation auf. */
    void setCameraRotation(int mouseX, int mouseY);

	/**Setzt das aktuelle Licht mit seinen Eigenschaften*/
	int enableLight(ageG_Light * correntLight, int index);

	/** initialisiert die Bilder fuer die Imagemask*/
	void InitMask();

	/** legt die ImageMask ueber den 3D-Screen -->ImageMask*/
	void ImageMask();

private:
    /** Größe des Fensters. */
    int * windowSize;

    /**
     * @supplierCardinality 1
     * @clientCardinality 1
     */
    StateManager * lnkStateManager;

    /**
     * Verwendete Kamera
     * @supplierCardinality 1..*
     * @clientCardinality 1
     */
    ageG_Camera * lnkCamera;
    
	  /**
     *  Verwendet Licht
     * @supplierCardinality 1..*
     * @clientCardinality 1
     */
	ageG_Light * lnkLight;
};

#endif //AGEG_GRAPHICPIPELINE_H