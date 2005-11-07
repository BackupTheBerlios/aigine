/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbParticleSystem.h
	==================
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Partikelsystem

	Autor:
	David Scherfgen

********************************************************************/

// Struktur für einen Partikel
struct TRIBASE_API tbParticle
{
	float		fLifeTime;		// Restliche Lebenszeit
	float		fType;			// Typ
	float		fDeltaType;		// Änderung des Typs pro Sekunde
	tbVector3	vPosition;		// Position
	tbVector3	vVelocity;		// Geschwindigkeitsvektor
	float		fFriction;		// Reibung (bremst die Partikel ab)
	float		fSize;			// Größe
	float		fDeltaSize;		// Änderung der Größe pro Sekunde
	tbColor		Color;			// Farbe
	tbColor		DeltaColor;		// Änderung der Farbe pro Sekunde
	float		fRotation;		// Rotation
	float		fDeltaRotation;	// Änderung der Größe pro Sekunde
};

// ******************************************************************
// Klasse des Partikelsystems
class TRIBASE_API tbParticleSystem
{
private:
	// Variablen
	DWORD		m_dwMaxParticles;	// Maximale Anzahl der Partikel
	DWORD		m_dwNumParticles;	// Anzahl der Partikel
	tbParticle*	m_pParticles;		// Die Partikel

public:
	// Konstruktor und Destruktor
	tbParticleSystem();
	~tbParticleSystem();

	// Methoden
	tbResult	Init(DWORD dwMaxParticles);																				// Initialisiert das Partikelsystem
	tbResult	Clear();																								// Löscht alle Partikel
	tbResult	Move(const float fTime);																				// Bewegt alle Partikel
	tbResult	AddToSpriteEngine(tbSpriteEngine* pSpriteEngine, int* piStartSprite = NULL, int* piEndSprite = NULL);	// Fügt die Partikel zu einer Sprite-Engine hinzu

	// Fügt einen Partikel hinzu
	int AddParticle(const float fLifeTime,
		            const float fStartType,
					const float fEndType,
					const tbVector3& vPosition,
					const tbVector3& vVelocity,
					const float fFriction = 1.0f,
					const float fStartSize = 1.0f,
					const float fEndSize = 10.0f,
					const tbColor& StartColor = tbColor(1.0f),
					const tbColor& EndColor = tbColor(0.0f),
					const float fStartRotation = 0.0f,
					const float fEndRotation = 0.0f);

	// Inline-Methoden
	inline DWORD		GetMaxParticles()	{return m_dwMaxParticles;}
	inline tbParticle*	GetParticles()		{return m_pParticles;}
};

// ******************************************************************