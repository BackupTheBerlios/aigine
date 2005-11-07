/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbStaticList.h
	==============
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Umgang mit dynamischen Listen aller möglichen Typen

	Autor:
	David Scherfgen

********************************************************************/

// Template-Klasse für eine dynamische Liste
template <typename Type> class tbDynList
{
private:
	// Variablen
	Type*	m_pEntries;		// Die Listeneinträge
	int		m_iNumEntries;	// Anzahl der Listeneinträge
	int		m_iMaxEntries;	// Maximale Anzahl der Listeneinträge

	// Konstruktur und Destruktor
	tbDynList();
	~tbDynList();

	// Methoden
	int			AddEntry(Type* pData);							// Eintrag hinzufügen
	int			FindEntry(Type* pData);							// Eintrag suchen
	tbResult	DeleteEntry(int iEntry);						// Eintrag löschen
	tbResult	Clear();										// Liste leeren
	tbResult	Traverse(tbResult (* pCallback)(int, Type*));	// Liste durchlaufen
	tbResult	Optimize();										// Optimieren der Liste

	// Inline-Methoden
	inline Type*	GetEntryData(int iEntry)	{return &m_pEntries[iEntry];}
	inline int		GetNumEntries()				{return m_iNumEntries;}
	inline int		GetMaxEntries()				{return m_iMaxEntries;}
};

// ******************************************************************
// Konstruktor der tbDynList-Klasse
template <typename Type> tbDynList<Type>::tbDynList()
{
	// Alles zurücksetzen
	ZeroMemory(this, sizeof(tbDynList<Type>));
}

// ******************************************************************
// Destruktor der tbDynList-Klasse
template <typename Type> tbDynList<Type>::~tbDynList()
{
	// Liste leeren
	Clear();
}

// ******************************************************************
// Diese Methode fügt einen neuen Eintrag zur Liste hinzu.
template <typename Type> int tbDynList<Type>::AddEntry(Type* pData)
{
	// Parameter prüfen
	if(pData == NULL) TB_ERROR_NULL_POINTER("pData", NULL);


	// Wenn kein Platz mehr ist, machen wir welchen.
	if(m_iNumEntries >= m_iMaxEntries)
	{
		m_iMaxEntries += 32;
		m_pEntries = (Type*)(tbMemReAlloc(m_pEntries, m_iMaxEntries * sizeof(Type)));
		if(m_pEntries == NULL) TB_ERROR_OUT_OF_MEMORY(-1);
	}

	// Listeneintrag erzeugen
	memcpy(&m_pEntries[m_iNumEntries], pData, sizeof(Type));

	// Nummer zurückliefern
	return m_iNumEntries++;
}

// ******************************************************************
// Diese Methode sucht einen Eintrag in der Liste mit den angegebenen Daten.
template <typename Type> int tbDynList<Type>::FindEntry(Type* pData)
{
	// Parameter prüfen
	if(pData == NULL) TB_ERROR_NULL_POINTER("pData", -1);


	// Suchen...
	for(int i = 0; i < m_iNumEntries; i++)
	{
		// Vergleichen
		if(!memcmp(&m_pEntries[i], pData, sizeof(Type))) return i;
	}

	return -1;
}

// ******************************************************************
// Diese Methode löscht einen Eintrag in der Liste.
template <typename Type> tbResult tbDynList<Type>::DeleteEntry(int iEntry)
{
	// Parameter prüfen
	if(iEntry < 0 || iEntry >= m_iNumEntries) TB_ERROR_INVALID_VALUE("iEntry", TB_ERROR);


	// Beim Löschen entsteht ein Loch in der Liste, welches nun "gestopft"
	// werden muss. Dazu kopieren wir einfach den letzten Listeneintrag.
	if(m_iNumEntries > 1) memcpy(&m_pEntries[iEntry], &m_pEntries[m_iNumEntries - 1], sizeof(Type));
	m_iNumEntries--;

	// Lohnt es sich, die Liste zu optimieren? Wir tun es ab 1 MB zu viel Speicher.
	if((m_iMaxEntries - m_iNumEntries) * sizeof(Type) >= 1048576) return Optimize();

	return TB_OK;
}

// ******************************************************************
// Diese Methode löscht die gesamte Liste.
template <typename Type> tbResult tbDynList<Type>::Clear()
{
	tbMemFree(m_pEntries);
	m_iNumEntries = 0;
	m_iMaxEntries = 0;

	return TB_OK;
}

// ******************************************************************
// Durchlaufen der Liste
template <typename Type> tbResult tbDynList<Type>::Traverse(tbResult (* pCallback)(int, Type*))
{
	// Parameter prüfen
	if(pCallback == NULL) TB_ERROR_NULL_POINTER("pCallback", TB_ERROR);


	// Die ganze Liste durchlaufen
	for(int i = 0; i < m_iNumEntries; i++) pCallback(i, &m_pEntries[i]);

	return TB_OK;
}

// ******************************************************************
// Liste optimieren
template <typename Type> tbResult tbDynList<Type>::Optimize()
{
	// Speicherüberschuss freigeben
	m_pEntries = (Type*)(tbMemReAlloc(m_pEntries, m_iNumEntries * sizeof(Type)));
	if(m_pEntries == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

	m_iMaxEntries = m_iNumEntries;

	return TB_OK;
}

// ******************************************************************