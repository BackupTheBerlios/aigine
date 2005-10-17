/* Game Engine Design
 * SceneManagenent.h
 */

#ifndef AGEG_SCENEMANAGER_H

#define AGEG_SCENEMANAGER_H

#include <string>
#include "ageG_SceneGraph.h"

#include "ageG_Object.h"
#include "ageG_GraphicPipeline.h"
#include "ageG_Object.h"
#include "ageGraphics/Control/ageGC_SceneGraph.h"

using namespace std;

class ageG_Camera;

class Renderer;

/**
 * Das "SceneManagement" realisiert den Zugriff auf die "Objekte" - Hinzuf�gen,
 * L�schen, Eigenschaften �ndern etc. - Also das �ndern der Daten die
 * der "SceneGraph" ausgibt. TODO: - geladene Objekte in den "SceneGraph" Baum
 * h�ngen (derzeitig wird noch ein einzelnes tmpObject verwendet)
 * - FrustumCulling (nur sichtbare Objekte zeichnen)
 * - Einteilen der Welt in einen Octtree und Abfrage f�r Volumen,
 * Sichtbarkeit etc.
 * @author Danny Graef, Tobias Harpering
 * @since 2004-01-06
 */
class ageG_SceneManager : public ageG_SceneGraph, public ageG_GraphicPipeline
{

public:
    /** Konstruktor */
    ageG_SceneManager();

    /**
     * L�dt ein 3D Objekt aus einer Datei und h�ngt es in die Wurzel des SceneGraph.
     * @param fileName Name der zu ladenen Datei
     */
    ageG_Object * addObject3DS(string fileName);

    /**
     * L�dt ein 3D Objekt aus einer Datei und h�ngt es an den parent entspr. von kind
     * @param fileName Name der zu ladenen Datei
     * @param parent Eltern-Objekt
     * @param kind Als "Next"- bzw. "Child"-Objekt anh�ngen
     */
	ageG_Object * addObject3DS(string fileName, ageG_Object* parent, string kind);



	ageG_Object * addObjectPrim(ageG_Object* obj);

	ageG_Object * addObjectPrim(ageG_Object* obj, ageG_Object* parent, string kind);




    /**
     * Ausgabe der Szene. Ruft die Methoden von "GraphicPipeline" und
     * "SceneGraph" f�r die Initialisierung und das Rendering der Objekte auf.
     */
    void display();
};

#endif //AGEG_SCENEMANAGER_H