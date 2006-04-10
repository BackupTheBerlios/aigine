/*
**	Space Runner
**	=========
**
**	Game.h
**	------
**	Das Spiel
*/

//#include "DriftElement.h"

// __________________________________________________________________
// Kameramodi
enum ECameraMode
{
	CM_COCKPIT,			// Sicht aus dem Cockpit
	CM_CHASE,			// Jagdkamera
	CM_FREECHASE,		// Freie Jagdkamera
	CM_FRONTCHASE,		// Jagdkamera von vorne
	CM_FLYBY,			// "Fly-By"-Kamera
	CM_PADLOCK,			// Padlock-Kamera
	CM_MISSILE,			// Raketenkamera
	CM_MISSILECHASE,	// Jagdraketenkamera
	CM_MISSILEFLYBY,		// "Fly-By"-Raketenkamera
	CM_FREE				//MOD: freie kamera
};

// __________________________________________________________________
// Klasse für das Spiel
class CGame
{
public:
	// Variablen
	float					m_fTime;				// Stoppuhr
	BOOL					m_bPaused;				// Pause?
	BOOL					m_bUseJoystick;			// Joystick benutzen?
	char*					m_apcCameraMode[16];	// Namen der Kameramodi
	
	tbSkyBox*				m_pSkyBox;				// Sky-Box
	PDIRECT3DCUBETEXTURE9	m_pSkyBoxTex;			// Textur der Sky-Box

	tbSpriteEngine*			m_pSprites;				// Sprites
	PDIRECT3DTEXTURE9		m_pSpritesTex;			// Textur der Sprites
	tbParticleSystem*		m_pPSystem;				// Partikelsystem
	tbEffect*				m_pSpritesEffect;		// Effekt für die Sprites

//	tbSound*				m_pHullHitSound;		// Sound für Hüllentreffer
//	tbSound*				m_pExplosionSound;		// Sound für Explosionen
//	tbSound*				m_pCollisionSound;		// Sound für Kollisionen

	tbModel*				m_pCockpitModel;		// Cockpitmodell
//	tbDraw2D*				m_pRadar;				// Zeichenklasse für das Radar
	float					m_fRadarRange;			// Radarreichweite

	int						m_iNumShipTypes;		// Anzahl der Schiffstypen
	int						m_iNumWeaponTypes;		// Anzahl der Waffentypen
	int						m_iNumCheckPointTypes;	// Anzahl der CheckPointtypen
	SShipType				m_aShipType[256];		// Die Schiffstypen
	SWeaponType				m_aWeaponType[256];		// Wie Waffentypen
	SCheckPointType			m_aCheckPointType[256];	//Die CheckPointTypen

	ECameraMode				m_CameraMode;			// Kameramodus;
	tbVector3				m_vCameraPos;			// Position der Kamera
	tbVector3				m_vCameraLookAt;		// Blickpunkt der Kamera
	tbVector3				m_vCameraUp;			// Nach-Oben-Vektor der Kamera
	float					m_fFOV;					// Sichtfeld
	tbPlane					m_aViewFrustum[6];		// View-Frustum

	CCamera					m_aCamera[1];
	CCamera*				m_pCamera;				//Camera-Object
	
	CShip					m_aShip[MAX_PLAYERS];			// Die Schiffe
	
	CShip*					m_pPlayer;				// Zeiger auf das Schiff des Spielers
	CCheckPoint				m_aCheckPoint[64];		// Die CheckPoints

	//CProjectile				m_aProjectile[256];		// Die Projektile

	int						m_iNumElementTypes;		// Anzahl der Streckenypen
	SRoadElementType		m_aElementType[256];	//Die Streckentypen	
	CDriftElement			m_aElement[100];		//Die Streckenstücke
	CDriftElement*			m_pRoadElement;			//Steckenobjekt

	// Konstruktor
	CGame();

	// Methoden
	tbResult	Init();																		// Initialisierung
	tbResult	Exit();																		// Herunterfahren
	tbResult	Load();																		// Laden
	tbResult	Unload();																	// Entladen
	tbResult	Move(float fTime);															// Bewegen
	tbResult	Render(float fTime);														// Rendern

	int			ReadINIInt(char* pcSection, char* pcKey);									// Liest einen int-Wert aus der INI-Datei
	float		ReadINIFloat(char* pcSection, char* pcKey);									// Liest einen float-Wert aus der INI-Datei
	tbVector3	ReadINIVector3(char* pcSection, char* pcKey);								// Liest einen 3D-Vektor aus der INI-Datei
	tbColor		ReadINIColor(char* pcSection, char* pcKey);									// Liest eine Farbe (4D-Vektor) aus der INI-Datei
	tbResult	ReadINIString(char* pcSection, char* pcKey, char* pcOut, int iBufferSize);	// Liest einen String aus der INI-Datei
	tbResult	LoadSpriteTypes();															// Lädt die Sprite-Typen
	tbResult	LoadShipTypes(BOOL bFullLoad);												// Lädt die Schiffstypen
//	tbResult	LoadWeaponTypes(BOOL bFullLoad);											// Lädt die Waffentypen
	tbResult	LoadTunnelType(int iTNum);	
	int			CreateTunnel(int iType);											// Erstellt eine Rennstrecke
// CheckPoints werden geladen und konfigurierut
	tbResult	LoadCheckPointTypes(BOOL bFullLoad);

	tbResult	SetupCamera();																// Setzt die Kamera
	int			CreateCamera();
	tbResult	MoveCameras(float fTime);
	int			CreateShip(int iTeam, int iType);											// Erstellt ein Schiff
	void		CreateAllCheckPoints();
	int			CreateCheckPoint(int iType);
	tbResult	MoveShips(float fTime);														// Bewegt alle Schiffe
	tbResult	RenderShips(float fTime);													// Rendert alle Schiffe
	tbResult	RenderCheckPoints(float fTime);													// Rendert alle Schiffe

//	tbResult	MoveProjectiles(float fTime);												// Bewegt alle Projektile
//	tbResult	RenderProjectiles(float fTime);												// Rendert alle Projektile
	
	int			RenderCoordinate(tbVector3 vPos);											// Rendert ein Koordinatensystem
	tbResult	RenderStarfield(float fTime);												// Rendert das "Sternenfeld"
	tbResult	RenderCockpit(float fTime);													// Rendert das Cockpit
//	tbResult	RenderRadar(float fTime);													// Rendert das Radar
//	tbResult	RenderSunFlares(float fTime);												// Rendert das Blenden der Sonne
	tbResult	RenderTunnel(float fTime, tbMatrix m_transform);
	tbResult	RenderPlain(float fTime);
	BOOL		ShipHitsShip(CShip* pShipA, CShip* pShipB, tbVector3* pvOut);				// Kollidieren zwei Schiffe?
	BOOL		ShipHitsCheckPoint(CShip* pShip, CCheckPoint* pCheckPoint, tbVector3* pvOut);

};
// __________________________________________________________________