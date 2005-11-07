/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbGUIList.cpp
	==============
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Listen für die Benutzeroberfläche

	Autor:
	David Scherfgen

********************************************************************/

#include <TriBase.h>

// ******************************************************************
// Nachrichtenfunktion für eine Liste
tbResult tbGUIList::ReceiveMessage(const tbGUIMessage* pMsg)
{
	int								iEntry;
	tbColor							Color;
	tbListEntry<tbGUIListEntry>*	pEntry;
	tbGUIListEntry*					pEntry2;
	tbVector2						vPos;
	tbGUIMessage					Msg;


	switch(pMsg->Type)
	{
	case TB_GMT_ELEMENTCLICKED:
		pEntry2 = GetEntryByOrder(GetEntryOrder(m_iScrolling) + (int)(((pMsg->afFloat[3] - 5.0f) / m_fLineHeight)));
		if(pEntry2 != NULL) m_iCursor = pEntry2->iID;
		break;

	case TB_GMT_KEYDOWN:
	case TB_GMT_KEYREPEAT:
		if(m_pGUI->m_iFocus == m_iID ||
		   m_pGUI->m_iFocus == m_iScrollUpBtn ||
		   m_pGUI->m_iFocus == m_iScrollDnBtn)
		{
			switch(pMsg->aiInt[0])
			{
			case TB_KEY_UP:
				iEntry = GetPrevEntry(m_iCursor);
				if(iEntry != -1) m_iCursor = GetPrevEntry(m_iCursor);
				break;
			
			case TB_KEY_DOWN:
				iEntry = GetNextEntry(m_iCursor);
				if(iEntry != -1) m_iCursor = GetNextEntry(m_iCursor);
				break;
			}
		}
		break;

	case TB_GMT_MOVE:
		// Wurde der Cursor verändert? Wenn ja: Nachricht schicken
		if(m_iCursor != m_iOldCursor)
		{
			ZeroMemory(&Msg, sizeof(tbGUIMessage));
			Msg.Type = TB_GMT_SELECTION;
			Msg.aiInt[0] = m_iCursor;
			Msg.aiInt[1] = m_iOldCursor;
			m_pGUI->PostMessage(m_iID, &Msg);
			m_iOldCursor = m_iCursor;
		}
		break;

	case TB_GMT_RENDER:
		// Wenn das Element unsichtbar ist, wird es nicht gerendert.
		if(!m_bVisible) break;

		if(pMsg->aiInt[0] == 0)
		{
			// Die Farbe ergibt sich daraus, ob es aktiviert ist oder nicht.
			Color = m_bEnabled ? m_pGUI->m_Skin.EnabledColor : m_pGUI->m_Skin.DisabledColor;

			// Rahmen zeichnen
			m_pGUI->AddFrame(m_vPosition, m_vSize, Color, tbVector2(96.0f, 128.0f));
		}
		else if(pMsg->aiInt[0] == 1 &&
			    m_iCursor != -1 &&
				m_iScrolling != -1)
		{
			Correct();
			vPos = m_vPosition + tbVector2(5.0f);
			pEntry = GetEntryListEntry(m_iScrolling);
			while(pEntry != NULL &&
			      vPos.y + m_fLineHeight <= m_vPosition.y + m_vSize.y - 5.0f)
			{
				// Farbe bestimmen
				Color = m_bEnabled ? (pEntry->Data.iID == m_iCursor ? m_pGUI->m_Skin.HighlightTextColor : m_pGUI->m_Skin.EnabledColor) : m_pGUI->m_Skin.DisabledColor;
				m_pGUI->m_Skin.pFont->DrawText(vPos * m_pGUI->m_vScaling, pEntry->Data.acText, TB_FF_RELATIVESCALING, -1,
											   Color, Color, m_pGUI->m_Skin.vTextSize);
				pEntry = pEntry->pNextEntry;
				vPos.y += m_fLineHeight;
			}
		}
		break;
	}

	return TB_OK;
}

// ******************************************************************
// Liefert einen Listeneintrag
tbGUIListEntry* tbGUIList::GetEntry(int iID)
{
	static tbListEntry<tbGUIListEntry>* pEntry;

	// Die gesamte Liste durchgehen und suchen
	pEntry = m_pList->GetFirstEntry();
	while(pEntry != NULL)
	{
		if(pEntry->Data.iID == iID)
		{
			// Gefunden!
			return &pEntry->Data;
		}

		pEntry = pEntry->pNextEntry;
	}

	return NULL;
}

// ******************************************************************
// Liefert einen Listeneintrag
tbListEntry<tbGUIListEntry>* tbGUIList::GetEntryListEntry(int iID)
{
	static tbListEntry<tbGUIListEntry>* pEntry;

	// Die gesamte Liste durchgehen und suchen
	pEntry = m_pList->GetFirstEntry();
	while(pEntry != NULL)
	{
		if(pEntry->Data.iID == iID)
		{
			// Gefunden!
			return pEntry;
		}

		pEntry = pEntry->pNextEntry;
	}

	return NULL;
}

// ******************************************************************
// Fügt einen Eintrag hinzu
int tbGUIList::AddEntry(char* pcText,
						void* pData)
{
	static tbGUIListEntry NewEntry;

	if(pcText == NULL) pcText = "";

	// Daten eintragen
	NewEntry.iID = m_iNextEntryID++;
	strcpy(NewEntry.acText, pcText);
	NewEntry.pData = pData;

	// Zur Liste hinzufügen
	m_pList->AddEntry(&NewEntry);

	// Den Cursor auf diesen Eintrag setzen
	m_iCursor = NewEntry.iID;

	if(m_iScrolling == -1) m_iScrolling = m_iCursor;
	Correct();

	return NewEntry.iID;
}

// ******************************************************************
// Löscht einen Eintrag
tbResult tbGUIList::DeleteEntry(int iID)
{
	tbGUIListEntry*					pEntry;
	tbListEntry<tbGUIListEntry>*	pListEntry;


	// Eintrag suchen
	pEntry = GetEntry(iID);
	if(pEntry == NULL) return TB_ERROR;

	// In der Liste suchen
	m_pList->FindEntry(pEntry, &pListEntry);
	if(pListEntry == NULL) return TB_ERROR;

	// Cursor neu setzen
	if(pListEntry->pNextEntry != NULL) m_iCursor = pListEntry->pNextEntry->Data.iID;
	else if(pListEntry->pPrevEntry != NULL) m_iCursor = pListEntry->pPrevEntry->Data.iID;
	else
	{
		m_iCursor = -1;
		m_iScrolling = -1;
	}

	// Eintrag entfernen
	m_pList->DeleteEntry(pListEntry);

	Correct();

	return TB_OK;
}

// ******************************************************************
// Liefert die Ordnung eines Listeneintrags
int tbGUIList::GetEntryOrder(int iID)
{
	static tbListEntry<tbGUIListEntry>* pEntry;
	int iOrder = 0;


	// Die gesamte Liste durchgehen und suchen
	pEntry = m_pList->GetFirstEntry();
	while(pEntry != NULL)
	{
		if(pEntry->Data.iID == iID)
		{
			// Gefunden!
			return iOrder;
		}

		pEntry = pEntry->pNextEntry;
		iOrder++;
	}

	return -1;
}

// ******************************************************************
// Liefert den Eintrag einer bestimmten Ordnung
tbGUIListEntry* tbGUIList::GetEntryByOrder(int iOrder)
{
	static tbListEntry<tbGUIListEntry>* pEntry;
	int iCurrentOrder = 0;


	// Die gesamte Liste durchgehen und suchen
	pEntry = m_pList->GetFirstEntry();
	while(pEntry != NULL)
	{
		if(iCurrentOrder == iOrder) return &pEntry->Data;

		pEntry = pEntry->pNextEntry;
		iCurrentOrder++;
	}

	return NULL;
}

// ******************************************************************
// Liefert vorherigen Eintrag
int tbGUIList::GetPrevEntry(int iID)
{
	static tbListEntry<tbGUIListEntry>* pEntry;

	// Die gesamte Liste durchgehen und suchen
	pEntry = m_pList->GetFirstEntry();
	while(pEntry != NULL)
	{
		if(pEntry->Data.iID == iID)
		{
			// Gefunden!
			if(pEntry->pPrevEntry != NULL) return pEntry->pPrevEntry->Data.iID;
			else return -1;
		}
		
		pEntry = pEntry->pNextEntry;
	}

	return -1;
}

// ******************************************************************
// Liefert nächsten Eintrag
int tbGUIList::GetNextEntry(int iID)
{
	static tbListEntry<tbGUIListEntry>* pEntry;

	// Die gesamte Liste durchgehen und suchen
	pEntry = m_pList->GetFirstEntry();
	while(pEntry != NULL)
	{
		if(pEntry->Data.iID == iID)
		{
			// Gefunden!
			if(pEntry->pNextEntry != NULL) return pEntry->pNextEntry->Data.iID;
			else return -1;
		}

		pEntry = pEntry->pNextEntry;
	}

	return -1;
}

// ******************************************************************
// Korrigiert eventuelle Fehler
tbResult tbGUIList::Correct()
{
	int iMaxEntriesVisible;

	// Zeigt der Cursor auf einen existierenden Eintrag?
	while(GetEntry(m_iCursor) == NULL &&
	      m_iCursor >= 0)
	{
		m_iCursor--;
	}

	while(GetEntry(m_iCursor) == NULL &&
	      m_iCursor < m_iNextEntryID)
	{
		m_iCursor++;
	}

	// Es gibt anscheinend gar keine Einträge!
	if(GetEntry(m_iCursor) == NULL)
	{
		m_iCursor = -1;
		m_iScrolling = -1;
	}
	else
	{
		// Zeigt m_iScrolling auf einen existierenden Eintrag?
		while(GetEntry(m_iScrolling) == NULL &&
			  m_iScrolling >= 0)
		{
			m_iScrolling--;
		}

		while(GetEntry(m_iScrolling) == NULL &&
			  m_iScrolling < m_iNextEntryID)
		{
			m_iScrolling++;
		}

		// Maximale Anzahl der sichtbaren Einträge berechnen
		iMaxEntriesVisible = (int)((m_vSize.y - 10.0f) / m_fLineHeight);

		// Ist der Cursor vielleicht unsichtbar?
		if(GetEntryOrder(m_iCursor) < GetEntryOrder(m_iScrolling)) m_iScrolling = m_iCursor;
		while(GetEntryOrder(m_iCursor) >= GetEntryOrder(m_iScrolling) + iMaxEntriesVisible)	m_iScrolling = GetNextEntry(m_iScrolling);
	}

	return TB_OK;
}

// ******************************************************************
// Erstellung einer Liste
tbResult tbGUI::CreateList(int iID,
						   int iPage,
						   tbVector2 vPosition,
						   tbVector2 vSize,
						   float fLineHeight)
{
	tbGUIElement*	pElement;
	tbGUIList*		pList;


	// Liste erstellen
	pElement = CreateElement(iID, TB_GET_LIST, iPage, vPosition, vSize);
	if(pElement == NULL) TB_ERROR("Fehler beim Erstellen der Liste!", TB_ERROR);

	// Daten eintragen
	pList = (tbGUIList*)(pElement);
	pList->m_pList = new tbList<tbGUIListEntry>;
	pList->m_iCursor = -1;
	pList->m_iScrolling = -1;
	pList->m_iNextEntryID = 0;
	pList->m_fLineHeight = fLineHeight;
	pList->m_iOldCursor = -1;

	// Knopf zum Hochscrollen erstellen
	pList->m_iScrollUpBtn = -iID * 10; CreateButton(-iID * 10, iPage, vPosition + tbVector2(vSize.x, 0.0f), tbVector2(32.0f), "");
	((tbGUIButton*)(GetElement(-iID * 10)))->m_bIsListScrollBtn = TRUE;
	((tbGUIButton*)(GetElement(-iID * 10)))->m_bIsScrollUpBtn = TRUE;
	((tbGUIButton*)(GetElement(-iID * 10)))->m_iList = iID;

	// Knopf zum Runterscrollen erstellen
	pList->m_iScrollDnBtn = -iID * 10 - 1; CreateButton(-iID * 10 - 1, iPage, vPosition + tbVector2(vSize.x, vSize.y - 32.0f), tbVector2(32.0f), "");
	((tbGUIButton*)(GetElement(-iID * 10 - 1)))->m_bIsListScrollBtn = TRUE;
	((tbGUIButton*)(GetElement(-iID * 10 - 1)))->m_iList = iID;

	return TB_OK;
}

// ******************************************************************
