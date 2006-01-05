/*
**	GALACTICA
**	=========
**
**	Game.cpp
**	--------
**	Das Spiel
*/

#include "SpaceRunner.h"

// __________________________________________________________________
// Vertizes für das HUD
#define HUD_FVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
struct SHUDVertex
{
	tbVector3	vPosition;
	float		fRHW;
	D3DCOLOR	Color;
	tbVector2	vTex0;
};

// __________________________________________________________________
// Konstruktor
CGame::CGame()
{
	ZeroMemory(this, sizeof(CGame));
	m_bUseJoystick = tbDirectInput::GetNumButtons() > 122;
}

// __________________________________________________________________
// Initialisiert den Spielzustand
tbResult CGame::Init()
{
	int iShip;

	// Laden...
	if(Load()) TB_ERROR("Fehler beim Laden des Spielzustands!", TB_ERROR);

	// Kameramodus: Cockpit, Radarreichweite: 4000
//	m_CameraMode = CM_COCKPIT;
	//MOD: free_cam
	m_CameraMode = CM_FREE;

	m_vCameraPos = tbVector3(0.0f);

	CreateCamera();

	m_pCamera = &m_aCamera[0];

//	m_fRadarRange = 4000.0f;

	// Schiffe erstellen
	// Team 1
	for(int i = 0; i < 32; i++)
	{
		if(g_Ships[i] != -1)
		{
			iShip = CreateShip(0, g_Ships[i]);
			m_aShip[iShip].SetPosition(tbVector3((float)(i) * 100.0f, 0.0f, -2500.0f) + tbVector3Random() * 20.0f);
			m_aShip[iShip].Align(tbVector3(0.0f, 0.0f, 1.0f) + tbVector3Random() * 0.25f);
		}
	}
/*
	// Team 2
	for(i = 0; i < 32; i++)
	{
		if(g_aiTeam[1][i] != -1)
		{
			iShip = CreateShip(1, g_aiTeam[1][i]);
			m_aShip[iShip].SetPosition(tbVector3((float)(i) * 100.0f, 0.0f, 2500.0f) + tbVector3Random() * 20.0f);
			m_aShip[iShip].Align(tbVector3(0.0f, 0.0f, -1.0f) + tbVector3Random() * 0.25f);
		}
	}

	// Team 3
	for(i = 0; i < 32; i++)
	{
		if(g_aiTeam[2][i] != -1)
		{
			iShip = CreateShip(2, g_aiTeam[2][i]);
			m_aShip[iShip].SetPosition(tbVector3(-2500.0f, 0.0f, (float)(i) * 100.0f) + tbVector3Random() * 20.0f);
			m_aShip[iShip].Align(tbVector3(1.0f, 0.0f, 0.0f) + tbVector3Random() * 0.25f);
		}
	}

	// Team 4
	for(i = 0; i < 32; i++)
	{
		if(g_aiTeam[3][i] != -1)
		{
			iShip = CreateShip(3, g_aiTeam[3][i]);
			m_aShip[iShip].SetPosition(tbVector3(2500.0f, 0.0f, (float)(i) * 100.0f) + tbVector3Random() * 20.0f);
			m_aShip[iShip].Align(tbVector3(-1.0f, 0.0f, 0.0f) + tbVector3Random() * 0.25f);
		}
	}
*/
	// Der Spieler spielt immer das erste Schiff.
	m_pPlayer = &m_aShip[0];

	// Allen Schiffen zufällige Ziele zuweisen
//	for(int s = 0; s <= iShip; s++) m_aShip[s].m_iTarget = tbIntRandom(0, iShip);

	// Namen der Kameramodi eintragen
	m_apcCameraMode[CM_COCKPIT]			= "Cockpitkamera";
	m_apcCameraMode[CM_CHASE]			= "Jagdkamera";
	m_apcCameraMode[CM_FREECHASE]		= "Freie Jagdkamera";
	m_apcCameraMode[CM_FRONTCHASE]		= "Jagdkamera von vorne";
	m_apcCameraMode[CM_FLYBY]			= "Fly-By-Kamera";
	m_apcCameraMode[CM_PADLOCK]			= "Padlock-Kamera";
	m_apcCameraMode[CM_MISSILE]			= "Raketenkamera";
	m_apcCameraMode[CM_MISSILECHASE]	= "Jagdraketenkamera";
	m_apcCameraMode[CM_MISSILEFLYBY]	= "Fly-By-Raketenkamera";
	m_apcCameraMode[CM_FREE]			= "Free Camera";


	m_fTime = 0.0f;

	// Briefing-Musik stoppen und Action-Musik starten

	g_pSpaceRunner->m_pTitle->Stop();
/*	g_pSpaceRunner->m_pAction->Play(TRUE);
*/
	return TB_OK;
}

// __________________________________________________________________
// Fährt den Spielzustand herunter
tbResult CGame::Exit()
{
	// Schiffe und Projektile löschen
	ZeroMemory(m_aShip, 32 * sizeof(CShip));
//	ZeroMemory(m_aProjectile, 256 * sizeof(CProjectile));
	ZeroMemory(m_aCamera, 1 * sizeof(CCamera));

	// Entladen...
	Unload();

	// Musik stoppen
//	g_pSpaceRunner->m_pAction->Stop();

	return TB_OK;
}

// __________________________________________________________________
// Lädt den Spielzustand
tbResult CGame::Load()
{

	// Textur der Sky-Box laden
	m_pSkyBoxTex = tbTextureManager::GetCubeTexture("Data\\SkyBox.dds", TRUE, D3DX_DEFAULT, 1);
	if(m_pSkyBoxTex == NULL) TB_WARNING("Fehler beim Laden der Sky-Box-Textur!")
	else
	{
		// Sky-Box erstellen
		m_pSkyBox = new tbSkyBox;
		if(m_pSkyBox->Init(m_pSkyBoxTex)) TB_ERROR("Fehler beim Erstellen der Sky-Box!", TB_ERROR);
	}

	// ------------------------------------------------------------------

	// Sprites-Textur laden
	m_pSpritesTex = tbTextureManager::GetTexture("Data\\Sprites.bmp", TRUE, D3DX_DEFAULT, D3DX_DEFAULT, 1);
	if(m_pSpritesTex == NULL) TB_ERROR("Fehler beim Laden der Sprites-Textur!", TB_ERROR);

	// Sprite-Engine erzeugen
	m_pSprites = new tbSpriteEngine;
	if(m_pSprites->Init(10000)) TB_ERROR("Fehler beim Erstellen der Sprite-Engine!", TB_ERROR);

	// Effekt für die Sprites laden
	m_pSpritesEffect = new tbEffect;
	if(m_pSpritesEffect->Init("Data\\Sprites.fx")) TB_ERROR("Fehler beim Erstellen des Effekts für die Sprites!", TB_ERROR);
	m_pSpritesEffect->GetEffect()->SetTexture("Texture", m_pSpritesTex);

	// ------------------------------------------------------------------

	// Die Sprite-, Schiffstypen und die Waffentypen (mit Modellen) laden
	if(LoadSpriteTypes()) TB_ERROR("Fehler beim Laden der Sprite-Typen!", TB_ERROR);
	if(LoadShipTypes(TRUE)) TB_ERROR("Fehler beim Laden der Schiffstypen!", TB_ERROR);
//	if(LoadWeaponTypes(TRUE)) TB_ERROR("Fehler beim Laden der Waffentypen!", TB_ERROR);

	// ------------------------------------------------------------------

	for(int iType = 0; iType < 1024; iType++)
	{
		// Texturkoordinaten leicht nach innen schieben
		m_pSprites->GetSpriteTypes()[iType].vTopLeft += tbVector2(0.9f, 0.9f);
		m_pSprites->GetSpriteTypes()[iType].vTopRight += tbVector2(-0.9f, 0.9f);
		m_pSprites->GetSpriteTypes()[iType].vBottomRight += tbVector2(-0.9f, -0.9f);
		m_pSprites->GetSpriteTypes()[iType].vBottomLeft += tbVector2(0.9f, -0.9f);

		// Texturkoordinaten an die Texturgröße anpassen
		m_pSprites->GetSpriteTypes()[iType].vTopLeft /= tbVector2(255.0f, 255.0f);
		m_pSprites->GetSpriteTypes()[iType].vTopRight /= tbVector2(255.0f, 255.0f);
		m_pSprites->GetSpriteTypes()[iType].vBottomRight /= tbVector2(255.0f, 255.0f);
		m_pSprites->GetSpriteTypes()[iType].vBottomLeft /= tbVector2(255.0f, 255.0f);
	}

	// Partikelsystem erstellen
	m_pPSystem = new tbParticleSystem;
	if(m_pPSystem->Init(7500)) TB_ERROR("Fehler beim Erstellen des Partikelsystems!", TB_ERROR);

	// ------------------------------------------------------------------

	// Hüllentreffersound laden
/*	m_pHullHitSound = new tbSound;
	if(m_pHullHitSound->Init("Data\\HullHit.wav",
		                     DSBCAPS_STATIC | DSBCAPS_LOCDEFER | DSBCAPS_CTRL3D | DSBCAPS_CTRLFREQUENCY | DSBCAPS_MUTE3DATMAXDISTANCE,
							 DS3DALG_HRTF_FULL, 16))
	{
		// Fehler!
		TB_ERROR("Fehler beim Laden des Hüllentreffersounds!", TB_ERROR);
	}

	// Minimale und maximale Distanzen setzen
	m_pHullHitSound->SetDistances(200.0f, 2000.0f);
*/
	// Explosionssound laden
/*	m_pExplosionSound = new tbSound;
	if(m_pExplosionSound->Init("Data\\Explosion.wav",
							   DSBCAPS_STATIC | DSBCAPS_LOCDEFER | DSBCAPS_CTRL3D | DSBCAPS_CTRLFREQUENCY | DSBCAPS_MUTE3DATMAXDISTANCE,
							   DS3DALG_HRTF_FULL, 16))
	{
		// Fehler!
		TB_ERROR("Fehler beim Laden des Explosionssounds!", TB_ERROR);
	}

	// Minimale und maximale Distanzen setzen
	m_pExplosionSound->SetDistances(1000.0f, 6000.0f);
*/
	// Kollisionssound laden
/*	m_pCollisionSound = new tbSound;
	if(m_pCollisionSound->Init("Data\\Collision.wav",
							   DSBCAPS_STATIC | DSBCAPS_LOCDEFER | DSBCAPS_CTRL3D | DSBCAPS_CTRLFREQUENCY | DSBCAPS_MUTE3DATMAXDISTANCE,
							   DS3DALG_HRTF_FULL, 4))
	{
		// Fehler!
		TB_ERROR("Fehler beim Laden des Kollisionssounds!", TB_ERROR);
	}

	// Minimale und maximale Distanzen setzen
	m_pCollisionSound->SetDistances(200.0f, 2000.0f);
*/
	// ------------------------------------------------------------------

	// Cockpitmodell laden
//	m_pCockpitModel = new tbModel;
//	if(m_pCockpitModel->Init("Data\\Cockpit.tbm", "Data\\")) TB_ERROR("Fehler beim Laden des Cockpitmodells!", TB_ERROR);

	// Zeichenklasse für die Radartextur erstellen
//	m_pRadar = new tbDraw2D;
//	if(m_pRadar->Init((PDIRECT3DTEXTURE9)(m_pCockpitModel->GetEffects()[2].apTexture[0]), 0)) TB_ERROR("Fehler beim Initialisieren des Radars!", TB_ERROR);

	return TB_OK;
}

// __________________________________________________________________
// Entlädt den Spielzustand
tbResult CGame::Unload()
{
	// Sky-Box und Sky-Box-Textur löschen
	if(m_pSkyBox != NULL)
	{
		TB_SAFE_DELETE(m_pSkyBox);
		tbTextureManager::ReleaseTexture(m_pSkyBoxTex);
	}

	//Kamera löschen
	//TB_SAFE_DELETE(m_pCamera);

	// Sprite-Engine, Sprites-Textur und Partikelsystem löschen
	TB_SAFE_DELETE(m_pSprites);
	if(m_pSpritesEffect != NULL) m_pSpritesEffect->GetEffect()->SetTexture("Texture", NULL);
	TB_SAFE_DELETE(m_pSpritesEffect);
	tbTextureManager::ReleaseTexture(m_pSpritesTex);
	TB_SAFE_DELETE(m_pPSystem);

	// Sounds löschen
//	TB_SAFE_DELETE(m_pHullHitSound);
//	TB_SAFE_DELETE(m_pExplosionSound);
//	TB_SAFE_DELETE(m_pCollisionSound);

	// Cockpitmodell und Zeichenklasse für das Radar löschen
//	TB_SAFE_DELETE(m_pRadar);
//	TB_SAFE_DELETE(m_pCockpitModel);

	// Die Schiffstypen löschen
	for(int iType = 0; iType < m_iNumShipTypes; iType++)
	{
		TB_SAFE_DELETE(m_aShipType[iType].pModel);
		TB_SAFE_DELETE(m_aShipType[iType].pCollisionModel);
		TB_SAFE_DELETE(m_aShipType[iType].pEngineSound);
		TB_SAFE_DELETE(m_aShipType[iType].pShieldSound);
	}

	// Die Waffentypen löschen
/*	for(iType = 0; iType < m_iNumWeaponTypes; iType++)
	{
//		TB_SAFE_DELETE(m_aWeaponType[iType].pLauncherModel);
		TB_SAFE_DELETE(m_aWeaponType[iType].pProjectileModel);
		TB_SAFE_DELETE(m_aWeaponType[iType].pLauncherSound);
		TB_SAFE_DELETE(m_aWeaponType[iType].pExplosionSound);
	}
*/
	return TB_OK;
}

// __________________________________________________________________
// Bewegt den Spielzustand
tbResult CGame::Move(float fTime)
{
	if(m_fTime == 0.0f)
	{
		m_fTime += fTime;
		return TB_OK;
	}

	// Pause-Taste abfragen
	if(WasButtonPressed(TB_KEY_PAUSE)) m_bPaused = !m_bPaused;
	if(m_bPaused) return TB_OK;

	// ------------------------------------------------------------------

	// Sprite-Engine starten.
	// Das wird bereits hier erledigt, damit auch in den Move-Funktionen
	// Partikel erzeugt werden können.
	if(!m_bPaused) m_pSprites->Begin(tbDirect3D::GetTransform(D3DTS_VIEW), m_vCameraPos);

	// Alle Partikel bewegen
	m_pPSystem->Move(fTime);

	// Schiffe und Projektile bewegen
//	MoveProjectiles(fTime);
	MoveShips(fTime);

	//Kamera bewegen
	MoveCameras(fTime);
	// Stoppuhr aktualisieren
	m_fTime += fTime;

	// ------------------------------------------------------------------

	// Wenn die Escape-Taste gedrückt wird, geht's zurück ins Hauptmenü.
	if(g_pbButtons[TB_KEY_ESCAPE]) g_pSpaceRunner->SetGameState(GS_MENU);


	// Die F-Tasten bestimmen den Kameramodus
	if(g_pbButtons[TB_KEY_F1]) m_CameraMode = CM_FREE;
	if(g_pbButtons[TB_KEY_F2]) m_CameraMode = CM_COCKPIT;
	if(g_pbButtons[TB_KEY_F3]) m_CameraMode = CM_CHASE;
//	if(g_pbButtons[TB_KEY_F4]) m_CameraMode = CM_FRONTCHASE;
//	if(g_pbButtons[TB_KEY_F5]) m_CameraMode = CM_FLYBY;
//	if(g_pbButtons[TB_KEY_F6]) m_CameraMode = CM_PADLOCK;
//	if(g_pbButtons[TB_KEY_F7]) m_CameraMode = CM_MISSILE;
//	if(g_pbButtons[TB_KEY_F8]) m_CameraMode = CM_MISSILECHASE;
//	if(g_pbButtons[TB_KEY_F9]) m_CameraMode = CM_MISSILEFLYBY;

	// Musik verarbeiten
//	g_pSpaceRunner->m_pAction->Process();

	return TB_OK;
}

// __________________________________________________________________
// Rendert den Spielzustand
tbResult CGame::Render(float fTime)
{
	D3DLIGHT9	SunLight;
	char		acText[256];


#ifdef _3D_GLASSES_
	tbDirect3D::Clear(0, NULL, D3DCLEAR_TARGET, tbColor(0.0f), 1.0f, 0);

	for(int i = 0; i < 2; i++)
	{
		// Puffer leeren und Szene beginnen
		tbDirect3D::Clear(0, NULL, D3DCLEAR_ZBUFFER, tbColor(0.0f), 1.0f, 0);
		tbDirect3D::BeginScene();

		if(i == 0) tbDirect3D::SetRS(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_BLUE);
		else tbDirect3D::SetRS(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED);

		// ------------------------------------------------------------------

		// Kamera setzen
		SetupCamera();

		// ------------------------------------------------------------------

		if(g_pbButtons[TB_KEY_SPACE])
		{
			m_pPSystem->AddParticle(tbFloatRandom(1.0f, 4.0f),
									8.0f, 23.0f,
									tbVector3(0.0f, 0.0f, 2500.0f),
									tbVector3Random() * tbFloatRandom(0.5f, 2.0f),
									1.0f,
									50.0f, 1500.0f,
									tbColor(1.0f, 0.25f, 0.25f, 0.25f),
									tbColor(0.25f, 1.0f, 0.25f, 0.0f),
									tbFloatRandom(0.0f, 2.0f), tbFloatRandom(0.0f, 2.0f));
		}

		// Sonnenrichtungslicht (von links) erstellen
		ZeroMemory(&SunLight, sizeof(D3DLIGHT9));
		SunLight.Type = D3DLIGHT_DIRECTIONAL;
		SunLight.Diffuse = tbColor(1.0f, 0.75f, 0.5f);
		SunLight.Ambient = tbColor(0.25f, 0.25f, 0.25f);
		SunLight.Specular = tbColor(0.5f, 0.5f, 0.5f);
		SunLight.Direction = tbVector3(1.0f, 0.0f, 0.0f);
		tbDirect3D::SetLight(0, &SunLight);
		tbDirect3D::LightEnable(0, TRUE);

		// ------------------------------------------------------------------

		if(m_pSkyBox != NULL)
		{
			// Sky-Box rendern
			m_pSkyBox->Render(m_vCameraPos);
		}

		// Nebel einstellen (damit weit entfernte Objekte dunkler werden, bevor sie aus
		// dem Sichtbereich geraten)
		tbDirect3D::SetRS(D3DRS_FOGENABLE, TRUE);
		tbDirect3D::SetRS(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
		tbDirect3D::SetRS(D3DRS_FOGCOLOR, tbColor(0.0f, 0.0f, 0.0f));
		tbDirect3D::SetRSF(D3DRS_FOGSTART, 4000.0f);
		tbDirect3D::SetRSF(D3DRS_FOGEND, 5000.0f);

		// Schiffe und Projektile rendern
		RenderShips(fTime);
		//RenderProjectiles(fTime);

		// "Sternenfeld" rendern
		RenderStarfield(fTime);

		//Ebene zu Testzwecken rendern
		RenderPlain(fTime);

		// Partikel und Sprites rendern
		if(!m_bPaused)
		{
			m_pPSystem->AddToSpriteEngine(m_pSprites);
			m_pSprites->End();
		}
		m_pSpritesEffect->Begin();
		m_pSpritesEffect->Pass(0);
		tbDirect3D::SetTransform(D3DTS_WORLD, tbMatrixIdentity());
		m_pSprites->Render();
		m_pSpritesEffect->End();

		// Nebel wieder ausschalten
		tbDirect3D::SetRS(D3DRS_FOGENABLE, FALSE);

		// ------------------------------------------------------------------

		// Eventuell Cockpit und Anzeigen rendern
		if(m_CameraMode == CM_COCKPIT) RenderCockpit(fTime);

		if(m_pSkyBox != NULL)
		{
			// Blenden der Sonne rendern
			RenderSunFlares(fTime);
		}

		// ------------------------------------------------------------------

		g_pGalactica->m_pCourier_New_6_12->Begin();

		// Name des Kameramodus anzeigen
		g_pGalactica->m_pCourier_New_6_12->DrawText(tbVector2(10.0f, 10.0f), m_apcCameraMode[m_CameraMode]);

		if(m_bPaused)
		{
			// "Pause"-Text anzeigen
			g_pGalactica->m_pCourier_New_6_12->DrawText(tbVector2(0.5f, 0.5f), "[ P A U S E ]",
											 TB_FF_ALIGN_HCENTER | TB_FF_ALIGN_VCENTER |
											 TB_FF_RELATIVE | TB_FF_RELATIVESCALING);
		}

		if(g_pbButtons[TB_KEY_F])
		{
			// Framerate anzeigen
			sprintf(acText, "FPS: %.2f", 1.0f / fTime);
			g_pGalactica->m_pCourier_New_6_12->DrawText(tbVector2(10.0f, 30.0f), acText);
		}

		g_pGalactica->m_pCourier_New_6_12->End();

		// Szene beenden
		tbDirect3D::EndScene();
	}
#else
	// Puffer leeren und Szene beginnen
	tbDirect3D::Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
	              tbColor(0.0f), 1.0f, 0);
	tbDirect3D::BeginScene();

	// ------------------------------------------------------------------

	// Kamera setzen
	SetupCamera();

	// ------------------------------------------------------------------

	if(g_pbButtons[TB_KEY_SPACE])
	{
		m_pPSystem->AddParticle(tbFloatRandom(1.0f, 4.0f),
			                    8.0f, 23.0f,
								tbVector3(0.0f, 0.0f, 2500.0f),
								tbVector3Random() * tbFloatRandom(0.5f, 2.0f),
								1.0f,
								50.0f, 1500.0f,
								tbColor(1.0f, 0.25f, 0.25f, 0.25f),
								tbColor(0.25f, 1.0f, 0.25f, 0.0f),
								tbFloatRandom(0.0f, 2.0f), tbFloatRandom(0.0f, 2.0f));
	}

	// Sonnenrichtungslicht (von links) erstellen
	ZeroMemory(&SunLight, sizeof(D3DLIGHT9));
	SunLight.Type = D3DLIGHT_DIRECTIONAL;
	SunLight.Diffuse = tbColor(1.0f, 0.75f, 0.5f);
	SunLight.Ambient = tbColor(0.25f, 0.25f, 0.25f);
	SunLight.Specular = tbColor(0.5f, 0.5f, 0.5f);
	SunLight.Direction = tbVector3(1.0f, 0.0f, 0.0f);
	tbDirect3D::SetLight(0, &SunLight);
	tbDirect3D::LightEnable(0, TRUE);

	// ------------------------------------------------------------------

	if(m_pSkyBox != NULL)
	{
		// Sky-Box rendern
		m_pSkyBox->Render(m_vCameraPos);
	}

	// Nebel einstellen (damit weit entfernte Objekte dunkler werden, bevor sie aus
	// dem Sichtbereich geraten)
	tbDirect3D::SetRS(D3DRS_FOGENABLE, TRUE);
	tbDirect3D::SetRS(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
	tbDirect3D::SetRS(D3DRS_FOGCOLOR, tbColor(0.0f, 0.0f, 0.0f));
	tbDirect3D::SetRSF(D3DRS_FOGSTART, 4000.0f);
	tbDirect3D::SetRSF(D3DRS_FOGEND, 5000.0f);

	// Schiffe und Projektile rendern
	RenderShips(fTime);
//	RenderProjectiles(fTime);

	// "Sternenfeld" rendern
	RenderStarfield(fTime);

	// Partikel und Sprites rendern
	if(!m_bPaused)
	{
		m_pPSystem->AddToSpriteEngine(m_pSprites);
		m_pSprites->End();
	}
	m_pSpritesEffect->Begin();
	m_pSpritesEffect->BeginPass(0);
	tbDirect3D::SetTransform(D3DTS_WORLD, tbMatrixIdentity());
	m_pSprites->Render();
	m_pSpritesEffect->EndPass();	
	m_pSpritesEffect->End();

	// Nebel wieder ausschalten
	tbDirect3D::SetRS(D3DRS_FOGENABLE, FALSE);

	// ------------------------------------------------------------------

	// Eventuell Cockpit und Anzeigen rendern
//	if(m_CameraMode == CM_COCKPIT) RenderCockpit(fTime);
/*
	if(m_pSkyBox != NULL)
	{
		// Blenden der Sonne rendern
		RenderSunFlares(fTime);
	}
*/
	// ------------------------------------------------------------------

	g_pSpaceRunner->m_pCourier_New_6_12->Begin();

	// Name des Kameramodus anzeigen
	g_pSpaceRunner->m_pCourier_New_6_12->DrawText(tbVector2(10.0f, 10.0f), m_apcCameraMode[m_CameraMode]);

	if(m_bPaused)
	{
		// "Pause"-Text anzeigen
		g_pSpaceRunner->m_pCourier_New_6_12->DrawText(tbVector2(0.5f, 0.5f), "[ P A U S E ]",
			                             TB_FF_ALIGN_HCENTER | TB_FF_ALIGN_VCENTER |
										 TB_FF_RELATIVE | TB_FF_RELATIVESCALING);
	}

	if(g_pbButtons[TB_KEY_F])
	{
		// Framerate anzeigen
		sprintf(acText, "FPS: %.2f", 1.0f / fTime);
		g_pSpaceRunner->m_pCourier_New_6_12->DrawText(tbVector2(10.0f, 30.0f), acText);
	}

	g_pSpaceRunner->m_pCourier_New_6_12->End();

	// Szene beenden
	tbDirect3D::EndScene();
#endif

	return TB_OK;
}

// __________________________________________________________________
// Liest einen int-Wert aus der INI-Datei
int CGame::ReadINIInt(char* pcSection, char* pcKey)
{
	char acString[256];

	// String lesen
	ReadINIString(pcSection, pcKey, acString, 256);
	if(!strcmp(acString, "[NOT FOUND]")) return 12345678;

	// In int-Wert umwandeln
	return atoi(acString);
}
// __________________________________________________________________
// Liest einen float-Wert aus der INI-Datei
float CGame::ReadINIFloat(char* pcSection,
						  char* pcKey)
{
	char	acString[256];
	float	fValue;


	// String lesen
	ReadINIString(pcSection, pcKey, acString, 256);
	if(!strcmp(acString, "[NOT FOUND]")) return 12345678.0f;

	// In float-Wert umwandeln
	sscanf(acString, "%f", &fValue);

	return fValue;
}

// __________________________________________________________________
// Liest einen tbVector3-Wert aus der INI-Datei
tbVector3 CGame::ReadINIVector3(char* pcSection,
								char* pcKey)
{
	char		acString[256];
	tbVector3	vValue;


	// String lesen
	ReadINIString(pcSection, pcKey, acString, 256);
	if(!strcmp(acString, "[NOT FOUND]")) return tbVector3(12345678.0f, 12345678.0f, 12345678.0f);

	// Die Vektorkomponenten extrahieren
	sscanf(acString, "%f, %f, %f", &vValue.x, &vValue.y, &vValue.z);

	return vValue;
}

// __________________________________________________________________
// Liest einen tbColor-Wert aus der INI-Datei
tbColor CGame::ReadINIColor(char* pcSection,
							char* pcKey)
{
	char	acString[256];
	tbColor	Value;


	// String lesen
	ReadINIString(pcSection, pcKey, acString, 256);
	if(!strcmp(acString, "[NOT FOUND]")) return tbColor(12345678.0f, 12345678.0f, 12345678.0f, 12345678.0f);

	// Die Farbkomponenten extrahieren
	sscanf(acString, "%f, %f, %f, %f", &Value.r, &Value.g, &Value.b, &Value.a);

	return Value;
}
// __________________________________________________________________
// Liest einen String aus der INI-Datei
tbResult CGame::ReadINIString(char* pcSection,
							  char* pcKey,
							  char* pcOut,
							  int iBufferSize)
{
	// String lesen
	GetPrivateProfileString(pcSection, pcKey, "[NOT FOUND]",
		                    pcOut, iBufferSize,
							"Data\\SpaceRunner.ini");

	return TB_OK;
}
// __________________________________________________________________
// Lädt die Sprite-Typen
tbResult CGame::LoadSpriteTypes()
{
	tbColor	Sprite;
	char	acKey[256];


	// Jeden Typ durchgehen
	for(int iType = 0; iType < 1024; iType++)
	{
		// Daten lesen und Sprite erzeugen
		sprintf(acKey, "SpriteType%d", iType);
		Sprite = ReadINIColor("Sprite types", acKey);
		if(Sprite.r != 12345678.0f)
		{
			m_pSprites->CreateSpriteType(iType, tbVector2(Sprite.r, Sprite.g),
				                                tbVector2(Sprite.b - 1.0f, Sprite.a - 1.0f));
		}
	}

	return TB_OK;
}
// __________________________________________________________________
// Lädt die Schiffstypen
tbResult CGame::LoadShipTypes(BOOL bFullLoad)
{
	char		acSection[256];
	char		acKey[256];
	SShipType*	pType;


	// Anzahl der Schiffstypen lesen
	m_iNumShipTypes = ReadINIInt("Ships and weapons", "NumShipTypes");
	if(m_iNumShipTypes == 12345678) TB_ERROR("Fehler beim Lesen der INI-Datei!", TB_ERROR);

	// Jeden Schiffstyp durchgehen.
	// Die Sektionen heißen "Ship1", "Ship2" usw..
	for(int iType = 0; iType < m_iNumShipTypes; iType++)
	{
		// Namen der Sektion der Daten dieses Schiffs generieren
		sprintf(acSection, "Ship%d", iType + 1);
		pType = &m_aShipType[iType];
		pType->iIndex = iType;

		// Name, Beschreibung, Modell- und Ssounddateiname lesen
		ReadINIString(acSection, "Name", pType->acName, 256);
		ReadINIString(acSection, "Desc", pType->acDesc, 1024);
		ReadINIString(acSection, "Model", pType->acModel, 256);
		ReadINIString(acSection, "CollisionModel", pType->acCollisionModel, 256);
		ReadINIString(acSection, "EngineSound", pType->acEngineSound, 256);
		ReadINIString(acSection, "ShieldSound", pType->acShieldSound, 256);

		if(bFullLoad)
		{
			// Logbucheintrag erzeugen
			tbWriteToLog("Lade Schiffstyp \"%s\" aus \"%s\"...", pType->acName, pType->acModel);

			// Modell laden
			pType->pModel = new tbModel;
			if(pType->pModel->Init(pType->acModel, "Data\\", "",
				                   D3DPOOL_DEFAULT, 0, D3DPOOL_DEFAULT, 0,
								   FALSE, FALSE))
			{
				// Fehler!
				TB_ERROR("Fehler beim Laden eines Schiffsmodells!", TB_ERROR);
			}

			// Kollisionsmodell laden
			pType->pCollisionModel = new tbModel;
			if(pType->pCollisionModel->Init(pType->acCollisionModel, "Data\\", "",
											D3DPOOL_DEFAULT, 0, D3DPOOL_DEFAULT, 0,
											TRUE, TRUE))
			{
				// Fehler!
				TB_ERROR("Fehler beim Laden eines Schiffskollisionsmodells!", TB_ERROR);
			}

			// Antriebssound laden
			pType->pEngineSound = new tbSound;
			if(pType->pEngineSound->Init(pType->acEngineSound,
				                         DSBCAPS_STATIC | DSBCAPS_LOCDEFER | DSBCAPS_CTRL3D | DSBCAPS_CTRLFREQUENCY | DSBCAPS_MUTE3DATMAXDISTANCE,
										 DS3DALG_HRTF_FULL, 8))
			{
				// Fehler!
				TB_ERROR("Fehler beim Laden eines Antriebssounds!", TB_ERROR);
			}

			// Minimale und maximale Distanzen setzen
			pType->pEngineSound->SetDistances(200.0f, 2000.0f);

			// Schutzschildsound laden
			pType->pShieldSound = new tbSound;
			if(pType->pShieldSound->Init(pType->acShieldSound,
				                         DSBCAPS_STATIC | DSBCAPS_LOCDEFER | DSBCAPS_CTRL3D | DSBCAPS_CTRLFREQUENCY | DSBCAPS_MUTE3DATMAXDISTANCE,
										 DS3DALG_HRTF_FULL, 16))
			{
				// Fehler!
				TB_ERROR("Fehler beim Laden eines Schutzschildsounds!", TB_ERROR);
			}

			// Minimale und maximale Distanzen setzen
			pType->pShieldSound->SetDistances(200.0f, 2000.0f);
		}

		// Alle restlichen Schiffsparameter laden
		pType->fMass					= ReadINIFloat(acSection, "Mass");
		pType->fMovementFriction		= ReadINIFloat(acSection, "MovementFriction");
		pType->fRotationFriction		= ReadINIFloat(acSection, "RotationFriction");

		pType->fMaxHullDamage			= ReadINIFloat(acSection, "MaxHullDamage");
		pType->fHullRepairRate			= ReadINIFloat(acSection, "HullRepairRate");
		pType->DebrisColor				= ReadINIColor(acSection, "DebrisColor");
		
		pType->fMaxAccel				= ReadINIFloat(acSection, "MaxAccel");
		pType->fMaxAngularAccel			= ReadINIFloat(acSection, "MaxAngularAccel");
		pType->fMaxEngineDamage			= ReadINIFloat(acSection, "MaxEngineDamage");
		pType->fEngineRepairRate		= ReadINIFloat(acSection, "EngineRepairRate");
		pType->vEnginePos				= ReadINIVector3(acSection, "EnginePos");

		pType->iNumFlames				= ReadINIInt(acSection, "NumFlames");
		pType->avFlamePos[0]			= ReadINIVector3(acSection, "Flame1Pos");
		pType->avFlamePos[1]			= ReadINIVector3(acSection, "Flame2Pos");
		pType->avFlamePos[2]			= ReadINIVector3(acSection, "Flame3Pos");
		pType->avFlamePos[3]			= ReadINIVector3(acSection, "Flame4Pos");
		pType->iFlameStartSprite		= ReadINIInt(acSection, "FlameStartSprite");
		pType->iFlameEndSprite			= ReadINIInt(acSection, "FlameEndSprite");
		pType->fFlameSpeed				= ReadINIFloat(acSection, "FlameSpeed");
		pType->fFlameLifetime			= ReadINIFloat(acSection, "FlameLifetime");
		pType->fFlameStartSize			= ReadINIFloat(acSection, "FlameStartSize");
		pType->fFlameEndSize			= ReadINIFloat(acSection, "FlameEndSize");
		pType->FlameStartColor			= ReadINIColor(acSection, "FlameStartColor");
		pType->FlameEndColor			= ReadINIColor(acSection, "FlameEndColor");

		pType->fMaxShieldEnergy			= ReadINIFloat(acSection, "MaxShieldEnergy");
		pType->fMaxShieldReloadRate		= ReadINIFloat(acSection, "MaxShieldReloadRate");
		pType->fMaxShieldDamage			= ReadINIFloat(acSection, "MaxShieldDamage");
		pType->fShieldRepairRate		= ReadINIFloat(acSection, "ShieldRepairRate");
		pType->vShieldPos				= ReadINIVector3(acSection, "ShieldPos");
		pType->fShieldGlowStartSprite	= ReadINIFloat(acSection, "ShieldGlowStartSprite");
		pType->fShieldGlowEndSprite		= ReadINIFloat(acSection, "ShieldGlowEndSprite");
		pType->fShieldGlowLifetime		= ReadINIFloat(acSection, "ShieldGlowLifetime");
		pType->fShieldGlowStartSize		= ReadINIFloat(acSection, "ShieldGlowStartSize");
		pType->fShieldGlowEndSize		= ReadINIFloat(acSection, "ShieldGlowEndSize");
		pType->ShieldGlowStartColor		= ReadINIColor(acSection, "ShieldGlowStartColor");
		pType->ShieldGlowEndColor		= ReadINIColor(acSection, "ShieldGlowEndColor");

		pType->fMaxSensorsDamage		= ReadINIFloat(acSection, "MaxSensorsDamage");
		pType->fSensorsRepairRate		= ReadINIFloat(acSection, "SensorsRepairRate");
		pType->vSensorsPos				= ReadINIVector3(acSection, "SensorsPos");

		pType->iNumWeapons				= ReadINIInt(acSection, "NumWeapons");
		pType->fMaxWeaponEnergy			= ReadINIFloat(acSection,"MaxWeaponEnergy");
		pType->fWeaponEnergyReloadRate	= ReadINIFloat(acSection, "WeaponEnergyReloadRate");

		// Die einzelnen Waffen des Schiffstyps lesen
		for(int iWeapon = 0; iWeapon < pType->iNumWeapons; iWeapon++)
		{
			// Typ der Waffe lesen.
			// Achtung: In der INI-Datei ist 1 die erste Waffe, im Programm ist es 0.
			sprintf(acKey, "Weapon%dType", iWeapon + 1);
			pType->aiWeaponType[iWeapon] = ReadINIInt(acSection, acKey) - 1;

			// Position der Waffe lesen
			sprintf(acKey, "Weapon%dPos", iWeapon + 1);
			pType->avWeaponPos[iWeapon] = ReadINIVector3(acSection, acKey);

			// Zeiger auf den Waffentyp speichern
			pType->apWeaponType[iWeapon] = &m_aWeaponType[pType->aiWeaponType[iWeapon]];
		}

		// Cockpitposition lesen
		pType->vCockpitPos = ReadINIVector3(acSection, "CockpitPos");
	}

	return TB_OK;
}

// __________________________________________________________________
// Lädt die Waffentypen
/*
tbResult CGame::LoadWeaponTypes(BOOL bFullLoad)
{
	char			acSection[256];
	SWeaponType*	pType;


	// Anzahl der Waffentypen lesen
	m_iNumWeaponTypes = ReadINIInt("Ships and weapons", "NumWeaponTypes");
	if(m_iNumWeaponTypes == 12345678) TB_ERROR("Fehler beim Lesen der INI-Datei!", TB_ERROR);

	// Jeden Waffentyp durchgehen.
	// Die Sektionen heißen "Weapon1", "Weapon2" usw..
	for(int iType = 0; iType < m_iNumWeaponTypes; iType++)
	{
		sprintf(acSection, "Weapon%d", iType + 1);
		pType = &m_aWeaponType[iType];

		// Name und Modell- und Sounddateinamen lesen
		ReadINIString(acSection, "Name", pType->acName, 256);
		ReadINIString(acSection, "LauncherModel", pType->acLauncherModel, 256);
		ReadINIString(acSection, "ProjectileModel", pType->acProjectileModel, 256);
		ReadINIString(acSection, "LauncherSound", pType->acLauncherSound, 256);

		if(bFullLoad)
		{
			// Logbucheintrag erzeugen
			tbWriteToLog("Lade Waffentyp \"%s\"...", pType->acName);

			// Modell der Waffe und des Projektils laden - falls angegeben
			if(strcmp(pType->acLauncherModel, "[NOT FOUND]"))
			{
				// Es gibt ein Waffenmodell! Laden...
				pType->pLauncherModel = new tbModel;
				if(pType->pLauncherModel->Init(pType->acLauncherModel, "Data\\"))
				{
					// Fehler beim Laden!
					TB_ERROR("Fehler beim Laden eines Waffenmodells!", TB_ERROR);
				}
			}

			if(strcmp(pType->acProjectileModel, "[NOT FOUND]"))
			{
				// Es gibt ein Projektilmodell! Laden...
				pType->pProjectileModel = new tbModel;
				if(pType->pProjectileModel->Init(pType->acProjectileModel, "Data\\"))
				{
					// Fehler beim Laden!
					TB_ERROR("Fehler beim Laden eines Projektilmodells!", TB_ERROR);
				}
			}

			// Abschusssound laden
			pType->pLauncherSound = new tbSound;
			if(pType->pLauncherSound->Init(pType->acLauncherSound,
				                           DSBCAPS_STATIC | DSBCAPS_LOCDEFER | DSBCAPS_CTRL3D | DSBCAPS_CTRLFREQUENCY | DSBCAPS_MUTE3DATMAXDISTANCE,
				                           DS3DALG_HRTF_FULL, 16))
			{
				// Fehler beim Laden!
				TB_ERROR("Fehler beim Laden eines Sounds!", TB_ERROR);
			}

			// Minimale und maximale Distanzen setzen
			pType->pLauncherSound->SetDistances(200.0f, 2000.0f);

			// Raketenexplosionssound laden
			if(!pType->bIsLaserWeapon)
			{
				ReadINIString(acSection, "ExplosionSound", pType->acExplosionSound, 256);

				pType->pExplosionSound = new tbSound;
				if(pType->pExplosionSound->Init(pType->acExplosionSound,
												DSBCAPS_STATIC | DSBCAPS_LOCDEFER | DSBCAPS_CTRL3D | DSBCAPS_CTRLFREQUENCY | DSBCAPS_MUTE3DATMAXDISTANCE,
												DS3DALG_HRTF_FULL, 4))
				{
					// Fehler beim Laden!
					TB_ERROR("Fehler beim Laden eines Sounds!", TB_ERROR);
				}

				// Minimale und maximale Distanzen setzen
				pType->pExplosionSound->SetDistances(400.0f, 4000.0f);
			}
		}

		// Restliche Parameter laden
		pType->bIsLaserWeapon		= ReadINIInt(acSection, "IsLaserWeapon");
		pType->bRenderAsSprite		= ReadINIInt(acSection, "RenderAsSprite");
		pType->fMaxDamage			= ReadINIFloat(acSection, "MaxDamage");
		pType->fRepairRate			= ReadINIFloat(acSection, "RepairRate");
		pType->fProjectileLifetime	= ReadINIFloat(acSection, "ProjectileLifetime");
		pType->vProjectileStartPos	= ReadINIVector3(acSection, "ProjectileStartPos");
		pType->fMinReloadTime		= ReadINIFloat(acSection, "MinReloadTime");
		pType->fEnergyPerShot		= ReadINIFloat(acSection, "EnergyPerShot");
		pType->fDamageToShields		= ReadINIFloat(acSection, "DamageToShields");
		pType->fDamageToHull		= ReadINIFloat(acSection, "DamageToHull");
		pType->fHitForce			= ReadINIFloat(acSection, "HitForce");
		pType->iGlowSprite			= ReadINIInt(acSection, "GlowSprite");
		pType->fGlowSize			= ReadINIFloat(acSection, "GlowSize");
		pType->GlowColor			= ReadINIColor(acSection, "GlowColor");

		// Für Laserwaffen spezifische Parameter:
		if(pType->bIsLaserWeapon)
		{
			pType->bAutoAim			= ReadINIInt(acSection, "AutoAim");
			pType->fMaxAutoAimAngle	= ReadINIFloat(acSection, "MaxAutoAimAngle");
			pType->fLaserRadius		= ReadINIFloat(acSection, "LaserRadius");
			pType->fLaserLength		= ReadINIFloat(acSection, "LaserLength");
			pType->fLaserSpeed		= ReadINIFloat(acSection, "LaserSpeed");
			pType->iLaserSprite		= ReadINIInt(acSection, "LaserSprite");
			pType->LaserStartColor	= ReadINIColor(acSection, "LaserStartColor");
			pType->LaserEndColor	= ReadINIColor(acSection, "LaserEndColor");
		}

		// Für Raketenwaffen spezifische Parameter:
		if(!pType->bIsLaserWeapon)
		{
			pType->iMaxAmmo							= ReadINIInt(acSection, "MaxAmmo");
			pType->iMissileAIProc					= ReadINIInt(acSection, "MissileAIProc");
			pType->vMissileStartVector				= ReadINIVector3(acSection, "MissileStartVector");
			pType->fMissileStartSpeed				= ReadINIFloat(acSection, "MissileStartSpeed");
			pType->fMissileMass						= ReadINIFloat(acSection, "MissileMass");
			pType->fMissileMovementFriction			= ReadINIFloat(acSection, "MissileMovementFriction");
			pType->fMissileRotationFriction			= ReadINIFloat(acSection, "MissileRotationFriction");
			pType->fMissileAccel					= ReadINIFloat(acSection, "MissileAccel");
			pType->fMissileAngularAccel				= ReadINIFloat(acSection, "MissileAngularAccel");
			pType->fMissileExplosionStartSprite		= ReadINIFloat(acSection, "MissileExplosionStartSprite");
			pType->fMissileExplosionEndSprite		= ReadINIFloat(acSection, "MissileExplosionEndSprite");
			pType->fMissileExplosionLifetime		= ReadINIFloat(acSection, "MissileExplosionLifetime");
			pType->fMissileExplosionStartSize		= ReadINIFloat(acSection, "MissileExplosionStartSize");
			pType->fMissileExplosionEndSize			= ReadINIFloat(acSection, "MissileExplosionEndSize");
			pType->MissileExplosionStartColor		= ReadINIColor(acSection, "MissileExplosionStartColor");
			pType->MissileExplosionEndColor			= ReadINIColor(acSection, "MissileExplosionEndColor");
			pType->vMissileSmokeTrailPos			= ReadINIVector3(acSection, "MissileSmokeTrailPos");
			pType->iMissileSmokeTrailStartSprite	= ReadINIInt(acSection, "MissileSmokeTrailStartSprite");
			pType->iMissileSmokeTrailEndSprite		= ReadINIInt(acSection, "MissileSmokeTrailEndSprite");
			pType->fMissileSmokeTrailSpeed			= ReadINIFloat(acSection, "MissileSmokeTrailSpeed");
			pType->fMissileSmokeTrailLifetime		= ReadINIFloat(acSection, "MissileSmokeTrailLifetime");
			pType->fMissileSmokeTrailStartSize		= ReadINIFloat(acSection, "MissileSmokeTrailStartSize");
			pType->fMissileSmokeTrailEndSize		= ReadINIFloat(acSection, "MissileSmokeTrailEndSize");
			pType->MissileSmokeTrailStartColor		= ReadINIColor(acSection, "MissileSmokeTrailStartColor");
			pType->MissileSmokeTrailEndColor		= ReadINIColor(acSection, "MissileSmokeTrailEndColor");
		}
	}

	return TB_OK;
}
*/

// __________________________________________________________________
// Setzt die Kamera
tbResult CGame::SetupCamera()
{
	float			fDist;
//	CProjectile*	pMissile = NULL;
	tbMatrix		mCamera;
	tbMatrix		mProjection;
	tbVector3		vDirToTarget;


	// Position, Blickpunkt, Nach-Oben-Vektor und Blickfeld je nach Kameramodus setzen
	switch(m_CameraMode)
	{
	case CM_FREE:
	//MOD: S.Blaum
//		m_vCameraPos = m_pCamera->RelToAbsPos(m_pCamera->GetPosition());
		m_vCameraPos = m_pCamera->GetPosition();
		m_vCameraLookAt = m_vCameraPos + m_pCamera->m_vZAxis;
		m_vCameraUp = m_pCamera->m_vYAxis;
		m_fFOV = TB_DEG_TO_RAD(70.0f);
		break;


	case CM_COCKPIT:
		// Die Kamera befindet sich im Cockpit.
		// Wo das liegt (relativ zum Schiff), ist in der Variablen vCockpitPos
		// der SShipType-Struktur gespeichert.
		m_vCameraPos = m_pPlayer->RelToAbsPos(m_pPlayer->m_pType->vCockpitPos);
		m_vCameraLookAt = m_vCameraPos + m_pPlayer->m_vZAxis;
		m_vCameraUp = m_pPlayer->m_vYAxis;
		m_fFOV = TB_DEG_TO_RAD(70.0f);
		break;

	case CM_CHASE:
		// Die Kamera befindet sich hinter und über dem Schiff.
		m_vCameraPos = m_pPlayer->RelToAbsPos(tbVector3(0.0f, m_pPlayer->m_pType->pModel->GetBoundingSphereRadius() * 0.5f, -m_pPlayer->m_pType->pModel->GetBoundingSphereRadius() * 2.5f));
		m_vCameraLookAt = m_vCameraPos + m_pPlayer->m_vZAxis;
		m_vCameraUp = m_pPlayer->m_vYAxis;
		m_fFOV = TB_DEG_TO_RAD(80.0f);
		break;
/*
	case CM_FREECHASE:
		// Die Kamera befindet sich hinter und über dem Schiff.
		m_vCameraPos = m_pPlayer->RelToAbsPos(tbVector3(0.0f, m_pPlayer->m_pType->pModel->GetBoundingSphereRadius() * 0.5f, -m_pPlayer->m_pType->pModel->GetBoundingSphereRadius() * 2.5f));
		m_vCameraLookAt = m_vCameraPos + m_pPlayer->m_vZAxis;
		m_vCameraUp = tbVector3(0.0f, 1.0f, 0.0f);
		m_fFOV = TB_DEG_TO_RAD(80.0f);
		break;

	case CM_FRONTCHASE:
		// Die Kamera befindet sich vor und über dem Schiff.
		m_vCameraPos = m_pPlayer->RelToAbsPos(tbVector3(0.0f, m_pPlayer->m_pType->pModel->GetBoundingSphereRadius() * 0.5f, m_pPlayer->m_pType->pModel->GetBoundingSphereRadius() * 2.5f));
		m_vCameraLookAt = m_vCameraPos - m_pPlayer->m_vZAxis;
		m_vCameraUp = m_pPlayer->m_vYAxis;
		m_fFOV = TB_DEG_TO_RAD(80.0f);
		break;

	case CM_FLYBY:
		// Distanz zwischen Kamera und dem Spieler berechnen
		fDist = tbVector3Length(m_pPlayer->m_vPosition - m_vCameraPos);

		// Wenn die Distanz zu groß ist, wird die Kamera neu gesetzt:
		// genau vor den Spieler.
		if(fDist > 800.0f)
		{
			m_vCameraPos = m_pPlayer->m_vPosition +
				           tbVector3NormalizeEx(m_pPlayer->m_vVelocity) * 600.0f +
						   m_pPlayer->m_pType->pModel->GetBoundingSphereRadius() * m_pPlayer->m_vXAxis * 1.5f;
		}

		// Die Kamera schaut immer auf den Spieler.
		m_vCameraLookAt = m_pPlayer->m_vPosition;

		// Standardwerte für Nach-Oben-Vektor und Blickfeld
		m_vCameraUp = tbVector3(0.0f, 1.0f, 0.0f);
		m_fFOV = TB_DEG_TO_RAD(70.0f);
		break;

	case CM_PADLOCK:
		// Kein Ziel? Zurück zur Cockpitsicht!
		if(m_pPlayer->m_iTarget == -1)
		{
			m_CameraMode = CM_COCKPIT;
			return SetupCamera();
		}

		// Richtung vom Spieler zum erfassten Ziel berechnen
		vDirToTarget = tbVector3Normalize(m_aShip[m_pPlayer->m_iTarget].m_vPosition -
			                              m_pPlayer->m_vPosition);

		// Die Kamera so einstellen, dass sie beide Schiffe im Blick hat
		m_vCameraPos = m_pPlayer->m_vPosition -
			           vDirToTarget * m_pPlayer->m_pType->pModel->GetBoundingSphereRadius() * 2.5f +
					   m_pPlayer->m_vYAxis * m_pPlayer->m_pType->pModel->GetBoundingSphereRadius();
		m_vCameraLookAt = (m_aShip[m_pPlayer->m_iTarget].m_vPosition + m_pPlayer->m_vPosition) * 0.5f;
		m_vCameraUp = m_pPlayer->m_vYAxis;
		m_fFOV = TB_DEG_TO_RAD(80.0f);
		break;

	case CM_MISSILE:
	case CM_MISSILECHASE:
	case CM_MISSILEFLYBY:
		{
		// Die Rakete mit der größten verbleibenden Lebenszeit suchen,
		// die vom Spieler abgefeuert wurde
		for(int m = 0; m < 256; m++)
		{
			if(m_aProjectile[m].m_fLifetime > 0.0f &&
			   !m_aProjectile[m].m_pType->bIsLaserWeapon &&
			   m_aProjectile[m].m_iFiredBy == m_pPlayer->m_iIndex)
			{
				if(pMissile == NULL) pMissile = &m_aProjectile[m];
				else
				{
					// Bricht die Rakete den Rekord?
					if(m_aProjectile[m].m_fLifetime > pMissile->m_fLifetime) pMissile = &m_aProjectile[m];
				}
			}
		}

		// Keine Rakete (mehr) gefunden? Dann bleibt die Kamera, wie sie ist.
		if(pMissile == NULL) break;

		// CM_MISSILE, CM_MISSILECHASE oder CM_MISSILEFLYBY?
		if(m_CameraMode == CM_MISSILE)
		{
			// Die Kamera 2.5 Einheiten vor die Rakete setzen
			m_vCameraPos = pMissile->RelToAbsPos(tbVector3(0.0f, 0.0f, 2.5f));
			m_vCameraLookAt = m_vCameraPos + pMissile->m_vZAxis;
			m_vCameraUp = pMissile->m_vYAxis;
			m_fFOV = TB_DEG_TO_RAD(80.0f);
		}
		else if(m_CameraMode == CM_MISSILECHASE)
		{
			// Die Kamera hinter und über die Rakete setzen
			m_vCameraPos = pMissile->RelToAbsPos(tbVector3(0.0f, 5.0f, -15.0f));
			m_vCameraLookAt = pMissile->m_vPosition + pMissile->m_vZAxis;
			m_vCameraUp = tbVector3(0.0f, 1.0f, 0.0f);
			m_fFOV = TB_DEG_TO_RAD(80.0f);
		}
		else
		{
			// Distanz zwischen Kamera und der Rakete berechnen
			fDist = tbVector3Length(pMissile->m_vPosition - m_vCameraPos);

			// Wenn die Distanz zu groß ist, wird die Kamera neu gesetzt:
			// genau vor den Spieler.
			if(fDist > 300.0f)
			{
				m_vCameraPos = pMissile->m_vPosition +
							   tbVector3NormalizeEx(pMissile->m_vVelocity) * 250.0f +
							   pMissile->m_vXAxis * 5.0f;
			}

			// Die Kamera schaut immer auf die Rakete.
			m_vCameraLookAt = pMissile->m_vPosition;

			// Standardwerte für Nach-Oben-Vektor und Blickfeld
			m_vCameraUp = tbVector3(0.0f, 1.0f, 0.0f);
			m_fFOV = TB_DEG_TO_RAD(70.0f);
		}
		break;
		}
*/	}

#ifdef _3D_GLASSES_
	static DWORD dwFrame;

	dwFrame++;
	if(dwFrame % 2) m_vCameraPos -= tbVector3Cross(tbVector3Normalize(m_vCameraLookAt - m_vCameraPos), m_vCameraUp) * 0.01f;
	else m_vCameraPos += tbVector3Cross(tbVector3Normalize(m_vCameraLookAt - m_vCameraPos), m_vCameraUp) * 0.01f;
#endif

	// Kameramatrix erstellen und einsetzen
	mCamera = tbMatrixCamera(m_vCameraPos, m_vCameraLookAt, m_vCameraUp);
	tbDirect3D::SetTransform(D3DTS_VIEW, mCamera);

	// Projektionsmatrix erstellen und einsetzen
	mProjection = tbMatrixProjection(m_fFOV, tbDirect3D::GetAspect(), 5.0f, 5000.0f);
	tbDirect3D::SetTransform(D3DTS_PROJECTION, mProjection);

	// View-Frustum berechnen
	tbComputeClipPlanes(mCamera, mProjection, m_aViewFrustum);

	// DirectSound-Hörer setzen
	tbDirectSound::SetListener(m_vCameraPos,
							   tbVector3(mCamera.m12, mCamera.m22, mCamera.m32),
							   tbVector3(mCamera.m13, mCamera.m23, mCamera.m33),
							   1.0f);

	return TB_OK;
}

// __________________________________________________________________
//Erstellt die kamera
int CGame::CreateCamera() {
	CCamera* pCam;
	pCam = &m_aCamera[0];
	ZeroMemory(pCam, sizeof(CCamera));

	pCam->m_pGame = this;



	pCam->Reset();
	pCam->m_fMass = 0.0f;
	pCam->m_fMovementFriction = 0.0f;
	pCam->m_fRotationFriction = 0.0f;
	pCam->m_fRadius = 1.0f;
	pCam->m_fThrottle = 0.0f;

	return 0;
}

// Erstellt ein Schiff
int CGame::CreateShip(int iTeam,
					  int iType)
{
	CShip* pShip;

	// Freies Schiff suchen
	for(int iShip = 0; iShip < 32; iShip++)
	{
		pShip = &m_aShip[iShip];
		if(!pShip->m_bExists)
		{
			// Freies Schiff gefunden!
			// Speicherbereich zurücksetzen.
			ZeroMemory(pShip, sizeof(CShip));

			// Standardwerte eintragen
			pShip->m_pGame = this;
			pShip->m_bExists = TRUE;
			pShip->m_iIndex = iShip;

			// Team und Typ kopieren
			pShip->m_iTeam = iTeam;
			pShip->m_pType = &m_aShipType[iType];

			// Kein Ziel, Auto-Aim einschalten, Antriessound auf -1 (noch kein Sound),
			// Explosions-Count-Down auf -1
			pShip->m_iTarget = -1;
			pShip->m_bAutoAim = TRUE;
			pShip->m_iEngineSound = -1;
			pShip->m_fExplosionCountDown = 0.0f;
			pShip->m_fEvasiveManeuvers = 0.0f;
			pShip->m_vCockpitShaking = tbVector3(0.0f);

			// Objektsteuerung zurücksetzen.
			// Masse, Reibung und Radius übertragen.
			pShip->Reset();
			pShip->m_fMass = pShip->m_pType->fMass;
			pShip->m_fMovementFriction = pShip->m_pType->fMovementFriction;
			pShip->m_fRotationFriction = pShip->m_pType->fRotationFriction;
			pShip->m_fRadius = pShip->m_pType->pModel->GetBoundingSphereRadius();

			// Schub auf 0 setzen
			pShip->m_fThrottle = 0.0f;

			// Schild- und Waffenenergie auf Maximum
			pShip->m_fShieldEnergy = pShip->m_pType->fMaxShieldEnergy;
			pShip->m_fWeaponEnergy = pShip->m_pType->fMaxWeaponEnergy;

			// Wiederaufladung und Munition der Waffen auf 100%
			for(int iWeapon = 0; iWeapon < pShip->m_pType->iNumWeapons; iWeapon++)
			{
				pShip->m_afWeaponReload[iWeapon] = 0.0f;
				pShip->m_aiWeaponAmmo[iWeapon] = pShip->m_pType->apWeaponType[iWeapon]->iMaxAmmo;
			}

			// Index des Schiffs liefern
			return iShip;
		}
	}

	// Kein Platz mehr!
	return -1;
}
// __________________________________________________________________
//Bewegt die Kamera
tbResult CGame::MoveCameras(float fTime) {
	m_pCamera->MoveCamera(fTime);
	return TB_OK;
}

// Bewegt alle Schiffe
tbResult CGame::MoveShips(float fTime)
{
	BOOL		abChecked[32][32];
	CShip*		pShip1;
	CShip*		pShip2;
	int			iSound;
	tbVector3	vPosition;
	tbVector3	vCollision;
	tbVector3	v;
	tbVector3	vOldVelA1;
	tbVector3	vOldVelB1;
	tbVector3	vOldVelA2;
	tbVector3	vOldVelB2;
	tbVector3	vNewVelA;
	tbVector3	vNewVelB;
	tbVector3	vCenter;


	// Jedes Schiff durchgehen
	for(int iShip = 0; iShip < 32; iShip++)
	{
		// Existiert das Schiff?
		if(m_aShip[iShip].m_bExists)
		{
			// Bewegen!
			m_aShip[iShip].MoveShip(fTime);
		}
	}

	// Kollisionen zwischen Schiffen prüfen
	ZeroMemory(abChecked, 32 * 32 * sizeof(BOOL));
	for(int s1 = 0; s1 < 32; s1++)
	{
		for(int s2 = 0; s2 < 32; s2++)
		{
			if(abChecked[s1][s2] || abChecked[s2][s1]) continue;
			if(s1 == s2) continue;

			pShip1 = &m_aShip[s1];
			pShip2 = &m_aShip[s2];

			if(!pShip1->m_bExists || !pShip2->m_bExists) continue;

			// Kollision?
			if(!ShipHitsShip(pShip1, pShip2, &vCollision)) continue;

			// Beiden Schiffen Schaden zufügen. Er hängt von der Differenz der beiden
			// Geschwindigkeiten ab. Schiffe mit mehr Masse bekommen außerdem weniger
			// Schaden ab.
			pShip1->DoDamage(vCollision, tbVector3Length(pShip2->m_vVelocity - pShip1->m_vVelocity) * (pShip2->m_fMass / pShip1->m_fMass) * 0.4f);
			pShip2->DoDamage(vCollision, tbVector3Length(pShip1->m_vVelocity - pShip2->m_vVelocity) * (pShip1->m_fMass / pShip2->m_fMass) * 0.4f);

			// Der Vektor v ist der normalisierte Verbindunsvektor zwischen den
			// beiden Schiffsmittelpunkten.
			v = tbVector3NormalizeEx(pShip2->m_vPosition - pShip1->m_vPosition);

			// Den Geschwindigkeitsvektor des ersten Schiffs in eine Komponente
			// parallel zu v und eine Komponente senkrecht zu v zerlegen.
			// vOldVelA1 + vOldVelA2 ergeben zusammen wieder die ursprüngliche
			// Geschwindigkeit.
			vOldVelA1 = v * tbVector3Dot(v, pShip1->m_vVelocity);
			vOldVelA2 = pShip1->m_vVelocity - vOldVelA1;

			// Jetzt das gleiche mit dem anderen Schiff tun
			vOldVelB1 = v * tbVector3Dot(v, pShip2->m_vVelocity);
			vOldVelB2 = pShip2->m_vVelocity - vOldVelB1;

			// Die Schiffe "tauschen" nun ihre Geschwindigkeitskomponenten parallel zu v.
			// Das wird mit dem Stoßgesetz berechnet.
			vNewVelA = (((2.0f * pShip2->m_fMass * vOldVelB1) + (pShip1->m_fMass - pShip2->m_fMass) * vOldVelA1) / (pShip1->m_fMass + pShip2->m_fMass)) + vOldVelA2;
			vNewVelB = (((2.0f * pShip1->m_fMass * vOldVelA1) + (pShip2->m_fMass - pShip1->m_fMass) * vOldVelB1) / (pShip1->m_fMass + pShip2->m_fMass)) + vOldVelB2;

			// Den Schiffen die neue Geschwindigkeit zuweisen
			pShip1->m_vVelocity = vNewVelA;
			pShip2->m_vVelocity = vNewVelB;

			// Beide ein Stückchen voneinander weg bewegen, damit es nicht direkt
			// wieder eine Kollision gibt.
			vCenter = 0.5f * (pShip1->m_vPosition + pShip2->m_vPosition);
			pShip1->m_vPosition += tbVector3NormalizeEx(pShip1->m_vPosition - vCenter) * 10.0f;
			pShip2->m_vPosition += tbVector3NormalizeEx(pShip2->m_vPosition - vCenter) * 10.0f;

			// Die Cockpits rütteln lassen
//			pShip1->m_vCockpitShaking += tbVector3Random() * tbVector3Length(pShip1->m_vVelocity - pShip2->m_vVelocity) * 0.1f;
//			pShip2->m_vCockpitShaking += tbVector3Random() * tbVector3Length(pShip1->m_vVelocity - pShip2->m_vVelocity) * 0.1f;

			// Trümmerpartikel für beide Schiffe erzeugen
			for(int j = 0; j < (int)(tbVector3Length(pShip1->m_vVelocity - pShip2->m_vVelocity) * tbFloatRandom(0.05f, 0.15f)); j++)
			{
				m_pPSystem->AddParticle(tbFloatRandom(3.0f, 6.0f),
										tbFloatRandom(32.0f, 47.0f),
									    tbFloatRandom(32.0f, 47.0f),
										vCollision + tbVector3Random() * tbFloatRandom(0.0f, 5.0f),
										pShip1->m_vVelocity + tbVector3Random() * tbFloatRandom(0.0f, 50.0f),
										1.0f,
										tbFloatRandom(2.0f, 4.0f),
										tbFloatRandom(2.0f, 4.0f),
										pShip1->m_pType->DebrisColor + tbColorRandom(-1.0f) * 0.1f,
										tbColor(1.0f, 0.0f, 0.0f, 0.0f) + tbColorRandom(-1.0f) * 0.2f,
										tbFloatRandom(0.0f, 2.0f), tbFloatRandom(0.0f, 2.0f));

				m_pPSystem->AddParticle(tbFloatRandom(3.0f, 6.0f),
									    tbFloatRandom(32.0f, 47.0f),
										tbFloatRandom(32.0f, 47.0f),
										vCollision + tbVector3Random() * tbFloatRandom(0.0f, 5.0f),
										pShip2->m_vVelocity + tbVector3Random() * tbFloatRandom(0.0f, 50.0f),
										1.0f,
										tbFloatRandom(2.0f, 4.0f),
										tbFloatRandom(2.0f, 4.0f),
										pShip2->m_pType->DebrisColor + tbColorRandom(-1.0f) * 0.1f,
										tbColor(1.0f, 0.0f, 0.0f, 0.0f) + tbColorRandom(-1.0f) * 0.2f,
										tbFloatRandom(0.0f, 2.0f), tbFloatRandom(0.0f, 2.0f));
			}

			// Kollisionssound abspielen
/*			iSound = m_pCollisionSound->PlayAvailableBuffer();
			if(iSound != -1)
			{
				// Zufällige Frequenz und die 3D-Position setzen
				m_pCollisionSound->SetFrequency((DWORD)(iSound), tbIntRandom(34100, 44100));
				m_pCollisionSound->SetPosition((DWORD)(iSound), vCollision);
			}

			abChecked[s1][s2] = TRUE;
			abChecked[s2][s1] = TRUE;
*/
		}
	}

	return TB_OK;
}

// __________________________________________________________________
// Rendert alle Schiffe
tbResult CGame::RenderShips(float fTime)
{
	// Jedes Schiff durchgehen
	for(int iShip = 0; iShip < 32; iShip++)
	{
		// Existiert das Schiff?
		if(m_aShip[iShip].m_bExists)
		{
			// Wenn die Kamera im Cockpitmodus ist und das Schiff
			// das Schiff des Spielers ist, muss es nicht gezeichnet werden.
			if(!(m_CameraMode == CM_COCKPIT && iShip == 0))
			{
				// Ist es sichtbar?
				if(m_aShip[iShip].IsVisible())
				{
					// Rendern!
					m_aShip[iShip].Render(fTime);
				}
			}

			// Sound des Schiffs aktualisieren
			m_aShip[iShip].UpdateSound(fTime);
		}
	}

	return TB_OK;
}

// __________________________________________________________________
// Bewegt alle Projektile
/*
tbResult CGame::MoveProjectiles(float fTime)
{
	// Jedes Projektil durchgehen
	for(int p = 0; p < 256; p++)
	{
		// Existiert das Projektil?
		if(m_aProjectile[p].m_fLifetime > 0.0f)
		{
			// Bewegen!
			m_aProjectile[p].MoveProjectile(fTime);
		}
	}

	return TB_OK;
}
*/
// __________________________________________________________________
// Rendert alle Projektile
/*
tbResult CGame::RenderProjectiles(float fTime)
{
	// Jedes Projektil durchgehen
	for(int p = 0; p < 256; p++)
	{
		// Existiert das Projektil?
		if(m_aProjectile[p].m_fLifetime > 0.0f)
		{
			// Rendern!
			m_aProjectile[p].Render(fTime);
		}
	}

	return TB_OK;
}
*/
// __________________________________________________________________
// Rendert das "Sternenfeld"

tbResult CGame::RenderStarfield(float fTime)
{
	int			iSeed;
	tbVector3	vCameraPos;


	// Kameraposition auf 1000er runden (in int-Wert konvertieren und wieder zurück).
	// Nur bleiben die Kugeln immer an den gleichen Stellen.
	vCameraPos.x = (float)((int)(m_vCameraPos.x / 1000.0f) * 1000);
	vCameraPos.y = (float)((int)(m_vCameraPos.y / 1000.0f) * 1000);
	vCameraPos.z = (float)((int)(m_vCameraPos.z / 1000.0f) * 1000);

	// Die 27 die Kamera umgebenden Kugeln durchgehen
	for(float x = vCameraPos.x - 1000.0f; x <= vCameraPos.x + 1000.0f; x += 1000.0f)
	{
		for(float y = vCameraPos.y - 1000.0f; y <= vCameraPos.y + 1000.0f; y += 1000.0f)
		{
			for(float z = vCameraPos.z - 1000.0f; z <= vCameraPos.z + 1000.0f; z += 1000.0f)
			{
				// Ist die Kugel sichtbar?
				if(tbSphereVisible(tbVector3(x, y, z), 1500.0f, m_aViewFrustum))
				{
					// Zufallsgenerator für diese Kugel initialisieren
					// (durch eine willkürliche Formel)
					iSeed = (int)(x + 10.0f * y + 100.0f * z);
					srand(iSeed);

					// 200 "Sterne" in Form von Sprites erzeugen.
					// Es gibt drei verschiedene Typen mit den Nummern 2 bis 4
					// (das sind verschwommene weiße Flecken).
					for(int i = 0; i < 400; i++)
					{
						m_pSprites->AddSprite(tbIntRandom(2, 4),
											  tbVector3(x, y, z) + tbVector3Random() * tbFloatRandom(0.0f, 1500.0f),
											  tbColor(1.0f, 1.0f, 1.0f, tbFloatRandom(0.25f, 0.9f)) - tbColorRandom(0.0f) * 0.25f,
											  tbVector2(tbFloatRandom(1.0f, 5.0f)),
											  tbFloatRandom(0.0f, TB_PI),
											  TRUE);
					}
				}
			}
		}
	}

	// Zufallsgenerator mit der aktuellen Zeit initialisieren
	srand((int)(g_pSpaceRunner->m_fTime * 10000.0f) % 10000);

	return TB_OK;
}

// Rendert die Ebene
tbResult CGame::RenderPlain(float fTime)
{
/*
	static SHUDVertex	aVertex[256];
	tbVector3	vCameraPos;
	tbDirect3D::SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	aVertex[0].vPosition = tbVector3(-1000.0f, -1000.0f, 0.0f);
	aVertex[0].Color = tbColor(0.0f, 0.0f, 1.0f);
	aVertex[0].fRHW = 1.0f;
	aVertex[1].vPosition = tbVector3(-1000.0f, 1000.0f, 0.0f);
	aVertex[1].Color = tbColor(0.0f, 0.0f, 1.0f);
	aVertex[1].fRHW = 1.0f;
	aVertex[2].vPosition = tbVector3(1000.0f, -1000.0f, 0.0f);
	aVertex[2].Color = tbColor(0.0f, 0.0f, 1.0f);
	aVertex[2].fRHW = 1.0f;
	aVertex[3].vPosition = tbVector3(1000.0f, 1000.0f, 0.0f);
	aVertex[3].Color = tbColor(0.0f, 0.0f, 1.0f);
	aVertex[3].fRHW = 1.0f;
	// Linien zeichnen
	tbDirect3D::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, aVertex, sizeof(SHUDVertex));
*/
	return TB_OK;
}

// __________________________________________________________________
// Rendert das Cockpit
/*
tbResult CGame::RenderCockpit(float fTime)
{
	static tbVector3	vCockpitShaking;
	static char			acText[256];
	static tbColor		Color;
	static float		fReload;
	static CShip*		pTarget;
	static tbVector3	vTarget;
	static tbVector3	vWeapons;
	static tbVector3	vAimAt;
	static float		fWeaponSpeed;
	static tbMatrix		mViewProj;
	static SHUDVertex	aVertex[256];
	static float		fWeaponStartY;
	static int			iNumWeapons;


	// Wenn das Schiff des Spielers zerstört ist, wird kein Cockpit mehr gerendert.
	if(!m_pPlayer->m_bExists) return TB_OK;

	// Radar rendern
	RenderRadar(fTime);

	// Cockpitmodell rendern
	tbDirect3D::SetTransform(D3DTS_WORLD, tbMatrixTranslation(m_pPlayer->m_pType->vCockpitPos + tbVector3(0.0f, -10.0f, 5.0f) + m_pPlayer->m_vCockpitShaking) * m_pPlayer->m_mMatrix);
	m_pCockpitModel->Render();

	// Textur zurücksetzen, Alpha-Blending einschalten, Z-Buffer aus, Vertexformat setzen
	tbDirect3D::SetTexture(0, NULL);
	tbDirect3D::SetRS(D3DRS_ALPHABLENDENABLE, TRUE);
	tbDirect3D::SetRS(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	tbDirect3D::SetRS(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	tbDirect3D::SetRS(D3DRS_ZENABLE, FALSE);
	tbDirect3D::SetRS(D3DRS_ZWRITEENABLE, FALSE);
	tbDirect3D::SetFVF(HUD_FVF);

	if(m_pPlayer->m_iTarget != -1)
	{
		pTarget = &m_aShip[m_pPlayer->m_iTarget];

		// Ist das Ziel sichtbar?
		if(tbPointVisible(pTarget->m_vPosition, m_aViewFrustum))
		{
			// Sichtmatrix * Projektionsmatrix
			mViewProj = tbDirect3D::GetTransform(D3DTS_VIEW) * tbDirect3D::GetTransform(D3DTS_PROJECTION);

			// Die Position des erfassten Ziels auf dem Bildschirm bestimmen.
			// Dazu transformieren wir den Positionsvektor des Schiffs mit dem Produkt
			// aus Sicht- und Projektionsmatrix - heraus kommt dann die 2D-Position
			// des Schiffs.
			vTarget = tbVector3TransformCoords(pTarget->m_vPosition, mViewProj);

			// Die Position muss nun noch in Bildschirmkoordinaten umgewandelt werden.
			vTarget.x = tbDirect3D::GetScreenSize().x * 0.5f + (vTarget.x * tbDirect3D::GetScreenSize().x * 0.5f);
			vTarget.y = tbDirect3D::GetScreenSize().y * 0.5f - (vTarget.y * tbDirect3D::GetScreenSize().y * 0.5f);

			// Zielfadenkreuz zeichnen
			aVertex[0].vPosition = vTarget + tbVector3(-5.0f, 0.0f, 0.0f);
			aVertex[0].fRHW = 1.0f;
			aVertex[1].vPosition = vTarget + tbVector3(-20.0f, 0.0f, 0.0f);
			aVertex[1].fRHW = 1.0f;
			aVertex[2].vPosition = vTarget + tbVector3(5.0f, 0.0f, 0.0f);
			aVertex[2].fRHW = 1.0f;
			aVertex[3].vPosition = vTarget + tbVector3(20.0f, 0.0f, 0.0f);
			aVertex[3].fRHW = 1.0f;
			aVertex[4].vPosition = vTarget + tbVector3(0.0f, -5.0f, 0.0f);
			aVertex[4].fRHW = 1.0f;
			aVertex[5].vPosition = vTarget + tbVector3(0.0f, -20.0f, 0.0f);
			aVertex[5].fRHW = 1.0f;
			aVertex[6].vPosition = vTarget + tbVector3(0.0f, 5.0f, 0.0f);
			aVertex[6].fRHW = 1.0f;
			aVertex[7].vPosition = vTarget + tbVector3(0.0f, 20.0f, 0.0f);
			aVertex[7].fRHW = 1.0f;

			// Die Farbe hängt davon ab, ob das Ziel ein Freund oder ein Feind ist.
			if(pTarget->m_iTeam != m_pPlayer->m_iTeam)
			{
				// Feind: Rot!
				aVertex[0].Color = aVertex[2].Color = aVertex[4].Color = aVertex[6].Color = tbColor(1.0f, 0.25f, 0.25f, 0.75f);
				aVertex[1].Color = aVertex[3].Color = aVertex[5].Color = aVertex[7].Color = tbColor(1.0f, 0.25f, 0.25f, 0.0f);
			}
			else
			{
				// Freund: Grün!
				aVertex[0].Color = aVertex[2].Color = aVertex[4].Color = aVertex[6].Color = tbColor(0.25f, 1.0f, 0.25f, 0.75f);
				aVertex[1].Color = aVertex[3].Color = aVertex[5].Color = aVertex[7].Color = tbColor(0.25f, 1.0f, 0.25f, 0.0f);
			}

			// ------------------------------------------------------------------

			// Kleines Vorhaltekreuz zeichnen.
			// Mittelwert der Waffenpositionen und der Geschossgeschwindigkeiten berechnen.
			vWeapons = tbVector3(0.0f);
			fWeaponSpeed = 0.0f;
			iNumWeapons = 0;
			for(int i = 0; i < m_pPlayer->m_pType->iNumWeapons; i++)
			{
				if(m_pPlayer->m_pType->apWeaponType[i]->bIsLaserWeapon &&
				   !(m_pPlayer->m_pType->apWeaponType[i]->bAutoAim && m_pPlayer->m_bAutoAim))
				{
					vWeapons += m_pPlayer->m_pType->avWeaponPos[i];
					fWeaponSpeed += m_pPlayer->m_pType->apWeaponType[i]->fLaserSpeed;
					iNumWeapons++;
				}
			}

			if(iNumWeapons == 0)
			{
				for(int i = 0; i < m_pPlayer->m_pType->iNumWeapons; i++)
				{
					if(m_pPlayer->m_pType->apWeaponType[i]->bIsLaserWeapon)
					{
						vWeapons += m_pPlayer->m_pType->avWeaponPos[i];
						fWeaponSpeed += m_pPlayer->m_pType->apWeaponType[i]->fLaserSpeed;
						iNumWeapons++;
					}
				}
			}

			if(iNumWeapons == 0)
			{
				vWeapons = tbVector3(0.0f);
				fWeaponSpeed = 100.0f;
				iNumWeapons = 1;
			}

			vWeapons = (float)(iNumWeapons);
			fWeaponSpeed /= (float)(iNumWeapons);

			// Zielvektor berechnen
			vAimAt = tbComputeAimingVector(m_pPlayer->RelToAbsPos(vWeapons),
				                           pTarget->m_vPosition,
										   pTarget->m_vVelocity,
										   fWeaponSpeed);

			// Nun können wir den Punkt berechnen, auf den gezielt werden muss.
			// Dazu nehmen wir die neu vorhergesagte Zielposition und verschieben
			// sie noch ein wenig nach oben bzw. nach unten.
			// Wenn die Waffen beispielsweise unten am Schiff angebracht sind,
			// muss der Spieler das Zielfadenkreuz ja höher positionieren, um
			// zu treffen.
			// Außerdem muss noch die Cockpitposition mit eingerechnet werden.
			vAimAt -= m_pPlayer->RelToAbsDir(vWeapons);
			vAimAt += m_pPlayer->RelToAbsDir(m_pPlayer->m_pType->vCockpitPos);

			// Schaden an den Sensoren: das Kreuz ist ungenauer!
			vAimAt += tbVector3Random() * tbFloatRandom(0.0f, 100.0f * (1.0f - m_pPlayer->m_fSensorsEfficiency));

			// Kameratransformation und Projektion durchführen
			vAimAt = tbVector3TransformCoords(vAimAt, mViewProj);

			// Die Position muss nun noch in Bildschirmkoordinaten umgewandelt werden.
			vAimAt.x = tbDirect3D::GetScreenSize().x * 0.5f + (vAimAt.x * tbDirect3D::GetScreenSize().x * 0.5f);
			vAimAt.y = tbDirect3D::GetScreenSize().y * 0.5f - (vAimAt.y * tbDirect3D::GetScreenSize().y * 0.5f);

			// Kleines Vorhaltekreuz zeichnen
			aVertex[8].vPosition = vAimAt + tbVector3(-3.0f, 0.0f, 0.0f);
			aVertex[8].fRHW = 1.0f;
			aVertex[9].vPosition = vAimAt + tbVector3(-10.0f, 0.0f, 0.0f);
			aVertex[9].fRHW = 1.0f;
			aVertex[10].vPosition = vAimAt + tbVector3(3.0f, 0.0f, 0.0f);
			aVertex[10].fRHW = 1.0f;
			aVertex[11].vPosition = vAimAt + tbVector3(10.0f, 0.0f, 0.0f);
			aVertex[11].fRHW = 1.0f;
			aVertex[12].vPosition = vAimAt + tbVector3(0.0f, -3.0f, 0.0f);
			aVertex[12].fRHW = 1.0f;
			aVertex[13].vPosition = vAimAt + tbVector3(0.0f, -10.0f, 0.0f);
			aVertex[13].fRHW = 1.0f;
			aVertex[14].vPosition = vAimAt + tbVector3(0.0f, 3.0f, 0.0f);
			aVertex[14].fRHW = 1.0f;
			aVertex[15].vPosition = vAimAt + tbVector3(0.0f, 10.0f, 0.0f);
			aVertex[15].fRHW = 1.0f;
			aVertex[8].Color = aVertex[10].Color = aVertex[12].Color = aVertex[14].Color = tbColor(1.0f, 1.0f, 1.0f, 0.75f);
			aVertex[9].Color = aVertex[11].Color = aVertex[13].Color = aVertex[15].Color = tbColor(1.0f, 1.0f, 1.0f, 0.0f);

			// Linien zeichnen
			tbDirect3D::GetDevice()->DrawPrimitiveUP(D3DPT_LINELIST, 8, aVertex, sizeof(SHUDVertex));
		}
	}

	// ------------------------------------------------------------------

	// Zielfadenkreuz in der Mitte rendern
	aVertex[0].vPosition = tbVector3(-3.0f, 0.0f, 0.0f);
	aVertex[0].Color = tbColor(0.25f, 0.25f, 1.0f, 0.75f);
	aVertex[0].fRHW = 1.0f;
	aVertex[1].vPosition = tbVector3(-10.0f, 0.0f, 0.0f);
	aVertex[1].Color = tbColor(0.25f, 0.25f, 1.0f, 0.0f);
	aVertex[1].fRHW = 1.0f;
	aVertex[2].vPosition = tbVector3(3.0f, 0.0f, 0.0f);
	aVertex[2].Color = tbColor(0.25f, 0.25f, 1.0f, 0.75f);
	aVertex[2].fRHW = 1.0f;
	aVertex[3].vPosition = tbVector3(10.0f, 0.0f, 0.0f);
	aVertex[3].Color = tbColor(0.25f, 0.25f, 1.0f, 0.0f);
	aVertex[3].fRHW = 1.0f;
	aVertex[4].vPosition = tbVector3(0.0f, -3.0f, 0.0f);
	aVertex[4].Color = tbColor(0.25f, 0.25f, 1.0f, 0.75f);
	aVertex[4].fRHW = 1.0f;
	aVertex[5].vPosition = tbVector3(0.0f, -10.0f, 0.0f);
	aVertex[5].Color = tbColor(0.25f, 0.25f, 1.0f, 0.0f);
	aVertex[5].fRHW = 1.0f;
	aVertex[6].vPosition = tbVector3(0.0f, 3.0f, 0.0f);
	aVertex[6].Color = tbColor(0.25f, 0.25f, 1.0f, 0.75f);
	aVertex[6].fRHW = 1.0f;
	aVertex[7].vPosition = tbVector3(0.0f, 10.0f, 0.0f);
	aVertex[7].Color = tbColor(0.25f, 0.25f, 1.0f, 0.0f);
	aVertex[7].fRHW = 1.0f;

	for(int v = 0; v < 8; v++)
	{
		aVertex[v].vPosition.x += tbDirect3D::GetScreenSize().x * 0.5f;
		aVertex[v].vPosition.y += tbDirect3D::GetScreenSize().y * 0.5f;
	}

	// Linien zeichnen
	tbDirect3D::GetDevice()->DrawPrimitiveUP(D3DPT_LINELIST, 4, aVertex, sizeof(SHUDVertex));

	// Z-Buffer wieder einschalten, Alpha-Blending aus
	tbDirect3D::SetRS(D3DRS_ZENABLE, TRUE);
	tbDirect3D::SetRS(D3DRS_ZWRITEENABLE, TRUE);
	tbDirect3D::SetRS(D3DRS_ALPHABLENDENABLE, FALSE);

	// ------------------------------------------------------------------

	g_pSapceRunner->m_pFont2->Begin();

	// Radarreichweite anzeigen
	sprintf(acText, "Radar: %.0f", m_fRadarRange);
	g_pSpaceRunner->m_pFont2->DrawText(tbVector2(0.025f, 0.92f), acText, TB_FF_RELATIVE | TB_FF_RELATIVESCALING);

	// Auto-Aim-Status anzeigen
	sprintf(acText, m_pPlayer->m_bAutoAim ? "Auto-Aim ein" : "Auto-Aim aus");
	g_pSpaceRunner->m_pFont2->DrawText(tbVector2(0.025f, 0.95f), acText, TB_FF_RELATIVE | TB_FF_RELATIVESCALING);

	// Status aller Systeme anzeigen:
	// Hülle
	sprintf(acText, "Hülle:\t\t%.0f%%", m_pPlayer->m_fHullEfficiency * 100.0f);
	Color = tbColor(1.0f - m_pPlayer->m_fHullEfficiency, m_pPlayer->m_fHullEfficiency, 0.0f);
	g_pSpaceRunner->m_pFont2->DrawText(tbVector2(0.17f, 0.86f), acText, TB_FF_RELATIVE | TB_FF_RELATIVESCALING, -1, Color, Color);

	// Antrieb
	sprintf(acText, "Antrieb:\t%.0f%%", m_pPlayer->m_fEngineEfficiency * 100.0f);
	Color = tbColor(1.0f - m_pPlayer->m_fEngineEfficiency, m_pPlayer->m_fEngineEfficiency, 0.0f);
	g_pSpaceRunner->m_pFont2->DrawText(tbVector2(0.17f, 0.89f), acText, TB_FF_RELATIVE | TB_FF_RELATIVESCALING, -1, Color, Color);

	// Schildgenerator
	sprintf(acText, "Schilde:\t%.0f%%", m_pPlayer->m_fShieldEfficiency * 100.0f);
	Color = tbColor(1.0f - m_pPlayer->m_fShieldEfficiency, m_pPlayer->m_fShieldEfficiency, 0.0f);
	g_pSpaceRunner->m_pFont2->DrawText(tbVector2(0.17f, 0.92f), acText, TB_FF_RELATIVE | TB_FF_RELATIVESCALING, -1, Color, Color);

	// Sensoren
	sprintf(acText, "Sensoren:\t%.0f%%", m_pPlayer->m_fSensorsEfficiency * 100.0f);
	Color = tbColor(1.0f - m_pPlayer->m_fSensorsEfficiency, m_pPlayer->m_fSensorsEfficiency, 0.0f);
	g_pSpaceRunner->m_pFont2->DrawText(tbVector2(0.17f, 0.95f), acText, TB_FF_RELATIVE | TB_FF_RELATIVESCALING, -1, Color, Color);

	// Alle Waffen durchgehen.
	// Bestimmen, bei welcher y-Koordinate die erste Waffe angezeigt wird.
	// Das hängt davon ab, wie viele Waffen das Schiff des Spielers hat.
	// Sind es mehr als vier, müssen wir weiter oben anfangen, weil sonst kein
	// Platz wäre.
	if(m_pPlayer->m_pType->iNumWeapons > 4) fWeaponStartY = 0.86f - 0.03f * (float)(m_pPlayer->m_pType->iNumWeapons - 4);
	else fWeaponStartY = 0.86f;
	for(int iWeapon = 0; iWeapon < m_pPlayer->m_pType->iNumWeapons; iWeapon++)
	{
		// Text mit Name der Waffe und Schadenszustand generieren
		sprintf(acText, "%s: %.0f%%",
			    m_pPlayer->m_pType->apWeaponType[iWeapon]->acName,
				m_pPlayer->m_afWeaponEfficiency[iWeapon] * 100.0f);

		if(!m_pPlayer->m_pType->apWeaponType[iWeapon]->bIsLaserWeapon)
		{
			// Bei Raketen wird zusätzlich noch die Munition angezeigt.
			sprintf(acText, "%s (%d)", acText, m_pPlayer->m_aiWeaponAmmo[iWeapon]);
		}

		// Die Farbe deutet die verbleibende Nachladezeit an.
		if(m_pPlayer->m_afWeaponReload[iWeapon] <= 0.0f) Color = tbColor(0.5f, 1.0f, 0.5f);
		else
		{
			fReload = 1.0f - (m_pPlayer->m_afWeaponReload[iWeapon] / m_pPlayer->m_pType->apWeaponType[iWeapon]->fMinReloadTime);
			Color = tbColor(0.25f, 0.25f + fReload * 0.25f, 0.25f);
		}

		// Text rendern
		g_pGalactica->m_pFont2->DrawText(tbVector2(0.35f, fWeaponStartY + 0.03f * (float)(iWeapon)), acText, TB_FF_RELATIVE | TB_FF_RELATIVESCALING, -1, Color, Color);
	}

	// Schub anzeigen
	sprintf(acText, "Schub: %.0f%%", m_pPlayer->m_fThrottle * 100.0f);
	g_pGalactica->m_pFont2->DrawText(tbVector2(0.62f, 0.86f), acText, TB_FF_RELATIVE | TB_FF_RELATIVESCALING);

	// Geschwindigkeit anzeigen
	sprintf(acText, "v = %.0f", tbVector3Length(m_pPlayer->m_vVelocity));
	g_pGalactica->m_pFont2->DrawText(tbVector2(0.62f, 0.89f), acText, TB_FF_RELATIVE | TB_FF_RELATIVESCALING);

	// Schildenergie anzeigen
	sprintf(acText, "Schilde: %.0f%%", (m_pPlayer->m_fShieldEnergy / m_pPlayer->m_pType->fMaxShieldEnergy) * 100.0f);
	g_pGalactica->m_pFont2->DrawText(tbVector2(0.62f, 0.92f), acText, TB_FF_RELATIVE | TB_FF_RELATIVESCALING);

	// Waffenenergie anzeigen
	sprintf(acText, "Waffen: %.0f%%", (m_pPlayer->m_fWeaponEnergy / m_pPlayer->m_pType->fMaxWeaponEnergy) * 100.0f);
	g_pGalactica->m_pFont2->DrawText(tbVector2(0.62f, 0.95f), acText, TB_FF_RELATIVE | TB_FF_RELATIVESCALING);

	// ------------------------------------------------------------------

	if(m_pPlayer->m_iTarget != -1)
	{
		// Zieldaten anzeigen.
		// Die Farbe des Texts hängt davon ab, ob das Ziel zum eigenen Team
		// gehört oder zu einem gegnerischen.
		
		// Name des Zielschiffs anzeigen
		sprintf(acText, "Ziel: %s", pTarget->m_pType->acName);
		if(pTarget->m_iTeam != m_pPlayer->m_iTeam) Color = tbColor(1.0f, 0.5f, 0.5f);
		else Color = tbColor(0.5f, 1.0f, 0.5f);
		g_pGalactica->m_pFont2->DrawText(tbVector2(0.78f, 0.8f), acText, TB_FF_RELATIVE | TB_FF_RELATIVESCALING, -1, Color, Color);

		// Genauere Informationen anzeigen
		sprintf(acText, "d = %.0f, v = %.0f\nHülle: %.0f%%\nAntrieb: %.0f%%\nSchilde: %.0f%% (%.0f%%)\nSensoren: %.0f%%",
			    m_pPlayer->GetDistance(pTarget->m_vPosition),
				tbVector3Length(pTarget->m_vVelocity),
				pTarget->m_fHullEfficiency * 100.0f,
				pTarget->m_fEngineEfficiency * 100.0f,
				pTarget->m_fShieldEfficiency * 100.0f,
				(pTarget->m_fShieldEnergy / pTarget->m_pType->fMaxShieldEnergy) * 100.0f,
				pTarget->m_fSensorsEfficiency * 100.0f);

		if(pTarget->m_iTeam != m_pPlayer->m_iTeam) Color = tbColor(1.0f, 0.75f, 0.75f);
		else Color = tbColor(0.75f, 1.0f, 0.75f);
		g_pGalactica->m_pFont2->DrawText(tbVector2(0.78f, 0.83f), acText, TB_FF_RELATIVE | TB_FF_RELATIVESCALING,
			                             -1, Color, Color, tbVector2(1.0f), -2.0f, 0.25f, -8.0f);
	}

	g_pGalactica->m_pFont2->End();

	return TB_OK;
}
*/
// __________________________________________________________________
// Rendert das Radar
/*
tbResult CGame::RenderRadar(float fTime)
{
	tbVector3	vRelPos;
	DWORD		dwRadius;
	DWORD		x;
	DWORD		y;
	DWORD		y2;
	DWORD		dwColor;
	DWORD		dwLineColor;


	m_pRadar->BeginDraw(NULL, D3DLOCK_DISCARD);

	// Das Bild leeren
	m_pRadar->DrawFilledRect(0, 0, 127, 127, 0);

	// Einen pulsierenden Kreis zeichnen
	dwRadius = (DWORD)(m_fTime * 63.0f) % 64;
	dwColor = m_pRadar->MakeRGB(31, 127 - dwRadius * 2, 31);
	m_pRadar->DrawCircle(63, 63, dwRadius, dwColor);

	// Mehrere feste Kreise zeichnen
	m_pRadar->DrawCircle(63, 63, 13, m_pRadar->MakeRGB(0, 191, 0));
	m_pRadar->DrawCircle(63, 63, 29, m_pRadar->MakeRGB(0, 159, 0));
	m_pRadar->DrawCircle(63, 63, 45, m_pRadar->MakeRGB(0, 127, 0));
	m_pRadar->DrawCircle(63, 63, 61, m_pRadar->MakeRGB(0, 91, 0));

	// Hilfslinien einzeichnen
	m_pRadar->DrawLine(0, 0, 127, 127, m_pRadar->MakeRGB(0, 255, 0));
	m_pRadar->DrawLine(127, 0, 0, 127, m_pRadar->MakeRGB(0, 255, 0));
	m_pRadar->DrawLine(0, 63, 127, 63, m_pRadar->MakeRGB(0, 191, 0));
	m_pRadar->DrawLine(63, 0, 63, 127, m_pRadar->MakeRGB(0, 191, 0));

	// Ein Kreuz in der Mitte zeichnen (eigenes Schiff)
	m_pRadar->DrawLine(61, 63, 65, 63, m_pRadar->MakeRGB(255, 255, 255));
	m_pRadar->DrawLine(63, 61, 63, 65, m_pRadar->MakeRGB(255, 255, 255));

	// Jedes Schiff durchgehen
	for(DWORD s = 0; s < 32; s++)
	{
		if(m_aShip[s].m_bExists && &m_aShip[s] != m_pPlayer)
		{
			// Relative Position dieses Schiffs zum Spieler berechnen
			vRelPos = m_pPlayer->AbsToRelPos(m_aShip[s].m_vPosition);

			// Schaden an den Sensoren bringt Störungen
			vRelPos += tbVector3Random() * tbFloatRandom(0.0f, 100.0f * (1.0f - m_pPlayer->m_fSensorsEfficiency));

			// Daraus errechnen wir nun die Position auf dem Radar.
			// Je weiter das Schiff auf der z-Achse entfernt ist, desto weiter
			// oben erscheint das Schiff auf dem Radar.
			// Die x-Achse bleibt gleich.
			x = 63 + (DWORD)(vRelPos.x / m_fRadarRange * 63.0f);
			y = 63 - (DWORD)(vRelPos.z / m_fRadarRange * 63.0f);

			// Diese Position muss innerhalb gewisser Grenzen liegen, damit
			// wir nicht außerhalb des Radars zeichnen.
			if(x < 4 || x > 123 || y < 4 || y > 123) continue;

			// Jetzt berechnen wir den Endpunkt der Linie, die unterhalb bzw. überhalb
			// des Schiffs auf dem Radar erscheint. Dieser hängt von der relativen
			// y-Position des Schiffs ab.
			y2 = y + (DWORD)(vRelPos.y / m_fRadarRange * 63.0f);

			// Die Linie zurechtschneiden
			if(y2 < 4) y2 = 4;
			if(y2 > 123) y2 = 123;

			// Die Farbe des Schiffs auf dem Radar und der Linie bestimmen.
			// Beide hängen davon ab, ob das Schiff im selben Team ist wie der
			// Spieler.
			if(m_aShip[s].m_iTeam == m_pPlayer->m_iTeam)
			{
				// Freund - Grün!
				dwColor = m_pRadar->MakeRGB(0, 255, 0);
				dwLineColor = m_pRadar->MakeRGB(0, 127, 0);
			}
			else
			{
				// Feind - Rot!
				dwColor = m_pRadar->MakeRGB(255, 0, 0);
				dwLineColor = m_pRadar->MakeRGB(127, 0, 0);
			}

			// Die Linie zeichnen
			m_pRadar->DrawLine(x, y, x, y2, dwLineColor);

			// Das Schiff als kleinen Kreis zeichnen, mit einem Punkt in der Mitte
			m_pRadar->DrawCircle(x, y, 3, dwColor);
			m_pRadar->SetPixel(x, y, dwColor);

			// Wenn das Schiff das aktuell erfasste Ziel ist, wird das durch zwei
			// zusätzliche Kreise hervorgehoben.
			if((int)(s) == m_pPlayer->m_iTarget)
			{
				// Weiße Kreise zeichnen
				m_pRadar->DrawCircle(x, y, 2, m_pRadar->MakeRGB(255, 255, 255));
				m_pRadar->DrawCircle(x, y, 4, m_pRadar->MakeRGB(255, 255, 255));
			}
		}
	}

	// Jedes Projektil durchgehen und nach Raketen suchen
	for(DWORD p = 0; p < 256; p++)
	{
		if(m_aProjectile[p].m_fLifetime > 0.0f && !m_aProjectile[p].m_pType->bIsLaserWeapon)
		{
			// Relative Position dieser Rakete zum Spieler berechnen
			vRelPos = m_pPlayer->AbsToRelPos(m_aProjectile[p].m_vPosition);

			// Schaden an den Sensoren einrechnen
			vRelPos += tbVector3Random() * tbFloatRandom(0.0f, 100.0f * (1.0f - m_pPlayer->m_fSensorsEfficiency));

			// Die üblichen Berechnungen...
			x = 63 + (DWORD)(vRelPos.x / m_fRadarRange * 63.0f);
			y = 63 - (DWORD)(vRelPos.z / m_fRadarRange * 63.0f);
			if(x < 4 || x > 123 || y < 4 || y > 123) continue;
			y2 = y + (DWORD)(vRelPos.y / m_fRadarRange * 63.0f);
			if(y2 < 4) y2 = 4;
			if(y2 > 123) y2 = 123;

			// Die Farbe der Rakete auf dem Radar und der Linie bestimmen.
			// Beide hängen davon ab, ob das Schiff, das die Rakete abgefeuert hat,
			// im selben Team ist wie der Spieler.
			if(m_aShip[m_aProjectile[p].m_iFiredBy].m_iTeam == m_pPlayer->m_iTeam)
			{
				// Grün!
				dwColor = m_pRadar->MakeRGB(0, 255, 0);
				dwLineColor = m_pRadar->MakeRGB(0, 127, 0);
			}
			else
			{
				// Rot!
				dwColor = m_pRadar->MakeRGB(255, 0, 0);
				dwLineColor = m_pRadar->MakeRGB(127, 0, 0);
			}

			// Die Linie zeichnen
			m_pRadar->DrawLine(x, y, x, y2, dwLineColor);

			// Die Rakete als kleinen Kreis zeichnen
			m_pRadar->DrawFilledCircle(x, y, 1, dwColor);
		}
	}

	// Schaden an den Sensoren: Störungen im Radar
	for(int i = 0; i < tbIntRandom(0, (int)((1.0f - m_pPlayer->m_fSensorsEfficiency) * 8000.0f)); i++)
	{
		// Einen Pixel mit zufälliger Farbe zeichnen
		m_pRadar->SetPixel(tbIntRandom(0, 127),
			               tbIntRandom(0, 127),
						   m_pRadar->MakeRGB(tbIntRandom(0, 31),
						                     tbIntRandom(127, 255),
											 tbIntRandom(0, 31)));
	}

	m_pRadar->EndDraw();

	return TB_OK;
}
*/
// __________________________________________________________________
// Rendert das Blenden der Sonne
/*
tbResult CGame::RenderSunFlares(float fTime)
{
	SHUDVertex	aVertex[4];
	float		fBrightness;


	// Punktprodukt aus Kamera-z-Achse und Linie von Kamera zur Sonne berechnen.
	// Dieses wird dann direkt verwendet, um die Helligkeit zu beschreiben.
	fBrightness = tbVector3Dot(tbVector3NormalizeEx(m_vCameraLookAt - m_vCameraPos),
							   tbVector3(-1.0f, 0.0f, 0.0f));
	// Winkelbereich begrenzen
	if(fBrightness <= 0.0f) return TB_OK;

	// Helligkeit potenzieren, um einen stärkeren Verfall zu erhalten
	fBrightness = powf(fBrightness, 6.0f);

	// Vertizes für ein Rechteck über den ganzen Bildschirm erstellen
	// Links unten
	aVertex[0].vPosition = tbVector3(0.0f, tbDirect3D::GetScreenSize().y, 0.5f);
	aVertex[0].fRHW = 1.0f;
	aVertex[0].Color = tbColor(1.0f, 1.0f, 0.9f, fBrightness);

	// Links oben
	aVertex[1].vPosition = tbVector3(0.0f, 0.0f, 0.0f);
	aVertex[1].fRHW = 1.0f;
	aVertex[1].Color = tbColor(1.0f, 1.0f, 0.9f, fBrightness);

	// Rechts unten
	aVertex[2].vPosition = tbVector3(tbDirect3D::GetScreenSize().x, tbDirect3D::GetScreenSize().y, 0.5f);
	aVertex[2].fRHW = 1.0f;
	aVertex[2].Color = tbColor(1.0f, 1.0f, 0.9f, fBrightness);

	// Rechts oben
	aVertex[3].vPosition = tbVector3(tbDirect3D::GetScreenSize().x, 0.0f, 0.5f);
	aVertex[3].fRHW = 1.0f;
	aVertex[3].Color = tbColor(1.0f, 1.0f, 0.9f, fBrightness);

	// Vertexformat und Render-States setzen
	tbDirect3D::SetTexture(0, NULL);
	tbDirect3D::SetRS(D3DRS_ALPHABLENDENABLE, TRUE);
	tbDirect3D::SetRS(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	tbDirect3D::SetRS(D3DRS_DESTBLEND, D3DBLEND_ONE);
	tbDirect3D::SetRS(D3DRS_ZENABLE, FALSE);
	tbDirect3D::SetRS(D3DRS_ZWRITEENABLE, FALSE);
	tbDirect3D::SetFVF(HUD_FVF);

	// Rechteck zeichnen
	tbDirect3D::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, aVertex, sizeof(SHUDVertex));

	// Z-Buffer wieder einschalten, Alpha-Blending aus
	tbDirect3D::SetRS(D3DRS_ZENABLE, TRUE);
	tbDirect3D::SetRS(D3DRS_ZWRITEENABLE, TRUE);
	tbDirect3D::SetRS(D3DRS_ALPHABLENDENABLE, FALSE);

	return TB_OK;
}
*/
// __________________________________________________________________
// Prüft, ob zwei Schiffe kollidieren
BOOL CGame::ShipHitsShip(CShip* pShipA,
						 CShip* pShipB,
						 tbVector3* pvOut)
{
	// Prüfen, ob die Kollisionsmodelle der Schiffe sich schneiden
	return tbModelHitsModel(pShipA->m_pType->pCollisionModel, pShipA->m_mMatrix, pShipA->m_mInvMatrix,
		                    pShipB->m_pType->pCollisionModel, pShipB->m_mMatrix, pShipB->m_mInvMatrix,
							pvOut);
}

// __________________________________________________________________