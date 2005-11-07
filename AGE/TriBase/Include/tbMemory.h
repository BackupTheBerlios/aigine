/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbMemory.h
	==========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Speicherverwaltung

	Autor:
	David Scherfgen

********************************************************************/

// Funktionen deklarieren
TRIBASE_API tbResult	tbMemInit();								// Initialisierung des Speichermanagers
TRIBASE_API tbResult	tbMemExit();								// Herunterfahren des Speichermanagers
TRIBASE_API void*		tbMemAlloc(int iSize);						// Speicher reservieren
TRIBASE_API void*		tbMemReAlloc(void* pMemory, int iNewSize);	// Speicherbereich vergrößern/verkleinern
TRIBASE_API tbResult	tbMemFree(void* pMemory);					// Speicherbereich freigeben
TRIBASE_API BOOL		tbMemIsValid(void* pMemory);				// Gültigkeit eines Speicherbereichs prüfen
TRIBASE_API int			tbMemGetSize(void* pMemory);				// Größe eines Speicherbereichs abfragen
TRIBASE_API tbResult	tbMemFreeAll();								// Alle Speicherbereiche freigeben

// ******************************************************************