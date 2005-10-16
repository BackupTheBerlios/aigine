/* Game Engine Design */

/* Game Engine Design */


/**
 * Face schliesst je 3 Vertexes zu einem Polygon zusammen
 * @author Frank Otto
 * @author Tobias Harpering 
 */

#include "ageGM_List.h"
#include "ageGM_Vector2.h"
#include "ageGM_Vector3.h"

#ifndef AGEGM_MESH_H

#define AGEGM_MESH_H



/**
 * Ein <class>Mesh</class> beinhaltet die Vertices die benötigt werden, um
 * das zugehörige Objekt oder den Objektteil darstellen zu
 * können. Also die Vektoren und sofern benötigt auch die
 * Normalen, Texturkoordinaten und Farbe je Vertex.
 * @stereotype struct
 */

class ageGM_Mesh
{
public:    

    ageGM_Mesh();

	virtual ~ageGM_Mesh();

private:
    ageGM_List< Type > * vertexList;
	ageGM_List< Type > * texturCoordinates;   
};

#endif //AGEGM_MESH_H