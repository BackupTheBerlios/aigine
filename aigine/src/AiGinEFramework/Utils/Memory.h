// Memory.h: interface for the Memory class.
//
//////////////////////////////////////////////////////////////////////

#ifndef MEMORY_H
#define MEMORY_H

// Funktionen deklarieren
Result		MemInit();									// Initialisierung des Speichermanagers
Result		MemExit();									// Herunterfahren des Speichermanagers
void*		MemAlloc(int iSize);						// Speicher reservieren
void*		MemReAlloc(void* pMemory, int iNewSize);	// Speicherbereich vergrößern/verkleinern
Result		MemFree(void* pMemory);						// Speicherbereich freigeben
BOOL		MemIsValid(void* pMemory);					// Gültigkeit eines Speicherbereichs prüfen
int			MemGetSize(void* pMemory);					// Größe eines Speicherbereichs abfragen
Result		MemFreeAll();								// Alle Speicherbereiche freigeben

#endif //MEMORY_H
