/* Game Engine Design 
 * SceneManagenent.h  
 */

#ifndef SCENEMANAGEMENT_H

#define SCENEMANAGEMENT_H

#include <string>
#include "GraphicPipeline.h"
#include "SceneGraph.h"

using namespace std;

class Camera;

class Renderer;

/**
 * Das "SceneManagement" realisiert den Zugriff auf die "Objekte" - Hinzuf�gen,
 * L�schen, Eigenschaften �ndern etc. - Also das �ndern der Daten die
 * der "SceneGraph" ausgibt. 
 * 
 * TODO:
 * - geladene Objekte in den "SceneGraph" Baum h�ngen
 *   (derzeitig wird noch ein einzelnes tmpObject verwendet)
 * - FrustumCulling (nur sichtbare Objekte zeichnen)
 * - Einteilen der Welt in einen Octtree und Abfrage f�r Volumen, Sichtbarkeit etc.
 *
 * @author Danny Graef, Tobias Harpering
 * @date 2004-01-06
 */ 
class SceneManagement : public SceneGraph, public GraphicPipeline
{

public:
	/**
	 * Konstruktor	 
	 */
    SceneManagenent();

    /**
     * L�dt ein 3D Objekt aus einer Datei.
     * @param fileType Typ der Datei
     * @param fileName Name der zu ladenen Datei
     */
    void load(string fileType, string fileName);

    /**
     * Ausgabe der Szene.
     * Ruft die Methoden von "GraphicPipeline" und
     * "SceneGraph" f�r die Initialisierung und das
     * Rendering der Objekte auf.
     */
    void display();

private:

    /**
     * @supplierCardinality 1
     * @clientCardinality 1
     */
    Renderer * lnkRenderer;
};

#endif //SCENEMANAGEMENT_H