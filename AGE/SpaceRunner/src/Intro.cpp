/*
**	Space Runner
**	=========
**
**	Intro.cpp
**	---------
**	Der Vorspann
*/

#include "SpaceRunner.h"


// __________________________________________________________________
// Vertizes für das Titelbild
#define TITLE_FVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
struct STitleVertex
{
	tbVector3	vPosition;
	float		fRHW;
	D3DCOLOR	Color;
	tbVector2	vTex0;
};

// __________________________________________________________________
// Initialisiert den Spielzustand
tbResult CIntro::Init()
{
	// Laden...
	if(Load()) TB_ERROR("Fehler beim Laden des Spielzustands!", TB_ERROR);

	// Briefing-Musik abspielen
	if(g_pSpaceRunner->m_pTitle->GetState() != State_Running)
	{
		// Mit Looping...
		g_pSpaceRunner->m_pTitle->Play();
	}

	return TB_OK;
}

// __________________________________________________________________
// Fährt den Spielzustand herunter
tbResult CIntro::Exit()
{
	// Entladen...
	Unload();

	return TB_OK;
}

// __________________________________________________________________
// Lädt den Spielzustand
tbResult CIntro::Load()
{

	// Titelbild laden
	m_pTitle = tbTextureManager::GetTexture("Data\\Title.jpg", TRUE, D3DX_DEFAULT, D3DX_DEFAULT, 1);
	if(m_pTitle == NULL) TB_ERROR("Fehler beim Laden von Data\\Title.jpg!", TB_ERROR);

	return TB_OK;
}

// __________________________________________________________________
// Entlädt den Spielzustand
tbResult CIntro::Unload()
{
	// Die Textur löschen
	tbTextureManager::ReleaseTexture(m_pTitle);

	return TB_OK;
}

// __________________________________________________________________
// Bewegt den Spielzustand
tbResult CIntro::Move(float fTime)
{
	// Wenn eine der typischen Tasten gedrückt wurde: zum Hauptmenü!
	if(WasButtonPressed(TB_KEY_NUMPADENTER) ||
	   WasButtonPressed(TB_KEY_RETURN) ||
	   WasButtonPressed(TB_KEY_SPACE) ||
	   WasButtonPressed(TB_MOUSE_BUTTON(0)))
	{
		g_pSpaceRunner->SetGameState(GS_MENU);
	}

	// Musik überprüfen
	g_pSpaceRunner->m_pTitle->Process();

	return TB_OK;
}

// __________________________________________________________________
// Rendert den Spielzustand
tbResult CIntro::Render(float fTime)
{
	STitleVertex aVertex[4];


	// Puffer leeren und Szene beginnen
	tbDirect3D::Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
					  tbColor(0.0f, 0.0f, 0.0f), 1.0f, 0);
	tbDirect3D::BeginScene();

	// ------------------------------------------------------------------

	// Vertexformat und Titelbildtextur setzen, Z-Buffer aus
	tbDirect3D::SetFVF(TITLE_FVF);
	tbDirect3D::SetTexture(0, m_pTitle);
	tbDirect3D::SetRS(D3DRS_ZENABLE, D3DZB_FALSE);

	// Die vier Vertizes des Titelbilds erstellen (Rechteck)
	// Links unten
	aVertex[0].vPosition = tbVector3(0.0f, tbDirect3D::GetScreenSize().y, 0.5f);
	aVertex[0].fRHW = 1.0f;
//	aVertex[0].Color = tbColor(1.0f, 0.8f, 0.8f);
	aVertex[0].vTex0 = tbVector2(0.0f, 1.0f);

	// Links oben
	aVertex[1].vPosition = tbVector3(0.0f, 0.0f, 0.0f);
	aVertex[1].fRHW = 1.0f;
	aVertex[1].Color = tbColor(0.8f, 1.0f, 0.8f);
	aVertex[1].vTex0 = tbVector2(0.0f, 0.0f);

	// Rechts unten
	aVertex[2].vPosition = tbVector3(tbDirect3D::GetScreenSize().x, tbDirect3D::GetScreenSize().y, 0.5f);
	aVertex[2].fRHW = 1.0f;
	aVertex[2].Color = tbColor(0.8f, 0.8f, 1.0f);
	aVertex[2].vTex0 = tbVector2(1.0f, 1.0f);

	// Rechts oben
	aVertex[3].vPosition = tbVector3(tbDirect3D::GetScreenSize().x, 0.0f, 0.5f);
	aVertex[3].fRHW = 1.0f;
	aVertex[3].Color = tbColor(1.0f, 1.0f, 0.8f);
	aVertex[3].vTex0 = tbVector2(1.0f, 0.0f);

	// Als Dreiecksfolge zeichnen
	tbDirect3D::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, aVertex, sizeof(STitleVertex));

	// ------------------------------------------------------------------
	// Text anzeigen
	g_pSpaceRunner->m_pCourier24->Begin();
	g_pSpaceRunner->m_pCourier24->DrawText(tbVector2(1.0f, 0.85f), "Powered by TriBase", TB_FF_RELATIVE | TB_FF_RELATIVESCALING | TB_FF_ALIGN_HRIGHT);
	g_pSpaceRunner->m_pCourier24->DrawText(tbVector2(0.9f, 0.9f), "Version", TB_FF_RELATIVE | TB_FF_RELATIVESCALING | TB_FF_ALIGN_HRIGHT);
	g_pSpaceRunner->m_pCourier24->DrawText(tbVector2(1.0f, 0.9f), VERSION, TB_FF_RELATIVE | TB_FF_RELATIVESCALING | TB_FF_ALIGN_HRIGHT);
	g_pSpaceRunner->m_pCourier24->End();

	// ------------------------------------------------------------------

	// Szene beenden
	tbDirect3D::EndScene();

	return TB_OK;
}

// __________________________________________________________________