/* Game Engine Design
 * SceneManagenent.h
 */

#ifndef AGEGC_SCENEMANAGER_H

#define AGEGC_SCENEMANAGER_H

#include <string>
#include "ageGraphics/View/ageGV_GraphicPipeline.h"
#include "ageGC_SceneGraph.h"

#include "ageGraphics/View/ageGV_GraphicPipeline.h"
#include "ageGraphics/ageGC_Object.h"
#include "ageGV_GraphicPipeline.h"
#include "ageGC_Object.h"
#include "ageGraphics/Control/ageGC_SceneGraph.h"

using namespace std;

class ageGM_Camera;

class Renderer;

/**
 * Das "SceneManagement" realisiert den Zugriff auf die "Objekte" - Hinzufügen,
 * Löschen, Eigenschaften ändern etc. - Also das Ändern der Daten die
 * der "SceneGraph" ausgibt. TODO: - geladene Objekte in den "SceneGraph" Baum
 * hängen (derzeitig wird noch ein einzelnes tmpObject verwendet)
 * - FrustumCulling (nur sichtbare Objekte zeichnen)
 * - Einteilen der Welt in einen Octtree und Abfrage für Volumen,
 * Sichtbarkeit etc.
 * @author Danny Graef, Tobias Harpering
 * @since 2004-01-06
 */
class ageGC_SceneManager : public ageGC_SceneGraph, public ageGV_GraphicPipeline
{

public:
    /** Konstruktor */
    ageGC_SceneManager();

    /**
     * Lädt ein 3D Objekt aus einer Datei und hängt es in die Wurzel des SceneGraph.
     * @param fileName Name der zu ladenen Datei
     */
    ageGC_Object * addObject3DS(string fileName);

    /**
     * Lädt ein 3D Objekt aus einer Datei und hängt es an den parent entspr. von kind
     * @param fileName Name der zu ladenen Datei
     * @param parent Eltern-Objekt
     * @param kind Als "Next"- bzw. "Child"-Objekt anhängen
     */
	ageGC_Object * addObject3DS(string fileName, ageGC_Object* parent, string kind);



	ageGC_Object * addObjectPrim(ageGC_Object* obj);

	ageGC_Object * addObjectPrim(ageGC_Object* obj, ageGC_Object* parent, string kind);




    /**
     * Ausgabe der Szene. Ruft die Methoden von "GraphicPipeline" und
     * "SceneGraph" für die Initialisierung und das Rendering der Objekte auf.
     */
    void display();
};

#endif //AGEGC_SCENEMANAGER_H