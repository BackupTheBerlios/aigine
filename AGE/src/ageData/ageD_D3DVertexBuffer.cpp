/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	ageD_D3DVertexBuffer.cpp
	==================
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Umgang mit Vertex-Buffern

	Autor:
	David Scherfgen

********************************************************************/

#include <TriBase.h>

// ******************************************************************
// Konstruktor der ageD_D3DVertexBuffer-Klasse
ageD_D3DVertexBuffer::ageD_D3DVertexBuffer()
{
	// Alles zurücksetzen
	ZeroMemory(this, sizeof(ageD_D3DVertexBuffer));
}

// ******************************************************************
// Destruktor der ageD_D3DVertexBuffer-Klasse
ageD_D3DVertexBuffer::~ageD_D3DVertexBuffer()
{
	// Die Schnittstelle freigeben und die Speicherkopie löschen
	AGE_SAFE_RELEASE(m_pVertexBuffer);
	AGE_SAFE_MEMFREE(m_pBuffer);
}

// ******************************************************************
// Initialisierung
age_Result ageD_D3DVertexBuffer::Init(DWORD dwSize,
							  DWORD dwVertexSize,
							  DWORD dwFVF,
							  DWORD dwUsage,		// = D3DUSAGE_WRITEONLY
							  D3DPOOL Pool)			// = D3DPOOL_DEFAULT
{
	HRESULT hResult;

	// Parameter prüfen und sicherstellen, dass eine ageG_Direct3D-Klasse existiert
	if(dwSize == 0)						AGE_ERROR_INVALID_VALUE("dwSize", AGE_ERROR);
	if(dwVertexSize == 0)				AGE_ERROR_INVALID_VALUE("dwVertexSize", AGE_ERROR);
	if(!ageG_Direct3D::IsInitialized())	AGE_ERROR("Direct3D wurde noch nicht initialisiert!", AGE_ERROR);


	// Einen Vertex-Buffer mit den angegebenen Daten erstellen.
	// Zu dwUsage kommt in jedem Fall noch D3DUSAGE_WRITEONLY hinzu.
	dwUsage |= D3DUSAGE_WRITEONLY;
	if(FAILED(hResult = ageG_Direct3D::GetDevice()->CreateVertexBuffer(dwSize,
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
			AGE_WARNING("Erstellung eines dynamischen Vertex-Buffers fehlgeschlagen!");
			return Init(dwSize, dwVertexSize, dwFVF, dwUsage ^ D3DUSAGE_DYNAMIC, Pool);
		}
		else
		{
			// Fehler beim Erstellen des statischen Puffers!
			AGE_ERROR_DIRECTX("ageG_Direct3D::GetDevice()->CreateVertexBuffer", hResult, AGE_ERROR);
		}
	}

	// Die andere Initialisierungsmethode aufrufen
	if(Init(m_pVertexBuffer, dwVertexSize))
	{
		// Fehler!
		AGE_ERROR("Fehler beim Initialisieren des Vertex-Buffers!", AGE_ERROR);
	}

	return AGE_OK;
}

// ******************************************************************
// Initialisierung aus einem bereits vorhandenen Vertex-Buffer
age_Result ageD_D3DVertexBuffer::Init(PDIRECT3DVERTEXBUFFER9 pVertexBuffer,
							  DWORD dwVertexSize)
{
	D3DVERTEXBUFFER_DESC Desc;

	// Parameter prüfen
	if(pVertexBuffer == NULL) AGE_ERROR_NULL_POINTER(pVertexBuffer, AGE_ERROR);


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
	m_pBuffer = ageD_MemAlloc(m_dwSize);
	if(m_pBuffer == NULL)
	{
		// Fehler!
		AGE_ERROR_OUT_OF_MEMORY(AGE_ERROR);
	}

	// Ersten und letzten Vertex zurücksetzen
	m_dwFirstVertex = m_dwMaxVertices - 1;
	m_dwLastVertex = 0;

	return AGE_OK;
}

// ******************************************************************
// Diese Methode führt die Änderungen am Vertex-Buffer durch.
age_Result ageD_D3DVertexBuffer::Update()
{
	HRESULT	hResult;
	void*	pVertices;


	// Prüfen, ob m_dwFirstVertex und m_dwLastVertex in Ordnung sind
	if(m_dwFirstVertex > m_dwLastVertex)	return AGE_OK; // Es wurde nichts verändert!
	if(m_dwFirstVertex >= m_dwMaxVertices)	AGE_ERROR_INVALID_VALUE("m_dwFirstVertex", AGE_ERROR);
	if(m_dwLastVertex >= m_dwMaxVertices)	AGE_ERROR_INVALID_VALUE("m_dwLastVertex", AGE_ERROR);


	// Den Vertex-Buffer sperren.
	// Die Sperrflags sind 0 bei statischen Vertex-Buffern und
	// D3DLOCK_DISCARD bei dynamischen.
	if(FAILED(hResult = m_pVertexBuffer->Lock(m_dwFirstVertex * m_dwVertexSize,
		                                      (m_dwLastVertex - m_dwFirstVertex + 1) * m_dwVertexSize,
											  &pVertices,
											  (m_dwUsage & D3DUSAGE_DYNAMIC) ? D3DLOCK_DISCARD : 0)))
	{
		// Fehler beim Sperren!
		AGE_ERROR_DIRECTX("m_pVertexBuffer->Lock", hResult, AGE_ERROR);
	}

	// Die Daten kopieren
	memcpy(pVertices, (BYTE*)(m_pBuffer) + m_dwFirstVertex * m_dwVertexSize,
		   (m_dwLastVertex - m_dwFirstVertex + 1) * m_dwVertexSize);

	// Vertex-Buffer entsperren
	m_pVertexBuffer->Unlock();

	// Ersten und letzten Vertex zurücksetzen
	m_dwFirstVertex = m_dwMaxVertices - 1;
	m_dwLastVertex = 0;

	return AGE_OK;
}

// ******************************************************************
// Diese Methode verändert die Größe des Vertex-Buffers.
age_Result ageD_D3DVertexBuffer::SetSize(DWORD dwNewSize)
{
	HRESULT					hResult;
	PDIRECT3DVERTEXBUFFER9	pVertexBuffer;
	DWORD					dwOldFirstVertex;
	DWORD					dwOldLastVertex;


	// Parameter prüfen und sicherstellen, dass eine ageG_Direct3D-Klasse existiert
	if(dwNewSize == m_dwSize)			return AGE_OK;
	if(dwNewSize == 0)					AGE_ERROR_INVALID_VALUE("dwNewSize", AGE_ERROR);
	if(!ageG_Direct3D::IsInitialized())	AGE_ERROR("Direct3D wurde noch nicht initialisiert!", AGE_ERROR);


	// Die neue Größe eintragen und die maximale Anzahl der Vertizes neu berechnen
	m_dwSize = dwNewSize;
	m_dwMaxVertices = m_dwSize / m_dwVertexSize;

	// Einen neuen Vertex-Buffer erstellen
	if(FAILED(hResult = ageG_Direct3D::GetDevice()->CreateVertexBuffer(m_dwSize,
		                                                            m_dwUsage,
																	m_dwFVF,
																	m_Pool,
																	&pVertexBuffer,
																	NULL)))
	{
		// Fehler!
		AGE_ERROR_DIRECTX("ageG_Direct3D::GetDevice()->CreateVertexBuffer", hResult, AGE_ERROR);
	}

	// Die neue Schnittstelle eintragen, vorher den alten Vertex-Buffer freigeben
	AGE_SAFE_RELEASE(m_pVertexBuffer);
	m_pVertexBuffer = pVertexBuffer;

	// Die Größe der Speicherkopie anpassen
	m_pBuffer = ageD_MemReAlloc(m_pBuffer, dwNewSize);
	if(m_pBuffer == NULL)
	{
		// Fehler!
		AGE_ERROR_OUT_OF_MEMORY(AGE_ERROR);
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
		AGE_ERROR("Vertex-Buffer konnte nicht aktualisiert werden!", AGE_ERROR);
	}

	// Die ursprünglichen Werte von m_dwFirstVertex und m_dwLastVertex wiederherstellen
	// und auf die neue Größe anpassen
	m_dwFirstVertex = dwOldFirstVertex;
	m_dwLastVertex = dwOldLastVertex;
	if(m_dwFirstVertex >= m_dwMaxVertices) m_dwFirstVertex = m_dwMaxVertices - 1;
	if(m_dwLastVertex >= m_dwMaxVertices) m_dwLastVertex = m_dwMaxVertices - 1;

	// Die Cursorposition überprüfen
	if(m_dwCursor >= m_dwMaxVertices) m_dwCursor = m_dwMaxVertices - 1;

	return AGE_OK;
}

// ******************************************************************