// IndexBuffer.cpp: implementation of the IndexBuffer class.
//
//////////////////////////////////////////////////////////////////////

#include "../AiGinE.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IndexBuffer::IndexBuffer()
{
	// Alles zurücksetzen
	ZeroMemory(this, sizeof(IndexBuffer));
}

// ******************************************************************
// Destruktor der IndexBuffer-Klasse
IndexBuffer::~IndexBuffer()
{
	// Die Schnittstelle freigeben und die Speicherkopie löschen
	AGE_SAFE_RELEASE(m_pIndexBuffer);
	AGE_SAFE_MEMFREE(m_pBuffer);
}

// ******************************************************************
// Initialisierung
Result IndexBuffer::Init(DWORD dwSize,
							 DWORD dwIndexSize,
							 D3DFORMAT IndexFormat,
							 DWORD dwUsage,			// = D3DUSAGE_WRITEONLY
							 D3DPOOL Pool)			// = D3DPOOL_DEFAULT
{
	HRESULT hResult;

	// Parameter prüfen und sicherstellen, dass eine Direct3D-Klasse existiert
	if(dwSize == 0)						AGE_ERROR_INVALID_VALUE("dwSize", AGE_ERROR);
	if(dwIndexSize == 0)				AGE_ERROR_INVALID_VALUE("dwIndexSize", AGE_ERROR);
	if(!Direct3D::IsInitialized())	AGE_ERROR("Direct3D wurde noch nicht initialisiert!", AGE_ERROR);


	// Einen Index-Buffer mit den angegebenen Daten erstellen.
	// Zu dwUsage kommt in jedem Fall noch D3DUSAGE_WRITEONLY hinzu.
	dwUsage |= D3DUSAGE_WRITEONLY;
	if(FAILED(hResult = Direct3D::GetDevice()->CreateIndexBuffer(dwSize,
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
			AGE_WARNING("Erstellung eines dynamischen Index-Buffers fehlgeschlagen!");
			return Init(dwSize, dwIndexSize, IndexFormat, dwUsage ^ D3DUSAGE_DYNAMIC, Pool);
		}
		else
		{
			// Fehler beim Erstellen des statischen Puffers!
			AGE_ERROR_DIRECTX("Direct3D::m_pD3DDevice->CreateIndexBuffer", hResult, AGE_ERROR);
		}
	}

	// Die andere Initialisierungsmethode aufrufen
	if(Init(m_pIndexBuffer, dwIndexSize))
	{
		// Fehler!
		AGE_ERROR("Fehler beim Initialisieren des Index-Buffers!", AGE_ERROR);
	}

	return AGE_OK;
}

// ******************************************************************
// Initialisierung aus einem bereits vorhandenen Index-Buffer
Result IndexBuffer::Init(PDIRECT3DINDEXBUFFER9 pIndexBuffer,
							 DWORD dwIndexSize)
{
	D3DINDEXBUFFER_DESC Desc;

	// Parameter prüfen
	if(pIndexBuffer == NULL) AGE_ERROR_NULL_POINTER(pIndexBuffer, AGE_ERROR);


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
	m_pBuffer = MemAlloc(m_dwSize);
	if(m_pBuffer == NULL)
	{
		// Fehler!
		AGE_ERROR_OUT_OF_MEMORY(AGE_ERROR);
	}

	// Ersten und letzten Index zurücksetzen
	m_dwFirstIndex = m_dwMaxIndices - 1;
	m_dwLastIndex = 0;

	return AGE_OK;
}

// ******************************************************************
// Diese Methode führt die Änderungen am Index-Buffer durch.
Result IndexBuffer::Update()
{
	HRESULT	hResult;
	void*	pIndices;


	// Prüfen, ob m_dwFirstIndex und m_dwLastIndex in Ordnung sind
	if(m_dwFirstIndex > m_dwLastIndex)		return AGE_OK; // Es wurde nichts verändert!
	if(m_dwFirstIndex >= m_dwMaxIndices)	AGE_ERROR_INVALID_VALUE("m_dwFirstIndex", AGE_ERROR);
	if(m_dwLastIndex >= m_dwMaxIndices)		AGE_ERROR_INVALID_VALUE("m_dwLastIndex", AGE_ERROR);


	// Den Index-Buffer sperren.
	// Die Sperrflags sind 0 bei statischen Index-Buffern und
	// D3DLOCK_DISCARD bei dynamischen.
	if(FAILED(hResult = m_pIndexBuffer->Lock(m_dwFirstIndex * m_dwIndexSize,
		                                     (m_dwLastIndex - m_dwFirstIndex + 1) * m_dwIndexSize,
											 &pIndices,
											 (m_dwUsage & D3DUSAGE_DYNAMIC) ? D3DLOCK_DISCARD : 0)))
	{
		// Fehler beim Sperren!
		AGE_ERROR_DIRECTX("m_pIndexBuffer->Lock", hResult, AGE_ERROR);
	}

	// Die Daten kopieren
	memcpy(pIndices, (BYTE*)(m_pBuffer) + m_dwFirstIndex * m_dwIndexSize,
		   (m_dwLastIndex - m_dwFirstIndex + 1) * m_dwIndexSize);

	// Index-Buffer entsperren
	m_pIndexBuffer->Unlock();

	// Ersten und letzten Index zurücksetzen
	m_dwFirstIndex = m_dwMaxIndices - 1;
	m_dwLastIndex = 0;

	return AGE_OK;
}

// ******************************************************************
// Diese Methode verändert die Größe des Index-Buffers.
Result IndexBuffer::SetSize(DWORD dwNewSize)
{
	HRESULT					hResult;
	PDIRECT3DINDEXBUFFER9	pIndexBuffer;
	DWORD					dwOldFirstIndex;
	DWORD					dwOldLastIndex;


	// Parameter prüfen und sicherstellen, dass eine Direct3D-Klasse existiert
	if(dwNewSize == m_dwSize)			return AGE_OK;
	if(dwNewSize == 0)					AGE_ERROR_INVALID_VALUE("dwNewSize", AGE_ERROR);
	if(!Direct3D::IsInitialized())	AGE_ERROR("Direct3D wurde noch nicht initialisiert!", AGE_ERROR);


	// Die neue Größe eintragen und die maximale Anzahl der Vertizes neu berechnen
	m_dwSize = dwNewSize;
	m_dwMaxIndices = m_dwSize / m_dwIndexSize;

	// Einen neuen Index-Buffer erstellen
	if(FAILED(hResult = Direct3D::GetDevice()->CreateIndexBuffer(m_dwSize,
		                                                           m_dwUsage,
																   m_IndexFormat,
																   m_Pool,
																   &pIndexBuffer,
																   NULL)))
	{
		// Fehler!
		AGE_ERROR_DIRECTX("Direct3D::GetDevice()->CreateIndexBuffer", hResult, AGE_ERROR);
	}

	// Die neue Schnittstelle eintragen, vorher den alten Index-Buffer freigeben
	AGE_SAFE_RELEASE(m_pIndexBuffer);
	m_pIndexBuffer = pIndexBuffer;

	// Die Größe der Speicherkopie anpassen
	m_pBuffer = MemReAlloc(m_pBuffer, dwNewSize);
	if(m_pBuffer == NULL)
	{
		// Fehler!
		AGE_ERROR_OUT_OF_MEMORY(AGE_ERROR);
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
		AGE_ERROR("Index-Buffer konnte nicht aktualisiert werden!", AGE_ERROR);
	}

	// Die ursprünglichen Werte von m_dwFirstIndex und m_dwLastIndex wiederherstellen
	// und auf die neue Größe anpassen
	m_dwFirstIndex = dwOldFirstIndex;
	m_dwLastIndex = dwOldLastIndex;
	if(m_dwFirstIndex >= m_dwMaxIndices) m_dwFirstIndex = m_dwMaxIndices - 1;
	if(m_dwLastIndex >= m_dwMaxIndices) m_dwLastIndex = m_dwMaxIndices - 1;

	// Die Cursorposition überprüfen
	if(m_dwCursor >= m_dwMaxIndices) m_dwCursor = m_dwMaxIndices - 1;

	return AGE_OK;
}
