// Memory.cpp: implementation of the Memory class.
//
//////////////////////////////////////////////////////////////////////

#include "../AiGinE.h"

// ******************************************************************
// Interne Struktur für einen Speicherbereich
struct MemListEntry
{
	void*	pMemory;		// Zeiger auf den gesamten Speicherbereich
	void*	pUserMemory;	// Zeiger auf den Benutzerspeicher
	int		iSize;			// Größe des Speicherbereichs
	char	acCheck[8];		// Checkcode (= "TriBase")
};

// ******************************************************************
// Interne Variablen
List<MemListEntry>*	g_pMemoryList = NULL;	// Speicherliste
int						g_iByteCounter = 0;		// Speicherzähler

// ******************************************************************
// Interne Funktion zum Abfragen der Datenstruktur eines Speicherbereichs
Result MemGetEntry(void* pMemory,
					 ListEntry<MemListEntry>** ppOut)
{
	// Parameter prüfen
	if(pMemory == NULL)	AGE_ERROR_NULL_POINTER("pMemory", AGE_ERROR);
	if(ppOut == NULL)	AGE_ERROR_NULL_POINTER("ppOut", AGE_ERROR);

	
	// Speicherbereich prüfen
	if(!MemIsValid(pMemory)) return AGE_ERROR;

	// Vom angegebenen Zeiger muss die Größe eines Zeigers auf eine
	// MemListEntry-Struktur abgezogen werden.
	pMemory = (BYTE*)(pMemory) - sizeof(ListEntry<MemListEntry>*);

	// pMemory zeigt jetzt auf einen Zeiger auf die zum Speicher
	// gehörige Listenstruktur - dieser wird geliefert.
	*ppOut = *((ListEntry<MemListEntry>**)(pMemory));

	return AGE_OK;
}

// ******************************************************************
// Interne Funktion zum Hinzufügen eines Speicherbereichs zur Liste
Result MemAddToList(void* pMemory,
					  int iSize)
{
	MemListEntry					NewEntry;				// Neuer Listeneintrag
	ListEntry<MemListEntry>*	pNewMemoryListEntry;	// Neuer Speicherlisteneintrag

	
	// Parameter prüfen
	if(pMemory == NULL)	AGE_ERROR_NULL_POINTER("pMemory", AGE_ERROR);
	if(iSize < 0)		AGE_ERROR_INVALID_VALUE("iSize", AGE_ERROR);


	// Neuen Listeneintrag ausfüllen
	NewEntry.pMemory = pMemory;
	NewEntry.pUserMemory = (BYTE*)(pMemory) + sizeof(ListEntry<MemListEntry>*);
	NewEntry.iSize = iSize;
	strcpy(NewEntry.acCheck, "TriBase");

	// Eintrag zur Speicherliste hinzufügen
	pNewMemoryListEntry = g_pMemoryList->AddEntry(&NewEntry);
	if(pNewMemoryListEntry == NULL) AGE_ERROR("Fehler beim Erzeugen des neuen Listeneintrags!", AGE_ERROR);

	// Zeiger auf den Speicherlisteneintrag in den Speicher schreiben
	*((ListEntry<MemListEntry>**)(pMemory)) = pNewMemoryListEntry;

	return AGE_OK;
}

// ******************************************************************
// Initialisieren des Speichermanagers
TRIBASE_API Result MemInit()
{
	// Speicherliste erstellen
	g_pMemoryList = new List<MemListEntry>;
	if(g_pMemoryList == NULL) AGE_ERROR_OUT_OF_MEMORY(AGE_ERROR);
	
	return AGE_OK;
}

// ******************************************************************
// Herunterfahren des Speichermanagers
TRIBASE_API Result MemExit()
{
	// Alle Speicherbereiche löschen
	MemFreeAll();

	// Liste löschen
	AGE_SAFE_DELETE(g_pMemoryList);

	// Fertig! Statusbericht schreiben.
	if(g_iByteCounter > 0) AGE_INFO("Nicht alle Speicherbereiche wurden freigegeben!")
	else AGE_INFO("Alle Speicherbereiche freigegeben!");

	return AGE_OK;
}

// ******************************************************************
// Funktion zum Reservieren von Speicher
TRIBASE_API void* MemAlloc(int iSize)
{
	void* pMemory;

	// Parameter prüfen
	if(iSize < 0) AGE_ERROR_INVALID_VALUE("iSize", NULL);


	// Speicher reservieren und leeren. Beim Reservieren werden ein
	// paar Bytes mehr gebraucht, damit der Zeiger auf den Listeneintrag
	// mitgespeichert werden kann.
	pMemory = ::operator new(iSize + sizeof(ListEntry<MemListEntry>*));
	if(pMemory == NULL) return NULL;

	g_iByteCounter += iSize + sizeof(ListEntry<MemListEntry>*);
	ZeroMemory(pMemory, iSize + sizeof(ListEntry<MemListEntry>*));

	// Zur Liste hinzufügen
	if(MemAddToList(pMemory, iSize)) AGE_ERROR("Hinzufügen zur Liste fehlgeschlagen!", NULL);

	// Zeiger zurückliefern
	return (BYTE*)(pMemory) + sizeof(ListEntry<MemListEntry>*);
}

// ******************************************************************
// Funktion zum Vergrößern eines Speicherbereichs
TRIBASE_API void* MemReAlloc(void* pMemory,
							   int iNewSize)
{
	int		iSize;
	void*	pNewMemory;


	// Größe des Speicherbereichs abfragen
	iSize = MemGetSize(pMemory);
	if(iSize == -1) return MemAlloc(abs(iNewSize));

	// Bei negativer neuer Größe wird der Speicherbereich vergrößert.
	if(iNewSize < 0) iNewSize = iSize - iNewSize;

	// Neuen Speicherbereich reservieren
	pNewMemory = MemAlloc(iNewSize);
	if(pNewMemory == NULL) AGE_ERROR_OUT_OF_MEMORY(NULL);

	// Speicher hinüberkopieren und den alten Speicherbereich löschen
	memcpy(pNewMemory, pMemory, AGE_MIN(iSize, iNewSize));
	AGE_SAFE_MEMFREE(pMemory);

	// Neuen Zeiger liefern
	return pNewMemory;
}

// ******************************************************************
// Freigeben eines Speicherbereichs
TRIBASE_API Result MemFree(void* pMemory)
{
	ListEntry<MemListEntry>*	pMemoryListEntry;
	MemListEntry					Entry;


	// Parameter prüfen
	if(pMemory == NULL) AGE_ERROR_NULL_POINTER("pMemory", AGE_ERROR);


	// Listeneintrag abfragen
	if(MemGetEntry(pMemory, &pMemoryListEntry))
	{
		AGE_ERROR("Ungültiger Speicherbereich angegeben!", AGE_ERROR);
	}

	// Speicher und Listeneintrag löschen
	Entry = pMemoryListEntry->Data;
	g_iByteCounter -= Entry.iSize + sizeof(ListEntry<MemListEntry>*);
	::operator delete(Entry.pMemory);
	g_pMemoryList->DeleteEntry(pMemoryListEntry);

	return AGE_OK;
}

// ******************************************************************
// Prüfen der Gültigkeit eines Speicherbereichs
TRIBASE_API BOOL MemIsValid(void* pMemory)
{
	ListEntry<MemListEntry>* pEntry;

	// Nullzeiger gelten als ungültig!
	if(pMemory == NULL) return FALSE;


	// Zeiger auf die Struktur des Listeneintrags abfragen. Wenn
	// in der Struktur ein Nullzeiger ist, ist der Bereich ungültig.
	pEntry = *((ListEntry<MemListEntry>**)((BYTE*)(pMemory) - sizeof(ListEntry<MemListEntry>*)));
	if(pEntry->Data.pMemory == NULL || pEntry->Data.pUserMemory == NULL) return FALSE;

	// Größe und "TriBase"-Signatur prüfen
	if(pEntry->Data.iSize < 0) return FALSE;
	if(strncmp(pEntry->Data.acCheck, "TriBase", 8)) return FALSE;

	// Der Speicherbereich scheint in Ordnung zu sein.
	return TRUE;
}

// ******************************************************************
// Abfragen der Größe eines Speicherbereichs
TRIBASE_API int MemGetSize(void* pMemory)
{
	ListEntry<MemListEntry>* pMemoryListEntry;

	// Parameter prüfen
	if(pMemory == NULL) return -1;

	// Listeneintrag abfragen
	if(MemGetEntry(pMemory, &pMemoryListEntry))
	{
		AGE_ERROR("FEHLER in MemGetSize: Ungültiger Speicherbereich angegeben!", -1);
	}

	// Größe liefern
	return pMemoryListEntry->Data.iSize;
}

// ******************************************************************
// Löschen aller Speicherbereiche
TRIBASE_API Result MemFreeAll()
{
	ListEntry<MemListEntry>* pEntry;

	// pEntry zeigt zuerst auf den ersten Eintrag.
	pEntry = g_pMemoryList->GetFirstEntry();

	// Die gesamte Liste durchgehen, bis das Ende erreicht ist
	while(pEntry != NULL)
	{
		// Aktuellen Speicherbereich löschen
		MemFree(pEntry->Data.pUserMemory);

		// Neuen ersten Eintrag abfragen
		pEntry = g_pMemoryList->GetFirstEntry();
	}

	return AGE_OK;
}
