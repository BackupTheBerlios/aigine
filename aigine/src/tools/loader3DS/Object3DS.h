#ifndef _OBJECT3DS_H
#define _OBJECT3DS_H

#include "Types3DS.h"
#include "CLoad3DS.h"

#define TEXTURE_BMP 0
#define TEXTURE_TGA 1  // not available
#define TEXTURE_JPG 2  // not available
#define TEXTURE_GIF 3  // not available

#define WIREFRAME 0
#define TEXTURE 1

class Object3DS {

	public: 
		Object3DS();
		void loadObject(char *fileName);
		void renderObject();
		void setViewMode(int mode);
		void toString();
		UINT getTextureID(int i);

	private:
		UINT texture[MAX_TEXTURES];	// This holds the texture info, referenced by an ID

		CLoad3DS load3ds;			// This is 3DS class.  This should go in a good model class.
		t3DModel model;				// This holds the 3D Model info that we load in
		char objectName[256];

		GLuint objectDL;
		GLuint objectWireframeDL;
		
		int textureType;
		int viewMode;

		void createTexture(UINT textureArray[], LPSTR strFileName, int textureID);
		void createObjectDL(int type); // WIREFRAME | TEXTURE	
		int setTextureType(LPSTR strFileName);
};

#endif 

