/* Game Engine Design */

/* Game Engine Design */


/**
 * Face schliesst je 3 Vertexes zu einem Polygon zusammen
 * @author Frank Otto
 * @author Tobias Harpering 
 */

#include "ageG_List.h"
#include "ageG_Vector2.h"
#include "ageG_Vector3.h"

#ifndef AGEG_MESH_H

#define AGEG_MESH_H



/**
 * Ein <class>Mesh</class> beinhaltet die Vertices die ben�tigt werden, um
 * das zugeh�rige Objekt oder den Objektteil darstellen zu
 * k�nnen. Also die Vektoren und sofern ben�tigt auch die
 * Normalen, Texturkoordinaten und Farbe je Vertex.
 * @stereotype struct
 */

class ageG_Mesh
{
public:    

    ageG_Mesh();

	virtual ~ageG_Mesh();

private:
    ageG_List< Type > * vertexList;
	ageG_List< Type > * texturCoordinates;   
};

#endif //AGEG_MESH_H