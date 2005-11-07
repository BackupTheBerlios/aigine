/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbGUIButton.cpp
	===============
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Knöpfe für die Benutzeroberfläche

	Autor:
	David Scherfgen

********************************************************************/

#include <TriBase.h>

// ******************************************************************
// Nachrichtenfunktion für einen Knopf
tbResult tbGUIButton::ReceiveMessage(const tbGUIMessage* pMsg)
{
	tbColor Color;

	switch(pMsg->Type)
	{
	case TB_GMT_MOVE:
		if(m_bIsListScrollBtn &&
		   m_bPressed)
		{
			// Tastendruck simulieren
			m_pGUI->m_pbButtons[m_bIsScrollUpBtn ? TB_KEY_UP : TB_KEY_DOWN] = TRUE;

			// Fokus auf die Liste setzen
			m_pGUI->m_iFocus = m_iList;
		}
		break;

	case TB_GMT_RENDER:
		// Wenn das Element unsichtbar ist, wird es nicht gerendert.
		if(!m_bVisible) break;

		if(pMsg->aiInt[0] == 0)
		{
			// Die Farbe ergibt sich daraus, ob es aktiviert ist oder nicht.
			// Außerdem ändert sie sich, wenn der Knopf gerade gedrückt wird.
			Color = m_bEnabled ? (m_bPressed ? m_pGUI->m_Skin.HighlightColor : m_pGUI->m_Skin.EnabledColor) : m_pGUI->m_Skin.DisabledColor;

			if(m_bIsListScrollBtn)
			{
				// Scrollknopf erstellen
				m_pGUI->AddRect(m_vPosition, m_vSize, Color,
					            tbVector2(m_bIsScrollUpBtn ? 192.0f : 224.0f, 0.0f) / 255.0f,
								tbVector2(32.0f) / 255.0f);
			}
			else
			{
				// Rahmen zeichnen
				m_pGUI->AddFrame(m_vPosition, m_vSize, Color, tbVector2(96.0f, 32.0f));
			}
		}
		else if(pMsg->aiInt[0] == 1 &&
		        !m_bIsListScrollBtn)
		{
			// Die Farbe ergibt sich daraus, ob es aktiviert ist oder nicht.
			// Außerdem ändert sie sich, wenn der Knopf gerade gedrückt wird.
			Color = m_bEnabled ? (m_bPressed ? m_pGUI->m_Skin.HighlightTextColor : m_pGUI->m_Skin.EnabledTextColor) : m_pGUI->m_Skin.DisabledTextColor;

			// Text zeichnen
			m_pGUI->m_Skin.pFont->DrawText((m_vPosition + 0.5f * m_vSize) * m_pGUI->m_vScaling, m_acText,
				                           TB_FF_ALIGN_HCENTER | TB_FF_ALIGN_VCENTER | TB_FF_RELATIVESCALING, -1,
										   Color, Color, m_pGUI->m_Skin.vTextSize);
		}
		break;
	}

	return TB_OK;
}

// ******************************************************************
// Erstellung eines Knopfes
tbResult tbGUI::CreateButton(int iID,
							 int iPage,
							 tbVector2 vPosition,
							 tbVector2 vSize,
							 char* pcText)
{
	tbGUIElement*	pElement;
	tbGUIButton*	pButton;


	// Parameter anpassen
	if(pcText == NULL) pcText = "";

	// Knopf erstellen
	pElement = CreateElement(iID, TB_GET_BUTTON, iPage, vPosition, vSize);
	if(pElement == NULL) TB_ERROR("Fehler beim Erstellen des Knopfes!", TB_ERROR);

	// Daten eintragen
	pButton = (tbGUIButton*)(pElement);
	strcpy(pButton->m_acText, pcText);
	pButton->m_bIsListScrollBtn = FALSE;
	pButton->m_bIsScrollUpBtn = FALSE;
	pButton->m_iList = -1;

	return TB_OK;
}

// ******************************************************************