/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbGUIText.cpp
	=============
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Texte für die Benutzeroberfläche

	Autor:
	David Scherfgen

********************************************************************/

#include <TriBase.h>

// ******************************************************************
// Nachrichtenfunktion für einen Text
tbResult tbGUIText::ReceiveMessage(const tbGUIMessage* pMsg)
{
	tbColor Color;

	switch(pMsg->Type)
	{
	case TB_GMT_RENDER:
		// Wenn das Element unsichtbar ist, wird es nicht gerendert.
		if(!m_bVisible) break;

		if(pMsg->aiInt[0] == 1)
		{
			// Farbe berechnen
			Color = m_bEnabled ? m_Color : m_pGUI->m_Skin.DisabledTextColor;

			// Text zeichnen
			m_pGUI->m_Skin.pFont->DrawText(m_vPosition * m_pGUI->m_vScaling, m_acText,
				                           m_dwTextFlags, -1, Color, Color, m_vTextSize,
										   -2.0f, 0.0f, -6.0f, 25.0f * m_pGUI->m_vScaling.x);
		}
		break;
	}

	return TB_OK;
}

// ******************************************************************
// Erstellung eines Texts
tbResult tbGUI::CreateText(int iID,
						   int iPage,
						   tbVector2 vPosition,
						   char* pcText,
						   tbColor Color,		// = tbColor(1.0f)
						   tbVector2 vTextSize,	// = tbVector2(1.0f)
						   DWORD dwTextFlags)	// = TB_FF_RELATIVESCALING
{
	tbGUIElement*	pElement;
	tbGUIText*		pText;


	// Parameter anpassen
	if(pcText == NULL) pcText = "";

	// Text erstellen
	pElement = CreateElement(iID, TB_GET_TEXT, iPage, vPosition, tbVector2(32.0f, 32.0f));
	if(pElement == NULL) TB_ERROR("Fehler beim Erstellen des Texts!", TB_ERROR);

	// Daten eintragen
	pText = (tbGUIText*)(pElement);
	strcpy(pText->m_acText, pcText);
	pText->m_Color = Color;
	pText->m_vTextSize = vTextSize;
	pText->m_dwTextFlags = dwTextFlags;

	return TB_OK;
}

// ******************************************************************