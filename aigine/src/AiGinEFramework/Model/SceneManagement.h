/* Game Engine Design */



#ifndef SCENEMANAGEMENT_H

#define SCENEMANAGEMENT_H

#include "SceneGraph.h"
class Camera;

class Renderer;
class ObjectList;



/**
 * Das "SceneManagement" realisiert den Zugriff auf die "Objekte" - Hinzuf�gen,
 * L�schen, Eigenschaften �ndern etc.. z.B. Aufruf an ein Model seine Vertizes
 * zwischen 2 Keyframes zu interpolieren.  Also das �ndern der Daten die
 * der "SceneGraph" ausgibt. weiter Aufgaben:
 * Einteilen der Welt in einen Octtree und Abfrage f�r Volumen, Sichtbarkeit etc.
 * Vorhandensein eines Spiegels (Scene muss zwei Mal gerendert werden)
 * ist daf�r verantwortlich, dass alle sichtbaren "Objekte" gezeichnet werden.
 */

class SceneManagement : public SceneGraph
{

public:

    void load(String fileType, String fileName, object insertAt);

    void display();

private:



    /**
     * @clientCardinality 1
     * @supplierCardinality 1
     * @label SceneObjects
     */

    ObjectList * lnkObjectList;

    /**
     * @clientCardinality 1
     * @supplierCardinality 1..*
     */
    Camera * lnkRenderer;
};

#endif //SCENEMANAGEMENT_H