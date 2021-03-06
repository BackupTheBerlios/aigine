/**
 * Author: DigiBen,	digiben@gametutorials.com 06/01/2001
 *		   Michael Haller, haller@fhs-hagenberg.ac.at, 11/15/2001        
 */

/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// This was a HUGE amount of knowledge and probably the largest tutorial yet!
// In the next tutorial we will show you how to load a text file format called .obj.
// This is the most common 3D file format that almost ANY 3D software will import.
//
// CHUNKS: What is a chunk anyway?
// 
// "The chunk ID is a unique code which identifies the type of data in this chunk 
// and also may indicate the existence of subordinate chunks. The chunk length indicates 
// the length of following data to be associated with this chunk. Note, this may 
// contain more data than just this chunk. If the length of data is greater than that 
// needed to fill in the information for the chunk, additional subordinate chunks are 
// attached to this chunk immediately following any data needed for this chunk, and 
// should be parsed out. These subordinate chunks may themselves contain subordinate chunks. 
// Unfortunately, there is no indication of the length of data, which is owned by the current 
// chunk, only the total length of data attached to the chunk, which means that the only way 
// to parse out subordinate chunks is to know the exact format of the owning chunk. On the 
// other hand, if a chunk is unknown, the parsing program can skip the entire chunk and 
// subordinate chunks in one jump. " - Jeff Lewis (werewolf@worldgate.com)
//
// In a short amount of words, a chunk is defined this way:
// 2 bytes - Stores the chunk ID (OBJECT, MATERIAL, PRIMARY, etc...)
// 4 bytes - Stores the length of that chunk.  That way you know when that
//           chunk is done and there is a new chunk.
//
// So, to start reading the 3DS file, you read the first 2 bytes of it, then
// the length (using fread()).  It should be the PRIMARY chunk, otherwise it isn't
// a .3DS file.  
//
// Below is a list of the order that you will find the chunks and all the know chunks.
// If you go to www.wosit.org you can find a few documents on the 3DS file format.
// You can also take a look at the 3DS Format.rtf that is included with this tutorial.
//
//
//
//      MAIN3DS  (0x4D4D)
//     |
//     +--EDIT3DS  (0x3D3D)
//     |  |
//     |  +--EDIT_MATERIAL (0xAFFF)
//     |  |  |
//     |  |  +--MAT_NAME01 (0xA000) (See mli Doc) 
//     |  |
//     |  +--EDIT_CONFIG1  (0x0100)
//     |  +--EDIT_CONFIG2  (0x3E3D) 
//     |  +--EDIT_VIEW_P1  (0x7012)
//     |  |  |
//     |  |  +--TOP            (0x0001)
//     |  |  +--BOTTOM         (0x0002)
//     |  |  +--LEFT           (0x0003)
//     |  |  +--RIGHT          (0x0004)
//     |  |  +--FRONT          (0x0005) 
//     |  |  +--BACK           (0x0006)
//     |  |  +--USER           (0x0007)
//     |  |  +--CAMERA         (0xFFFF)
//     |  |  +--LIGHT          (0x0009)
//     |  |  +--DISABLED       (0x0010)  
//     |  |  +--BOGUS          (0x0011)
//     |  |
//     |  +--EDIT_VIEW_P2  (0x7011)
//     |  |  |
//     |  |  +--TOP            (0x0001)
//     |  |  +--BOTTOM         (0x0002)
//     |  |  +--LEFT           (0x0003)
//     |  |  +--RIGHT          (0x0004)
//     |  |  +--FRONT          (0x0005) 
//     |  |  +--BACK           (0x0006)
//     |  |  +--USER           (0x0007)
//     |  |  +--CAMERA         (0xFFFF)
//     |  |  +--LIGHT          (0x0009)
//     |  |  +--DISABLED       (0x0010)  
//     |  |  +--BOGUS          (0x0011)
//     |  |
//     |  +--EDIT_VIEW_P3  (0x7020)
//     |  +--EDIT_VIEW1    (0x7001) 
//     |  +--EDIT_BACKGR   (0x1200) 
//     |  +--EDIT_AMBIENT  (0x2100)
//     |  +--EDIT_OBJECT   (0x4000)
//     |  |  |
//     |  |  +--OBJ_TRIMESH   (0x4100)      
//     |  |  |  |
//     |  |  |  +--TRI_VERTEXL          (0x4110) 
//     |  |  |  +--TRI_VERTEXOPTIONS    (0x4111)
//     |  |  |  +--TRI_MAPPINGCOORS     (0x4140) 
//     |  |  |  +--TRI_MAPPINGSTANDARD  (0x4170)
//     |  |  |  +--TRI_FACEL1           (0x4120)
//     |  |  |  |  |
//     |  |  |  |  +--TRI_SMOOTH            (0x4150)   
//     |  |  |  |  +--TRI_MATERIAL          (0x4130)
//     |  |  |  |
//     |  |  |  +--TRI_LOCAL            (0x4160)
//     |  |  |  +--TRI_VISIBLE          (0x4165)
//     |  |  |
//     |  |  +--OBJ_LIGHT    (0x4600)
//     |  |  |  |
//     |  |  |  +--LIT_OFF              (0x4620)
//     |  |  |  +--LIT_SPOT             (0x4610) 
//     |  |  |  +--LIT_UNKNWN01         (0x465A) 
//     |  |  | 
//     |  |  +--OBJ_CAMERA   (0x4700)
//     |  |  |  |
//     |  |  |  +--CAM_UNKNWN01         (0x4710)
//     |  |  |  +--CAM_UNKNWN02         (0x4720)  
//     |  |  |
//     |  |  +--OBJ_UNKNWN01 (0x4710)
//     |  |  +--OBJ_UNKNWN02 (0x4720)
//     |  |
//     |  +--EDIT_UNKNW01  (0x1100)
//     |  +--EDIT_UNKNW02  (0x1201) 
//     |  +--EDIT_UNKNW03  (0x1300)
//     |  +--EDIT_UNKNW04  (0x1400)
//     |  +--EDIT_UNKNW05  (0x1420)
//     |  +--EDIT_UNKNW06  (0x1450)
//     |  +--EDIT_UNKNW07  (0x1500)
//     |  +--EDIT_UNKNW08  (0x2200)
//     |  +--EDIT_UNKNW09  (0x2201)
//     |  +--EDIT_UNKNW10  (0x2210)
//     |  +--EDIT_UNKNW11  (0x2300)
//     |  +--EDIT_UNKNW12  (0x2302)
//     |  +--EDIT_UNKNW13  (0x2000)
//     |  +--EDIT_UNKNW14  (0xAFFF)
//     |
//     +--KEYF3DS (0xB000)
//        |
//        +--KEYF_UNKNWN01 (0xB00A)
//        +--............. (0x7001) ( viewport, same as editor )
//        +--KEYF_FRAMES   (0xB008)
//        +--KEYF_UNKNWN02 (0xB009)
//        +--KEYF_OBJDES   (0xB002)
//           |
//           +--KEYF_OBJHIERARCH  (0xB010)
//           +--KEYF_OBJDUMMYNAME (0xB011)
//           +--KEYF_OBJUNKNWN01  (0xB013)
//           +--KEYF_OBJUNKNWN02  (0xB014)
//           +--KEYF_OBJUNKNWN03  (0xB015)  
//           +--KEYF_OBJPIVOT     (0xB020)  
//           +--KEYF_OBJUNKNWN04  (0xB021)  
//           +--KEYF_OBJUNKNWN05  (0xB022)  
//
// Once you know how to read chunks, all you have to know is the ID you are looking for
// and what data is stored after that ID.  You need to get the file format for that.
// I can give it to you if you want, or you can go to www.wosit.org for several versions.
// Because this is a proprietary format, it isn't a official document.
//
// I know there was a LOT of information blown over, but it is too much knowledge for
// one tutorial.  In the animation tutorial that I eventually will get to, some of
// the things explained here will be explained in more detail.  I do not claim that
// this is the best .3DS tutorial, or even a GOOD one :)  But it is a good start, and there
// isn't much code out there that is simple when it comes to reading .3DS files.
// So far, this is the best I have seen.  That is why I made it :)
// 
// I would like to thank www.wosit.org and Terry Caton (tcaton@umr.edu) for his help on this.
//
// Let me know if this helps you out!
// 
// 
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
//

#include "CLoad3DS.h"
#include <iostream>

/** This file handles all of the code needed to load a .3DS file.
 * Basically, how it works is, you load a chunk, then you check
 * the chunk ID.  Depending on the chunk ID, you load the information
 * that is stored in that chunk.  If you do not want to read that information,
 * you read past it.  You know how many bytes to read past the chunk because
 * every chunk stores the length in bytes of that chunk.
 */
CLoad3DS::CLoad3DS() {
	m_CurrentChunk = new tChunk; // Initialize and allocate our current chunk
	m_TempChunk = new tChunk;	 // Initialize and allocate a temporary chunk
}


/**
 * Reads the 3DS file with the filename strFileName and returns true or false
 * depending of successfull reading
 */
bool 
CLoad3DS::import3DS(t3DModel *pModel, char *strFileName) {

	// Open the 3DS file
	m_FilePointer = fopen(strFileName, "rb");

	// Make sure we have a valid file pointer (we found the file)
	if(!m_FilePointer) {
		cout <<  "Unable to find the file: " << strFileName << "\n";
		cout.flush();
		return false;
	}

	// Once we have the file open, we need to read the very first data chunk
	// to see if it's a 3DS file. 
	// If it is a 3DS file, then the first chunk ID will be equal to PRIMARY (some hex num)

	// Read the first chuck of the file to see if it's a 3DS file
	readChunk(m_CurrentChunk);

	// Make sure this is a 3DS file
	if (m_CurrentChunk->ID != PRIMARY) {
		cout << "Unable to load PRIMARY chuck from file \n";
		cout.flush();
		return false;
	}

	// Now we actually start reading in the data.  ProcessNextChunk() is recursive
	// Begin loading objects, by calling this recursive function
	processNextChunk(pModel, m_CurrentChunk);

	// After we have read the whole 3DS file, we want to calculate our own vertex normals.
	computeNormals(pModel);

	// Clean up after everything
	cleanUp();

	return 0;
}

/** 
 * This function cleans up our allocated memory and closes the file
 */
void 
CLoad3DS::cleanUp() {
	fclose(m_FilePointer);		// Close the current file pointer
	delete m_CurrentChunk;		// Free the current chunk
	delete m_TempChunk;			// Free our temporary chunk
}


/**
 * This function reads the main sections of the .3DS file, then dives deeper with recursion
 */
void 
CLoad3DS::processNextChunk(t3DModel *pModel, tChunk *pPreviousChunk) {
	t3DObject newObject = {0};		// This is used to add to our object list
	tMaterialInfo newTexture = {0};	// This is used to add to our material list
	unsigned short version = 0;		// This will hold the file version
	int buffer[50000] = {0};	    // This is used to read past unwanted data

	m_CurrentChunk = new tChunk;	// Allocate a new chunk				

	// Below we check our chunk ID each time we read a new chunk.  Then, if
	// we want to extract the information from that chunk, we do so.
	// If we don't want a chunk, we just read past it.  
	// Continue to read the sub chunks until we have reached the length.
	// After we read ANYTHING we add the bytes read to the chunk and then check
	// check against the length.
	while (pPreviousChunk->bytesRead < pPreviousChunk->length) {

		// Read next Chunk
		readChunk(m_CurrentChunk);

		// Check the chunk ID
		switch (m_CurrentChunk->ID) 	{
			case VERSION:							// This holds the version of the file
				
				// This chunk has an unsigned short that holds the file version.
				// Since there might be new additions to the 3DS file format in 4.0,
				// we give a warning to that problem.

				// Read the file version and add the bytes read to our bytesRead variable
				m_CurrentChunk->bytesRead += fread(&version, 1, m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);

				// If the file version is over 3, give a warning that there could be a problem
				if (version > 0x03)
					cout << "This 3DS file is over version 3 so it may load incorrectly \n";
				break;

			case OBJECTINFO:						// This holds the version of the mesh
				
				// This chunk holds the version of the mesh.  It is also the head of the MATERIAL
				// and OBJECT chunks.  From here on we start reading in the material and object info.
				// Read the next chunk
				readChunk(m_TempChunk);

				// Get the version of the mesh
				m_TempChunk->bytesRead += fread(&version, 1, m_TempChunk->length - m_TempChunk->bytesRead, m_FilePointer);

				// Increase the bytesRead by the bytes read from the last chunk
				m_CurrentChunk->bytesRead += m_TempChunk->bytesRead;

				// Go to the next chunk, which is the object has a texture, it should be MATERIAL, then OBJECT.
				processNextChunk(pModel, m_CurrentChunk);
				break;

			case MATERIAL:							// This holds the material information

				// This chunk is the header for the material info chunks
				// Increase the number of materials
				pModel->numOfMaterials++;

				// Add a empty texture structure to our texture list.
				// If you are unfamiliar with STL's "vector" class, all push_back()
				// does is add a new node onto the list.  I used the vector class
				// so I didn't need to write my own link list functions.  
				pModel->pMaterials.push_back(newTexture);

				// Proceed to the material loading function
				processNextMaterialChunk(pModel, m_CurrentChunk);
				break;

			case OBJECT:							// This holds the name of the object being read
					
				// This chunk is the header for the object info chunks.  It also
				// holds the name of the object.

				// Increase the object count
				pModel->numOfObjects++;
			
				// Add a new tObject node to our list of objects (like a link list)
				pModel->pObject.push_back(newObject);
				
				// Initialize the object and all it's data members
				memset(&(pModel->pObject[pModel->numOfObjects - 1]), 0, sizeof(t3DObject));

				// Get the name of the object and store it, then add the read bytes to our byte counter.
				m_CurrentChunk->bytesRead += getString(pModel->pObject[pModel->numOfObjects - 1].strName);
				
				// Now proceed to read in the rest of the object information
				processNextObjectChunk(pModel, &(pModel->pObject[pModel->numOfObjects - 1]), m_CurrentChunk);
				break;

			case EDITKEYFRAME:

				// Because I wanted to make this a SIMPLE tutorial as possible, I did not include
				// the key frame information.  This chunk is the header for all the animation info.
				// In a later tutorial this will be the subject and explained thoroughly.
				// processNextKeyFrameChunk(pModel, m_CurrentChunk);

				// Read past this chunk and add the bytes read to the byte counter
				m_CurrentChunk->bytesRead += fread(buffer, 1, m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);
				break;

			default: 			
				// If we didn't care about a chunk, then we get here.  We still need
				// to read past the unknown or ignored chunk and add the bytes read to the byte counter.
				m_CurrentChunk->bytesRead += fread(buffer, 1, m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);
				break;
		}

		// Add the bytes read from the last chunk to the previous chunk passed in.
		pPreviousChunk->bytesRead += m_CurrentChunk->bytesRead;
	}

	// Free the current chunk and set it back to the previous chunk (since it started that way)
	delete m_CurrentChunk;
	m_CurrentChunk = pPreviousChunk;
}


/**
 *	This function handles all the information about the objects in the file
 */ 
void 
CLoad3DS::processNextObjectChunk(t3DModel *pModel, t3DObject *pObject, tChunk *pPreviousChunk) {

	int buffer[50000] = {0};					// This is used to read past unwanted data

	// Allocate a new chunk to work with
	m_CurrentChunk = new tChunk;

	// Continue to read these chunks until we read the end of this sub chunk
	while (pPreviousChunk->bytesRead < pPreviousChunk->length) {
		
		// Read the next chunk
		readChunk(m_CurrentChunk);

		// Check which chunk we just read
		switch (m_CurrentChunk->ID) {
			case OBJECT_MESH:					// This lets us know that we are reading a new object
			
				// We found a new object, so let's read in it's info using recursion
				processNextObjectChunk(pModel, pObject, m_CurrentChunk);
				break;

			case OBJECT_VERTICES:				// This is the objects vertices
				readVertices(pObject, m_CurrentChunk);
				break;

			case OBJECT_FACES:					// This is the objects face information
				readVertexIndices(pObject, m_CurrentChunk);
				break;

			case OBJECT_MATERIAL:				// This holds the material name that the object has
				
				// This chunk holds the name of the material that the object has assigned to it.
				// This could either be just a color or a texture map.  This chunk also holds
				// the faces that the texture is assigned to (In the case that there is multiple
				// textures assigned to one object, or it just has a texture on a part of the object.
				// Since most of my game objects just have the texture around the whole object, and 
				// they aren't multitextured, I just want the material name.
				// We now will read the name of the material assigned to this object
				readObjectMaterial(pModel, pObject, m_CurrentChunk);			
				break;

			case OBJECT_UV:						// This holds the UV texture coordinates for the object

				// This chunk holds all of the UV coordinates for our object.  Let's read them in.
				readUVCoordinates(pObject, m_CurrentChunk);
				break;

			default:  

				// Read past the ignored or unknown chunks
				m_CurrentChunk->bytesRead += fread(buffer, 1, m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);
				break;
		}

		// Add the bytes read from the last chunk to the previous chunk passed in.
		pPreviousChunk->bytesRead += m_CurrentChunk->bytesRead;
	}

	// Free the current chunk and set it back to the previous chunk (since it started that way)
	delete m_CurrentChunk;
	m_CurrentChunk = pPreviousChunk;
}


/**
 * This function handles all the information about the material (Texture)
 */
void 
CLoad3DS::processNextMaterialChunk(t3DModel *pModel, tChunk *pPreviousChunk) {
	int buffer[50000] = {0};					// This is used to read past unwanted data

	// Allocate a new chunk to work with
	m_CurrentChunk = new tChunk;

	// Continue to read these chunks until we read the end of this sub chunk
	while (pPreviousChunk->bytesRead < pPreviousChunk->length)	{
		
		// Read the next chunk
		readChunk(m_CurrentChunk);

		// Check which chunk we just read in
		switch (pModel, m_CurrentChunk->ID) {

			case MATNAME:							// This chunk holds the name of the material				
			
				// Here we read in the material name
				m_CurrentChunk->bytesRead += fread(pModel->pMaterials[pModel->numOfMaterials - 1].strName, 1, m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);
				break;

			case MATDIFFUSE:						// This holds the R G B color of our object
				readColorChunk(&(pModel->pMaterials[pModel->numOfMaterials - 1]), m_CurrentChunk);
				break;
			
			case MATMAP:							// This is the header for the texture info
				
				// Proceed to read in the material information
				processNextMaterialChunk(pModel, m_CurrentChunk);
				break;

			case MATMAPFILE:						// This stores the file name of the material

				// Here we read in the material's file name
				m_CurrentChunk->bytesRead += fread(pModel->pMaterials[pModel->numOfMaterials - 1].strFile, 1, m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);
				break;
			
			default:  

				// Read past the ignored or unknown chunks
				m_CurrentChunk->bytesRead += fread(buffer, 1, m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);
				break;
		}

		// Add the bytes read from the last chunk to the previous chunk passed in.
		pPreviousChunk->bytesRead += m_CurrentChunk->bytesRead;
	}

	// Free the current chunk and set it back to the previous chunk (since it started that way)
	delete m_CurrentChunk;
	m_CurrentChunk = pPreviousChunk;
}


/**
 * reads the chunk ID and its length in bytes
 */
void 
CLoad3DS::readChunk(tChunk *pChunk) {
	// This reads the chunk ID which is 2 bytes.
	// The chunk ID is like OBJECT or MATERIAL.  It tells what data is
	// able to be read in within the chunks section.  
	pChunk->bytesRead = fread(&pChunk->ID, 1, 2, m_FilePointer);

	// Then, we read the length of the chunk which is 4 bytes.
	// This is how we know how much to read in, or read past.
	pChunk->bytesRead += fread(&pChunk->length, 1, 4, m_FilePointer);
}

/**
 * This function reads in a string of characters
 */
int 
CLoad3DS::getString(char *pBuffer)
{
	int index = 0;

	// Read 1 byte of data which is the first letter of the string
	fread(pBuffer, 1, 1, m_FilePointer);

	// Loop until we get NULL
	while (*(pBuffer + index++) != 0) {

		// Read in a character at a time until we hit NULL.
		fread(pBuffer + index, 1, 1, m_FilePointer);
	}

	// Return the string length, which is how many bytes we read in (including the NULL)
	return strlen(pBuffer) + 1;
}


/**
 *	This function reads in the RGB color data
 */
void 
CLoad3DS::readColorChunk(tMaterialInfo *pMaterial, tChunk *pChunk) {
	// Read the color chunk info
	readChunk(m_TempChunk);

	// Read in the R G B color (3 bytes - 0 through 255)
	m_TempChunk->bytesRead += fread(pMaterial->color, 1, m_TempChunk->length - m_TempChunk->bytesRead, m_FilePointer);

	// Add the bytes read to our chunk
	pChunk->bytesRead += m_TempChunk->bytesRead;
}


/**
 *	This function reads in the indices for the vertex array
 */
void 
CLoad3DS::readVertexIndices(t3DObject *pObject, tChunk *pPreviousChunk) {
	unsigned short index = 0;					// This is used to read in the current face index

	// In order to read in the vertex indices for the object, we need to first
	// read in the number of them, then read them in.  Remember,
	// we only want 3 of the 4 values read in for each face.  The fourth is
	// a visibility flag for 3D Studio Max that doesn't mean anything to us.

	// Read in the number of faces that are in this object (int)
	pPreviousChunk->bytesRead += fread(&pObject->numOfFaces, 1, 2, m_FilePointer);

	// Alloc enough memory for the faces and initialize the structure
	pObject->pFaces = new tFace [pObject->numOfFaces];
	memset(pObject->pFaces, 0, sizeof(tFace) * pObject->numOfFaces);

	// Go through all of the faces in this object
	for(int i = 0; i < pObject->numOfFaces; i++)
	{
		// Next, we read in the A then B then C index for the face, but ignore the 4th value.
		// The fourth value is a visibility flag for 3D Studio Max, we don't care about this.
		for(int j = 0; j < 4; j++)
		{
			// Read the first vertice index for the current face 
			pPreviousChunk->bytesRead += fread(&index, 1, sizeof(index), m_FilePointer);

			if(j < 3)
			{
				// Store the index in our face structure.
				pObject->pFaces[i].vertIndex[j] = index;
			}
		}
	}
}


/**
 *	This function reads in the UV coordinates for the object
 */
void 
CLoad3DS::readUVCoordinates(t3DObject *pObject, tChunk *pPreviousChunk) {
	// In order to read in the UV indices for the object, we need to first
	// read in the amount there are, then read them in.
	// Read in the number of UV coordinates there are (int)
	pPreviousChunk->bytesRead += fread(&pObject->numTexVertex, 1, 2, m_FilePointer);

	// Allocate memory to hold the UV coordinates
	pObject->pTexVerts = new tVector2 [pObject->numTexVertex];

	// Read in the texture coodinates (an array 2 float)
	pPreviousChunk->bytesRead += fread(pObject->pTexVerts, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);
}


/**
 *	This function reads in the vertices for the object
 */
void 
CLoad3DS::readVertices(t3DObject *pObject, tChunk *pPreviousChunk) {
	// Like most chunks, before we read in the actual vertices, we need
	// to find out how many there are to read in.  Once we have that number
	// we then fread() them into our vertice array.

	// Read in the number of vertices (int)
	pPreviousChunk->bytesRead += fread(&(pObject->numOfVerts), 1, 2, m_FilePointer);

	// Allocate the memory for the verts and initialize the structure
	pObject->pVerts = new tVector3 [pObject->numOfVerts];
	memset(pObject->pVerts, 0, sizeof(tVector3) * pObject->numOfVerts);

	// Read in the array of vertices (an array of 3 floats)
	pPreviousChunk->bytesRead += fread(pObject->pVerts, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);
}


/**
 *	This function reads in the material name assigned to the object and sets the materialID
 */
void 
CLoad3DS::readObjectMaterial(t3DModel *pModel, t3DObject *pObject, tChunk *pPreviousChunk) {
	char strMaterial[255] = {0};			// This is used to hold the objects material name
	int buffer[50000] = {0};				// This is used to read past unwanted data

	// A material is either the color or the texture map of the object.
	// It can also hold other information like the brightness, shine, etc... Stuff we don't
	// really care about.  We just want the color, or the texture map file name really.
	// Here we read the material name that is assigned to the current object.
	// strMaterial should now have a string of the material name, like "Material #2" etc..
	pPreviousChunk->bytesRead += getString(strMaterial);

	// Now that we have a material name, we need to go through all of the materials
	// and check the name against each material.  When we find a material in our material
	// list that matches this name we just read in, then we assign the materialID
	// of the object to that material index.  You will notice that we passed in the
	// model to this function.  This is because we need the number of textures.
	// Yes though, we could have just passed in the model and not the object too.

	// Go through all of the textures
	for(int i = 0; i < pModel->numOfMaterials; i++)
	{
		// If the material we just read in matches the current texture name
		if(strcmp(strMaterial, pModel->pMaterials[i].strName) == 0)
		{
			// Set the material ID to the current index 'i' and stop checking
			pObject->materialID = i;

			// Now that we found the material, check if it's a texture map.
			// If the strFile has a string length of 1 and over it's a texture
			if(strlen(pModel->pMaterials[i].strFile) > 0) {

				// Set the object's flag to say it has a texture map to bind.
				pObject->bHasTexture = true;
			}	
			break;
		}
	}

	// Read past the rest of the chunk since we don't care about shared vertices
	// You will notice we subtract the bytes already read in this chunk from the total length.
	pPreviousChunk->bytesRead += fread(buffer, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);
}			


// This computes the magnitude of a normal.   (magnitude = sqrt(x^2 + y^2 + z^2)
#define MAG(Normal) (sqrt(Normal.x*Normal.x + Normal.y*Normal.y + Normal.z*Normal.z))

/**
 * This calculates a vector between 2 points and returns the result
 */
tVector3 
Vector(tVector3 vPoint1, tVector3 vPoint2) {
	tVector3 vVector;							// The variable to hold the resultant vector

	vVector.x = vPoint1.x - vPoint2.x;			// Subtract point1 and point2 x's
	vVector.y = vPoint1.y - vPoint2.y;			// Subtract point1 and point2 y's
	vVector.z = vPoint1.z - vPoint2.z;			// Subtract point1 and point2 z's

	return vVector;								// Return the resultant vector
}

/**
 * This adds 2 vectors together and returns the result
 */
tVector3 
addVector(tVector3 vVector1, tVector3 vVector2) {
	tVector3 vResult;					
	
	vResult.x = vVector2.x + vVector1.x; // Add Vector1 and Vector2 x's
	vResult.y = vVector2.y + vVector1.y; // Add Vector1 and Vector2 y's
	vResult.z = vVector2.z + vVector1.z; // Add Vector1 and Vector2 z's

	return vResult;								// Return the resultant vector
}

/**
 * This divides a vector by a single number (scalar) and returns the result
 */
tVector3 
divideVectorByScaler(tVector3 vVector1, float scaler) {
	tVector3 vResult;							
	
	vResult.x = vVector1.x / scaler;			
	vResult.y = vVector1.y / scaler;			
	vResult.z = vVector1.z / scaler;			

	return vResult;								
}

/** 
 * This returns the cross product between 2 vectors
 */
tVector3 
cross(tVector3 vVector1, tVector3 vVector2) {
	tVector3 vCross;	
				
	vCross.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));				
	vCross.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));												
	vCross.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));
	return vCross;								
}

/** 
 * This returns the normal of a vector
 */
tVector3 
normalize(tVector3 vNormal) {
	double magnitude;							// This holds the magitude			

	magnitude = MAG(vNormal);					// Get the magnitude

	vNormal.x /= (float)magnitude;			
	vNormal.y /= (float)magnitude;			
	vNormal.z /= (float)magnitude;			

	return vNormal;							
}




/**
 * This function computes the normals and vertex normals of the objects
 */
void 
CLoad3DS::computeNormals(t3DModel *pModel) {
	tVector3 vVector1, vVector2, vNormal, vPoly[3];

	// If there are no objects, we can skip this part
	if(pModel->numOfObjects <= 0)
		return;
	
	// Go through each of the objects to calculate their normals
	for(int index = 0; index < pModel->numOfObjects; index++)
	{
		// Get the current object
		t3DObject *pObject = &(pModel->pObject[index]);

		// Here we allocate all the memory we need to calculate the normals
		tVector3 *pNormals		= new tVector3 [pObject->numOfFaces];
		tVector3 *pTempNormals	= new tVector3 [pObject->numOfFaces];
		pObject->pNormals		= new tVector3 [pObject->numOfVerts];

		// Go though all of the faces of this object
		for(int i=0; i < pObject->numOfFaces; i++)
		{												
			// To cut down LARGE code, we extract the 3 points of this face
			vPoly[0] = pObject->pVerts[pObject->pFaces[i].vertIndex[0]];
			vPoly[1] = pObject->pVerts[pObject->pFaces[i].vertIndex[1]];
			vPoly[2] = pObject->pVerts[pObject->pFaces[i].vertIndex[2]];

			// Now let's calculate the face normals (Get 2 vectors and find the cross product of those 2)

			vVector1 = Vector(vPoly[0], vPoly[2]);		// Get the vector of the polygon (we just need 2 sides for the normal)
			vVector2 = Vector(vPoly[2], vPoly[1]);		// Get a second vector of the polygon

			vNormal  = cross(vVector1, vVector2);		// Return the cross product of the 2 vectors (normalize vector, but not a unit vector)
			pTempNormals[i] = vNormal;					// Save the un-normalized normal for the vertex normals
			vNormal  = normalize(vNormal);				// Normalize the cross product to give us the polygons normal
			
			pNormals[i] = vNormal;						// Assign the normal to the list of normals
		}

		//////////////// Now Get The Vertex Normals /////////////////

		tVector3 vSum = {0.0, 0.0, 0.0};
		tVector3 vZero = vSum;
		int shared=0;

		for (i = 0; i < pObject->numOfVerts; i++)			// Go through all of the vertices
		{
			for (int j = 0; j < pObject->numOfFaces; j++)	// Go through all of the triangles
			{												// Check if the vertex is shared by another face
				if (pObject->pFaces[j].vertIndex[0] == i || 
					pObject->pFaces[j].vertIndex[1] == i || 
					pObject->pFaces[j].vertIndex[2] == i)
				{
					vSum = addVector(vSum, pTempNormals[j]);// Add the un-normalized normal of the shared face
					shared++;								// Increase the number of shared triangles
				}
			}      
			
			// Get the normal by dividing the sum by the shared.  We negate the shared so it has the normals pointing out.
			pObject->pNormals[i] = divideVectorByScaler(vSum, float(-shared));

			// Normalize the normal for the final vertex normal
			pObject->pNormals[i] = normalize(pObject->pNormals[i]);	

			vSum = vZero;									// Reset the sum
			shared = 0;										// Reset the shared
		}
	
		// Free our memory and start over on the next object
		delete [] pTempNormals;
		delete [] pNormals;
	}
}




