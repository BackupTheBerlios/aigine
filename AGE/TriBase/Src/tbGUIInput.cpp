/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbGUIInput.cpp
	==============
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Eingabefelder für die Benutzeroberfläche

	Autor:
	David Scherfgen

********************************************************************/

#include <TriBase.h>

// ******************************************************************
// Nachrichtenfunktion für ein Eingabefeld
tbResult tbGUIInput::ReceiveMessage(const tbGUIMessage* pMsg)
{
	tbColor	Color;
	int		iNumCharsToDisplay;
	float	fCursor;
	float	fTextWidth;
	float	fDiff;
	float	fMinDiff;
	char	cChar;
	char	acText[4096];


	switch(pMsg->Type)
	{
	case TB_GMT_ELEMENTCLICKED:
		{
		memcpy(acText, m_acText, 4096 * sizeof(char));
		if(m_bPassword) for(int i = 0; i < 4096 && acText[i] != 0; i++) acText[i] = m_PasswordChar;

		// Die Stelle finden, an der geklickt wurde
		fMinDiff = 100000.0f;
		for(int i = m_iScrolling; i <= (int)(strlen(acText)); i++)
		{
			fTextWidth = m_pGUI->m_Skin.pFont->ComputeRealTextWidth(tbVector2(0.0f), &acText[m_iScrolling], TB_FF_RELATIVESCALING, i - m_iScrolling,
				                                                    m_pGUI->m_Skin.vTextSize);
			fDiff = fabsf((fTextWidth + (m_vPosition.x + 5.0f) * m_pGUI->m_vScaling.x) - (pMsg->afFloat[0] * m_pGUI->m_vScaling.x));
			if(fDiff < fMinDiff)
			{
				fMinDiff = fDiff;
				m_iCursor = i;
			}
		}
		}
		break;

	case TB_GMT_KEYDOWN:
	case TB_GMT_KEYREPEAT:
		if(m_pGUI->m_iFocus == m_iID)
		{
			switch(pMsg->aiInt[0])
			{
			case TB_KEY_DELETE:
			case TB_KEY_DECIMAL:
				{
				// Zeichen löschen
				for(int i = m_iCursor; i < (int)(strlen(m_acText)); i++) m_acText[i] = m_acText[i + 1];
				}
				break;

			case TB_KEY_BACK:
				if(m_iCursor > 0)
				{
					// Zeichen löschen
					for(int i = m_iCursor - 1; i < (int)(strlen(m_acText)); i++) m_acText[i] = m_acText[i + 1];
					m_iCursor--;
				}
				break;

			case TB_KEY_LEFT:
			case TB_KEY_NUMPAD4:
				m_iCursor--;
				break;

			case TB_KEY_RIGHT:
			case TB_KEY_NUMPAD6:
				m_iCursor++;
				break;

			case TB_KEY_HOME:
			case TB_KEY_NUMPAD7:
				m_iCursor = 0;
				break;

			case TB_KEY_END:
			case TB_KEY_NUMPAD1:
				m_iCursor = strlen(m_acText);
				break;

			default:
				if(strlen(m_acText) < 4095)
				{
					cChar = tbKeyToChar(pMsg->aiInt[0], pMsg->aiInt[1], pMsg->aiInt[2]);
					if(cChar != 0)
					{
						for(int i = 4095; i >= m_iCursor + 1; i--) m_acText[i] = m_acText[i - 1];
						m_acText[m_iCursor] = cChar;
						m_iCursor++;
					}
				}
				break;
			}
		}
		break;

	case TB_GMT_RENDER:
		// Wenn das Element unsichtbar ist, wird es nicht gerendert.
		if(!m_bVisible) break;

		if(pMsg->aiInt[0] == 0)
		{
			// Die Farbe ergibt sich daraus, ob es aktiviert ist oder nicht.
			// Außerdem ändert sie sich, wenn das Element den Fokus hat.
			Color = m_bEnabled ? (m_pGUI->m_iFocus == m_iID ? m_pGUI->m_Skin.HighlightColor : m_pGUI->m_Skin.EnabledColor) : m_pGUI->m_Skin.DisabledColor;

			// Rahmen zeichnen
			m_pGUI->AddFrame(m_vPosition, m_vSize, Color, tbVector2(0.0f, 128.0f));
		}
		else if(pMsg->aiInt[0] == 1)
		{
			memcpy(acText, m_acText, 4096 * sizeof(char));
			if(m_bPassword) for(int i = 0; i < 4096 && acText[i] != 0; i++) acText[i] = m_PasswordChar;

			// Farbe berechnen
			Color = m_bEnabled ? (m_pGUI->m_iFocus == m_iID ? m_pGUI->m_Skin.HighlightTextColor : m_pGUI->m_Skin.EnabledTextColor) : m_pGUI->m_Skin.DisabledTextColor;

			// Berechnen, wie viele Zeichen dargestellt werden sollen
			for(int i = strlen(m_acText) - m_iScrolling; i > 0; i--)
			{
				if(m_pGUI->m_Skin.pFont->ComputeRealTextWidth(tbVector2(0.0f), &acText[m_iScrolling], TB_FF_ALIGN_VCENTER | TB_FF_RELATIVESCALING, i,
					                                          m_pGUI->m_Skin.vTextSize) <
				   (m_vSize.x - 10.0f) * m_pGUI->m_vScaling.x)
				{
					iNumCharsToDisplay = i;
					break;
				}
			}

			// Text zeichnen
			m_pGUI->m_Skin.pFont->DrawText((m_vPosition + tbVector2(5.0f, m_vSize.y / 2.0f)) * m_pGUI->m_vScaling, &acText[m_iScrolling],
				                           TB_FF_ALIGN_VCENTER | TB_FF_RELATIVESCALING, iNumCharsToDisplay, Color, Color, m_pGUI->m_Skin.vTextSize);

			// Cursor zeichnen (blinkend)
			if(m_pGUI->m_iFocus == m_iID)
			{
				// Position des Cursors berechnen
				fCursor = m_pGUI->m_Skin.pFont->ComputeRealTextWidth(tbVector2(0.0f), &acText[m_iScrolling], TB_FF_RELATIVESCALING, m_iCursor - m_iScrolling,
					                                                 m_pGUI->m_Skin.vTextSize);

				// Cursor zeichnen
				Color.a *= fabsf(sinf(tb_g_fTime * 8.0f));
				m_pGUI->m_Skin.pFont->DrawText((m_vPosition + tbVector2(0.0f, m_vSize.y / 2.0f)) * m_pGUI->m_vScaling + tbVector2(fCursor, 0.0f), "|",
					                           TB_FF_ALIGN_VCENTER | TB_FF_RELATIVESCALING, -1, Color, Color, m_pGUI->m_Skin.vTextSize);
			}
		}
		break;
	}

	if(m_iCursor < 0) m_iCursor = 0;
	if(m_iCursor > (int)(strlen(m_acText))) m_iCursor = strlen(m_acText);
	if(m_iCursor < m_iScrolling) m_iScrolling = m_iCursor - 5;
	if(m_iScrolling < 0) m_iScrolling = 0;
	if(m_iScrolling > (int)(strlen(m_acText))) m_iScrolling = strlen(m_acText) - 5;

	memcpy(acText, m_acText, 4096 * sizeof(char));
	if(m_bPassword) for(int i = 0; i < 4096 && acText[i] != 0; i++) acText[i] = m_PasswordChar;

	// Scrolling
	fCursor = 100000.0f;
	m_iScrolling--;
	while(fCursor > (m_vSize.x - 10.0f) * m_pGUI->m_vScaling.x)
	{
		fCursor = m_pGUI->m_Skin.pFont->ComputeRealTextWidth(tbVector2(0.0f), &acText[m_iScrolling], TB_FF_RELATIVESCALING, m_iCursor - m_iScrolling,
				                                             m_pGUI->m_Skin.vTextSize);
		m_iScrolling++;
	}

	return TB_OK;
}

// ******************************************************************
// Erstellung eines Eingabefelds
tbResult tbGUI::CreateInput(int iID,
							int iPage,
							tbVector2 vPosition,
							tbVector2 vSize,
							char* pcText,		// = ""
							BOOL bPassword,		// = FALSE
							char PasswordChar)	// = '*'
{
	tbGUIElement*	pElement;
	tbGUIInput*		pInput;


	// Parameter anpassen
	if(pcText == NULL) pcText = "";

	// Eingabefeld erstellen
	pElement = CreateElement(iID, TB_GET_INPUT, iPage, vPosition, vSize);
	if(pElement == NULL) TB_ERROR("Fehler beim Erstellen des Eingabefelds!", TB_ERROR);

	// Daten eintragen
	pInput = (tbGUIInput*)(pElement);
	strcpy(pInput->m_acText, pcText);
	pInput->m_bPassword = bPassword;
	pInput->m_PasswordChar = PasswordChar;
	pInput->m_iCursor = strlen(pcText);
	pInput->m_iScrolling = 0;

	return TB_OK;
}

// ******************************************************************