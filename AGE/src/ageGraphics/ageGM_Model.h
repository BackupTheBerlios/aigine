/* Game Engine Design */

/*
// This holds all the information for our model/scene. 
// You should eventually turn into a robust class that 
// has loading/drawing/querying functions like:
// LoadModel(...); DrawObject(...); DrawModel(...); DestroyModel(...);
struct t3DObject 
{
	int  numOfVerts;			// The number of verts in the model
	int  numOfFaces;			// The number of faces in the model
	int  numTexVertex;			// The number of texture coordinates
	int  materialID;			// The texture ID to use, which is the index into our texture array
	bool bHasTexture;			// This is TRUE if there is a texture map for this object
	char strName[255];			// The name of the object
	tVector3  *pVerts;			// The object's vertices
	tVector3  *pNormals;		// The object's normals
	tVector2  *pTexVerts;		// The texture's UV coordinates
	tFace *pFaces;				// The faces information of the object
};
/**/






#ifndef AGEGM_MODEL_H

#define AGEGM_MODEL_H

#include <string>
using namespace std;
#include "ageGM_List.h"

#include "ageGM_Translation3D.h"
#include "ageGM_Rotation3D.h"
#include "ageGM_Scale3D.h"
#include "ageGM_Mesh.h"
#include "ageGM_Vector3.h"
#include "ageGM_List.h"
#include "ageGraphics/ageGM_Translation3D.h"
#include "ageGraphics/ageGM_Rotation3D.h"
#include "ageGraphics/ageGM_Vector3.h"
#include "ageGraphics/ageGM_Scale3D.h"
#include "ageGraphics/Model/ageGM_List.h"
class ageGM_Vector2;


/**
 * Jedes <class>Object</class> hat genau einen
 * <class>Shader</class> und ein oder mehrere
 * <class>Meshes</class>. Das Objekt setzt die
 * Werte des Shaders, und rendert dann die
 * einzelnen Meshes (wenn benötigt auch öfter als einmal).
 * @stereotype interface
 * @author Tobias Harpering
 * @author Danny Graef
 * @author Frank Otto
 */
class ageGM_Model
{

public:

    /**
     * Constructor 
     */
    ageGM_Model();

    ~ageGM_Model();

    ageGM_Translation3D *  getTranslation();

    void setTranslation(ageGM_Translation3D * translation);

    /**
     * Setzt die Translation auf die angebenen Koordinaten.
     * @param x Koordinate.
     * @param y Koordinate.
     * @param z Koordinate.
     */
    void setTranslation(float x, float y, float z);

  /**
   * Rotiert das erste Gelenk entsprechend der Achse.
   * @param angle Winkel um den gedreht werden soll.
   * @param axis Achse, die für die Drehung verwendet 
   *             werden soll 'x' = links, rechts
   *                         'z' = hoch, runter.
   */
  void rotate(int angle, char axis);

    ageGM_Rotation3D*  getRotation();

    void setRotation(ageGM_Rotation3D * rotation);

    ageGM_Scale3D* getScale();

    void setScale(ageGM_Scale3D * scale);

    /**
     * Init Funktion des Objektes. Zur Testzwecken mal ein kleines Object initiert 
     */
    void init(string Filename);

    ageGM_Vector3* getDirectionVector3D();

    void setDirectionVector3D(ageGM_Vector3 * directionVector3D);

private:


	int  numVertices;			// The number of verts in the model
	int  vacesCount;			// The number of faces in the model
	int  vertexCount;			// The number of texture coordinates
	int  materialID;			// The texture ID to use, which is the index into our texture array
	bool isTexture;			// This is TRUE if there is a texture map for this object
	string strName;			// The name of the object
	ageGM_Vector3 * vertices;			// The object's vertices
	ageGM_Vector3 * normals;		// The object's normals
	ageGM_Vector2 * texVertices;		// The texture's UV coordinates
	ageGM_List< Type > meshList;	// The faces information of the object
    
	    
    ageGM_Translation3D * translation;
    ageGM_Rotation3D * rotation;  
    ageGM_Scale3D * scale;
    ageGM_Vector3* directionVector3D;
};

#endif //AGEGM_MODEL_H