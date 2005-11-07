/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbGUIRadioBox.cpp
	=================
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Radio-Boxes für die Benutzeroberfläche

	Autor:
	David Scherfgen

********************************************************************/

#include <TriBase.h>

// ******************************************************************
// Nachrichtenfunktion für eine Radio-Box
tbResult tbGUIRadioBox::ReceiveMessage(const tbGUIMessage* pMsg)
{
	tbColor Color;

	switch(pMsg->Type)
	{
	case TB_GMT_ELEMENTCLICKED:
		// Ankreuzen
		Check(TRUE);
		break;

	case TB_GMT_RENDER:
		// Wenn das Element unsichtbar ist, wird es nicht gerendert.
		if(!m_bVisible) break;

		if(pMsg->aiInt[0] == 0)
		{
			// Die Farbe ergibt sich daraus, ob es aktiviert ist oder nicht.
			// Außerdem ändert sie sich, wenn der Knopf gerade gedrückt wird.
			Color = m_bEnabled ? (m_bPressed ? m_pGUI->m_Skin.HighlightColor : m_pGUI->m_Skin.EnabledColor) : m_pGUI->m_Skin.DisabledColor;

			// Radio-Box zeichnen - je nach dem, ob sie angekreuz ist oder nicht
			m_pGUI->AddRect(m_vPosition, tbVector2(32.0f, 32.0f), Color,
						    (m_bChecked ? tbVector2(160.0f, 0.0f) : tbVector2(128.0f, 0.0f)) / 255.0f,
						    tbVector2(32.0f, 32.0f) / 255.0f);
		}
		else if(pMsg->aiInt[0] == 1)
		{
			// Die Farbe ergibt sich daraus, ob die Radio-Box aktiviert ist oder nicht.
			// Außerdem ändert sie sich, wenn sie gerade gedrückt wird.
			Color = m_bEnabled ? (m_bPressed ? m_pGUI->m_Skin.HighlightTextColor : m_pGUI->m_Skin.EnabledTextColor) : m_pGUI->m_Skin.DisabledTextColor;

			// Text zeichnen
			m_pGUI->m_Skin.pFont->DrawText(tbVector2(m_vPosition.x + m_vSize.x + 10.0f, m_vPosition.y + 0.5f * m_vSize.y) * m_pGUI->m_vScaling,
				                           m_acText,
				                           TB_FF_ALIGN_VCENTER | TB_FF_RELATIVESCALING, -1,
										   Color, Color, m_pGUI->m_Skin.vTextSize);
		}
		break;
	}

	return TB_OK;
}

// ******************************************************************
// Ändert den Status einer Radio-Box
tbResult tbGUIRadioBox::Check(BOOL bCheck)
{
	tbListEntry<tbGUIElement*>* pEntry;

	// Alle anderen Radio-Boxes auf dieser Seite suchen, die der selben
	// Gruppe angehören und ihren Zustand auf "nicht angekreuzt" setzen
	pEntry = m_pGUI->m_pElements->GetFirstEntry();
	while(pEntry != NULL)
	{
		if(pEntry->Data->m_iPage == m_iPage &&
		   pEntry->Data->m_Type == TB_GET_RADIOBOX)
		{
			if(((tbGUIRadioBox*)(pEntry->Data))->m_iGroup == m_iGroup)
			{
				// Status dieser Radio-Box setzen
				((tbGUIRadioBox*)(pEntry->Data))->m_bChecked = FALSE;
			}
		}

		pEntry = pEntry->pNextEntry;
	}

	m_bChecked = bCheck;

	return TB_OK;
}

// ******************************************************************
// Erstellung einer Radio-Box
tbResult tbGUI::CreateRadioBox(int iID,
							   int iPage,
							   tbVector2 vPosition,
							   tbVector2 vSize,
							   char* pcText,
							   int iGroup,		// = 0
							   BOOL bChecked)	// = FALSE
{
	tbGUIElement*	pElement;
	tbGUIRadioBox*	pRadioBox;


	// Parameter anpassen
	if(pcText == NULL) pcText = "";

	// Radio-Box erstellen
	pElement = CreateElement(iID, TB_GET_RADIOBOX, iPage, vPosition, vSize);
	if(pElement == NULL) TB_ERROR("Fehler beim Erstellen der Radio-Box!", TB_ERROR);

	// Daten eintragen
	pRadioBox = (tbGUIRadioBox*)(pElement);
	strcpy(pRadioBox->m_acText, pcText);
	pRadioBox->m_bChecked = bChecked;
	pRadioBox->m_iGroup = iGroup;

	return TB_OK;
}

// ******************************************************************