// Memory.h: interface for the Memory class.
//
//////////////////////////////////////////////////////////////////////

#ifndef MEMORY_H
#define MEMORY_H

// Funktionen deklarieren
Result		MemInit();									// Initialisierung des Speichermanagers
Result		MemExit();									// Herunterfahren des Speichermanagers
void*		MemAlloc(int iSize);						// Speicher reservieren
void*		MemReAlloc(void* pMemory, int iNewSize);	// Speicherbereich vergr��ern/verkleinern
Result		MemFree(void* pMemory);						// Speicherbereich freigeben
BOOL		MemIsValid(void* pMemory);					// G�ltigkeit eines Speicherbereichs pr�fen
int			MemGetSize(void* pMemory);					// Gr��e eines Speicherbereichs abfragen
Result		MemFreeAll();								// Alle Speicherbereiche freigeben

#endif //MEMORY_H
