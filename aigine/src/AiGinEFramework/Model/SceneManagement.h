/* Game Engine Design
 * SceneManagenent.h
 */

#ifndef SCENEMANAGEMENT_H

#define SCENEMANAGEMENT_H

#include "../AiGinE.h"

class Camera;

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
class SceneManagement : public SceneGraph, public GraphicPipeline
{

public:
    /** Konstruktor */
    SceneManagement();

    /**
     * Lädt ein 3D Objekt aus einer Datei und hängt es in die Wurzel des SceneGraph.
     * @param fileName Name der zu ladenen Datei
     */
    AiGinEObject * addObject3DS(string fileName);

    /**
     * Lädt ein 3D Objekt aus einer Datei und hängt es an den parent entspr. von kind
     * @param fileName Name der zu ladenen Datei
     * @param parent Eltern-Objekt
     * @param kind Als "Next"- bzw. "Child"-Objekt anhängen
     */
	AiGinEObject * addObject3DS(string fileName, AiGinEObject* parent, string kind);



	AiGinEObject * addObjectPrim(AiGinEObject* obj);

	AiGinEObject * addObjectPrim(AiGinEObject* obj, AiGinEObject* parent, string kind);




    /**
     * Ausgabe der Szene. Ruft die Methoden von "GraphicPipeline" und
     * "SceneGraph" für die Initialisierung und das Rendering der Objekte auf.
     */
    void display();
};

#endif //SCENEMANAGEMENT_H