/* Game Engine Design */

/* Game Engine Design */


/**
 * Face schliesst je 3 Vertexes zu einem Polygon zusammen
 * @author Frank Otto
 * @author Tobias Harpering 
 */

#ifndef MESH_H

#define MESH_H

#include "../AiGinE.h"

/**
 * Ein <class>Mesh</class> beinhaltet die Vertices die ben�tigt werden, um
 * das zugeh�rige Objekt oder den Objektteil darstellen zu
 * k�nnen. Also die Vektoren und sofern ben�tigt auch die
 * Normalen, Texturkoordinaten und Farbe je Vertex.
 * @stereotype struct
 */

class Mesh
{
public:    

    Mesh();

	virtual ~Mesh();

private:
    List <Vector3D *> * vertexList;
	List <Vector2D *> * texturCoordinates;   
};

#endif //MESH_H