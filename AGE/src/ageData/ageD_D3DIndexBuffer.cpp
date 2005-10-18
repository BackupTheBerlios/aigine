/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbIndexBuffer.cpp
	==================
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Umgang mit Index-Buffern

	Autor:
	David Scherfgen

********************************************************************/

#include <AGE.h>

// ******************************************************************
// Konstruktor der tbIndexBuffer-Klasse
tbIndexBuffer::tbIndexBuffer()
{
	// Alles zurücksetzen
	ZeroMemory(this, sizeof(tbIndexBuffer));
}

// ******************************************************************
// Destruktor der tbIndexBuffer-Klasse
tbIndexBuffer::~tbIndexBuffer()
{
	// Die Schnittstelle freigeben und die Speicherkopie löschen
	TB_SAFE_RELEASE(m_pIndexBuffer);
	TB_SAFE_MEMFREE(m_pBuffer);
}

// ******************************************************************
// Initialisierung
tbResult tbIndexBuffer::Init(DWORD dwSize,
							 DWORD dwIndexSize,
							 D3DFORMAT IndexFormat,
							 DWORD dwUsage,			// = D3DUSAGE_WRITEONLY
							 D3DPOOL Pool)			// = D3DPOOL_DEFAULT
{
	HRESULT hResult;

	// Parameter prüfen und sicherstellen, dass eine tbDirect3D-Klasse existiert
	if(dwSize == 0)						TB_ERROR_INVALID_VALUE("dwSize", TB_ERROR);
	if(dwIndexSize == 0)				TB_ERROR_INVALID_VALUE("dwIndexSize", TB_ERROR);
	if(!tbDirect3D::IsInitialized())	TB_ERROR("Direct3D wurde noch nicht initialisiert!", TB_ERROR);


	// Einen Index-Buffer mit den angegebenen Daten erstellen.
	// Zu dwUsage kommt in jedem Fall noch D3DUSAGE_WRITEONLY hinzu.
	dwUsage |= D3DUSAGE_WRITEONLY;
	if(FAILED(hResult = tbDirect3D::GetDevice()->CreateIndexBuffer(dwSize,
																   dwUsage,
																   IndexFormat,
																   Pool,
																   &m_pIndexBuffer,
																   NULL)))
	{
		// Wenn der Puffer dynamisch werden sollte, versuchen wir es mit einem
		// statischen. Wenn es dann immer noch nicht funktioniert, gibt es einen Fehler.
		if(dwUsage & D3DUSAGE_DYNAMIC)
		{
			// Warnung ausgeben
			TB_WARNING("Erstellung eines dynamischen Index-Buffers fehlgeschlagen!");
			return Init(dwSize, dwIndexSize, IndexFormat, dwUsage ^ D3DUSAGE_DYNAMIC, Pool);
		}
		else
		{
			// Fehler beim Erstellen des statischen Puffers!
			TB_ERROR_DIRECTX("tbDirect3D::m_pD3DDevice->CreateIndexBuffer", hResult, TB_ERROR);
		}
	}

	// Die andere Initialisierungsmethode aufrufen
	if(Init(m_pIndexBuffer, dwIndexSize))
	{
		// Fehler!
		TB_ERROR("Fehler beim Initialisieren des Index-Buffers!", TB_ERROR);
	}

	return TB_OK;
}

// ******************************************************************
// Initialisierung aus einem bereits vorhandenen Index-Buffer
tbResult tbIndexBuffer::Init(PDIRECT3DINDEXBUFFER9 pIndexBuffer,
							 DWORD dwIndexSize)
{
	D3DINDEXBUFFER_DESC Desc;

	// Parameter prüfen
	if(pIndexBuffer == NULL) TB_ERROR_NULL_POINTER(pIndexBuffer, TB_ERROR);


	// Die Beschreibung des Index-Buffers abfragen
	pIndexBuffer->GetDesc(&Desc);

	// Alle Angaben übernehmen, auch die Schnittstelle selbst
	m_pIndexBuffer	= pIndexBuffer;
	m_dwSize		= Desc.Size;
	m_dwIndexSize	= dwIndexSize;
	m_IndexFormat	= Desc.Format;
	m_dwUsage		= Desc.Usage;
	m_Pool			= Desc.Pool;

	// Maximale Anzahl der Vertizes berechnen
	m_dwMaxIndices = m_dwSize / m_dwIndexSize;

	// Interne Speicherkopie anfertigen
	m_pBuffer = tbMemAlloc(m_dwSize);
	if(m_pBuffer == NULL)
	{
		// Fehler!
		TB_ERROR_OUT_OF_MEMORY(TB_ERROR);
	}

	// Ersten und letzten Index zurücksetzen
	m_dwFirstIndex = m_dwMaxIndices - 1;
	m_dwLastIndex = 0;

	return TB_OK;
}

// ******************************************************************
// Diese Methode führt die Änderungen am Index-Buffer durch.
tbResult tbIndexBuffer::Update()
{
	HRESULT	hResult;
	void*	pIndices;


	// Prüfen, ob m_dwFirstIndex und m_dwLastIndex in Ordnung sind
	if(m_dwFirstIndex > m_dwLastIndex)		return TB_OK; // Es wurde nichts verändert!
	if(m_dwFirstIndex >= m_dwMaxIndices)	TB_ERROR_INVALID_VALUE("m_dwFirstIndex", TB_ERROR);
	if(m_dwLastIndex >= m_dwMaxIndices)		TB_ERROR_INVALID_VALUE("m_dwLastIndex", TB_ERROR);


	// Den Index-Buffer sperren.
	// Die Sperrflags sind 0 bei statischen Index-Buffern und
	// D3DLOCK_DISCARD bei dynamischen.
	if(FAILED(hResult = m_pIndexBuffer->Lock(m_dwFirstIndex * m_dwIndexSize,
		                                     (m_dwLastIndex - m_dwFirstIndex + 1) * m_dwIndexSize,
											 &pIndices,
											 (m_dwUsage & D3DUSAGE_DYNAMIC) ? D3DLOCK_DISCARD : 0)))
	{
		// Fehler beim Sperren!
		TB_ERROR_DIRECTX("m_pIndexBuffer->Lock", hResult, TB_ERROR);
	}

	// Die Daten kopieren
	memcpy(pIndices, (BYTE*)(m_pBuffer) + m_dwFirstIndex * m_dwIndexSize,
		   (m_dwLastIndex - m_dwFirstIndex + 1) * m_dwIndexSize);

	// Index-Buffer entsperren
	m_pIndexBuffer->Unlock();

	// Ersten und letzten Index zurücksetzen
	m_dwFirstIndex = m_dwMaxIndices - 1;
	m_dwLastIndex = 0;

	return TB_OK;
}

// ******************************************************************
// Diese Methode verändert die Größe des Index-Buffers.
tbResult tbIndexBuffer::SetSize(DWORD dwNewSize)
{
	HRESULT					hResult;
	PDIRECT3DINDEXBUFFER9	pIndexBuffer;
	DWORD					dwOldFirstIndex;
	DWORD					dwOldLastIndex;


	// Parameter prüfen und sicherstellen, dass eine tbDirect3D-Klasse existiert
	if(dwNewSize == m_dwSize)			return TB_OK;
	if(dwNewSize == 0)					TB_ERROR_INVALID_VALUE("dwNewSize", TB_ERROR);
	if(!tbDirect3D::IsInitialized())	TB_ERROR("Direct3D wurde noch nicht initialisiert!", TB_ERROR);


	// Die neue Größe eintragen und die maximale Anzahl der Vertizes neu berechnen
	m_dwSize = dwNewSize;
	m_dwMaxIndices = m_dwSize / m_dwIndexSize;

	// Einen neuen Index-Buffer erstellen
	if(FAILED(hResult = tbDirect3D::GetDevice()->CreateIndexBuffer(m_dwSize,
		                                                           m_dwUsage,
																   m_IndexFormat,
																   m_Pool,
																   &pIndexBuffer,
																   NULL)))
	{
		// Fehler!
		TB_ERROR_DIRECTX("tbDirect3D::GetDevice()->CreateIndexBuffer", hResult, TB_ERROR);
	}

	// Die neue Schnittstelle eintragen, vorher den alten Index-Buffer freigeben
	TB_SAFE_RELEASE(m_pIndexBuffer);
	m_pIndexBuffer = pIndexBuffer;

	// Die Größe der Speicherkopie anpassen
	m_pBuffer = tbMemReAlloc(m_pBuffer, dwNewSize);
	if(m_pBuffer == NULL)
	{
		// Fehler!
		TB_ERROR_OUT_OF_MEMORY(TB_ERROR);
	}

	// Alle Vertizes aktualisieren, m_dwFirstIndex und m_dwLastIndex sichern
	dwOldFirstIndex = m_dwFirstIndex;
	dwOldLastIndex = m_dwLastIndex;
	m_dwFirstIndex = 0;
	m_dwLastIndex = m_dwMaxIndices - 1;

	// Die Aktualisierungsmethode aufrufen
	if(Update())
	{
		// Fehler beim Aktualisieren!
		TB_ERROR("Index-Buffer konnte nicht aktualisiert werden!", TB_ERROR);
	}

	// Die ursprünglichen Werte von m_dwFirstIndex und m_dwLastIndex wiederherstellen
	// und auf die neue Größe anpassen
	m_dwFirstIndex = dwOldFirstIndex;
	m_dwLastIndex = dwOldLastIndex;
	if(m_dwFirstIndex >= m_dwMaxIndices) m_dwFirstIndex = m_dwMaxIndices - 1;
	if(m_dwLastIndex >= m_dwMaxIndices) m_dwLastIndex = m_dwMaxIndices - 1;

	// Die Cursorposition überprüfen
	if(m_dwCursor >= m_dwMaxIndices) m_dwCursor = m_dwMaxIndices - 1;

	return TB_OK;
}

// ******************************************************************