/*
**	Space Runner
**	=========
**
**	Menu.cpp
**	------------
**	Das Hauptmenü
*/

#include "SpaceRunner.h"


// __________________________________________________________________
// Vertizes für das Hintergrundbild
#define BACKGROUND_FVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
struct SBackgroundVertex {
	tbVector3	vPosition;
	float		fRHW;
	D3DCOLOR	Color;
	tbVector2	vTex0;
};

BOOL g_bStartGame = FALSE;

// __________________________________________________________________
// Initialisiert den Spielzustand
tbResult CMenu::Init() {
	// Laden...
	if(Load()) TB_ERROR("Fehler beim Laden des Spielzustands!", TB_ERROR);

	// Briefing-Musik abspielen
	if(g_pSpaceRunner->m_pTitle->GetState() != State_Running) {
		// Mit Looping...
		g_pSpaceRunner->m_pTitle->Play();
	}

	return TB_OK;
}

// __________________________________________________________________
// Fährt den Spielzustand herunter
tbResult CMenu::Exit() {
	// Entladen...
	Unload();

	return TB_OK;
}

// Nachrichtenfunktion für das Hauptmenü
tbResult MenuMsgProc(int iID, tbGUIMessage* pMsg, tbGUI* pGUI) {
	switch(pMsg->Type) {
		case TB_GMT_ELEMENTCLICKED:
			switch(iID) {
				case 101:
					pGUI->SetCurrentPage(2);
					break;
				case 102:
					pGUI->SetCurrentPage(1);
					break;
				case 103:
					PostQuitMessage(0);
					break;
				case 201: // OK
					pGUI->SetCurrentPage(0);
					break;
			}
			break;
		case TB_GMT_RENDER:
			if(pGUI->GetCurrentPage() == 2) {
			// Aha - der Ladebildschirm wird angezeigt! Jetzt setzen wir
			// g_bStartGame auf TRUE, damit das Spiel gleich gestartet werden kann.
				//g_bStartGame = TRUE;
				PostQuitMessage(0);
			}
			break;

	}
	return TB_OK;
}
//				PostQuitMessage(0);
/*	switch(pMsg->Type) {

	case TB_GMT_ELEMENTCLICKED:
		switch(iID) {
	case 101: { // Spiel starten
		PostQuitMessage(0);
		/*		tbGUIList*	pTeamList;
		int			iNumShips = 0;

		g_pGalactica->m_pGame->m_bUseJoystick = ((tbGUICheckBox*)(pGUI->GetElement(204)))->IsChecked();

		// Team-Array leeren (-1 steht für "kein Schiff")
		for(int t = 0; t < 4; t++) {
			for(int s = 0; s < 32; s++) {
				g_aiTeam[t][s] = -1;
			}
		}

		// Team 1
		pTeamList = (tbGUIList*)(pGUI->GetElement(121));
		for(int i = 0; i < pTeamList->GetNumEntries() && i < 32; i++) {
			// Schiff hinzufügen
			g_aiTeam[0][i] = ((SShipType*)(pTeamList->GetEntryByOrder(i)->pData))->iIndex;
			iNumShips++;
		}

		// Team 2
		pTeamList = (tbGUIList*)(pGUI->GetElement(131));
		for(i = 0; i < pTeamList->GetNumEntries() && i < 32; i++) {
			// Schiff hinzufügen
			g_aiTeam[1][i] = ((SShipType*)(pTeamList->GetEntryByOrder(i)->pData))->iIndex;
			iNumShips++;
		}

		// Team 3
		pTeamList = (tbGUIList*)(pGUI->GetElement(141));
		for(i = 0; i < pTeamList->GetNumEntries() && i < 32; i++) {
			// Schiff hinzufügen
			g_aiTeam[2][i] = ((SShipType*)(pTeamList->GetEntryByOrder(i)->pData))->iIndex;
			iNumShips++;
		}

		// Team 4
		pTeamList = (tbGUIList*)(pGUI->GetElement(151));
		for(i = 0; i < pTeamList->GetNumEntries() && i < 32; i++) {
			// Schiff hinzufügen
			g_aiTeam[3][i] = ((SShipType*)(pTeamList->GetEntryByOrder(i)->pData))->iIndex;
			iNumShips++;
		}

		// Wenn mindestens ein Schiff erstellt wurde, geht's jetzt los!
		if(iNumShips > 0) pGUI->SetCurrentPage(2);
			  }
			  break;

	case 102: // Steuerung
		pGUI->SetCurrentPage(1);
		break;

	case 103: // Spiel beenden
		PostQuitMessage(0);
		break;

	case 122: // Schiff zu einem Team hinzufügen
	case 132:
	case 142:
	case 152: {
		tbGUIList* pShipList;
		tbGUIList* pTeamList;
		SShipType* pShipType;

		pShipList = (tbGUIList*)(pGUI->GetElement(106));
		pTeamList = (tbGUIList*)(pGUI->GetElement(iID - 1));
		pShipType = (SShipType*)(pShipList->GetSelectedEntry()->pData);
		pTeamList->AddEntry(pShipType->acName, pShipType);
			  }
			  break;

	case 123: // Schiff aus einem Team löschen
	case 133: 
	case 143: 
	case 153: {
		tbGUIList* pTeamList;

		pTeamList = (tbGUIList*)(pGUI->GetElement(iID - 2));
		pTeamList->DeleteEntry(pTeamList->GetCursor());
			  }
			  break;

			  // ------------------------------------------------------------------

	case 201: // OK
		pGUI->SetCurrentPage(0);
		break;
		}
		break;

	case TB_GMT_SELECTION:
		switch(iID) {
	case 106: {

		tbGUIList* pList;
		tbGUIText* pText;
		SShipType* pShipType;

		pList = (tbGUIList*)(pGUI->GetElement(106));
		pText = (tbGUIText*)(pGUI->GetElement(108));
		pShipType = (SShipType*)(pList->GetSelectedEntry()->pData);
		pText->SetText(pShipType->acDesc);
			  }
			  break;
		}
		break;

	case TB_GMT_RENDER:
		if(pGUI->GetCurrentPage() == 2) {
			// Aha - der Ladebildschirm wird angezeigt! Jetzt setzen wir
			// g_bStartGame auf TRUE, damit das Spiel gleich gestartet werden kann.
			g_bStartGame = TRUE;
		}
		break;
	}
*/

// __________________________________________________________________
// Lädt den Spielzustand
tbResult CMenu::Load()
{
	CGame* pGame;

	// Zeiger kopieren
	pGame = g_pSpaceRunner->m_pGame;

	// Hintergrundbild laden
	m_pBackground = tbTextureManager::GetTexture("Data\\Menu.jpg", TRUE, D3DX_DEFAULT, D3DX_DEFAULT, 1);
	if(m_pBackground == NULL) TB_ERROR("Fehler beim Laden von Data\\Menu.jpg!", TB_ERROR);

	// Skin der Benutzeroberfläche erstellen
	m_GUISkin.pSkinTexture			= tbTextureManager::GetTexture("Data\\GUI.bmp", TRUE, D3DX_DEFAULT, D3DX_DEFAULT, 1, D3DFMT_UNKNOWN, 0, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255));
	m_GUISkin.pEffect				= new tbEffect; m_GUISkin.pEffect->Init("Data\\GUI.fx"); m_GUISkin.pEffect->GetEffect()->SetTexture("Texture", m_GUISkin.pSkinTexture);
	m_GUISkin.pFont					= g_pSpaceRunner->m_pCourier_New_6_18;
	m_GUISkin.vCursorSize			= tbVector2(32.0f, 32.0f);
	m_GUISkin.CursorColor1			= tbColor(1.0f, 1.0f, 1.0f, 0.75f);
	m_GUISkin.CursorColor2			= tbColor(1.0f, 1.0f, 1.0f, 0.75f);
	m_GUISkin.vCursorHotSpot		= tbVector2(1.0f, 1.0f);
	m_GUISkin.fCursorSpeed			= 300.0f;
	m_GUISkin.EnabledColor			= tbColor(1.0f, 1.0f, 1.0f, 0.5f);
	m_GUISkin.DisabledColor			= tbColor(1.0f, 1.0f, 1.0f, 0.25f);
	m_GUISkin.HighlightColor		= tbColor(1.0f, 1.0f, 1.0f, 0.75f);
	m_GUISkin.vTextSize				= tbVector2(1.0f);
	m_GUISkin.EnabledTextColor		= tbColor(1.0f, 1.0f, 1.0f, 0.75f);
	m_GUISkin.DisabledTextColor		= tbColor(1.0f, 1.0f, 1.0f, 0.25f);
	m_GUISkin.HighlightTextColor	= tbColor(1.0f, 1.0f, 1.0f, 1.0f);

	if(m_GUISkin.pSkinTexture == NULL)	TB_ERROR("Fehler beim Laden der Skin-Textur für die Benutzeroberfläche!", TB_ERROR);
	if(m_GUISkin.pEffect == NULL)		TB_ERROR("Fehler beim Laden des Effekts für die Benutzeroberfläche!", TB_ERROR);

	// Benutzeroberfläche erstellen
	m_pGUI = new tbGUI;
	if(m_pGUI->Init(&m_GUISkin, MenuMsgProc, tbVector2(800.0f, 600.0f), tbDirect3D::GetScreenSize() / tbVector2(800.0f, 600.0f))) {
		// Fehler!
		TB_ERROR("Fehler bei der Erstellung der Benutzeroberfläche!", TB_ERROR);
	}

	// Benutzeroberfläche erstellen
	// Erste Seite
	m_pGUI->CreateFrame(100, 0, tbVector2(10.0f, 10.0f), tbVector2(780.0f, 580.0f));
	m_pGUI->CreateButton(101, 0, tbVector2(50.0f, 50.0f), tbVector2(120.0f, 50.0f), "Spiel starten");
	m_pGUI->CreateButton(102, 0, tbVector2(50.0f, 120.0f), tbVector2(120.0f, 50.0f), "Steuerung");
	m_pGUI->CreateButton(103, 0, tbVector2(50.0f, 190.0f), tbVector2(120.0f, 50.0f), "Spiel beenden");
	m_pGUI->CreateFrame(104, 0, tbVector2(230.0f, 50.0f), tbVector2(520.0f, 510.0f));
/*
	m_pGUI->CreateText(105, 0, tbVector2(250.0f, 70.0f), "Schiffs- und Teamauswahl");
	
	m_pGUI->CreateText(105, 0, tbVector2(250.0f, 110.0f), "Verfügbare Schiffstypen");
	m_pGUI->CreateList(106, 0, tbVector2(250.0f, 140.0f), tbVector2(192.0f, 120.0f), 20.0f);
	for(int i = 0; i < pGame->m_iNumShipTypes; i++)
	{
		((tbGUIList*)(m_pGUI->GetElement(106)))->AddEntry(pGame->m_aShipType[i].acName,
			                                              &pGame->m_aShipType[i]);
	}

	m_pGUI->CreateFrame(107, 0, tbVector2(490.0f, 140.0f), tbVector2(240.0f, 120.0f));
	m_pGUI->CreateText(108, 0, tbVector2(500.0f, 150.0f), "");

	m_pGUI->CreateText(120, 0, tbVector2(250.0f, 270.0f), "Team 1");
	m_pGUI->CreateList(121, 0, tbVector2(250.0f, 300.0f), tbVector2(192.0f, 100.0f), 20.0f);
	m_pGUI->CreateButton(122, 0, tbVector2(320.0f, 270.0f), tbVector2(30.0f, 30.0f), "+");
	m_pGUI->CreateButton(123, 0, tbVector2(355.0f, 270.0f), tbVector2(30.0f, 30.0f), "-");

	m_pGUI->CreateText(130, 0, tbVector2(500.0f, 270.0f), "Team 2");
	m_pGUI->CreateList(131, 0, tbVector2(500.0f, 300.0f), tbVector2(192.0f, 100.0f), 20.0f);
	m_pGUI->CreateButton(132, 0, tbVector2(570.0f, 270.0f), tbVector2(30.0f, 30.0f), "+");
	m_pGUI->CreateButton(133, 0, tbVector2(605.0f, 270.0f), tbVector2(30.0f, 30.0f), "-");

	m_pGUI->CreateText(140, 0, tbVector2(250.0f, 410.0f), "Team 3");
	m_pGUI->CreateList(141, 0, tbVector2(250.0f, 440.0f), tbVector2(192.0f, 100.0f), 20.0f);
	m_pGUI->CreateButton(142, 0, tbVector2(320.0f, 410.0f), tbVector2(30.0f, 30.0f), "+");
	m_pGUI->CreateButton(143, 0, tbVector2(355.0f, 410.0f), tbVector2(30.0f, 30.0f), "-");

	m_pGUI->CreateText(150, 0, tbVector2(500.0f, 410.0f), "Team 4");
	m_pGUI->CreateList(151, 0, tbVector2(500.0f, 440.0f), tbVector2(192.0f, 100.0f), 20.0f);
	m_pGUI->CreateButton(152, 0, tbVector2(570.0f, 410.0f), tbVector2(30.0f, 30.0f), "+");
	m_pGUI->CreateButton(153, 0, tbVector2(605.0f, 410.0f), tbVector2(30.0f, 30.0f), "-");
*/
	// Zweite Seite
	m_pGUI->CreateFrame(200, 1, tbVector2(10.0f, 10.0f), tbVector2(780.0f, 580.0f));
	m_pGUI->CreateButton(201, 1, tbVector2(50.0f, 50.0f), tbVector2(120.0f, 50.0f), "OK");
	m_pGUI->CreateFrame(202, 1, tbVector2(230.0f, 50.0f), tbVector2(520.0f, 510.0f));
	m_pGUI->CreateText(203, 1, tbVector2(250.0f, 70.0f), "Steuerung");
	m_pGUI->CreateCheckBox(204, 1, tbVector2(450.0f, 70.0f), tbVector2(32.0f), "Joystick benutzen", g_pSpaceRunner->m_pGame->m_bUseJoystick);
	m_pGUI->GetElement(204)->Enable(tbDirectInput::GetNumButtons() > 122);
	m_pGUI->CreateText(205, 1, tbVector2(250.0f, 120.0f), "Aktion", tbColor(1.0f, 0.75f, 0.75f));
	m_pGUI->CreateText(206, 1, tbVector2(250.0f, 150.0f), "Lenkung\nSchubkontrolle\nVoller Stopp\nLaser abfeuern\nRaketen abfeuern\nNächstes Ziel\nAuto-Aim an/aus\nRadarreichweite erhöhen\nRadarreichweite verringern\nSelbstzerstörung\nCockpitkamera\nJagdkamera\nFreie Jagdkamera\nJagdkamera von vorne\nFlyBy-Kamera\nPadlock-Kamera\nRaketenkamera\nJagdraketenkamera\nFlyBy-Raketenkamera", m_pGUI->GetSkin().EnabledTextColor, tbVector2(0.85f, 1.0f));
	m_pGUI->CreateText(207, 1, tbVector2(450.0f, 120.0f), "Joystick", tbColor(1.0f, 0.75f, 0.75f));
	m_pGUI->CreateText(208, 1, tbVector2(450.0f, 150.0f), "Achsen\nSchieberegler\n[NICHT BELEGT]\n1. Knopf\n2. Knopf\n3. Knopf\n[NICHT BELEGT]\n[NICHT BELEGT]\n[NICHT BELEGT]\n[NICHT BELEGT]\n[NICHT BELEGT]\n[NICHT BELEGT]\n[NICHT BELEGT]\n[NICHT BELEGT]\n[NICHT BELEGT]\n[NICHT BELEGT]\n[NICHT BELEGT]\n[NICHT BELEGT]\n[NICHT BELEGT]", m_pGUI->GetSkin().EnabledTextColor, tbVector2(0.85f, 1.0f));
	m_pGUI->CreateText(209, 1, tbVector2(580.0f, 120.0f), "Tastatur", tbColor(1.0f, 0.75f, 0.75f));
	m_pGUI->CreateText(210, 1, tbVector2(580.0f, 150.0f), "Cursortasten, Q, W\nPlus/Minus (NumPad)\nRücktaste\nShift links\nStrg links\nT\nA\nBild auf\nBild ab\nS + D\nF1\nF2\nF3\nF4\nF5\nF6\nF7\nF8\nF9", m_pGUI->GetSkin().EnabledTextColor, tbVector2(0.85f, 1.0f));

	// Dritte Seite
	m_pGUI->CreateFrame(300, 2, tbVector2(10.0f, 10.0f), tbVector2(780.0f, 580.0f));
	m_pGUI->CreateText(301, 2, tbVector2(400.0f, 300.0f), "LADEVORGANG", tbColor(0.25f, 1.0f, 0.25f, 1.0f), tbVector2(2.0f, 4.0f), TB_FF_RELATIVESCALING | TB_FF_ALIGN_HCENTER | TB_FF_ALIGN_VCENTER);
	m_pGUI->CreateText(302, 2, tbVector2(400.0f, 350.0f), "Bitte warten Sie einen Moment...", tbColor(1.0f), tbVector2(1.0f, 2.0f), TB_FF_RELATIVESCALING | TB_FF_ALIGN_HCENTER | TB_FF_ALIGN_VCENTER);

	// Erste Seite ist aktiv
	m_pGUI->SetCurrentPage(0);
	g_bStartGame = FALSE;

	return TB_OK;
}

// __________________________________________________________________
// Entlädt den Spielzustand
tbResult CMenu::Unload()
{
	// Die Textur löschen
	tbTextureManager::ReleaseTexture(m_pBackground);

	// Skin löschen
	m_GUISkin.pEffect->GetEffect()->SetTexture("Texture", NULL);
	TB_SAFE_DELETE(m_GUISkin.pEffect);
	tbTextureManager::ReleaseTexture(m_GUISkin.pSkinTexture);

	// Benutzeroberfläche löschen
	TB_SAFE_DELETE(m_pGUI);

	return TB_OK;
}

// __________________________________________________________________
// Bewegt den Spielzustand
tbResult CMenu::Move(float fTime)
{
	// Benutzeroberfläche bewegen
	m_pGUI->Move(fTime, g_pbButtons, g_pfButtons);

	if(g_bStartGame == TRUE)
	{
		// Spiel starten
		//g_SpaceRunner->SetGameState(GS_GAME);
		PostQuitMessage(0);
	}

	// Musik überprüfen
	g_pSpaceRunner->m_pTitle->Process();

	return TB_OK;
}

// __________________________________________________________________
// Rendert den Spielzustand
tbResult CMenu::Render(float fTime)
{
	SBackgroundVertex aVertex[4];


	// Puffer leeren und Szene beginnen
	tbDirect3D::Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
					  tbColor(0.0f, 1.0f, 0.0f), 1.0f, 0);
	tbDirect3D::BeginScene();

	// ------------------------------------------------------------------

	// Vertexformat und Hintergrundbildtextur setzen, Z-Buffer aus,
	// Alpha-Blending einschalten
	tbDirect3D::SetFVF(BACKGROUND_FVF);
	tbDirect3D::SetTexture(0, m_pBackground);
	tbDirect3D::SetRS(D3DRS_ZENABLE, D3DZB_FALSE);
	tbDirect3D::SetRS(D3DRS_ALPHABLENDENABLE, TRUE);
	tbDirect3D::SetRS(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	tbDirect3D::SetRS(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	tbDirect3D::SetTSS(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	tbDirect3D::SetTSS(0, D3DTSS_ALPHAARG1, D3DTA_CURRENT);

	// Die vier Vertizes des Hintergrundbilds erstellen (Rechteck)
	// Links unten
	aVertex[0].vPosition = tbVector3(0.0f, tbDirect3D::GetScreenSize().y, 0.5f);
	aVertex[0].fRHW = 1.0f;
	aVertex[0].Color = tbColor(0.4f, 0.4f, 0.4f, 0.5f);
	aVertex[0].vTex0 = tbVector2(0.0f, 1.0f);

	// Links oben
	aVertex[1].vPosition = tbVector3(0.0f, 0.0f, 0.5f);
	aVertex[1].fRHW = 1.0f;
	aVertex[1].Color = tbColor(0.4f, 0.4f, 0.4f, 0.5f);
	aVertex[1].vTex0 = tbVector2(0.0f, 0.0f);

	// Rechts unten
	aVertex[2].vPosition = tbVector3(tbDirect3D::GetScreenSize().x, tbDirect3D::GetScreenSize().y, 0.5f);
	aVertex[2].fRHW = 1.0f;
	aVertex[2].Color = tbColor(0.4f, 0.4f, 0.4f, 0.5f);
	aVertex[2].vTex0 = tbVector2(1.0f, 1.0f);

	// Rechts oben
	aVertex[3].vPosition = tbVector3(tbDirect3D::GetScreenSize().x, 0.0f, 0.5f);
	aVertex[3].fRHW = 1.0f;
	aVertex[3].Color = tbColor(0.4f, 0.4f, 0.4f, 0.5f);
	aVertex[3].vTex0 = tbVector2(1.0f, 0.0f);

	// Als Dreiecksfolge zeichnen
	tbDirect3D::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, aVertex, sizeof(SBackgroundVertex));

	// ------------------------------------------------------------------

	// Benutzeroberfläche rendern
	m_pGUI->Render(fTime);

	// ------------------------------------------------------------------

	// Szene beenden
	tbDirect3D::EndScene();

	return TB_OK;
}

// __________________________________________________________________