#include <GL/glut.h> 
#include "../GraphicEngine/GraphicPipeline.h"
#include "../GraphicEngine/SceneGraph.h"

GraphicPipeline* gp;
SceneGraph* sg;

void display(void)
{
    sg->draw();
}

void reshape (int w, int h)
{
    gp->reshape(w, h);
}

void keyboard(unsigned char key, int x, int y)
{
}

int main(int argc, char** argv)
{ 
    gp = new GraphicPipeline();
    gp->initOpenGL(argc, argv);
    sg = new SceneGraph();

    // Bekanntmachung der benoetigten Callback-Funktionen
    glutDisplayFunc(display); // .. fuer die Bildgenerierung
    glutReshapeFunc(reshape); // .. fuer die Aenderung der Fenstergroesse
    glutKeyboardFunc(keyboard); // .. fuer die Tastaturabfrage
    glutMainLoop(); // Kontrolle wird an GLUT uebergeben
   
    return 0;
} 