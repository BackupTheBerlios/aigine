#ifndef MAKROS_H
#define MAKROS_H


#ifndef BYTE
typedef unsigned char       BYTE;
#endif
#ifndef DWORD
typedef unsigned long       DWORD;
#endif

// Allgemeine Makros und Definitionen
#define SAFE_DELETE(x)		{if((x) != NULL) {delete (x); (x) = NULL;}}		// Sicheres Löschen
#define SAFE_DELETE_ARRAY(x)	{if((x) != NULL) {delete[] (x); (x) = NULL;}}	// Sicheres Löschen eines Arrays
#define SAFE_RELEASE(x)		{if((x) != NULL) {(x)->Release(); (x) = NULL;}}	// Sicheres Releasen eines COM-Objekts
#define SAFE_MEMFREE(x)		{if((x) != NULL) {MemFree((x)); (x) = NULL;}}	// Sicheres Freigeben von Speicher
#define MIN(a, b)			((a) < (b) ? (a) : (b))							// Minimum
#define MAX(a, b)			((a) > (b) ? (a) : (b))							// Maximum
#define PI					(3.1415926535897932384626433832795f)			// Pi
#define DEG_TO_RAD(x)		((x) * 0.0174532925199432957692369076848f)		// Grad -> Bogenmaß
#define RAD_TO_DEG(x)		((x) * 57.295779513082320876798154814105f)		// Bogenmaß -> Grad
#define COLOR_CONV (0.003921568627450980392156862745098f) // = 1/255

/*
enum Result {
	OK			= 0,	// Erfolg
	ERROR		= 1,	// Allgemeiner Fehler
	CANCELED	= 2,	// Aktion abgebrochen
	NOT_FOUND	= 3,	// Nicht gefunden
	STOP		= 4,	// Stopp (Vorgang abbrechen)
};
*/
/*
inline float FloatRandom(const float fMin, const float fMax) {
	return fMin + (fMax - fMin) * ((float)(rand() % 10001) / 10000.0f);
};
*/

#endif //MAKROS_H
