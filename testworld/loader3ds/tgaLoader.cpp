#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
#include <windows.h>
#endif

#include <GL/glut.h>
#include "tgaLoader.h"

//GLenum texFormat;


/**
 * Make sure its a power of 2.
 */
int checkSize (int x) {
    if (x == 2	 || x == 4 || 
        x == 8	 || x == 16 || 
        x == 32  || x == 64 ||
        x == 128 || x == 256 || x == 512)
        return 1;
    else return 0;
}

/** 
 * Reads in RGBA data for a 32bit image. 
 */
unsigned char * 
TGAImage::getRGBA () {
    unsigned char *rgba;
    unsigned char temp;
    int bread;
    int i;

    rgba = (unsigned char*) malloc (size * 4); 

    if (rgba == NULL)
        return 0;

    bread = fread (rgba, sizeof (unsigned char), size * 4, s); 

    
    /* TGA is stored in BGRA, make it RGBA */
    if (bread != size * 4) {
        free (rgba);
        return 0;
    }

    for (i = 0; i < size * 4; i += 4 ) {
        temp = rgba[i];
        rgba[i] = rgba[i + 2];
        rgba[i + 2] = temp;
    }


    texFormat = GL_RGBA;
    return rgba;
}

/**
 * Reads in RGB data for a 24bit image. 
 */
unsigned char *
TGAImage::getRGB () {
    unsigned char *rgb;
    unsigned char temp;
    int bread;
    int i;

    rgb = (unsigned char*) malloc (size * 3); 

    if (rgb == NULL)
        return 0;

    bread = fread (rgb, sizeof (unsigned char), size * 3, s);

    if (bread != size * 3) {
        free (rgb);
        return 0;
    }

    // TGA is stored in BGR, make it RGB 
    for (i = 0; i < size * 3; i += 3) {
        temp = rgb[i];
        rgb[i] = rgb[i + 2];
        rgb[i + 2] = temp;
    }

    texFormat = GL_RGB;

    return rgb;
}


/**
 * Gets the grayscale image data.  Used as an alpha channel.
 */
unsigned char *
TGAImage::getGray () {
    unsigned char *grayData;
    int bread;

    grayData = (unsigned char*) malloc (size);

    if (grayData == NULL)
        return 0;

    bread = fread (grayData, sizeof (unsigned char), size, s);

    if (bread != size) {
        free (grayData);
        return 0;
    }

    //texFormat = GL_ALPHA;

    return grayData;
}


/**
 * Gets the image data for the specified bit depth.
 */
unsigned char *
TGAImage::getData (int iBits) {
    if (iBits == 32)
        return getRGBA ();
    else if (iBits == 24)
        return getRGB ();	
    else if (iBits == 8)
        return getGray ();
}


/**
 * Called when there is an error loading the .tga file.
 */
int returnError (int error) {
    //fclose (s);
    return error;
}


TGAImage::TGAImage() {
}


TGAImage::~TGAImage() {
	free (imageData);
}

/**
 * Loads up a targa file.  Supported types are 8,24 and 32 uncompressed images.  
 * id is the texture ID to bind too.
 * Textures have to be exported with Photoshop and 32 bits/pixel
 */
int 
TGAImage::loadTGA (char *name) {
    unsigned char type[4];
    unsigned char info[7];
   
    if (!(s = fopen (name, "rb")))
        return TGA_FILE_NOT_FOUND;

    fread (&type, sizeof (char), 3, s); 
    fseek (s, 12, SEEK_SET);		
    fread (&info, sizeof (char), 6, s);

    if (type[1] != 0 || (type[2] != 2 && type[2] != 3))
        returnError (TGA_BAD_IMAGE_TYPE);

    imageWidth = info[0] + info[1] * 256; 
    imageHeight = info[2] + info[3] * 256;
    imageBits =	info[4]; 

    size = imageWidth * imageHeight; 

    /* make sure dimension is a power of 2 */
    if (!checkSize (imageWidth) || !checkSize (imageHeight))
        returnError (TGA_BAD_DIMENSION);

    /* make sure we are loading a supported type */
    if (imageBits != 32 && imageBits != 24 && imageBits != 8)
        returnError (TGA_BAD_BITS);
	
    imageData = getData (imageBits);

    /* no image data */
    if (imageData == NULL)
        returnError (TGA_BAD_DATA);

    fclose (s);  

    /* release data, its been uploaded */
    /* free (imageData);*/

    return 1;
}


int 
TGAImage::loadAndBindTGA(char *name, GLenum mode) {
	loadTGA(name);	

	glGenTextures ( 1, &textureID );
	glBindTexture (GL_TEXTURE_2D, textureID);

	GLenum internal_format;

    if (mode == GL_LUMINANCE4) {		
		texFormat = GL_LUMINANCE4;
	}
	
    glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	

	// glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);	
    // glTexImage2D (GL_TEXTURE_2D, 0, texFormat, imageWidth, imageHeight, 0, texFormat, GL_UNSIGNED_BYTE, imageData);			

	gluBuild2DMipmaps(GL_TEXTURE_2D, 
					  texFormat,
					  imageWidth,
					  imageHeight,
					  texFormat,
					  GL_UNSIGNED_BYTE, imageData);
	return textureID;
}



