/* Game Engine Design
 * SceneManagenent.h
 */

#ifndef SCENEMANAGEMENT_H

#define SCENEMANAGEMENT_H

#include "../AiGinE.h"

class Camera;

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
class SceneManagement : public SceneGraph, public GraphicPipeline
{

public:
    /** Konstruktor */
    SceneManagement();

    /**
     * L�dt ein 3D Objekt aus einer Datei und h�ngt es in die Wurzel des SceneGraph.
     * @param fileName Name der zu ladenen Datei
     */
    AiGinEObject * addObject3DS(string fileName);

    /**
     * L�dt ein 3D Objekt aus einer Datei und h�ngt es an den parent entspr. von kind
     * @param fileName Name der zu ladenen Datei
     * @param parent Eltern-Objekt
     * @param kind Als "Next"- bzw. "Child"-Objekt anh�ngen
     */
	AiGinEObject * addObject3DS(string fileName, AiGinEObject* parent, string kind);



	AiGinEObject * addObjectPrim(AiGinEObject* obj);

	AiGinEObject * addObjectPrim(AiGinEObject* obj, AiGinEObject* parent, string kind);




    /**
     * Ausgabe der Szene. Ruft die Methoden von "GraphicPipeline" und
     * "SceneGraph" f�r die Initialisierung und das Rendering der Objekte auf.
     */
    void display();
};

#endif //SCENEMANAGEMENT_H