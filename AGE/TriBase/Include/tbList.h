/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbList.h
	========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Umgang mit verketteten Listen aller möglichen Typen

	Autor:
	David Scherfgen

********************************************************************/

// Template-Struktur für einen Eintrag einer verketteten Liste
template <typename Type> struct tbListEntry
{
	tbListEntry<Type>*	pPrevEntry;	// Zeiger auf den vorherigen Eintrag
	tbListEntry<Type>*	pNextEntry;	// Zeiger auf den nächsten Eintrag
	Type				Data;		// Die eigentlichen Listendaten
};

// ******************************************************************
// Template-Klasse für eine verkettete Liste
template <typename Type> class tbList
{
private:
	// Variablen
	tbListEntry<Type>*	m_pFirstEntry;	// Erster Listeneintrag
	tbListEntry<Type>*	m_pLastEntry;	// Letzter Listeneintrag
	int					m_iNumEntries;	// Anzahl der Listeneinträge

public:
	// Konstruktur und Destruktor
	tbList();
	~tbList();

	// Methoden
	tbListEntry<Type>*	AddEntry(Type* pData);									// Eintrag hinzufügen
	tbResult			FindEntry(Type* pData, tbListEntry<Type>** ppOut);		// Eintrag suchen
	tbResult			DeleteEntry(tbListEntry<Type>* pEntry);					// Eintrag löschen
	tbResult			Clear();												// Liste leeren
	tbResult			Traverse(tbResult (* pCallback)(tbListEntry<Type>*));	// Liste durchlaufen

	// Inline-Methoden
	inline Type*				GetEntryData(tbListEntry<Type>* pEntry)	{return &pEntry->Data;}
	inline tbListEntry<Type>*	GetFirstEntry()							{return m_pFirstEntry;}
	inline tbListEntry<Type>*	GetLastEntry()							{return m_pLastEntry;}
	inline int					GetNumEntries()							{return m_iNumEntries;}
};

// ******************************************************************
// Konstruktor der tbList-Klasse
template <typename Type> tbList<Type>::tbList()
{
	// Alles zurücksetzen
	ZeroMemory(this, sizeof(tbList<Type>));
}

// ******************************************************************
// Destruktor der tbList-Klasse
template <typename Type> tbList<Type>::~tbList()
{
	// Liste leeren
	Clear();
}

// ******************************************************************
// Diese Methode fügt einen neuen Eintrag zur Liste hinzu.
template <typename Type> tbListEntry<Type>* tbList<Type>::AddEntry(Type* pData)
{
	tbListEntry<Type>* pNewEntry;

	// Parameter prüfen
	if(pData == NULL) TB_ERROR_NULL_POINTER("pData", NULL);


	// Speicher für die Listeneintragsstruktur reservieren
	pNewEntry = new tbListEntry<Type>;
	if(pNewEntry == NULL) TB_ERROR_OUT_OF_MEMORY(NULL);

	// Der neue Eintrag steht an letzter Stelle der Liste.
	// Daher gibt es keinen nächsten Eintrag und der vorherige Eintrag
	// ist der ursprüngliche letzte Listeneintrag.
	pNewEntry->pPrevEntry = m_pLastEntry;
	if(m_pLastEntry != NULL) m_pLastEntry->pNextEntry = pNewEntry;
	pNewEntry->pNextEntry = NULL;
	m_pLastEntry = pNewEntry;

	// Wenn die Liste noch ganz leer ist, dann ist dieser Eintrag auch
	// gleichzeitig der erste Eintrag.
	if(m_pFirstEntry == NULL) m_pFirstEntry = pNewEntry;

	// Daten kopieren
	memcpy(&pNewEntry->Data, pData, sizeof(Type));

	// Eintragszähler erhöhen
	m_iNumEntries++;

	// Zeiger auf die Listeneintragsstruktur zurückliefern
	return pNewEntry;
}

// ******************************************************************
// Diese Methode sucht einen Eintrag in der Liste mit den angegebenen Daten.
template <typename Type> tbResult tbList<Type>::FindEntry(Type* pData,
														  tbListEntry<Type>** ppOut)
{
	tbListEntry<Type>* pCurrentEntry;

	// Parameter prüfen
	if(pData == NULL) TB_ERROR_NULL_POINTER("pData", TB_ERROR);


	// Zuerst den allerersten Listeneintrag testen
	pCurrentEntry = m_pFirstEntry;

	while(pCurrentEntry != NULL)
	{
		// Die Daten des aktuellen Eintrags mit den angegebenen Daten
		// vergleichen. Falls sie übereinstimmen, ist die Suche beendet.
		if(!memcmp(&pCurrentEntry->Data, pData, sizeof(Type)))
		{
			// Falls gewünscht, wird der als Parameter angegebene Doppelzeiger
			// nun auf die Adresse der Listeneintragsstruktur gesetzt.
			if(ppOut != NULL) *ppOut = pCurrentEntry;
			return TB_OK;
		}

		// Fortschreiten
		pCurrentEntry = pCurrentEntry->pNextEntry;
	}

	// Es wurde nichts gefunden!
	return TB_NOT_FOUND;
}

// ******************************************************************
// Diese Methode löscht einen Eintrag in der Liste.
template <typename Type> tbResult tbList<Type>::DeleteEntry(tbListEntry<Type>* pEntry)
{
	// Parameter prüfen
	if(pEntry == NULL) TB_ERROR_NULL_POINTER("pEntry", TB_ERROR);


	// Beim Löschen entsteht ein Loch in der Liste, welches nun "gestopft"
	// werden muss. Dabei spielt es eine Rolle, ob der Eintrag an erster
	// oder letzter Stelle oder irgendwo in der Mitte der Liste steht.
	if(pEntry == m_pFirstEntry &&
	   pEntry == m_pLastEntry)
	{
		// Der Eintrag ist der erste und einzige.
		m_pFirstEntry = NULL;
		m_pLastEntry = NULL;
	}
	else if(pEntry == m_pFirstEntry)
	{
		// Der Eintrag steht an erster Stelle.
		// Der neue erste Eintrag ist nun der Folgende des zu löschenden Eintrags.
		m_pFirstEntry = pEntry->pNextEntry;
		m_pFirstEntry->pPrevEntry = NULL;
	}
	else if(pEntry == m_pLastEntry)
	{
		// Der Eintrag steht an letzter Stelle.
		// Der neue letzte Eintrag ist nun der Vorherige des zu löschenden Eintrags.
		m_pLastEntry = pEntry->pPrevEntry;
		m_pLastEntry->pNextEntry = NULL;
	}
	else
	{
		// Der Eintrag steht irgendwo in der Mitte.
		// Der vorherige und der folgende Eintrag werden nun verknüpft.
		pEntry->pPrevEntry->pNextEntry = pEntry->pNextEntry;
		pEntry->pNextEntry->pPrevEntry = pEntry->pPrevEntry;
	}

	// Der Speicher für sie wurde beim Erstellen eines neuen Eintrags
	// reserviert und kann nun wieder freigegeben werden.
	TB_SAFE_DELETE(pEntry);

	// Eintragszähler verringern
	m_iNumEntries--;

	return TB_OK;
}

// ******************************************************************
// Diese Methode löscht die gesamte Liste.
template <typename Type> tbResult tbList<Type>::Clear()
{
	// Es wird so lange der erste Listeneintrag gelöscht, bis keiner mehr da ist.
	while(m_pFirstEntry != NULL) DeleteEntry(m_pFirstEntry);

	return TB_OK;
}

// ******************************************************************
// Durchlaufen der Liste
template <typename Type> tbResult tbList<Type>::Traverse(tbResult (* pCallback)(tbListEntry<Type>*))
{
	tbListEntry<Type>* pCurrentEntry;

	// Parameter prüfen
	if(pCallback == NULL) TB_ERROR_NULL_POINTER("pCallback", TB_ERROR);


	// Die ganze Liste durchlaufen
	pCurrentEntry = m_pFirstEntry;

	while(pCurrentEntry != NULL)
	{
		// Rückruffunktion aufrufen
		if(pCallback(pCurrentEntry) == TB_STOP) break;

		// Fortfahren
		pCurrentEntry = pCurrentEntry->pNextEntry;
	}

	return TB_OK;
}

// ******************************************************************