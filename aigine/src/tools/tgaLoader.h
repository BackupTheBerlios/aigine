/**
 * Author: Michael Haller, haller@fhs-hagenberg.ac.at, 11/15/2001        
 * based on Nate Miller's C based TGA loader vandals1@home.com
 */

//liest .TGA und .BMP
 
#include <stdio.h>

#define TGA_FILE_NOT_FOUND          13 /* file was not found */
#define TGA_BAD_IMAGE_TYPE          14 /* color mapped image or image is not uncompressed */
#define TGA_BAD_DIMENSION			15 /* dimension is not a power of 2 */
#define TGA_BAD_BITS				16 /* image bits is not 8, 24 or 32 */
#define TGA_BAD_DATA				17 /* image data could not be loaded */

#define TGA_DEFAULT            0x0000000000000000   /* In case we don't want any parameters */
#define TGA_FREE               0x0000000000000001   /* Bit flag 0 */
#define TGA_NO_PASS            0x0000000000000010   /* Bit flag 1 */
#define TGA_ALPHA              0x0000000000000100   /* Bit flag 2 */
#define TGA_LUMINANCE          0x0000000000001000   /* Bit flag 3 */
#define TGA_NO_MIPMAPS         0x0000000000010000   /* Bit flag 4 */
#define TGA_LOW_QUALITY        0x0000000000100000   /* Bit flag 5 */
#define TGA_COMPRESS           0x0000000001000000   /* Bit flag 6 */

typedef unsigned int tgaFlag;

class TGAImage {

	public:
		unsigned char *imageData;
		int imageWidth, imageHeight;
		int imageBits, size;
		GLenum texFormat;
		GLuint textureID;

		TGAImage();
		~TGAImage();
		int loadTGA (char *name); 
		int loadAndBindTGA(char *name, GLenum mode);

	private:
		unsigned char *getRGBA ();
        unsigned char *getRGB ();
		unsigned char *getGray ();
		unsigned char *getData (int iBits);
		FILE *s;
};

