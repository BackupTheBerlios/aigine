/* Game Engine Design
 * GraphicPipeline.h
 */

#ifndef GRAPHICPIPELINE_H

#define GRAPHICPIPELINE_H

#define map_nb 2

#include "../AiGinE.h"

class Light;

class SceneGraph;

class Renderer;

class Viewport;

class StateManager;

/**
 * Die "GraphicPipeline" ist verantwortlich f�r die Steuerung der "Camera",
 * die Initialisierung des OpenGL Fensters und stellt
 * eine Abstraktionsebene f�r den Zugriff auf OpenGL dar. TODO:
 * - Verwaltung von mehreren Kameras - Projektion von 3D auf 2D - Viewport
 * - StateManager - Parameter f�r initOpenGL() und reshape() aus einer
 * Config-Datei lesen und in einem Hash speichern
 * @author Danny Graef, Tobias Harpering ,Frank Otto
 * @date 2004-01-06
 */
class GraphicPipeline
{

public:

      /** Konstruktor */
    GraphicPipeline();

    /** 
     * Konstruktor f�r �bergabe der Programmparameter und Aufruf der
     * initOpenGL Funktion.
     * @param argc Anzahl der Parameter.
     * @param argv Liste mit Anzahl Parametern.
     */
    GraphicPipeline(int argc, char * * argv);

    /** Destruktor */
    virtual ~GraphicPipeline();

    /** Funktion f�r OpenGL reshape() Pointer. Setzt Viewport, Frustum, etc. */
    void reshape(int width, int height);

    /** Initialisiert die OpenGL Umgebung und erzeugt eine neue Kamera. */
    void initOpenGL(int argc, char * * argv);
    
    /**
     * Initialisiert die Bildschirmausgabe f�r
     * die verwendete Kamera und die eingestellten OpenGl States.
     */
    void initDisplay();

    /** Gibt einen Pointer auf die aktuelle Kamera zur�ck. */
    Camera * getCamera();

    /**F�hgt ein std. Licht an die angegebenen Stelle mit der angegebenen Blickrichtung*/
	  void addLightAt(Vector3D *position, Vector3D * lookAt);
    

    /** Gibt die aktuelle Fenstergr��e zur�ck. */
    int * getWindowSize();

    /** Ruft die Kamera Rotation auf. */
    void setCameraRotation(int mouseX, int mouseY);

	/**Setzt das aktuelle Licht mit seinen Eigenschaften*/
	int enableLight(Light * correntLight, int index);

	/** initialisiert die Bilder fuer die Imagemask*/
	void InitMask();

	/** legt die ImageMask ueber den 3D-Screen -->ImageMask*/
	void ImageMask();

private:
    /** Gr��e des Fensters. */
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
    Camera * lnkCamera;
    
	  /**
     *  Verwendet Licht
     * @supplierCardinality 1..*
     * @clientCardinality 1
     */
	Light * lnkLight;


	HDC g_hDC; // wird von GLswapBuffer benoetigt

	void CreateTexture(GLuint textureArray[], LPSTR strFileName, int textureID);
	GLuint g_Texture[map_nb];

   	/** schaltet in die 2D Projektion um -->ImageMask*/
	void OrthoMode(int left, int top, int right, int bottom);

	/** schaltet in die 3D Projektion um -->ImageMask*/
	void PerspectiveMode();

};

#endif //GRAPHICPIPELINE_H