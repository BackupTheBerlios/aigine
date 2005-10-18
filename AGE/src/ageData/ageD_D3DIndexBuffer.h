/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbIndexBuffer.h
	================
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Umgang mit Index-Buffern

	Autor:
	David Scherfgen

********************************************************************/

// Klasse für einen Index-Buffer
class ageD_IndexBuffer
{
private:
	// Variablen
	PDIRECT3DINDEXBUFFER9	m_pIndexBuffer;		// Der Index-Buffer
	void*					m_pBuffer;			// Speicherkopie
	DWORD					m_dwSize;			// Größe des Index-Buffers (in Bytes)
	DWORD					m_dwIndexSize;		// Indexgröße
	DWORD					m_dwMaxIndices;		// Maximale Anzahl der Vertizes
	D3DFORMAT				m_IndexFormat;		// FVF-Bezeichner (flexibles Indexformat)
	DWORD					m_dwUsage;			// Verwendungszweck
	D3DPOOL					m_Pool;				// Speicherklasse
	DWORD					m_dwFirstIndex;		// Erster veränderter Index
	DWORD					m_dwLastIndex;		// Letzter veränderter Index
	DWORD					m_dwCursor;			// Indexcursorposition

public:
	// Konstruktur und Destruktor
	ageD_IndexBuffer();
	~ageD_IndexBuffer();

	// Methoden
	age_Result Init(DWORD dwSize, DWORD dwIndexSize, D3DFORMAT IndexFormat, DWORD dwUsage = D3DUSAGE_WRITEONLY, D3DPOOL Pool = D3DPOOL_DEFAULT);	// Initialisierung
	age_Result Init(PDIRECT3DINDEXBUFFER9 pIndexBuffer, DWORD dwIndexSize);																		// Initialisierung aus einem Index-Buffer
	age_Result Update();																															// Speichert die Änderungen im Index-Buffer
	age_Result SetSize(DWORD dwNewSize);																											// Setzt die Größe des Index-Buffers

	// Inline-Methoden
	inline PDIRECT3DINDEXBUFFER9	GetIB()					{return m_pIndexBuffer;}
	inline void*					GetBuffer()				{return m_pBuffer;}
	inline DWORD					GetSize()				{return m_dwSize;}
	inline DWORD					GetIndexSize()			{return m_dwIndexSize;}
	inline DWORD					GetMaxIndices()			{return m_dwMaxIndices;}
	inline D3DFORMAT				GetIndexFormat()		{return m_IndexFormat;}
	inline DWORD					GetUsage()				{return m_dwUsage;}
	inline D3DPOOL					GetPool()				{return m_Pool;}
	inline DWORD					GetCursor()				{return m_dwCursor;}
	inline void						SetFirstIndex(DWORD i)	{m_dwFirstIndex = i;}
	inline void						SetLastIndex(DWORD i)	{m_dwLastIndex = i;}

	// Abfragen eines Index
	inline age_Result GetIndex(const DWORD dwIndex,
		                     void* pOut)
	{
/*
#ifdef _DEBUG
		// Parameter prüfen
		if(dwIndex >= m_dwMaxIndices)	TB_ERROR_INVALID_VALUE("dwIndex", TB_ERROR);
		if(pOut == NULL)				TB_ERROR_NULL_POINTER("pOut", TB_ERROR);
#endif
*/
		// Index zurückliefern
		memcpy(pOut, (BYTE*)(m_pBuffer) + dwIndex * m_dwIndexSize, m_dwIndexSize);

		return AGE_OK;
	}

	// Abfragen mehrerer Vertizes
	inline age_Result GetIndices(const DWORD dwStart,
							   const DWORD dwNumIndices,
							   void* pOut)
	{
/*
#ifdef _DEBUG
		// Parameter prüfen
		if(dwStart + dwNumIndices > m_dwMaxIndices ||
		   dwNumIndices == 0 ||
		   pOut == NULL)
		{
			// Fehler!
			TB_ERROR("Ungültige Angabe!", TB_ERROR);
		}
#endif
*/

		// Vertizes zurückliefern
		memcpy(pOut, (BYTE*)(m_pBuffer) + dwStart * m_dwIndexSize,
			   dwNumIndices * m_dwIndexSize);

		return AGE_OK;
	}

	// Setzen eines Index
	inline age_Result SetIndex(const DWORD dwIndex,
							 const void* pIndex)
	{
/*
#ifdef _DEBUG
		// Parameter prüfen
		if(dwIndex >= m_dwMaxIndices)	TB_ERROR_INVALID_VALUE("dwIndex", TB_ERROR);
		if(pIndex == NULL)				TB_ERROR_NULL_POINTER("pIndex", TB_ERROR);
#endif
*/

		// Den Index eintragen
		memcpy((BYTE*)(m_pBuffer) + dwIndex * m_dwIndexSize, pIndex, m_dwIndexSize);

		// m_dwFirstIndex und m_dwLastIndex aktualisieren
		if(dwIndex < m_dwFirstIndex) m_dwFirstIndex = dwIndex;
		if(dwIndex > m_dwLastIndex) m_dwLastIndex = dwIndex;

		return AGE_OK;
	}

	// Setzen mehrerer Vertizes
	inline age_Result SetIndices(const DWORD dwStart,
							   const DWORD dwNumIndices,
							   const void* pIndices)
	{
/*
#ifdef _DEBUG
		// Parameter prüfen
		if(dwStart + dwNumIndices > m_dwMaxIndices ||
		   pIndices == NULL)
		{
			// Fehler
			TB_ERROR("Ungültige Angabe!", TB_ERROR);
		}
#endif
*/
		// Die Vertizes kopieren
		memcpy((BYTE*)(m_pBuffer) + dwStart * m_dwIndexSize, pIndices,
			   dwNumIndices * m_dwIndexSize);

		// m_dwFirstIndex und m_dwLastIndex aktualisieren
		if(dwStart < m_dwFirstIndex) m_dwFirstIndex = dwStart;
		if(dwStart + dwNumIndices - 1 > m_dwLastIndex) m_dwLastIndex = dwStart + dwNumIndices - 1;

		return AGE_OK;
	}

	// Hinzufügen eines Index
	inline DWORD AddIndex(const void* pIndex)
	{
/*
#ifdef _DEBUG
		// Die Cursorposition prüfen
		if(m_dwCursor >= m_dwMaxIndices)
		{
			// Fehler!
			TB_ERROR("Ungültige Cursorposition!", 0xFFFFFFFF);
		}
#endif
*/		
		// Den Index setzen
		memcpy((BYTE*)(m_pBuffer) + m_dwCursor * m_dwIndexSize, pIndex, m_dwIndexSize);

		// m_dwFirstIndex und m_dwLastIndex aktualisieren
		if(m_dwCursor < m_dwFirstIndex) m_dwFirstIndex = m_dwCursor;
		if(m_dwCursor > m_dwLastIndex) m_dwLastIndex = m_dwCursor;

		// Nummer des neuen Index zurückliefern und den Cursor weiter bewegen
		return m_dwCursor++;
	}

	// Hinzufügen mehrerer Vertizes
	inline DWORD AddIndices(const DWORD dwNumIndices,
		                    const void* pIndices)
	{
/*
#ifdef _DEBUG
		// Parameter prüfen
		if(dwNumIndices == 0)	TB_ERROR_INVALID_VALUE("dwNumIndices", 0xFFFFFFFF);
		if(pIndices == NULL)	TB_ERROR_NULL_POINTER("pIndices", 0xFFFFFFFF);

		// Die Cursorposition prüfen
		if(m_dwCursor + dwNumIndices > m_dwMaxIndices)
		{
			// Fehler!
			TB_ERROR("Ungültige Cursorposition - Index-Buffer ist voll!", 0xFFFFFFFF);
		}
#endif
*/
		// Die Vertizes kopieren
		memcpy((BYTE*)(m_pBuffer) + m_dwCursor * m_dwIndexSize, pIndices,
			   dwNumIndices * m_dwIndexSize);

		// m_dwFirstIndex und m_dwLastIndex aktualisieren
		if(m_dwCursor < m_dwFirstIndex) m_dwFirstIndex = m_dwCursor;
		if(m_dwCursor + dwNumIndices - 1 > m_dwLastIndex) m_dwLastIndex = m_dwCursor + dwNumIndices - 1;

		// Nummer des ersten neuen Index zurückliefern und Cursor erhöhen
		m_dwCursor += dwNumIndices;
		return m_dwCursor - dwNumIndices;
	}

	// Setzen der Cursorposition
	inline age_Result SetCursor(const DWORD dwCursor)
	{
/*
#ifdef _DEBUG
		// Die Cursorposition prüfen
		if(dwCursor >= m_dwMaxIndices)
		{
			// Fehler!
			TB_ERROR("Ungültige Cursorposition!", TB_ERROR);
		}
#endif
*/
		// Den Cursor setzen
		m_dwCursor = dwCursor;

		return AGE_OK;
	}
};

// ******************************************************************