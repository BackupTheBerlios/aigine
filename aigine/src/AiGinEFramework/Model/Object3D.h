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






#ifndef OBJECT3D_H

#define OBJECT3D_H

#include "../AiGinE.h"

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
class Object3D
{

public:

    /**
     * Constructor 
     */
    Object3D();

    ~Object3D();

    Translation3D *  getTranslation();

    void setTranslation(Translation3D * translation);

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

    Rotation3D*  getRotation();

    void setRotation(Rotation3D * rotation);

    Scale3D* getScale();

    void setScale(Scale3D * scale);

    /**
     * Init Funktion des Objektes. Zur Testzwecken mal ein kleines Object initiert 
     */
    void initObject();

    Vector3D* getDirectionVector3D();

    void setDirectionVector3D(Vector3D * directionVector3D);

    

private:


	int  vertsCount;			// The number of verts in the model
	int  vacesCount;			// The number of faces in the model
	int  vertexCount;			// The number of texture coordinates
	int  materialID;			// The texture ID to use, which is the index into our texture array
	bool isTexture;			// This is TRUE if there is a texture map for this object
	string strName;			// The name of the object
	Vector3D * vertices;			// The object's vertices
	Vector3D * normals;		// The object's normals
	Vector2D * texVertices;		// The texture's UV coordinates
	List <Mesh *> meshList;	// The faces information of the object
    
	    
    Translation3D * translation;
    Rotation3D * rotation;  
    Scale3D * scale;
    Vector3D* directionVector3D;
};

#endif //OBJECT3D_H