/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`�__\ /\ \ \ \  __\ /\ __\_ /\  __\   /�__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbGUI.cpp
	=========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Eine grafische Benutzeroberfl�che

	Autor:
	David Scherfgen

********************************************************************/

#include <TriBase.h>

// ******************************************************************
// Konstruktor der tbGUI-Klasse
tbGUI::tbGUI()
{
	// Alles zur�cksetzen
	ZeroMemory(this, sizeof(tbGUI));
}

// ******************************************************************
// Destruktor der tbGUI-Klasse
tbGUI::~tbGUI()
{
	tbListEntry<tbGUIElement*>* pEntry;
	tbListEntry<tbGUIElement*>* pNext;


	// Jedes Element l�schen
	pEntry = m_pElements->GetFirstEntry();
	while(pEntry != NULL)
	{
		pNext = pEntry->pNextEntry;
		DeleteElement(pEntry->Data->GetID());
		pEntry = pNext;
	}

	// Die Liste selbst l�schen
	TB_SAFE_DELETE(m_pElements);

	// Vertex-Buffer l�schen
	TB_SAFE_DELETE(m_pVertexBuffer);

	// Speicherplatz f�r die analogen Kn�pfe l�schen
	TB_SAFE_MEMFREE(m_pbButtons);
	TB_SAFE_MEMFREE(m_pbOldButtons);
	TB_SAFE_MEMFREE(m_pfButtons);
}

// ******************************************************************
// Initialisiert die Oberfl�che
tbResult tbGUI::Init(tbGUISkin* pSkin,
					 void* pMsgProc,
					 tbVector2 vSize,
					 tbVector2 vScaling) // = tbVector2(1.0f)
{
	// Parameter pr�fen
	if(pSkin == NULL) TB_ERROR_NULL_POINTER("pSkin", TB_ERROR);

	// Sicherstellen, dass Direct3D und DirectInput korrekt initialisiert wurden
	if(!tbDirect3D::IsInitialized()) TB_ERROR("Direct3D wurde noch nicht initialisiert!", TB_ERROR);
	if(!tbDirectInput::IsInitialized()) TB_ERROR("Initialisieren Sie zuerst DirectInput!", TB_ERROR);


	// Liste f�r die Elemente erstellen
	m_pElements = new tbList<tbGUIElement*>;

	// Vertex-Buffer f�r 1200 Vertizes erstellen
	m_pVertexBuffer = new tbVertexBuffer;
	if(m_pVertexBuffer->Init(1200 * sizeof(tbGUIVertex), sizeof(tbGUIVertex),
							 TB_GUI_FVF))
	{
		// Fehler!
		TB_ERROR("Fehler beim Erstellen des Vertex-Buffers!", TB_ERROR);
	}

	// Platz f�r alle analogen Kn�pfe reservieren
	m_pbButtons = (BOOL*)(tbMemAlloc(tbDirectInput::GetNumButtons() * sizeof(BOOL)));
	m_pbOldButtons = (BOOL*)(tbMemAlloc(tbDirectInput::GetNumButtons() * sizeof(BOOL)));
	m_pfButtons = (float*)(tbMemAlloc(tbDirectInput::GetNumButtons() * sizeof(float)));

	// Pr�fen
	if(m_pbButtons == NULL ||
	   m_pbOldButtons == NULL ||
	   m_pfButtons == NULL)
	{
		// Fehler!
		TB_ERROR_OUT_OF_MEMORY(TB_ERROR);
	}

	// Den Skin, die Nachrichtenfunktion, die Gr��e und die Skalierung kopieren
	m_Skin = *pSkin;
	m_pMsgProc = pMsgProc;
	m_vSize = vSize;
	m_vScaling = vScaling;

	// Mauszeiger in die Mitte setzen
	m_vCursorPos = m_vSize * 0.5f;

	return TB_OK;
}

// ******************************************************************
// Bewegen
tbResult tbGUI::Move(const float fTime,
					 BOOL* pbButtons,
					 float* pfButtons)
{
	static tbGUIMessage					Msg;
	static DWORD						dwTime;
	static tbListEntry<tbGUIElement*>*	pEntry;


	// Status der analogen Kn�pfe abfragen, alten Status kopieren
	memcpy(m_pbOldButtons, m_pbButtons, tbMemGetSize(m_pbButtons));
	memcpy(m_pbButtons, pbButtons, tbMemGetSize(m_pbButtons));
	memcpy(m_pfButtons, pfButtons, tbMemGetSize(m_pfButtons));

	// Zeit abfragen
	dwTime = timeGetTime();
	if(m_dwOldTime == 0) m_dwOldTime = dwTime;

	// Mauszeiger bewegen
	m_vCursorPos.x += (m_pfButtons[TB_MOUSE_X_POS] - m_pfButtons[TB_MOUSE_X_NEG]) * m_Skin.fCursorSpeed;
	m_vCursorPos.y += (m_pfButtons[TB_MOUSE_Y_POS] - m_pfButtons[TB_MOUSE_Y_NEG]) * m_Skin.fCursorSpeed;
	if(m_vCursorPos.x < 0.0f) m_vCursorPos.x = 0.0f;
	if(m_vCursorPos.y < 0.0f) m_vCursorPos.y = 0.0f;
	if(m_vCursorPos.x > m_vSize.x) m_vCursorPos.x = m_vSize.x;
	if(m_vCursorPos.y > m_vSize.y) m_vCursorPos.y = m_vSize.y;

	// Wurde der Mausknopf gedr�ckt?
	if(WasButtonPressed(TB_MOUSE_BUTTON(0)))
	{
		// Klicknachricht versenden
		ZeroMemory(&Msg, sizeof(tbGUIMessage));
		Msg.Type = TB_GMT_LBUTTONDOWN;
		Msg.afFloat[0] = m_vCursorPos.x;
		Msg.afFloat[1] = m_vCursorPos.y;
		BroadcastMessage(&Msg);

		// Nach Elementen suchen, die angeklickt wurden
		pEntry = m_pElements->GetFirstEntry();
		while(pEntry != NULL)
		{
			if((pEntry->Data->m_iPage == m_iCurrentPage || pEntry->Data->m_iPage == -1) &&
			   pEntry->Data->m_bVisible &&
			   pEntry->Data->m_bEnabled &&
			   m_vCursorPos.x > pEntry->Data->m_vPosition.x &&
			   m_vCursorPos.y > pEntry->Data->m_vPosition.y &&
			   m_vCursorPos.x < pEntry->Data->m_vPosition.x + pEntry->Data->m_vSize.x &&
			   m_vCursorPos.y < pEntry->Data->m_vPosition.y + pEntry->Data->m_vSize.y)
			{
				// Klickzustand ver�ndern
				pEntry->Data->m_bPressed = TRUE;
			}
			else
			{
				pEntry->Data->m_bPressed = FALSE;
			}

			pEntry = pEntry->pNextEntry;
		}
	}

	// Wurde der Mausknopf losgelassen?
	if(WasButtonReleased(TB_MOUSE_BUTTON(0)))
	{
		// Klicknachricht versenden
		ZeroMemory(&Msg, sizeof(tbGUIMessage));
		Msg.Type = TB_GMT_LBUTTONUP;
		Msg.afFloat[0] = m_vCursorPos.x;
		Msg.afFloat[1] = m_vCursorPos.y;
		BroadcastMessage(&Msg);

		// Nach Elementen suchen, die angeklickt wurden
		pEntry = m_pElements->GetFirstEntry();
		while(pEntry != NULL)
		{
			if((pEntry->Data->m_iPage == m_iCurrentPage || pEntry->Data->m_iPage == -1) &&
			   pEntry->Data->m_bPressed &&
			   m_vCursorPos.x > pEntry->Data->m_vPosition.x &&
			   m_vCursorPos.y > pEntry->Data->m_vPosition.y &&
			   m_vCursorPos.x < pEntry->Data->m_vPosition.x + pEntry->Data->m_vSize.x &&
			   m_vCursorPos.y < pEntry->Data->m_vPosition.y + pEntry->Data->m_vSize.y)
			{
				// Dieses Element wurde angeklickt! Fokus setzen und Nachricht schicken!
				m_iFocus = pEntry->Data->m_iID;

				ZeroMemory(&Msg, sizeof(tbGUIMessage));
				Msg.Type = TB_GMT_ELEMENTCLICKED;
				Msg.afFloat[0] = m_vCursorPos.x;
				Msg.afFloat[1] = m_vCursorPos.y;
				Msg.afFloat[2] = m_vCursorPos.x - pEntry->Data->m_vPosition.x;
				Msg.afFloat[3] = m_vCursorPos.y - pEntry->Data->m_vPosition.y;
				PostMessage(pEntry->Data->m_iID, &Msg);
			}

			pEntry->Data->m_bPressed = FALSE;		
			pEntry = pEntry->pNextEntry;
		}
	}

	// Rechten Mausknopf verarbeiten
	if(WasButtonPressed(TB_MOUSE_BUTTON(1)))
	{
		// Klicknachricht versenden
		ZeroMemory(&Msg, sizeof(tbGUIMessage));
		Msg.Type = TB_GMT_RBUTTONDOWN;
		Msg.afFloat[0] = m_vCursorPos.x;
		Msg.afFloat[1] = m_vCursorPos.y;
		BroadcastMessage(&Msg);
	}
	else if(WasButtonReleased(TB_MOUSE_BUTTON(1)))
	{
		// Klicknachricht versenden
		ZeroMemory(&Msg, sizeof(tbGUIMessage));
		Msg.Type = TB_GMT_RBUTTONUP;
		Msg.afFloat[0] = m_vCursorPos.x;
		Msg.afFloat[1] = m_vCursorPos.y;
		BroadcastMessage(&Msg);
	}

	// Allen Elementen eine Bewegungsnachricht schicken
	ZeroMemory(&Msg, sizeof(tbGUIMessage));
	Msg.Type = TB_GMT_MOVE;
	Msg.afFloat[0] = fTime;
	BroadcastMessage(&Msg);

	// Nachsehen, ob eine Taste auf der Tastatur gedr�ckt oder losgelassen wurde
	for(int iKey = 0; iKey < 108; iKey++)
	{
		if(m_pbButtons[iKey])
		{
			if(m_aiKeyRepeat[iKey] > 0)
			{
				m_aiKeyRepeat[iKey] -= dwTime - m_dwOldTime;
				if(m_aiKeyRepeat[iKey] <= 0)
				{
					// Wiederholungsnachricht schicken
					Msg.Type = TB_GMT_KEYREPEAT;
					Msg.aiInt[0] = iKey;
					Msg.aiInt[1] = m_pbButtons[TB_KEY_LSHIFT] || m_pbButtons[TB_KEY_RSHIFT];
					Msg.aiInt[2] = (m_pbButtons[TB_KEY_LALT] && (m_pbButtons[TB_KEY_LCONTROL] || m_pbButtons[TB_KEY_RCONTROL])) || m_pbButtons[TB_KEY_RALT];
					BroadcastMessage(&Msg);
					m_aiKeyRepeat[iKey] = 50;
				}
			}
		}

		if(WasButtonPressed(iKey))
		{
			// Nachricht schicken
			ZeroMemory(&Msg, sizeof(tbGUIMessage));
			Msg.Type = TB_GMT_KEYDOWN;
			Msg.aiInt[0] = iKey;
			Msg.aiInt[1] = m_pbButtons[TB_KEY_LSHIFT] || m_pbButtons[TB_KEY_RSHIFT];
			Msg.aiInt[2] = (m_pbButtons[TB_KEY_LALT] && (m_pbButtons[TB_KEY_LCONTROL] || m_pbButtons[TB_KEY_RCONTROL])) || m_pbButtons[TB_KEY_RALT];
			BroadcastMessage(&Msg);

			// Zeit setzen
			m_aiKeyRepeat[iKey] = 250;
		}
		else if(WasButtonReleased(iKey))
		{
			// Nachricht schicken
			ZeroMemory(&Msg, sizeof(tbGUIMessage));
			Msg.Type = TB_GMT_KEYUP;
			Msg.aiInt[0] = iKey;
			Msg.aiInt[1] = m_pbButtons[TB_KEY_LSHIFT] || m_pbButtons[TB_KEY_RSHIFT];
			Msg.aiInt[2] = (m_pbButtons[TB_KEY_LALT] && (m_pbButtons[TB_KEY_LCONTROL] || m_pbButtons[TB_KEY_RCONTROL])) || m_pbButtons[TB_KEY_RALT];
			BroadcastMessage(&Msg);

			// Zeit zur�cksetzen
			m_aiKeyRepeat[iKey] = 0;
		}
	}

	// Alte Zeit eintragen
	m_dwOldTime = dwTime;

	return TB_OK;
}

// ******************************************************************
// Rendern
tbResult tbGUI::Render(const float fTime)
{
	static tbGUIMessage	Msg;
	static int			iNumPasses;


	// Vertex-Buffer zur�cksetzen
	m_pVertexBuffer->SetCursor(0);

	// Allen Elementen der aktiven Seite die erste Render-Nachricht schicken
	ZeroMemory(&Msg, sizeof(tbGUIMessage));
	Msg.Type = TB_GMT_RENDER;
	Msg.afFloat[0] = fTime;
	Msg.aiInt[0] = 0;
	BroadcastMessage(&Msg, m_iCurrentPage);

	// Mauszeiger rendern
	RenderCursor(fTime);

	// Skalierung aller Vertizes
	for(DWORD i = 0; i < m_pVertexBuffer->GetCursor(); i++)
	{
		((tbGUIVertex*)(m_pVertexBuffer->GetBuffer()))[i].vPosition.x *= m_vScaling.x;
		((tbGUIVertex*)(m_pVertexBuffer->GetBuffer()))[i].vPosition.y *= m_vScaling.y;
	}

	// Vertex-Buffer aktualisieren
	m_pVertexBuffer->Update();

	// Vertexformat und Vertex-Buffer als Quelle setzen
	tbDirect3D::SetFVF(TB_GUI_FVF);
	tbDirect3D::GetDevice()->SetStreamSource(0, m_pVertexBuffer->GetVB(), 0, sizeof(tbGUIVertex));

	// Den Effekt starten
	iNumPasses = m_Skin.pEffect->Begin();

	// Alle Durchg�nge rendern
	for(int iPass = 0; iPass < iNumPasses; iPass++)
	{
		// Durchgang aktivieren und Dreiecke rendern
		m_Skin.pEffect->BeginPass(iPass);
		tbDirect3D::GetDevice()->DrawPrimitive(D3DPT_TRIANGLELIST,
			                                   0, m_pVertexBuffer->GetCursor() / 3 - 2);
		m_Skin.pEffect->EndPass();
	}

	// Effekt beenden
	m_Skin.pEffect->End();

	// ------------------------------------------------------------------

	m_Skin.pFont->Begin();

	// Allen Elementen der aktiven Seite die zweite Render-Nachricht schicken.
	// Dadurch wird ihr Text gerendert.
	ZeroMemory(&Msg, sizeof(tbGUIMessage));
	Msg.Type = TB_GMT_RENDER;
	Msg.afFloat[0] = fTime;
	Msg.aiInt[0] = 1;
	BroadcastMessage(&Msg, m_iCurrentPage);

	m_Skin.pFont->End();

	// ------------------------------------------------------------------
	
	// Den Effekt starten
	iNumPasses = m_Skin.pEffect->Begin();

	// Allen Elementen der aktiven Seite die dritte Render-Nachricht schicken
	ZeroMemory(&Msg, sizeof(tbGUIMessage));
	Msg.Type = TB_GMT_RENDER;
	Msg.afFloat[0] = fTime;
	Msg.aiInt[0] = 2;
	BroadcastMessage(&Msg, m_iCurrentPage);

	// Effekt beenden
	m_Skin.pEffect->End();

	// ------------------------------------------------------------------

	// Vertexformat und Vertex-Buffer als Quelle setzen
	tbDirect3D::SetFVF(TB_GUI_FVF);
	tbDirect3D::GetDevice()->SetStreamSource(0, m_pVertexBuffer->GetVB(), 0, sizeof(tbGUIVertex));

	// Den Effekt starten
	iNumPasses = m_Skin.pEffect->Begin();

	// Alle Durchg�nge rendern (f�r Mauszeiger)
	for(iPass = 0; iPass < iNumPasses; iPass++)
	{
		// Durchgang aktivieren und Dreiecke rendern
		m_Skin.pEffect->BeginPass(iPass);
		tbDirect3D::GetDevice()->DrawPrimitive(D3DPT_TRIANGLELIST,
			                                   m_pVertexBuffer->GetCursor() - 6, 2);
		m_Skin.pEffect->EndPass();
	}

	// Effekt beenden
	m_Skin.pEffect->End();

	return TB_OK;
}

// ******************************************************************
// Rendert den Mauszeiger
tbResult tbGUI::RenderCursor(const float fTime)
{
	static tbVector2 vPos;

	vPos = m_vCursorPos - m_Skin.vCursorHotSpot;

	// Rechteck hinzuf�gen
	AddRect(vPos, m_Skin.vCursorSize,
			m_pbButtons[TB_MOUSE_BUTTON(0)] ? m_Skin.CursorColor1 : m_Skin.CursorColor2,
			m_pbButtons[TB_MOUSE_BUTTON(0)] ? tbVector2(32.0f, 0.0f) / 255.0f : tbVector2(0.0f, 0.0f) / 255.0f,
			tbVector2(31.0f, 31.0f) / 255.0f);

	return TB_OK;
}

// ******************************************************************
// Senden einer Nachricht
tbResult tbGUI::PostMessage(int iID,
							tbGUIMessage* pMsg)
{
	static tbGUIElement*	pElement;
	static tbResult			(* pMsgProc)(int, tbGUIMessage*, tbGUI*);


	// Die Nachrichtenfunktion des Elements aufrufen
	pElement = GetElement(iID);
	if(pElement == NULL) TB_ERROR("Das Element wurde nicht gefunden!", TB_ERROR);
	pElement->ReceiveMessage(pMsg);

	// Die allgemeine Nachrichtenfunktion aufrufen
	pMsgProc = (tbResult (*)(int, tbGUIMessage*, tbGUI*))(m_pMsgProc);
	pMsgProc(iID, pMsg, this);

	return TB_OK;
}

// ******************************************************************
// Senden einer Nachricht an alle Elemente
tbResult tbGUI::BroadcastMessage(tbGUIMessage* pMsg,
								 int iPage) // = -1
{
	static tbResult						(* pMsgProc)(int, tbGUIMessage*, tbGUI*);
	static tbListEntry<tbGUIElement*>*	pEntry;


	// Alle Elemente durchgehen und ihre Nachrichtenfunktion aufrufen
	pEntry = m_pElements->GetFirstEntry();
	while(pEntry != NULL)
	{
		if(pEntry->Data->m_iPage == iPage ||
		   iPage == -1)
		{
			// Nachrichtenfunktion aufrufen
			pEntry->Data->ReceiveMessage(pMsg);
		}

		pEntry = pEntry->pNextEntry;
	}

	// Die allgemeine Nachrichtenfunktion aufrufen
	pMsgProc = (tbResult (*)(int, tbGUIMessage*, tbGUI*))(m_pMsgProc);
	pMsgProc(-1, pMsg, this);

	return TB_OK;
}

// ******************************************************************
// Findet den Listeneintrag eines Elements
tbListEntry<tbGUIElement*>* tbGUI::GetElementListEntry(int iID)
{
	// Nach der ID suchen
	tbListEntry<tbGUIElement*>* pEntry = m_pElements->GetFirstEntry();
	while(pEntry != NULL)
	{
		if(pEntry->Data->m_iID == iID) return pEntry;
		pEntry = pEntry->pNextEntry;
	}

	// Nicht gefunden!
	return NULL;
}

// ******************************************************************
// Liefert den Zeiger auf ein Element
tbGUIElement* tbGUI::GetElement(int iID)
{
	// Listeneintrag abfragen
	tbListEntry<tbGUIElement*>* pEntry = GetElementListEntry(iID);
	if(pEntry == NULL) return NULL;

	return pEntry->Data;
}

// ******************************************************************
// L�scht ein Element
tbResult tbGUI::DeleteElement(int iID)
{
	// Element suchen
	tbListEntry<tbGUIElement*>* pEntry = GetElementListEntry(iID);
	if(pEntry == NULL) TB_ERROR("Das Element wurde nicht gefunden!", TB_ERROR);
	tbGUIElement* pElement = pEntry->Data;

	// L�schungsnachricht schicken
	tbGUIMessage Msg;
	ZeroMemory(&Msg, sizeof(tbGUIMessage));
	Msg.Type = TB_GMT_DELETE;
	PostMessage(iID, &Msg);

	// Das Element l�schen
	TB_SAFE_DELETE(pElement);
	m_pElements->DeleteEntry(pEntry);

	return TB_OK;
}

// ******************************************************************
// F�gt ein Rechteck hinzu
tbResult tbGUI::AddRect(const tbVector2& vTopLeft,
						const tbVector2& vSize,
						const tbColor& Color,
						const tbVector2& vTopLeftTex,
						const tbVector2& vSizeOnTex,
						tbGUIVertex* pOut) // = NULL
{
	tbGUIVertex aVertex[4];

	// Vertizes generieren
	// Links oben
	aVertex[0].vPosition = tbVector3(vTopLeft.x, vTopLeft.y, 0.5f);
	aVertex[0].fRHW = 1.0f;
	aVertex[0].Diffuse = Color;
	aVertex[0].vTexture = vTopLeftTex;

	// Rechts oben
	aVertex[1].vPosition = tbVector3(vTopLeft.x + vSize.x, vTopLeft.y, 0.5f);
	aVertex[1].fRHW = 1.0f;
	aVertex[1].Diffuse = Color;
	aVertex[1].vTexture = tbVector2(vTopLeftTex.x + vSizeOnTex.x, vTopLeftTex.y);

	// Rechts unten
	aVertex[2].vPosition = tbVector3(vTopLeft.x + vSize.x, vTopLeft.y + vSize.y, 0.5f);
	aVertex[2].fRHW = 1.0f;
	aVertex[2].Diffuse = Color;
	aVertex[2].vTexture = vTopLeftTex + vSizeOnTex;

	// Links unten
	aVertex[3].vPosition = tbVector3(vTopLeft.x, vTopLeft.y + vSize.y, 0.5f);
	aVertex[3].fRHW = 1.0f;
	aVertex[3].Diffuse = Color;
	aVertex[3].vTexture = tbVector2(vTopLeftTex.x, vTopLeftTex.y + vSizeOnTex.y);

	// Texturkoordinaten korrigieren
	aVertex[0].vTexture += tbVector2(0.003f, 0.003f);
	aVertex[1].vTexture += tbVector2(-0.003f, 0.003f);
	aVertex[2].vTexture += tbVector2(-0.003f, -0.003f);
	aVertex[3].vTexture += tbVector2(0.003f, -0.003f);

	if(pOut == NULL)
	{
		// Ist noch Platz im Vertex-Buffer?
		if(m_pVertexBuffer->GetCursor() + 6 > m_pVertexBuffer->GetMaxVertices())
		{
			// Vertex-Buffer vergr��ern
			if(m_pVertexBuffer->SetSize(m_pVertexBuffer->GetSize() +
										1200 * sizeof(tbGUIVertex)))
			{
				// Fehler!
				TB_ERROR("Fehler beim Vergr��ern des Vertex-Buffers!", TB_ERROR);
			}
		}

		// Die Vertizes zum Vertex-Buffer hinzuf�gen
		m_pVertexBuffer->AddVertex(&aVertex[0]);
		m_pVertexBuffer->AddVertex(&aVertex[1]);
		m_pVertexBuffer->AddVertex(&aVertex[2]);
		m_pVertexBuffer->AddVertex(&aVertex[0]);
		m_pVertexBuffer->AddVertex(&aVertex[2]);
		m_pVertexBuffer->AddVertex(&aVertex[3]);
	}
	else
	{
		// Vertizes kopieren
		pOut[0] = aVertex[3];
		pOut[1] = aVertex[0];
		pOut[2] = aVertex[2];
		pOut[3] = aVertex[1];
	}

	return TB_OK;
}

// ******************************************************************
// F�gt mehrere Rechtecke hinzu
tbResult tbGUI::AddRects1D(const tbVector2& vTopLeft,
						   const tbVector2& vSize,
						   float fNumRects,
						   const tbVector2& vTranslation,
						   const tbColor& Color,
						   const tbVector2& vTopLeftTex,
						   const tbVector2& vSizeOnTex)
{
	tbGUIVertex aVertex[4];

	if(fNumRects < 0.0f) return TB_OK;

	// Vertizes generieren
	// Links oben
	aVertex[0].vPosition = tbVector3(vTopLeft.x, vTopLeft.y, 0.5f);
	aVertex[0].fRHW = 1.0f;
	aVertex[0].Diffuse = Color;
	aVertex[0].vTexture = vTopLeftTex;

	// Rechts oben
	aVertex[1].vPosition = tbVector3(vTopLeft.x + vSize.x, vTopLeft.y, 0.5f);
	aVertex[1].fRHW = 1.0f;
	aVertex[1].Diffuse = Color;
	aVertex[1].vTexture = tbVector2(vTopLeftTex.x + vSizeOnTex.x, vTopLeftTex.y);

	// Rechts unten
	aVertex[2].vPosition = tbVector3(vTopLeft.x + vSize.x, vTopLeft.y + vSize.y, 0.5f);
	aVertex[2].fRHW = 1.0f;
	aVertex[2].Diffuse = Color;
	aVertex[2].vTexture = vTopLeftTex + vSizeOnTex;

	// Links unten
	aVertex[3].vPosition = tbVector3(vTopLeft.x, vTopLeft.y + vSize.y, 0.5f);
	aVertex[3].fRHW = 1.0f;
	aVertex[3].Diffuse = Color;
	aVertex[3].vTexture = tbVector2(vTopLeftTex.x, vTopLeftTex.y + vSizeOnTex.y);

	// Texturkoordinaten korrigieren
	aVertex[0].vTexture += tbVector2(0.003f, 0.003f);
	aVertex[1].vTexture += tbVector2(-0.003f, 0.003f);
	aVertex[2].vTexture += tbVector2(-0.003f, -0.003f);
	aVertex[3].vTexture += tbVector2(0.003f, -0.003f);

	// Ist noch Platz im Vertex-Buffer?
	if(m_pVertexBuffer->GetCursor() + 6 * ceil(fNumRects) > m_pVertexBuffer->GetMaxVertices())
	{
		// Vertex-Buffer vergr��ern
		if(m_pVertexBuffer->SetSize(m_pVertexBuffer->GetSize() +
									1200 * sizeof(tbGUIVertex)))
		{
			// Fehler!
			TB_ERROR("Fehler beim Vergr��ern des Vertex-Buffers!", TB_ERROR);
		}
	}

	while(fNumRects > 0.0f)
	{
		if(fNumRects < 1.0f)
		{
			// Das letzte Rechteck muss geteilt werden.
			if(fabsf(vTranslation.x) > 0.0f)
			{
				aVertex[1].vPosition.x -= (1.0f - fNumRects) * vSize.x;
				aVertex[2].vPosition.x -= (1.0f - fNumRects) * vSize.x;
				aVertex[1].vTexture.x = vTopLeftTex.x + fNumRects * vSizeOnTex.x;
				aVertex[2].vTexture.x = vTopLeftTex.x + fNumRects * vSizeOnTex.x;
				aVertex[1].vTexture.x -= 0.003f;
				aVertex[2].vTexture.x -= 0.003f;
			}

			if(fabsf(vTranslation.y) > 0.0f)
			{
				aVertex[2].vPosition.y -= (1.0f - fNumRects) * vSize.y;
				aVertex[3].vPosition.y -= (1.0f - fNumRects) * vSize.y;
				aVertex[2].vTexture.y = vTopLeftTex.y + fNumRects * vSizeOnTex.y;
				aVertex[3].vTexture.y = vTopLeftTex.y + fNumRects * vSizeOnTex.y;
				aVertex[2].vTexture.y -= 0.003f;
				aVertex[3].vTexture.y -= 0.003f;
			}
		}

		// Die Vertizes zum Vertex-Buffer hinzuf�gen
		m_pVertexBuffer->AddVertex(&aVertex[0]);
		m_pVertexBuffer->AddVertex(&aVertex[1]);
		m_pVertexBuffer->AddVertex(&aVertex[2]);
		m_pVertexBuffer->AddVertex(&aVertex[0]);
		m_pVertexBuffer->AddVertex(&aVertex[2]);
		m_pVertexBuffer->AddVertex(&aVertex[3]);

		// Vertizes verschieben
		aVertex[0].vPosition.x += vTranslation.x;
		aVertex[0].vPosition.y += vTranslation.y;
		aVertex[1].vPosition.x += vTranslation.x;
		aVertex[1].vPosition.y += vTranslation.y;
		aVertex[2].vPosition.x += vTranslation.x;
		aVertex[2].vPosition.y += vTranslation.y;
		aVertex[3].vPosition.x += vTranslation.x;
		aVertex[3].vPosition.y += vTranslation.y;

		fNumRects -= 1.0f;
	}

	return TB_OK;
}

// ******************************************************************
// F�gt mehrere Rechtecke zweidimensional hinzu
tbResult tbGUI::AddRects2D(tbVector2 vTopLeft,
						   const tbVector2& vSize,
						   const float fNumColumns,
						   float fNumRows,
						   const tbColor& Color,
						   const tbVector2& vTopLeftTex,
						   const tbVector2& vSizeOnTex)
{
	if(fNumColumns < 0.0f || fNumRows < 0.0f) return TB_OK;

	// Mehrere Zeilen voller Rechtecke zeichnen
	while(fNumRows > 0.0f)
	{
		if(fNumRows < 1.0f)
		{
			AddRects1D(vTopLeft, tbVector2(vSize.x, vSize.y * fNumRows), fNumColumns, tbVector2(vSize.x, 0.0f),
				       Color, vTopLeftTex, tbVector2(vSizeOnTex.x, vSizeOnTex.y * fNumRows));
		}
		else
		{
			AddRects1D(vTopLeft, vSize, fNumColumns, tbVector2(vSize.x, 0.0f),
				       Color, vTopLeftTex, vSizeOnTex);
		}

		vTopLeft.y += vSize.y;
		fNumRows -= 1.0f;
	}

	return TB_OK;
}

// ******************************************************************
// F�gt einen ganzen Rahmen hinzu
tbResult tbGUI::AddFrame(const tbVector2& vTopLeft,
						 const tbVector2& vSize,
						 const tbColor& Color,
						 const tbVector2& vTopLeftOnTex)
{
	float fXFactor = vSize.x < 64.0f ? vSize.x / 64.0f : 1.0f;
	float fYFactor = vSize.y < 64.0f ? vSize.y / 64.0f : 1.0f;

	// Linke obere Ecke zeichnen
	AddRect(vTopLeft, tbVector2(32.0f * fXFactor, 32.0f * fYFactor), Color,
			vTopLeftOnTex / 255.0f, tbVector2(32.0f * fXFactor, 32.0f * fYFactor) / 255.0f);

	// Rechte obere Ecke zeichnen
	AddRect(vTopLeft + tbVector2(vSize.x - 32.0f * fXFactor, 0.0f), tbVector2(32.0f * fXFactor, 32.0f * fYFactor), Color,
			(vTopLeftOnTex + tbVector2(96 - 32.0f * fXFactor, 0.0f)) / 255.0f, tbVector2(32.0f * fXFactor, 32.0f * fYFactor) / 255.0f);

	// Rechte untere Ecke zeichnen
	AddRect(vTopLeft + vSize - tbVector2(32.0f * fXFactor, 32.0f * fYFactor), tbVector2(32.0f * fXFactor, 32.0f * fYFactor), Color,
			(vTopLeftOnTex + tbVector2(96.0f - 32.0f * fXFactor, 96.0f - 32.0f * fYFactor)) / 255.0f, tbVector2(32.0f * fXFactor, 32.0f * fYFactor) / 255.0f);

	// Linke untere Ecke zeichnen
	AddRect(vTopLeft + tbVector2(0.0f, vSize.y - 32.0f * fYFactor), tbVector2(32.0f * fXFactor, 32.0f * fYFactor), Color,
			(vTopLeftOnTex + tbVector2(0.0f, 96.0f - 32.0f * fYFactor)) / 255.0f, tbVector2(32.0f * fXFactor, 32.0f * fYFactor) / 255.0f);

	// Obere Seite zeichnen
	AddRects1D(vTopLeft + tbVector2(32.0f, 0.0f), tbVector2(32.0f, 32.0f * fYFactor),
			   vSize.x / 32.0f - 2.0f, tbVector2(32.0f, 0.0f), Color,
			   (vTopLeftOnTex + tbVector2(32.0f, 0.0f)) / 255.0f, tbVector2(32.0f, 32.0f * fYFactor) / 255.0f);

	// Untere Seite zeichnen
	AddRects1D(vTopLeft + tbVector2(32.0f, vSize.y - 32.0f * fYFactor), tbVector2(32.0f, 32.0f * fYFactor),
			   vSize.x / 32.0f - 2.0f, tbVector2(32.0f, 0.0f), Color,
			   (vTopLeftOnTex + tbVector2(32.0f, 96.0f - 32.0f * fYFactor)) / 255.0f, tbVector2(32.0f, 32.0f * fYFactor) / 255.0f);

	// Linke Seite zeichnen
	AddRects1D(vTopLeft + tbVector2(0.0f, 32.0f), tbVector2(32.0f * fXFactor, 32.0f),
			   vSize.y / 32.0f - 2.0f, tbVector2(0.0f, 32.0f), Color,
			   (vTopLeftOnTex + tbVector2(0.0f, 32.0f)) / 255.0f, tbVector2(32.0f * fXFactor, 32.0f) / 255.0f);

	// Rechte Seite zeichnen
	AddRects1D(vTopLeft + tbVector2(vSize.x - 32.0f * fXFactor, 32.0f), tbVector2(32.0f * fXFactor, 32.0f),
			   vSize.y / 32.0f - 2.0f, tbVector2(0.0f, 32.0f), Color,
			   (vTopLeftOnTex + tbVector2(96.0f - 32.0f * fXFactor, 32.0f)) / 255.0f, tbVector2(32.0f * fXFactor, 32.0f) / 255.0f);

	// Hintergrund zeichnen
	AddRects2D(vTopLeft + tbVector2(32.0f, 32.0f), tbVector2(32.0f, 32.0f),
			   vSize.x / 32.0f - 2.0f, vSize.y / 32.0f - 2.0f, Color,
			   (vTopLeftOnTex + tbVector2(32.0f, 32.0f)) / 255.0f, tbVector2(32.0f, 32.0f) / 255.0f);

	return TB_OK;
}

// ******************************************************************
// Erstellen eines Elements
tbGUIElement* tbGUI::CreateElement(int iID,
								   tbGUIElementType Type,
								   int iPage,
								   tbVector2 vPosition,
								   tbVector2 vSize)
{
	tbGUIElement* pElement;

	switch(Type)
	{
	case TB_GET_FRAME:		pElement = new tbGUIFrame; break;
	case TB_GET_BUTTON:		pElement = new tbGUIButton; break;
	case TB_GET_TEXT:		pElement = new tbGUIText; break;
	case TB_GET_IMAGE:		pElement = new tbGUIImage; break;
	case TB_GET_CHECKBOX:	pElement = new tbGUICheckBox; break;
	case TB_GET_RADIOBOX:	pElement = new tbGUIRadioBox; break;
	case TB_GET_INPUT:		pElement = new tbGUIInput; break;
	case TB_GET_LIST:		pElement = new tbGUIList; break;

	default:
		TB_ERROR("Unbekannter Elementtyp angegeben!", NULL);
		break;
	}

	// Daten eintragen
	pElement->m_iID = iID;
	pElement->m_Type = Type;
	pElement->m_iPage = iPage;
	pElement->m_pGUI = this;
	pElement->m_vPosition = vPosition;
	pElement->m_vSize = vSize;
	pElement->m_bVisible = TRUE;
	pElement->m_bEnabled = TRUE;
	pElement->m_bPressed = FALSE;

	// Liste erweitern
	tbListEntry<tbGUIElement*>* pEntry = m_pElements->AddEntry(&pElement);

	// Erstellungsnachricht verschicken
	tbGUIMessage Msg;
	ZeroMemory(&Msg, sizeof(tbGUIMessage));
	Msg.Type = TB_GMT_CREATE;
	PostMessage(iID, &Msg);

	return pEntry->Data;
}

// ******************************************************************
// Betreten einer Seite
tbResult tbGUI::SetCurrentPage(int iPage)
{
	// Nachricht senden, dass eine neue Seite betreten wird
	tbGUIMessage Msg;
	ZeroMemory(&Msg, sizeof(tbGUIMessage));
	Msg.Type = TB_GMT_ENTERPAGE;
	Msg.aiInt[0] = iPage;
	Msg.aiInt[1] = m_iCurrentPage;
	BroadcastMessage(&Msg);

	m_iCurrentPage = iPage;

	return TB_OK;
}

// ******************************************************************