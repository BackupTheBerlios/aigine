/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbGUIFrame.cpp
	==============
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Rahmen für die Benutzeroberfläche

	Autor:
	David Scherfgen

********************************************************************/

#include <TriBase.h>

// ******************************************************************
// Nachrichtenfunktion für einen Rahmen
tbResult tbGUIFrame::ReceiveMessage(const tbGUIMessage* pMsg)
{
	tbColor Color;

	switch(pMsg->Type)
	{
	case TB_GMT_RENDER:
		// Wenn das Element unsichtbar ist, wird es nicht gerendert.
		if(!m_bVisible) break;
		
		if(pMsg->aiInt[0] == 0)
		{
			// Die Farbe ergibt sich daraus, ob es aktiviert ist oder nicht.
			Color = m_bEnabled ? m_pGUI->m_Skin.EnabledColor : m_pGUI->m_Skin.DisabledColor;

			// Rahmen zeichnen
			m_pGUI->AddFrame(m_vPosition, m_vSize, Color, tbVector2(0.0f, 32.0f));
		}
		break;
	}

	return TB_OK;
}

// ******************************************************************
// Erstellung eines Rahmens
tbResult tbGUI::CreateFrame(int iID,
							int iPage,
							tbVector2 vPosition,
							tbVector2 vSize)
{
	tbGUIElement* pElement;

	// Rahmen erstellen
	pElement = CreateElement(iID, TB_GET_FRAME, iPage, vPosition, vSize);
	if(pElement == NULL) TB_ERROR("Fehler beim Erstellen des Rahmens!", TB_ERROR);

	return TB_OK;
}

// ******************************************************************