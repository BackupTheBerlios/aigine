// IndexBuffer.h: interface for the IndexBuffer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include "../AiGinE.h"

// Klasse für einen Index-Buffer
class IndexBuffer
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
	IndexBuffer();
	~IndexBuffer();

	// Methoden
	Result Init(DWORD dwSize, DWORD dwIndexSize, D3DFORMAT IndexFormat, DWORD dwUsage = D3DUSAGE_WRITEONLY, D3DPOOL Pool = D3DPOOL_DEFAULT);	// Initialisierung
	Result Init(PDIRECT3DINDEXBUFFER9 pIndexBuffer, DWORD dwIndexSize);																		// Initialisierung aus einem Index-Buffer
	Result Update();																															// Speichert die Änderungen im Index-Buffer
	Result SetSize(DWORD dwNewSize);																											// Setzt die Größe des Index-Buffers

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
	inline Result GetIndex(const DWORD dwIndex,
		                     void* pOut)
	{
#ifdef _DEBUG
		// Parameter prüfen
		if(dwIndex >= m_dwMaxIndices)	AGE_ERROR_INVALID_VALUE("dwIndex", AGE_ERROR);
		if(pOut == NULL)				AGE_ERROR_NULL_POINTER("pOut", AGE_ERROR);
#endif

		// Index zurückliefern
		memcpy(pOut, (BYTE*)(m_pBuffer) + dwIndex * m_dwIndexSize, m_dwIndexSize);

		return AGE_OK;
	}

	// Abfragen mehrerer Vertizes
	inline Result GetIndices(const DWORD dwStart,
							   const DWORD dwNumIndices,
							   void* pOut)
	{
#ifdef _DEBUG
		// Parameter prüfen
		if(dwStart + dwNumIndices > m_dwMaxIndices ||
		   dwNumIndices == 0 ||
		   pOut == NULL)
		{
			// Fehler!
			AGE_ERROR("Ungültige Angabe!", AGE_ERROR);
		}
#endif

		// Vertizes zurückliefern
		memcpy(pOut, (BYTE*)(m_pBuffer) + dwStart * m_dwIndexSize,
			   dwNumIndices * m_dwIndexSize);

		return AGE_OK;
	}

	// Setzen eines Index
	inline Result SetIndex(const DWORD dwIndex,
							 const void* pIndex)
	{
#ifdef _DEBUG
		// Parameter prüfen
		if(dwIndex >= m_dwMaxIndices)	AGE_ERROR_INVALID_VALUE("dwIndex", AGE_ERROR);
		if(pIndex == NULL)				AGE_ERROR_NULL_POINTER("pIndex", AGE_ERROR);
#endif

		// Den Index eintragen
		memcpy((BYTE*)(m_pBuffer) + dwIndex * m_dwIndexSize, pIndex, m_dwIndexSize);

		// m_dwFirstIndex und m_dwLastIndex aktualisieren
		if(dwIndex < m_dwFirstIndex) m_dwFirstIndex = dwIndex;
		if(dwIndex > m_dwLastIndex) m_dwLastIndex = dwIndex;

		return AGE_OK;
	}

	// Setzen mehrerer Vertizes
	inline Result SetIndices(const DWORD dwStart,
							   const DWORD dwNumIndices,
							   const void* pIndices)
	{
#ifdef _DEBUG
		// Parameter prüfen
		if(dwStart + dwNumIndices > m_dwMaxIndices ||
		   pIndices == NULL)
		{
			// Fehler
			AGE_ERROR("Ungültige Angabe!", AGE_ERROR);
		}
#endif

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
#ifdef _DEBUG
		// Die Cursorposition prüfen
		if(m_dwCursor >= m_dwMaxIndices)
		{
			// Fehler!
			AGE_ERROR("Ungültige Cursorposition!", 0xFFFFFFFF);
		}
#endif
		
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
#ifdef _DEBUG
		// Parameter prüfen
		if(dwNumIndices == 0)	AGE_ERROR_INVALID_VALUE("dwNumIndices", 0xFFFFFFFF);
		if(pIndices == NULL)	AGE_ERROR_NULL_POINTER("pIndices", 0xFFFFFFFF);

		// Die Cursorposition prüfen
		if(m_dwCursor + dwNumIndices > m_dwMaxIndices)
		{
			// Fehler!
			AGE_ERROR("Ungültige Cursorposition - Index-Buffer ist voll!", 0xFFFFFFFF);
		}
#endif

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
	inline Result SetCursor(const DWORD dwCursor)
	{
#ifdef _DEBUG
		// Die Cursorposition prüfen
		if(dwCursor >= m_dwMaxIndices)
		{
			// Fehler!
			AGE_ERROR("Ungültige Cursorposition!", AGE_ERROR);
		}
#endif

		// Den Cursor setzen
		m_dwCursor = dwCursor;

		return AGE_OK;
	}
};

#endif //INDEXBUFFER_H
