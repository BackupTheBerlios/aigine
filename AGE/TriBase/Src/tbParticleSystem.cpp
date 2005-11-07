/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbParticleSystem.cpp
	====================
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Ein Partikelsystem

	Autor:
	David Scherfgen

********************************************************************/

#include <TriBase.h>


// ******************************************************************
// Konstruktor der tbParticleSystem-Klasse
tbParticleSystem::tbParticleSystem()
{
	// Alles zurücksetzen
	ZeroMemory(this, sizeof(tbParticleSystem));
}

// ******************************************************************
// Destruktor der tbParticleSystem-Klasse
tbParticleSystem::~tbParticleSystem()
{
	// Partikel löschen
	TB_SAFE_MEMFREE(m_pParticles);
}

// ******************************************************************
// Initialisierung des Partikelsystems
tbResult tbParticleSystem::Init(DWORD dwMaxParticles)
{
	// Parameter prüfen
	if(dwMaxParticles == 0) TB_ERROR_INVALID_VALUE("dwMaxParticles", TB_ERROR);


	// Maximale Anzahl der Partikel kopieren
	m_dwMaxParticles = dwMaxParticles;

	// Speicherplatz reservieren
	m_pParticles = (tbParticle*)(tbMemAlloc((int)(dwMaxParticles) * sizeof(tbParticle)));
	if(m_pParticles == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

	return TB_OK;
}


// ******************************************************************
// Zurücksetzen des Partikelsystems
tbResult tbParticleSystem::Clear()
{
	// Speicherbereich zurücksetzen
	ZeroMemory(m_pParticles, m_dwMaxParticles * sizeof(tbParticle));

	// Zähler zurücksetzen
	m_dwNumParticles = 0;

	return TB_OK;
}

// ******************************************************************
// Bewegt alle Partikel
tbResult tbParticleSystem::Move(const float fTime)
{
	tbParticle* pParticle;

	// Alle Partikel durchgehen
	for(DWORD dwParticle = 0; dwParticle < m_dwNumParticles; dwParticle++)
	{
		pParticle = &m_pParticles[dwParticle];

		// Lebenszeit verringern
		pParticle->fLifeTime -= fTime;

		// Ist der Partikel verschwunden?
		if(pParticle->fLifeTime <= 0.0f)
		{
			// Den Partikel löschen und die Anzahl der Partikel verringern
			if(m_dwNumParticles > 1) m_pParticles[dwParticle] = m_pParticles[m_dwNumParticles - 1];
			m_dwNumParticles--;
			continue;
		}

		// Partikel bewegen
		pParticle->vPosition += pParticle->vVelocity * fTime;

		if(pParticle->fFriction != 1.0f)
		{
			// Abbremsen
			pParticle->vVelocity *= powf(pParticle->fFriction, fTime);
		}

		// Partikel animieren
		pParticle->fType += pParticle->fDeltaType * fTime;
		pParticle->fSize += pParticle->fDeltaSize * fTime;
		pParticle->Color += pParticle->DeltaColor * fTime;
		pParticle->fRotation += pParticle->fDeltaRotation * fTime;
	}

	return TB_OK;
}

// ******************************************************************
// Hinzufügen der Partikel zu einer Sprite-Engine
tbResult tbParticleSystem::AddToSpriteEngine(tbSpriteEngine* pSpriteEngine,
											 int* piStartSprite,	// = NULL
											 int* piEndSprite)		// = NULL
{
	int iStartSprite = -1;
	int iEndSprite;


	// Parameter prüfen
	if(pSpriteEngine == NULL) TB_ERROR_NULL_POINTER("pSpriteEngine", TB_ERROR);


	// Alle Partikel durchgehen
	tbParticle* pParticle = &m_pParticles[0];
	for(DWORD dwParticle = 0; dwParticle < m_dwNumParticles; dwParticle++)
	{
		iEndSprite = pSpriteEngine->AddSprite((int)(pParticle->fType),
											  pParticle->vPosition,
											  pParticle->Color,
											  tbVector2(pParticle->fSize),
											  pParticle->fRotation);
		if(iEndSprite == -1) break;
		if(iStartSprite == -1) iStartSprite = iEndSprite;

		pParticle++;
	}

	// Start- und End-Sprite - wenn erwünscht - speichern
	if(piStartSprite != NULL) *piStartSprite = iStartSprite;
	if(piEndSprite != NULL) *piEndSprite = iEndSprite;

	return TB_OK;
}

// ******************************************************************
// Fügt einen Partikel hinzu
int tbParticleSystem::AddParticle(const float fLifeTime,
								  const float fStartType,
								  const float fEndType,
								  const tbVector3& vPosition,
								  const tbVector3& vVelocity,
								  const float fFriction,		// = 1.0f
								  const float fStartSize,		// = 1.0f
								  const float fEndSize,			// = 0.0f
								  const tbColor& StartColor,	// = tbColor(1.0f)
								  const tbColor& EndColor,		// = tbColor(0.0f)
								  const float fStartRotation,	// = 0.0f
								  const float fEndRotation)		// = 0.0f
{
	// Ist noch Platz?
	if(m_dwNumParticles >= m_dwMaxParticles) return TB_ERROR;

	// Partikel eintragen
	tbParticle* pParticle = &m_pParticles[m_dwNumParticles];
	float fInvLifeTime = 1.0f / fLifeTime;
	pParticle->fLifeTime		= fLifeTime;
	pParticle->fType			= fStartType;
	pParticle->fDeltaType		= ((fEndType + 1.0f) - fStartType) * fInvLifeTime;
	pParticle->vPosition		= vPosition;
	pParticle->vVelocity		= vVelocity;
	pParticle->fFriction		= fFriction;
	pParticle->fSize			= fStartSize;
	pParticle->fDeltaSize		= (fEndSize - fStartSize) * fInvLifeTime;
	pParticle->Color			= StartColor;
	pParticle->DeltaColor		= (EndColor - StartColor) * fInvLifeTime;
	pParticle->fRotation		= fStartRotation;
	pParticle->fDeltaRotation	= (fEndRotation - fStartRotation) * fInvLifeTime;

	// Anzahl der Partikel erhöhen
	m_dwNumParticles++;

	return (int)(m_dwNumParticles - 1);
}

// ******************************************************************