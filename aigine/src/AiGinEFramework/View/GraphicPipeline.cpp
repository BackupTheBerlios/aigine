/* Game Engine Design */

#include "GraphicPipeline.h"

void GraphicPipeline::reshape(int width, int height)
{
   glViewport (0, 0, (GLsizei) width, (GLsizei) height);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 50.0);
   glMatrixMode (GL_MODELVIEW);
}

void GraphicPipeline::initOpenGL(int argc, char** argv){
    glutInit(&argc, argv); // initialisiert GLUT und übergibt die Parameter
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // mit Tiefeneffekt fuer Licht

    glutInitWindowSize (500, 500);  // festlegen der Fenstergroesse
    glutInitWindowPosition (100, 100); // Positionierung des Fensters
    glutCreateWindow (argv[0]); // Ausgabe des Fensters
}

GraphicPipeline::~GraphicPipeline()
{
}

GraphicPipeline::GraphicPipeline()
{
}