/**
 * Author: Michael Haller, haller@fhs-hagenberg.ac.at, 11/15/2001        
 */
#include <gl\glut.h>
#include <gl\glaux.h>
#include <iostream>

#include "Object3DS.h"
#include "../tgaLoader.h"


Object3DS::Object3DS() {	
	textureType = TEXTURE_BMP;
	viewMode = TEXTURE;

	model.numOfMaterials = 0;
	model.numOfObjects = 0;

}

/**
 * LoadObject: fileName is the name for the object
 */
void
Object3DS::loadObject(char* fileName) {

	if (fileName) {
		strcpy(objectName, fileName);
	}

	load3ds.import3DS(&model, objectName);

	// Depending on how many textures we found, load each one (Assuming .BMP)
	// If you want to load other files than bitmaps, you will need to adjust CreateTexture().
	// Below, we go through all of the materials and check if they have a texture map to load.
	// Otherwise, the material just holds the color information and we don't need to load a texture.
	// Go through all the materials
	for(int i = 0; i < model.numOfMaterials; i++) {
		// Check to see if there is a file name to load in this material
		if(strlen(model.pMaterials[i].strFile) > 0)
		{
			// Use the name of the texture file to load the bitmap, with a texture ID (i).
			// We pass in our global texture array, the name of the texture, and an ID to reference it.	
			createTexture(texture, model.pMaterials[i].strFile, i);			
		}

		// Set the texture ID for this material
		model.pMaterials[i].texureId = i;
	}

	createObjectDL(WIREFRAME);
	createObjectDL(TEXTURE);
}


int 
Object3DS::setTextureType(LPSTR strFileName) {
	char suffixBuffer[5];    	
    int i = 0;
	int j = 0;

	while ((strFileName[i] != '.') && (strFileName[i] != '\0')) {
		i++;
	}

	i++;

	while ((strFileName[i] != '\0') && (j<3)) {
       suffixBuffer[j] = strFileName[i];
	   j++;
	   i++;
	}

	suffixBuffer[j] = '\0';

	if (strcmp(suffixBuffer, "BMP") == 0) {
		textureType = TEXTURE_BMP;
	}
	else if (strcmp(suffixBuffer, "TGA") == 0) {
		textureType = TEXTURE_TGA;
	}
	else {
		printf("%s is a non supported texture type\n", suffixBuffer);
		return -1;
	}

	return 0;
}


void 
Object3DS::createTexture(UINT textureArray[], LPSTR strFileName, int textureID) {
	
	AUX_RGBImageRec *pBitmap = NULL;
    TGAImage *tgaImage = NULL;

	FILE *pFile = NULL;					// The File Handle we will use to read the bitmap

	if(!strFileName)					// Return from the function if no file name was passed in
		return;
	
	pFile = fopen(strFileName,"r");		// Check To See If The File Exists

	setTextureType(strFileName);

	if(pFile) {
		if (textureType == TEXTURE_BMP) {
			pBitmap = auxDIBImageLoad(strFileName);	// Load the bitmap and store the data	
		}
		else if (textureType == TEXTURE_TGA) {
			tgaImage = new TGAImage();
			tgaImage->loadTGA(strFileName);
		}
		else {
			exit(0);
		}
	}
	else												// If we can't find the file, quit!
	{													// Prompt the error message
		printf("Couldn't find a texture!\n");
		exit(0);
	}
	
	glGenTextures(1, &textureArray[textureID]);
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);

	if (textureType == TEXTURE_BMP) {
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->sizeX, pBitmap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitmap->data);
	}
	else if (textureType == TEXTURE_TGA) {
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, tgaImage->imageWidth, tgaImage->imageHeight, tgaImage->texFormat, GL_UNSIGNED_BYTE, tgaImage->imageData);				
	}

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
	
	if (pBitmap) {
		if (pBitmap->data)
			free(pBitmap->data);						// Free the texture data, we don't need it anymore		
		free(pBitmap);									// Free the bitmap structure
	}	
}


UINT 
Object3DS::getTextureID(int i) {
	return texture[i];
}

void 
Object3DS::setViewMode(int mode) {
	viewMode = mode;
}

void
Object3DS::renderObject() {
	if (viewMode ==  TEXTURE) 
		glCallList(objectDL);
	if (viewMode == WIREFRAME)
		glCallList(objectWireframeDL);
}



void
Object3DS::createObjectDL(int type)  {
	

	if (type == TEXTURE) {	
		objectDL = glGenLists(1); // Create the id for the list
		glNewList(objectDL, GL_COMPILE);
	}
	else if (type == WIREFRAME) {
		objectWireframeDL = glGenLists(1); // Create the id for the list
		glNewList(objectWireframeDL, GL_COMPILE);
	}

	glPushMatrix();
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		// Since we know how many objects our model has, go through each of them.
		for(int i=0; i<model.numOfObjects; i++) {

			// Make sure we have valid objects just in case. (size() is in the vector class)
			if(model.pObject.size() <= 0) break;

			// Get the current object that we are displaying
			t3DObject *pObject = &model.pObject[i];
				
			// Check to see if this object has a texture map, if so bind the texture to it.
			if(pObject->bHasTexture) {

				// Turn on texture mapping and turn off color
				glEnable(GL_TEXTURE_2D);

				// Reset the color to normal again
				glColor3ub(255, 255, 255);

				// Bind the texture map to the object by it's materialID
				glBindTexture(GL_TEXTURE_2D, texture[pObject->materialID]);
			} else {

				// Turn off texture mapping and turn on color
				glDisable(GL_TEXTURE_2D);

				// Reset the color to normal again
				//glColor3ub(255, 255, 255);
			}
		
			if (type == TEXTURE)
				glBegin(GL_TRIANGLES);					// Begin drawing with our selected mode (triangles or lines)
			else if (type == WIREFRAME)
				glBegin(GL_LINE_STRIP);

				// Go through all of the faces (polygons) of the object and draw them
				for(int j = 0; j < pObject->numOfFaces; j++) {
					// Go through each corner of the triangle and draw it.
					for(int whichVertex = 0; whichVertex < 3; whichVertex++) {
						// Get the index for each point of the face
						int index = pObject->pFaces[j].vertIndex[whichVertex];
				
						// Give OpenGL the normal for this vertex.
						glNormal3f(pObject->pNormals[ index ].x, pObject->pNormals[ index ].y, pObject->pNormals[ index ].z);				    
						
						// If the object has a texture associated with it, give it a texture coordinate.
						if(pObject->bHasTexture) {

							// Make sure there was a UVW map applied to the object or else it won't have tex coords.
							if(pObject->pTexVerts) {
								glTexCoord2f(pObject->pTexVerts[ index ].x, pObject->pTexVerts[ index ].y);
							}
						} else {

							// Get and set the color that the object is, since it must not have a texture
							BYTE *pColor = model.pMaterials[pObject->materialID].color;
							glColor3ub(pColor[0], pColor[1], pColor[2]);
						}

						// Pass in the current vertex of the object (Corner of current face)
						glVertex3f(pObject->pVerts[ index ].x, pObject->pVerts[ index ].y, pObject->pVerts[ index ].z);				
					}
				}

			glEnd();								// End the drawing
		}
	glPopMatrix();
	glEndList();
}


void
Object3DS::toString() {
	// Since we know how many objects our model has, go through each of them.
	for(int i=0; i<model.numOfObjects; i++) {

		// Make sure we have valid objects just in case. (size() is in the vector class)
		if(model.pObject.size() <= 0) break;

		// Get the current object that we are displaying
		t3DObject *pObject = &model.pObject[i];
		
		cout << "Name = "<< pObject->strName << "\n";
		cout << "NumOfFaces = " << pObject->numOfFaces << "\n";
		cout << "NumOfVerts = "<< pObject->numOfVerts << "\n";
		cout << "NumOfTexVertex = "<< pObject->numTexVertex << "\n\n";
		
	}
}





