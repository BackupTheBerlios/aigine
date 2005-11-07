/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbVertexBuffer.h
	================
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Umgang mit Vertex-Buffern

	Autor:
	David Scherfgen

********************************************************************/

// Klasse für einen Vertex-Buffer
class TRIBASE_API tbVertexBuffer
{
private:
	// Variablen
	PDIRECT3DVERTEXBUFFER9	m_pVertexBuffer;	// Der Vertex-Buffer
	void*					m_pBuffer;			// Speicherkopie
	DWORD					m_dwSize;			// Größe des Vertex-Buffers (in Bytes)
	DWORD					m_dwVertexSize;		// Vertexgröße
	DWORD					m_dwMaxVertices;	// Maximale Anzahl der Vertizes
	DWORD					m_dwFVF;			// FVF-Bezeichner (flexibles Vertexformat)
	DWORD					m_dwUsage;			// Verwendungszweck
	D3DPOOL					m_Pool;				// Speicherklasse
	DWORD					m_dwFirstVertex;	// Erster veränderter Vertex
	DWORD					m_dwLastVertex;		// Letzter veränderter Vertex
	DWORD					m_dwCursor;			// Vertexcursorposition

public:
	// Konstruktur und Destruktor
	tbVertexBuffer();
	~tbVertexBuffer();

	// Methoden
	tbResult Init(DWORD dwSize, DWORD dwVertexSize, DWORD dwFVF, DWORD dwUsage = D3DUSAGE_WRITEONLY, D3DPOOL Pool = D3DPOOL_DEFAULT);	// Initialisierung
	tbResult Init(PDIRECT3DVERTEXBUFFER9 pVertexBuffer, DWORD dwVertexSize);															// Initialisierung aus einem Vertex-Buffer
	tbResult Update();																													// Speichert die Änderungen im Vertex-Buffer
	tbResult SetSize(DWORD dwNewSize);																									// Setzt die Größe des Vertex-Buffers

	// Inline-Methoden
	inline PDIRECT3DVERTEXBUFFER9	GetVB()					{return m_pVertexBuffer;}
	inline void*					GetBuffer()				{return m_pBuffer;}
	inline DWORD					GetSize()				{return m_dwSize;}
	inline DWORD					GetVertexSize()			{return m_dwVertexSize;}
	inline DWORD					GetMaxVertices()		{return m_dwMaxVertices;}
	inline DWORD					GetFVF()				{return m_dwFVF;}
	inline DWORD					GetUsage()				{return m_dwUsage;}
	inline D3DPOOL					GetPool()				{return m_Pool;}
	inline DWORD					GetCursor()				{return m_dwCursor;}
	inline void						SetFirstVertex(DWORD v)	{m_dwFirstVertex = v;}
	inline void						SetLastVertex(DWORD v)	{m_dwLastVertex = v;}

	// Abfragen eines Vertex
	inline tbResult GetVertex(const DWORD dwVertex,
		                      void* pOut)
	{
#ifdef _DEBUG
		// Parameter prüfen
		if(dwVertex >= m_dwMaxVertices)	TB_ERROR_INVALID_VALUE("dwVertex", TB_ERROR);
		if(pOut == NULL)				TB_ERROR_NULL_POINTER("pOut", TB_ERROR);
#endif

		// Vertex zurückliefern
		memcpy(pOut, (BYTE*)(m_pBuffer) + dwVertex * m_dwVertexSize, m_dwVertexSize);

		return TB_OK;
	}

	// Abfragen mehrerer Vertizes
	inline tbResult GetVertices(const DWORD dwStart,
								const DWORD dwNumVertices,
								void* pOut)
	{
#ifdef _DEBUG
		// Parameter prüfen
		if(dwStart + dwNumVertices > m_dwMaxVertices ||
		   dwNumVertices == 0 ||
		   pOut == NULL)
		{
			// Fehler!
			TB_ERROR("Ungültige Angabe!", TB_ERROR);
		}
#endif

		// Vertizes zurückliefern
		memcpy(pOut, (BYTE*)(m_pBuffer) + dwStart * m_dwVertexSize,
			   dwNumVertices * m_dwVertexSize);

		return TB_OK;
	}

	// Setzen eines Vertex
	inline tbResult SetVertex(const DWORD dwVertex,
							  const void* pVertex)
	{
#ifdef _DEBUG
		// Parameter prüfen
		if(dwVertex >= m_dwMaxVertices)	TB_ERROR_INVALID_VALUE("dwVertex", TB_ERROR);
		if(pVertex == NULL)				TB_ERROR_NULL_POINTER("pVertex", TB_ERROR);
#endif

		// Den Vertex eintragen
		memcpy((BYTE*)(m_pBuffer) + dwVertex * m_dwVertexSize, pVertex, m_dwVertexSize);

		// m_dwFirstVertex und m_dwLastVertex aktualisieren
		if(dwVertex < m_dwFirstVertex) m_dwFirstVertex = dwVertex;
		if(dwVertex > m_dwLastVertex) m_dwLastVertex = dwVertex;

		return TB_OK;
	}

	// Setzen mehrerer Vertizes
	inline tbResult SetVertices(const DWORD dwStart,
								const DWORD dwNumVertices,
								const void* pVertices)
	{
#ifdef _DEBUG
		// Parameter prüfen
		if(dwStart + dwNumVertices > m_dwMaxVertices ||
		   pVertices == NULL)
		{
			// Fehler
			TB_ERROR("Ungültige Angabe!", TB_ERROR);
		}
#endif

		// Die Vertizes kopieren
		memcpy((BYTE*)(m_pBuffer) + dwStart * m_dwVertexSize, pVertices,
			   dwNumVertices * m_dwVertexSize);

		// m_dwFirstVertex und m_dwLastVertex aktualisieren
		if(dwStart < m_dwFirstVertex) m_dwFirstVertex = dwStart;
		if(dwStart + dwNumVertices - 1 > m_dwLastVertex) m_dwLastVertex = dwStart + dwNumVertices - 1;

		return TB_OK;
	}

	// Hinzufügen eines Vertex
	inline DWORD AddVertex(const void* pVertex)
	{
#ifdef _DEBUG
		// Die Cursorposition prüfen
		if(m_dwCursor >= m_dwMaxVertices)
		{
			// Fehler!
			TB_ERROR("Ungültige Cursorposition!", 0xFFFFFFFF);
		}
#endif
		
		// Den Vertex setzen
		memcpy((BYTE*)(m_pBuffer) + m_dwCursor * m_dwVertexSize, pVertex, m_dwVertexSize);

		// m_dwFirstVertex und m_dwLastVertex aktualisieren
		if(m_dwCursor < m_dwFirstVertex) m_dwFirstVertex = m_dwCursor;
		if(m_dwCursor > m_dwLastVertex) m_dwLastVertex = m_dwCursor;

		// Nummer des neuen Vertex zurückliefern und den Cursor weiter bewegen
		return m_dwCursor++;
	}

	// Hinzufügen mehrerer Vertizes
	inline DWORD AddVertices(const DWORD dwNumVertices,
		                     const void* pVertices)
	{
#ifdef _DEBUG
		// Parameter prüfen
		if(dwNumVertices == 0)	TB_ERROR_INVALID_VALUE("dwNumVertices", 0xFFFFFFFF);
		if(pVertices == NULL)	TB_ERROR_NULL_POINTER("pVertices", 0xFFFFFFFF);

		// Die Cursorposition prüfen
		if(m_dwCursor + dwNumVertices > m_dwMaxVertices)
		{
			// Fehler!
			TB_ERROR("Ungültige Cursorposition - Vertex-Buffer ist voll!", 0xFFFFFFFF);
		}
#endif

		// Die Vertizes kopieren
		memcpy((BYTE*)(m_pBuffer) + m_dwCursor * m_dwVertexSize, pVertices,
			   dwNumVertices * m_dwVertexSize);

		// m_dwFirstVertex und m_dwLastVertex aktualisieren
		if(m_dwCursor < m_dwFirstVertex) m_dwFirstVertex = m_dwCursor;
		if(m_dwCursor + dwNumVertices - 1 > m_dwLastVertex) m_dwLastVertex = m_dwCursor + dwNumVertices - 1;

		// Nummer des ersten neuen Vertex zurückliefern und Cursor erhöhen
		m_dwCursor += dwNumVertices;
		return m_dwCursor - dwNumVertices;
	}

	// Setzen der Cursorposition
	inline tbResult SetCursor(const DWORD dwCursor)
	{
#ifdef _DEBUG
		// Die Cursorposition prüfen
		if(dwCursor >= m_dwMaxVertices)
		{
			// Fehler!
			TB_ERROR("Ungültige Cursorposition!", TB_ERROR);
		}
#endif

		// Den Cursor setzen
		m_dwCursor = dwCursor;

		return TB_OK;
	}
};

// ******************************************************************