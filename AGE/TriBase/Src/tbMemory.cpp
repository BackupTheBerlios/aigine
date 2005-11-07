/********************************************************************
	 _____        _    ___
	/\__  __\      /\_\  /\  _\
	\/_/\ \_/ _  _\/_/_ \ \ \_\\   __    ___      _
	    \ \ \  /\`�_\ /\ \ \ \  _\ /\ _\_ /\  _\   /�_`\
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
// Interne Struktur f�r einen Speicherbereich
struct tbMemListEntry
{
	void*	pMemory;		// Zeiger auf den gesamten Speicherbereich
	void*	pUserMemory;	// Zeiger auf den Benutzerspeicher
	int		iSize;			// Gr��e des Speicherbereichs
	char	acCheck[8];		// Checkcode (= "TriBase")
};

// ******************************************************************
// Interne Variablen
tbList<tbMemListEntry>*	g_pMemoryList = NULL;	// Speicherliste
int						g_iByteCounter = 0;		// Speicherz�hler

// ******************************************************************
// Interne Funktion zum Abfragen der Datenstruktur eines Speicherbereichs
tbResult MemGetEntry(void* pMemory,
					 tbListEntry<tbMemListEntry>** ppOut)
{
	// Parameter pr�fen
	if(pMemory == NULL)	TB_ERROR_NULL_POINTER("pMemory", TB_ERROR);
	if(ppOut == NULL)	TB_ERROR_NULL_POINTER("ppOut", TB_ERROR);

	
	// Speicherbereich pr�fen
	if(!tbMemIsValid(pMemory)) return TB_ERROR;

	// Vom angegebenen Zeiger muss die Gr��e eines Zeigers auf eine
	// tbMemListEntry-Struktur abgezogen werden.
	pMemory = (BYTE*)(pMemory) - sizeof(tbListEntry<tbMemListEntry>*);

	// pMemory zeigt jetzt auf einen Zeiger auf die zum Speicher
	// geh�rige Listenstruktur - dieser wird geliefert.
	*ppOut = *((tbListEntry<tbMemListEntry>**)(pMemory));

	return TB_OK;
}

// ******************************************************************
// Interne Funktion zum Hinzuf�gen eines Speicherbereichs zur Liste
tbResult MemAddToList(void* pMemory,
					  int iSize)
{
	tbMemListEntry					NewEntry;				// Neuer Listeneintrag
	tbListEntry<tbMemListEntry>*	pNewMemoryListEntry;	// Neuer Speicherlisteneintrag

	
	// Parameter pr�fen
	if(pMemory == NULL)	TB_ERROR_NULL_POINTER("pMemory", TB_ERROR);
	if(iSize < 0)		TB_ERROR_INVALID_VALUE("iSize", TB_ERROR);


	// Neuen Listeneintrag ausf�llen
	NewEntry.pMemory = pMemory;
	NewEntry.pUserMemory = (BYTE*)(pMemory) + sizeof(tbListEntry<tbMemListEntry>*);
	NewEntry.iSize = iSize;
	strcpy(NewEntry.acCheck, "TriBase");

	// Eintrag zur Speicherliste hinzuf�gen
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
	// Alle Speicherbereiche l�schen
	tbMemFreeAll();

	// Liste l�schen
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

	// Parameter pr�fen
	if(iSize < 0) TB_ERROR_INVALID_VALUE("iSize", NULL);


	// Speicher reservieren und leeren. Beim Reservieren werden ein
	// paar Bytes mehr gebraucht, damit der Zeiger auf den Listeneintrag
	// mitgespeichert werden kann.
	pMemory = ::operator new(iSize + sizeof(tbListEntry<tbMemListEntry>*));
	if(pMemory == NULL) return NULL;

	g_iByteCounter += iSize + sizeof(tbListEntry<tbMemListEntry>*);
	ZeroMemory(pMemory, iSize + sizeof(tbListEntry<tbMemListEntry>*));

	// Zur Liste hinzuf�gen
	if(MemAddToList(pMemory, iSize)) TB_ERROR("Hinzuf�gen zur Liste fehlgeschlagen!", NULL);

	// Zeiger zur�ckliefern
	return (BYTE*)(pMemory) + sizeof(tbListEntry<tbMemListEntry>*);
}

// ******************************************************************
// Funktion zum Vergr��ern eines Speicherbereichs
TRIBASE_API void* tbMemReAlloc(void* pMemory,
							   int iNewSize)
{
	int		iSize;
	void*	pNewMemory;


	// Gr��e des Speicherbereichs abfragen
	iSize = tbMemGetSize(pMemory);
	if(iSize == -1) return tbMemAlloc(abs(iNewSize));

	// Bei negativer neuer Gr��e wird der Speicherbereich vergr��ert.
	if(iNewSize < 0) iNewSize = iSize - iNewSize;

	// Neuen Speicherbereich reservieren
	pNewMemory = tbMemAlloc(iNewSize);
	if(pNewMemory == NULL) TB_ERROR_OUT_OF_MEMORY(NULL);

	// Speicher hin�berkopieren und den alten Speicherbereich l�schen
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


	// Parameter pr�fen
	if(pMemory == NULL) TB_ERROR_NULL_POINTER("pMemory", TB_ERROR);


	// Listeneintrag abfragen
	if(MemGetEntry(pMemory, &pMemoryListEntry))
	{
		TB_ERROR("Ung�ltiger Speicherbereich angegeben!", TB_ERROR);
	}

	// Speicher und Listeneintrag l�schen
	Entry = pMemoryListEntry->Data;
	g_iByteCounter -= Entry.iSize + sizeof(tbListEntry<tbMemListEntry>*);
	::operator delete(Entry.pMemory);
	g_pMemoryList->DeleteEntry(pMemoryListEntry);

	return TB_OK;
}

// ******************************************************************
// Pr�fen der G�ltigkeit eines Speicherbereichs
TRIBASE_API BOOL tbMemIsValid(void* pMemory)
{
	tbListEntry<tbMemListEntry>* pEntry;

	// Nullzeiger gelten als ung�ltig!
	if(pMemory == NULL) return FALSE;


	// Zeiger auf die Struktur des Listeneintrags abfragen. Wenn
	// in der Struktur ein Nullzeiger ist, ist der Bereich ung�ltig.
	pEntry = *((tbListEntry<tbMemListEntry>**)((BYTE*)(pMemory) - sizeof(tbListEntry<tbMemListEntry>*)));
	if(pEntry->Data.pMemory == NULL || pEntry->Data.pUserMemory == NULL) return FALSE;

	// Gr��e und "TriBase"-Signatur pr�fen
	if(pEntry->Data.iSize < 0) return FALSE;
	if(strncmp(pEntry->Data.acCheck, "TriBase", 8)) return FALSE;

	// Der Speicherbereich scheint in Ordnung zu sein.
	return TRUE;
}

// ******************************************************************
// Abfragen der Gr��e eines Speicherbereichs
TRIBASE_API int tbMemGetSize(void* pMemory)
{
	tbListEntry<tbMemListEntry>* pMemoryListEntry;

	// Parameter pr�fen
	if(pMemory == NULL) return -1;

	// Listeneintrag abfragen
	if(MemGetEntry(pMemory, &pMemoryListEntry))
	{
		TB_ERROR("FEHLER in tbMemGetSize: Ung�ltiger Speicherbereich angegeben!", -1);
	}

	// Gr��e liefern
	return pMemoryListEntry->Data.iSize;
}

// ******************************************************************
// L�schen aller Speicherbereiche
TRIBASE_API tbResult tbMemFreeAll()
{
	tbListEntry<tbMemListEntry>* pEntry;

	// pEntry zeigt zuerst auf den ersten Eintrag.
	pEntry = g_pMemoryList->GetFirstEntry();

	// Die gesamte Liste durchgehen, bis das Ende erreicht ist
	while(pEntry != NULL)
	{
		// Aktuellen Speicherbereich l�schen
		tbMemFree(pEntry->Data.pUserMemory);

		// Neuen ersten Eintrag abfragen
		pEntry = g_pMemoryList->GetFirstEntry();
	}

	return TB_OK;
}

// ******************************************************************