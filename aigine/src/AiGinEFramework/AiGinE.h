#ifndef AIGINE_H
#define AIGINE_H

#pragma warning (disable: 4786)

#include <iostream>
#include <string>
#include <map>
//using namespace std;



#ifndef BYTE
	typedef unsigned char	BYTE;
#endif
#ifndef DWORD
	typedef unsigned long	DWORD;
#endif

// Allgemeine Makros und Definitionen
#define SAFE_DELETE(x)		{if((x) != NULL) {delete (x); (x) = NULL;}}		// Sicheres Löschen
#define SAFE_DELETE_ARRAY(x)	{if((x) != NULL) {delete[] (x); (x) = NULL;}}	// Sicheres Löschen eines Arrays
#define SAFE_RELEASE(x)		{if((x) != NULL) {(x)->Release(); (x) = NULL;}}	// Sicheres Releasen eines COM-Objekts
#define SAFE_MEMFREE(x)		{if((x) != NULL) {tbMemFree((x)); (x) = NULL;}}	// Sicheres Freigeben von Speicher
#define MIN(a, b)			((a) < (b) ? (a) : (b))							// Minimum
#define MAX(a, b)			((a) > (b) ? (a) : (b))							// Maximum
#define PI					(3.1415926535897932384626433832795f)			// Pi
#define DEG_TO_RAD(x)		((x) * 0.0174532925199432957692369076848f)		// Grad -> Bogenmaß
#define RAD_TO_DEG(x)		((x) * 57.295779513082320876798154814105f)		// Bogenmaß -> Grad
#define COLOR_CONV (0.003921568627450980392156862745098f) // = 1/255

const enum Result {
	OK			= 0,	// Erfolg
	ERROR		= 1,	// Allgemeiner Fehler
	CANCELED	= 2,	// Aktion abgebrochen
	NOT_FOUND	= 3,	// Nicht gefunden
	STOP		= 4,	// Stopp (Vorgang abbrechen)
};

float FloatRandom(const float fMin, const float fMax);

#include "Utils/Knot.h"
#include "Utils/List.h"
#include "Utils/Tree.h"
#include "Utils/fileControl.h"

#include "Control/Keyboard.h"
#include "Control/Mouse.h"


#include "../Tools/loader3DS/Object3DS.h"


#include "Model/Vector2D.h"
#include "Model/Vector3D.h"
#include "Model/Translation3D.h"
#include "Model/Rotation3D.h"
#include "Model/Scale3D.h"
#include "Model/Mesh.h"
#include "Model/Object3D.h"
#include "Model/AiGinEObject.h"
#include "Model/LookAtObjects.h"
#include "View/Camera.h"
#include "View/GraphicPipeline.h"
#include "Model/SceneGraph.h"
#include "Model/SceneManagement.h"
#include "Model/AgeObjects/ageObject3DS.h"
#include "Model/AgeObjects/ageObjectPrim.h"


#include "Model/Color.h"
#include "Model/Matrix4x4.h"
#include "Model/MeshList.h"
#include "Model/ObjectList.h"
#include "Model/Plane.h"
#include "Model/Shader.h"

#include "View/Light.h"
#include "View/Renderer.h"
#include "View/Viewport.h"
	
/*
#include "../Tools/loader3DS/CLoad3DS.h"
#include "../Tools/loader3DS/Types3DS.h"
*/

#endif //AIGINE_H
