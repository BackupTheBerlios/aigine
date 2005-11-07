/********************************************************************
	 _____        _    ___
	/\__  __\      /\_\  /\  _\
	\/_/\ \_/ _  _\/_/_ \ \ \_\\   __    ___      _
	    \ \ \  /\`´_\ /\ \ \ \  _\ /\ _\_ /\  _\   /´_`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \__\ /\  _/
	      \ \_\ \ \_\   \ \_\ \ \__\\ \__\ \ \__\\ \__\
	       \/_/  \/_/    \/_/  \/__/ \/_/   \/__/ \/__/

	tbMemory.cpp
	============
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Speicherverwaltung

	Autor:
	David Scherfgen

********************************************************************/

#include <TriBase.h>

// ******************************************************************
// Interne Struktur für einen Speicherbereich
struct tbMemListEntry
{
	void*	pMemory;		// Zeiger auf den gesamten Speicherbereich
	void*	pUserMemory;	// Zeiger auf den Benutzerspeicher
	int		iSize;			// Größe des Speicherbereichs
	char	acCheck[8];		// Checkcode (= "TriBase")
};

// ******************************************************************
// Interne Variablen
tbList<tbMemListEntry>*	g_pMemoryList = NULL;	// Speicherliste
int						g_iByteCounter = 0;		// Speicherzähler

// ******************************************************************
// Interne Funktion zum Abfragen der Datenstruktur eines Speicherbereichs
tbResult MemGetEntry(void* pMemory,
					 tbListEntry<tbMemListEntry>** ppOut)
{
	// Parameter prüfen
	if(pMemory == NULL)	TB_ERROR_NULL_POINTER("pMemory", TB_ERROR);
	if(ppOut == NULL)	TB_ERROR_NULL_POINTER("ppOut", TB_ERROR);

	
	// Speicherbereich prüfen
	if(!tbMemIsValid(pMemory)) return TB_ERROR;

	// Vom angegebenen Zeiger muss die Größe eines Zeigers auf eine
	// tbMemListEntry-Struktur abgezogen werden.
	pMemory = (BYTE*)(pMemory) - sizeof(tbListEntry<tbMemListEntry>*);

	// pMemory zeigt jetzt auf einen Zeiger auf die zum Speicher
	// gehörige Listenstruktur - dieser wird geliefert.
	*ppOut = *((tbListEntry<tbMemListEntry>**)(pMemory));

	return TB_OK;
}

// ******************************************************************
// Interne Funktion zum Hinzufügen eines Speicherbereichs zur Liste
tbResult MemAddToList(void* pMemory,
					  int iSize)
{
	tbMemListEntry					NewEntry;				// Neuer Listeneintrag
	tbListEntry<tbMemListEntry>*	pNewMemoryListEntry;	// Neuer Speicherlisteneintrag

	
	// Parameter prüfen
	if(pMemory == NULL)	TB_ERROR_NULL_POINTER("pMemory", TB_ERROR);
	if(iSize < 0)		TB_ERROR_INVALID_VALUE("iSize", TB_ERROR);


	// Neuen Listeneintrag ausfüllen
	NewEntry.pMemory = pMemory;
	NewEntry.pUserMemory = (BYTE*)(pMemory) + sizeof(tbListEntry<tbMemListEntry>*);
	NewEntry.iSize = iSize;
	strcpy(NewEntry.acCheck, "TriBase");

	// Eintrag zur Speicherliste hinzufügen
	pNewMemoryListEntry = g_pMemoryList->AddEntry(&NewEntry);
	if(pNewMemoryListEntry == NULL) TB_ERROR("Fehler beim Erzeugen des neuen Listeneintrags!", TB_ERROR);

	// Zeiger auf den Speicherlisteneintrag in den Speicher schreiben
	*((tbListEntry<tbMemListEntry>**)(pMemory)) = pNewMemoryListEntry;

	return TB_OK;
}

// ******************************************************************
// Initialisieren des Speichermanagers
TRIBASE_API tbResult tbMemInit()
{
	// Speicherliste erstellen
	g_pMemoryList = new tbList<tbMemListEntry>;
	if(g_pMemoryList == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);
	
	return TB_OK;
}

// ******************************************************************
// Herunterfahren des Speichermanagers
TRIBASE_API tbResult tbMemExit()
{
	// Alle Speicherbereiche löschen
	tbMemFreeAll();

	// Liste löschen
	TB_SAFE_DELETE(g_pMemoryList);

	// Fertig! Statusbericht schreiben.
	if(g_iByteCounter > 0) TB_INFO("Nicht alle Speicherbereiche wurden freigegeben!")
	else TB_INFO("Alle Speicherbereiche freigegeben!");

	return TB_OK;
}

// ******************************************************************
// Funktion zum Reservieren von Speicher
TRIBASE_API void* tbMemAlloc(int iSize)
{
	void* pMemory;

	// Parameter prüfen
	if(iSize < 0) TB_ERROR_INVALID_VALUE("iSize", NULL);


	// Speicher reservieren und leeren. Beim Reservieren werden ein
	// paar Bytes mehr gebraucht, damit der Zeiger auf den Listeneintrag
	// mitgespeichert werden kann.
	pMemory = ::operator new(iSize + sizeof(tbListEntry<tbMemListEntry>*));
	if(pMemory == NULL) return NULL;

	g_iByteCounter += iSize + sizeof(tbListEntry<tbMemListEntry>*);
	ZeroMemory(pMemory, iSize + sizeof(tbListEntry<tbMemListEntry>*));

	// Zur Liste hinzufügen
	if(MemAddToList(pMemory, iSize)) TB_ERROR("Hinzufügen zur Liste fehlgeschlagen!", NULL);

	// Zeiger zurückliefern
	return (BYTE*)(pMemory) + sizeof(tbListEntry<tbMemListEntry>*);
}

// ******************************************************************
// Funktion zum Vergrößern eines Speicherbereichs
TRIBASE_API void* tbMemReAlloc(void* pMemory,
							   int iNewSize)
{
	int		iSize;
	void*	pNewMemory;


	// Größe des Speicherbereichs abfragen
	iSize = tbMemGetSize(pMemory);
	if(iSize == -1) return tbMemAlloc(abs(iNewSize));

	// Bei negativer neuer Größe wird der Speicherbereich vergrößert.
	if(iNewSize < 0) iNewSize = iSize - iNewSize;

	// Neuen Speicherbereich reservieren
	pNewMemory = tbMemAlloc(iNewSize);
	if(pNewMemory == NULL) TB_ERROR_OUT_OF_MEMORY(NULL);

	// Speicher hinüberkopieren und den alten Speicherbereich löschen
	memcpy(pNewMemory, pMemory, TB_MIN(iSize, iNewSize));
	TB_SAFE_MEMFREE(pMemory);

	// Neuen Zeiger liefern
	return pNewMemory;
}

// ******************************************************************
// Freigeben eines Speicherbereichs
TRIBASE_API tbResult tbMemFree(void* pMemory)
{
	tbListEntry<tbMemListEntry>*	pMemoryListEntry;
	tbMemListEntry					Entry;


	// Parameter prüfen
	if(pMemory == NULL) TB_ERROR_NULL_POINTER("pMemory", TB_ERROR);


	// Listeneintrag abfragen
	if(MemGetEntry(pMemory, &pMemoryListEntry))
	{
		TB_ERROR("Ungültiger Speicherbereich angegeben!", TB_ERROR);
	}

	// Speicher und Listeneintrag löschen
	Entry = pMemoryListEntry->Data;
	g_iByteCounter -= Entry.iSize + sizeof(tbListEntry<tbMemListEntry>*);
	::operator delete(Entry.pMemory);
	g_pMemoryList->DeleteEntry(pMemoryListEntry);

	return TB_OK;
}

// ******************************************************************
// Prüfen der Gültigkeit eines Speicherbereichs
TRIBASE_API BOOL tbMemIsValid(void* pMemory)
{
	tbListEntry<tbMemListEntry>* pEntry;

	// Nullzeiger gelten als ungültig!
	if(pMemory == NULL) return FALSE;


	// Zeiger auf die Struktur des Listeneintrags abfragen. Wenn
	// in der Struktur ein Nullzeiger ist, ist der Bereich ungültig.
	pEntry = *((tbListEntry<tbMemListEntry>**)((BYTE*)(pMemory) - sizeof(tbListEntry<tbMemListEntry>*)));
	if(pEntry->Data.pMemory == NULL || pEntry->Data.pUserMemory == NULL) return FALSE;

	// Größe und "TriBase"-Signatur prüfen
	if(pEntry->Data.iSize < 0) return FALSE;
	if(strncmp(pEntry->Data.acCheck, "TriBase", 8)) return FALSE;

	// Der Speicherbereich scheint in Ordnung zu sein.
	return TRUE;
}

// ******************************************************************
// Abfragen der Größe eines Speicherbereichs
TRIBASE_API int tbMemGetSize(void* pMemory)
{
	tbListEntry<tbMemListEntry>* pMemoryListEntry;

	// Parameter prüfen
	if(pMemory == NULL) return -1;

	// Listeneintrag abfragen
	if(MemGetEntry(pMemory, &pMemoryListEntry))
	{
		TB_ERROR("FEHLER in tbMemGetSize: Ungültiger Speicherbereich angegeben!", -1);
	}

	// Größe liefern
	return pMemoryListEntry->Data.iSize;
}

// ******************************************************************
// Löschen aller Speicherbereiche
TRIBASE_API tbResult tbMemFreeAll()
{
	tbListEntry<tbMemListEntry>* pEntry;

	// pEntry zeigt zuerst auf den ersten Eintrag.
	pEntry = g_pMemoryList->GetFirstEntry();

	// Die gesamte Liste durchgehen, bis das Ende erreicht ist
	while(pEntry != NULL)
	{
		// Aktuellen Speicherbereich löschen
		tbMemFree(pEntry->Data.pUserMemory);

		// Neuen ersten Eintrag abfragen
		pEntry = g_pMemoryList->GetFirstEntry();
	}

	return TB_OK;
}

// ******************************************************************