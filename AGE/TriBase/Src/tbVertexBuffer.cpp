/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbVertexBuffer.cpp
	==================
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Umgang mit Vertex-Buffern

	Autor:
	David Scherfgen

********************************************************************/

#include <TriBase.h>

// ******************************************************************
// Konstruktor der tbVertexBuffer-Klasse
tbVertexBuffer::tbVertexBuffer()
{
	// Alles zurücksetzen
	ZeroMemory(this, sizeof(tbVertexBuffer));
}

// ******************************************************************
// Destruktor der tbVertexBuffer-Klasse
tbVertexBuffer::~tbVertexBuffer()
{
	// Die Schnittstelle freigeben und die Speicherkopie löschen
	TB_SAFE_RELEASE(m_pVertexBuffer);
	TB_SAFE_MEMFREE(m_pBuffer);
}

// ******************************************************************
// Initialisierung
tbResult tbVertexBuffer::Init(DWORD dwSize,
							  DWORD dwVertexSize,
							  DWORD dwFVF,
							  DWORD dwUsage,		// = D3DUSAGE_WRITEONLY
							  D3DPOOL Pool)			// = D3DPOOL_DEFAULT
{
	HRESULT hResult;

	// Parameter prüfen und sicherstellen, dass eine tbDirect3D-Klasse existiert
	if(dwSize == 0)						TB_ERROR_INVALID_VALUE("dwSize", TB_ERROR);
	if(dwVertexSize == 0)				TB_ERROR_INVALID_VALUE("dwVertexSize", TB_ERROR);
	if(!tbDirect3D::IsInitialized())	TB_ERROR("Direct3D wurde noch nicht initialisiert!", TB_ERROR);


	// Einen Vertex-Buffer mit den angegebenen Daten erstellen.
	// Zu dwUsage kommt in jedem Fall noch D3DUSAGE_WRITEONLY hinzu.
	dwUsage |= D3DUSAGE_WRITEONLY;
	if(FAILED(hResult = tbDirect3D::GetDevice()->CreateVertexBuffer(dwSize,
																	dwUsage,
																	dwFVF,
																	Pool,
																	&m_pVertexBuffer,
																	NULL)))
	{
		// Wenn der Puffer dynamisch werden sollte, versuchen wir es mit einem
		// statischen. Wenn es dann immer noch nicht funktioniert, gibt es einen Fehler.
		if(dwUsage & D3DUSAGE_DYNAMIC)
		{
			// Warnung ausgeben
			TB_WARNING("Erstellung eines dynamischen Vertex-Buffers fehlgeschlagen!");
			return Init(dwSize, dwVertexSize, dwFVF, dwUsage ^ D3DUSAGE_DYNAMIC, Pool);
		}
		else
		{
			// Fehler beim Erstellen des statischen Puffers!
			TB_ERROR_DIRECTX("tbDirect3D::GetDevice()->CreateVertexBuffer", hResult, TB_ERROR);
		}
	}

	// Die andere Initialisierungsmethode aufrufen
	if(Init(m_pVertexBuffer, dwVertexSize))
	{
		// Fehler!
		TB_ERROR("Fehler beim Initialisieren des Vertex-Buffers!", TB_ERROR);
	}

	return TB_OK;
}

// ******************************************************************
// Initialisierung aus einem bereits vorhandenen Vertex-Buffer
tbResult tbVertexBuffer::Init(PDIRECT3DVERTEXBUFFER9 pVertexBuffer,
							  DWORD dwVertexSize)
{
	D3DVERTEXBUFFER_DESC Desc;

	// Parameter prüfen
	if(pVertexBuffer == NULL) TB_ERROR_NULL_POINTER(pVertexBuffer, TB_ERROR);


	// Die Beschreibung des Vertex-Buffers abfragen
	pVertexBuffer->GetDesc(&Desc);

	// Alle Angaben übernehmen, auch die Schnittstelle selbst
	m_pVertexBuffer	= pVertexBuffer;
	m_dwSize		= Desc.Size;
	m_dwVertexSize	= dwVertexSize;
	m_dwFVF			= Desc.FVF;
	m_dwUsage		= Desc.Usage;
	m_Pool			= Desc.Pool;

	// Maximale Anzahl der Vertizes berechnen
	m_dwMaxVertices = m_dwSize / m_dwVertexSize;

	// Interne Speicherkopie anfertigen
	m_pBuffer = tbMemAlloc(m_dwSize);
	if(m_pBuffer == NULL)
	{
		// Fehler!
		TB_ERROR_OUT_OF_MEMORY(TB_ERROR);
	}

	// Ersten und letzten Vertex zurücksetzen
	m_dwFirstVertex = m_dwMaxVertices - 1;
	m_dwLastVertex = 0;

	return TB_OK;
}

// ******************************************************************
// Diese Methode führt die Änderungen am Vertex-Buffer durch.
tbResult tbVertexBuffer::Update()
{
	HRESULT	hResult;
	void*	pVertices;


	// Prüfen, ob m_dwFirstVertex und m_dwLastVertex in Ordnung sind
	if(m_dwFirstVertex > m_dwLastVertex)	return TB_OK; // Es wurde nichts verändert!
	if(m_dwFirstVertex >= m_dwMaxVertices)	TB_ERROR_INVALID_VALUE("m_dwFirstVertex", TB_ERROR);
	if(m_dwLastVertex >= m_dwMaxVertices)	TB_ERROR_INVALID_VALUE("m_dwLastVertex", TB_ERROR);


	// Den Vertex-Buffer sperren.
	// Die Sperrflags sind 0 bei statischen Vertex-Buffern und
	// D3DLOCK_DISCARD bei dynamischen.
	if(FAILED(hResult = m_pVertexBuffer->Lock(m_dwFirstVertex * m_dwVertexSize,
		                                      (m_dwLastVertex - m_dwFirstVertex + 1) * m_dwVertexSize,
											  &pVertices,
											  (m_dwUsage & D3DUSAGE_DYNAMIC) ? D3DLOCK_DISCARD : 0)))
	{
		// Fehler beim Sperren!
		TB_ERROR_DIRECTX("m_pVertexBuffer->Lock", hResult, TB_ERROR);
	}

	// Die Daten kopieren
	memcpy(pVertices, (BYTE*)(m_pBuffer) + m_dwFirstVertex * m_dwVertexSize,
		   (m_dwLastVertex - m_dwFirstVertex + 1) * m_dwVertexSize);

	// Vertex-Buffer entsperren
	m_pVertexBuffer->Unlock();

	// Ersten und letzten Vertex zurücksetzen
	m_dwFirstVertex = m_dwMaxVertices - 1;
	m_dwLastVertex = 0;

	return TB_OK;
}

// ******************************************************************
// Diese Methode verändert die Größe des Vertex-Buffers.
tbResult tbVertexBuffer::SetSize(DWORD dwNewSize)
{
	HRESULT					hResult;
	PDIRECT3DVERTEXBUFFER9	pVertexBuffer;
	DWORD					dwOldFirstVertex;
	DWORD					dwOldLastVertex;


	// Parameter prüfen und sicherstellen, dass eine tbDirect3D-Klasse existiert
	if(dwNewSize == m_dwSize)			return TB_OK;
	if(dwNewSize == 0)					TB_ERROR_INVALID_VALUE("dwNewSize", TB_ERROR);
	if(!tbDirect3D::IsInitialized())	TB_ERROR("Direct3D wurde noch nicht initialisiert!", TB_ERROR);


	// Die neue Größe eintragen und die maximale Anzahl der Vertizes neu berechnen
	m_dwSize = dwNewSize;
	m_dwMaxVertices = m_dwSize / m_dwVertexSize;

	// Einen neuen Vertex-Buffer erstellen
	if(FAILED(hResult = tbDirect3D::GetDevice()->CreateVertexBuffer(m_dwSize,
		                                                            m_dwUsage,
																	m_dwFVF,
																	m_Pool,
																	&pVertexBuffer,
																	NULL)))
	{
		// Fehler!
		TB_ERROR_DIRECTX("tbDirect3D::GetDevice()->CreateVertexBuffer", hResult, TB_ERROR);
	}

	// Die neue Schnittstelle eintragen, vorher den alten Vertex-Buffer freigeben
	TB_SAFE_RELEASE(m_pVertexBuffer);
	m_pVertexBuffer = pVertexBuffer;

	// Die Größe der Speicherkopie anpassen
	m_pBuffer = tbMemReAlloc(m_pBuffer, dwNewSize);
	if(m_pBuffer == NULL)
	{
		// Fehler!
		TB_ERROR_OUT_OF_MEMORY(TB_ERROR);
	}

	// Alle Vertizes aktualisieren, m_dwFirstVertex und m_dwLastVertex sichern
	dwOldFirstVertex = m_dwFirstVertex;
	dwOldLastVertex = m_dwLastVertex;
	m_dwFirstVertex = 0;
	m_dwLastVertex = m_dwMaxVertices - 1;

	// Die Aktualisierungsmethode aufrufen
	if(Update())
	{
		// Fehler beim Aktualisieren!
		TB_ERROR("Vertex-Buffer konnte nicht aktualisiert werden!", TB_ERROR);
	}

	// Die ursprünglichen Werte von m_dwFirstVertex und m_dwLastVertex wiederherstellen
	// und auf die neue Größe anpassen
	m_dwFirstVertex = dwOldFirstVertex;
	m_dwLastVertex = dwOldLastVertex;
	if(m_dwFirstVertex >= m_dwMaxVertices) m_dwFirstVertex = m_dwMaxVertices - 1;
	if(m_dwLastVertex >= m_dwMaxVertices) m_dwLastVertex = m_dwMaxVertices - 1;

	// Die Cursorposition überprüfen
	if(m_dwCursor >= m_dwMaxVertices) m_dwCursor = m_dwMaxVertices - 1;

	return TB_OK;
}

// ******************************************************************