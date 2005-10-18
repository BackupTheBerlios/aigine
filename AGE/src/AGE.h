// Allgemeine Makros und Definitionen
#define AGE_SAFE_DELETE(x)		{if((x) != NULL) {delete (x); (x) = NULL;}}		// Sicheres Löschen
#define AGE_SAFE_DELETE_ARRAY(x)	{if((x) != NULL) {delete[] (x); (x) = NULL;}}	// Sicheres Löschen eines Arrays
#define AGE_SAFE_RELEASE(x)		{if((x) != NULL) {(x)->Release(); (x) = NULL;}}	// Sicheres Releasen eines COM-Objekts
#define AGE_SAFE_MEMFREE(x)		{if((x) != NULL) {tbMemFree((x)); (x) = NULL;}}	// Sicheres Freigeben von Speicher
#define AGE_MIN(a, b)			((a) < (b) ? (a) : (b))							// Minimum
#define AGE_MAX(a, b)			((a) > (b) ? (a) : (b))							// Maximum
#define AGE_PI					(3.1415926535897932384626433832795f)			// Pi
#define AGE_DEG_TO_RAD(x)		((x) * 0.0174532925199432957692369076848f)		// Grad -> Bogenmaß
#define AGE_RAD_TO_DEG(x)		((x) * 57.295779513082320876798154814105f)		// Bogenmaß -> Grad

// Statuscodes
enum age_Result
{
	TB_OK			= 0,	// Erfolg
	TB_ERROR		= 1,	// Allgemeiner Fehler
	TB_CANCELED		= 2,	// Aktion abgebrochen
	TB_NOT_FOUND	= 3,	// Nicht gefunden
	TB_STOP			= 4,	// Stopp (Vorgang abbrechen)
};
