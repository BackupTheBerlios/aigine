/* Game Engine Design */



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
 * L�schen, Eigenschaften �ndern etc.. z.B. Aufruf an ein Model seine Vertizes
 * zwischen 2 Keyframes zu interpolieren.  Also das �ndern der Daten die
 * der "SceneGraph" ausgibt. weiter Aufgaben:
 * Einteilen der Welt in einen Octtree und Abfrage f�r Volumen, Sichtbarkeit etc.
 * Vorhandensein eines Spiegels (Scene muss zwei Mal gerendert werden)
 * ist daf�r verantwortlich, dass alle sichtbaren "Objekte" gezeichnet werden.
 */
class SceneManagement : public SceneGraph, public GraphicPipeline
{

public:

    SceneManagenent();

    void load(string fileType, string fileName);

    /**
     * Ausgabe der Szene. 
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