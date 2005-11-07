/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbGUICheckBox.cpp
	=================
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Check-Boxes für die Benutzeroberfläche

	Autor:
	David Scherfgen

********************************************************************/

#include <TriBase.h>

// ******************************************************************
// Nachrichtenfunktion für eine Check-Box
tbResult tbGUICheckBox::ReceiveMessage(const tbGUIMessage* pMsg)
{
	tbColor Color;

	switch(pMsg->Type)
	{
	case TB_GMT_ELEMENTCLICKED:
		// Status ändern
		m_bChecked = !m_bChecked;
		break;

	case TB_GMT_RENDER:
		// Wenn das Element unsichtbar ist, wird es nicht gerendert.
		if(!m_bVisible) break;

		if(pMsg->aiInt[0] == 0)
		{
			// Die Farbe ergibt sich daraus, ob es aktiviert ist oder nicht.
			// Außerdem ändert sie sich, wenn der Knopf gerade gedrückt wird.
			Color = m_bEnabled ? (m_bPressed ? m_pGUI->m_Skin.HighlightColor : m_pGUI->m_Skin.EnabledColor) : m_pGUI->m_Skin.DisabledColor;

			// Check-Box zeichnen - je nach dem, ob sie angekreuz ist oder nicht
			m_pGUI->AddRect(m_vPosition, tbVector2(32.0f, 32.0f), Color,
						    (m_bChecked ? tbVector2(96.0f, 0.0f) : tbVector2(64.0f, 0.0f)) / 255.0f,
						    tbVector2(32.0f, 32.0f) / 255.0f);
		}
		else if(pMsg->aiInt[0] == 1)
		{
			// Die Farbe ergibt sich daraus, ob die Check-Box aktiviert ist oder nicht.
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
// Erstellung einer Check-Box
tbResult tbGUI::CreateCheckBox(int iID,
							   int iPage,
							   tbVector2 vPosition,
							   tbVector2 vSize,
							   char* pcText,
							   BOOL bChecked) // = FALSE
{
	tbGUIElement*	pElement;
	tbGUICheckBox*	pCheckBox;


	// Parameter anpassen
	if(pcText == NULL) pcText = "";

	// Check-Box erstellen
	pElement = CreateElement(iID, TB_GET_CHECKBOX, iPage, vPosition, vSize);
	if(pElement == NULL) TB_ERROR("Fehler beim Erstellen der Check-Box!", TB_ERROR);

	// Daten eintragen
	pCheckBox = (tbGUICheckBox*)(pElement);
	strcpy(pCheckBox->m_acText, pcText);
	pCheckBox->m_bChecked = bChecked;

	return TB_OK;
}

// ******************************************************************