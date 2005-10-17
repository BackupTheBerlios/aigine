/* Game Engine Design
 * GraphicPipeline.h
 */

#ifndef AGEGV_GRAPHICPIPELINE_H

#define AGEGV_GRAPHICPIPELINE_H

#define map_nb 2

#include <GL/glut.h>
//#include <gl/glaux.h>// ich weiss...
#include "ageGraphics/Model/ageGM_Camera.h"
#include "ageGraphics/ageGM_Camera.h"
#include "ageGraphics/ageGM_Vector3.h"
#include "ageGraphics/ageGM_Light.h"
#include "ageGM_Camera.h"
#include "ageGM_Vector3.h"
#include "ageGM_Light.h"



class ageGC_SceneGraph;

class Renderer;

class ageGM_Viewport;

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
class ageGV_GraphicPipeline
{

public:

      /** Konstruktor */
    ageGV_GraphicPipeline();

    /** 
     * Konstruktor für Übergabe der Programmparameter und Aufruf der
     * initOpenGL Funktion.
     * @param argc Anzahl der Parameter.
     * @param argv Liste mit Anzahl Parametern.
     */
    ageGV_GraphicPipeline(int argc, char * * argv);

    /** Destruktor */
    virtual ~ageGV_GraphicPipeline();

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
    ageGM_Camera * getCamera();

    /**Fühgt ein std. Licht an die angegebenen Stelle mit der angegebenen Blickrichtung*/
	  void addLightAt(ageGM_Vector3 *position, ageGM_Vector3 * lookAt);
    

    /** Gibt die aktuelle Fenstergröße zurück. */
    int * getWindowSize();

    /** Ruft die Kamera Rotation auf. */
    void setCameraRotation(int mouseX, int mouseY);

	/**Setzt das aktuelle Licht mit seinen Eigenschaften*/
	int enableLight(ageGM_Light * correntLight, int index);

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
    ageGM_Camera * lnkCamera;
    
	  /**
     *  Verwendet Licht
     * @supplierCardinality 1..*
     * @clientCardinality 1
     */
	ageGM_Light * lnkLight;
};

#endif //AGEGV_GRAPHICPIPELINE_H

#include "ageGM_Camera.h"
#include "ageGM_Vector3.h"
#include "ageGM_Light.h"

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
class ageGV_GraphicPipeline
{

public:

      /** Konstruktor */
    ageGV_GraphicPipeline();

    /** 
     * Konstruktor für Übergabe der Programmparameter und Aufruf der
     * initOpenGL Funktion.
     * @param argc Anzahl der Parameter.
     * @param argv Liste mit Anzahl Parametern.
     */
    ageGV_GraphicPipeline(int argc, char * * argv);

    /** Destruktor */
    virtual ~ageGV_GraphicPipeline();

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
    ageGM_Camera * getCamera();

    /**Fühgt ein std. Licht an die angegebenen Stelle mit der angegebenen Blickrichtung*/
	  void addLightAt(ageGM_Vector3 *position, ageGM_Vector3 * lookAt);
    

    /** Gibt die aktuelle Fenstergröße zurück. */
    int * getWindowSize();

    /** Ruft die Kamera Rotation auf. */
    void setCameraRotation(int mouseX, int mouseY);

	/**Setzt das aktuelle Licht mit seinen Eigenschaften*/
	int enableLight(ageGM_Light * correntLight, int index);

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
    ageGM_Camera * lnkCamera;
    
	  /**
     *  Verwendet Licht
     * @supplierCardinality 1..*
     * @clientCardinality 1
     */
	ageGM_Light * lnkLight;
};


