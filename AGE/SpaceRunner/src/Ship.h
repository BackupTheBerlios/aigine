/*
**	Space Runner
**	=========
**
**	Ship.h
**	------
**	Alles, was etwas mit den Schiffen zu tun hat
*/

class CGame;

// __________________________________________________________________
// Struktur für einen Waffentyp
struct SWeaponType
{
	// Allgemein:
	char		acName[256];					// Name des Waffentyps
	char		acLauncherModel[256];			// Name der Modelldatei für die Vorrichtung
	char		acProjectileModel[256];			// Name der Geschossmodelldatei
	char		acLauncherSound[256];			// Name der Abschusssounddatei
	char		acExplosionSound[256];			// Name der Explosionssounddatei
	tbModel*	pLauncherModel;					// Modell der Vorrichtung
	tbModel*	pProjectileModel;				// Modell des Geschosses
	tbSound*	pLauncherSound;					// Sound für den Abschuss
	tbSound*	pExplosionSound;				// Sound für die Explosion
	BOOL		bIsLaserWeapon;					// Ist es eine Laserwaffe?
	BOOL		bRenderAsSprite;				// Als Sprite rendern?

	float		fMaxDamage;						// Schadenskapazität des Waffentyps
	float		fRepairRate;					// Reparierungsrate des Waffentyps

	float		fProjectileLifetime;			// Lebenszeit der Geschosse
	tbVector3	vProjectileStartPos;			// Startpunkt der Geschosse
	float		fMinReloadTime;					// Minimale Nachladezeit
	float		fEnergyPerShot;					// Benötigte Waffenenergie pro Schuss
	float		fDamageToShields;				// Schaden für die Schilde
	float		fDamageToHull;					// Schaden für die Hülle
	float		fHitForce;						// Kraft bei einem Treffer
	int			iGlowSprite;					// Leucht-Sprite
	float		fGlowSize;						// Größe des Leucht-Sprites
	tbColor		GlowColor;						// Farbe des Leucht-Sprites

	// Nur für Laserwaffen:
	BOOL		bAutoAim;						// Auto-Aim unterstützt?
	float		fMaxAutoAimAngle;				// Maximaler Winkel für Auto-Aim
	float		fLaserRadius;					// Radius des Strahls
	float		fLaserLength;					// Länge des Strahls
	float		fLaserSpeed;					// Strahlgeschwindigkeit
	int			iLaserSprite;					// Sprite des Strahls
	tbColor		LaserStartColor;				// Startfarbe des Lasers
	tbColor		LaserEndColor;					// Endfarbe des Lasers

	// Nur für Raketenwaffen:
	int			iMaxAmmo;						// Maximale Munition
	int			iMissileAIProc;					// KI-Programm der Rakete
	tbVector3	vMissileStartVector;			// Startvektor für Raketen
	float		fMissileStartSpeed;				// Startgeschwindigkeit für Raketen
	float		fMissileMass;					// Masse der Raketen
	float		fMissileMovementFriction;		// Reibung für Bewegung
	float		fMissileRotationFriction;		// Reibung für Rotation
	float		fMissileAccel;					// Beschleunigung der Raketen
	float		fMissileAngularAccel;			// Drehbeschleunigung der Raketen
	float		fMissileExplosionStartSprite;	// Explosionsstart-Sprite
	float		fMissileExplosionEndSprite;		// Explosionsend-Sprite
	float		fMissileExplosionLifetime;		// Explosionlebenszeit
	float		fMissileExplosionStartSize;		// Explosionsstartgröße
	float		fMissileExplosionEndSize;		// Explosionsendgröße
	tbColor		MissileExplosionStartColor;		// Startfarbe der Explosion
	tbColor		MissileExplosionEndColor;		// Endfarbe der Explosion
	tbVector3	vMissileSmokeTrailPos;			// Position der Rauchfahne
	int			iMissileSmokeTrailStartSprite;	// Startsprite für die Rauchfahne
	int			iMissileSmokeTrailEndSprite;	// Endsprite für die Rauchfahne
	float		fMissileSmokeTrailSpeed;		// Geschwindigkeit der Rauchpartikel
	float		fMissileSmokeTrailLifetime;		// Lebenszeit der Rauchpartikel
	float		fMissileSmokeTrailStartSize;	// Startgröße der Rauchfahne
	float		fMissileSmokeTrailEndSize;		// Endgröße der Rauchfahne
	tbColor		MissileSmokeTrailStartColor;	// Startfarbe der Rauchfahne
	tbColor		MissileSmokeTrailEndColor;		// Endfarbe der Rauchfahne
};

// __________________________________________________________________
// Struktur für einen Schiffstyp
struct SShipType
{
	int				iIndex;						// Index in der Liste
	char			acName[256];				// Name des Schiffstyps
	char			acDesc[1024];				// Beschreibungstext
	char			acModel[256];				// Name der Modelldatei
	char			acCollisionModel[256];		// Name des Kollisionsmodells
	char			acEngineSound[256];			// Sounddatei des Antriebs
	char			acShieldSound[256];			// Sounddatei für Schutzschild
	tbModel*		pModel;						// Modell des Schiffstyps
	tbModel*		pCollisionModel;			// Kollisionsmodell
	tbSound*		pEngineSound;				// Sound des Antriebs
	tbSound*		pShieldSound;				// Schutzschildsound

	float			fMass;						// Masse
	float			fMovementFriction;			// Reibung bei der Bewegung
	float			fRotationFriction;			// Reibung bei der Rotation

	float			fMaxHullDamage;				// Schadenskapazität der Hülle
	float			fHullRepairRate;			// Reparierungsrate der Hülle
	tbColor			DebrisColor;				// Farbe der Trümmerteile

	float			fMaxAccel;					// Maximale Beschleunigung
	float			fMaxAngularAccel;			// Maximale Drehbeschleunigung
	float			fMaxEngineDamage;			// Schadenskapazität des Antriebs
	float			fEngineRepairRate;			// Reparierungsrate des Antriebs
	tbVector3		vEnginePos;					// Relative Position des Antriebs

	int				iNumFlames;					// Anzahl der Flammen
	tbVector3		avFlamePos[4];				// Relative Ausgangspunkte der Flammen
	int				iFlameStartSprite;			// Startsprite der Flammen
	int				iFlameEndSprite;			// Endsprite der Flammen
	float			fFlameSpeed;				// Geschwindigkeit der Flammen
	float			fFlameLifetime;				// Lebenszeit der Flammenpartikel
	float			fFlameStartSize;			// Startgröße der Flammenpartikel
	float			fFlameEndSize;				// Endgröße der Flammenpartikel
	tbColor			FlameStartColor;			// Startfarbe der Flammenpartikel
	tbColor			FlameEndColor;				// Endfarbe der Flammenpartikel

	float			fMaxShieldEnergy;			// Maximale Schildenergie
	float			fMaxShieldReloadRate;		// Wiederaufladrate
	float			fMaxShieldDamage;			// Schadenskapazität des Schildgenerators
	float			fShieldRepairRate;			// Reparierungsrate des Schildgenerators
	tbVector3		vShieldPos;					// Relative Position des Schildgenerators
	float			fShieldGlowStartSprite;		// Start-Sprite für Schildleuchten
	float			fShieldGlowEndSprite;		// End-Sprite für Schildleuchten
	float			fShieldGlowLifetime;		// Lebenszeit für Schildleuchten
	float			fShieldGlowStartSize;		// Startgröße für Schildleuchten
	float			fShieldGlowEndSize;			// Endgröße für Schildleuchten
	tbColor			ShieldGlowStartColor;		// Startfarbe für Schildleuchten
	tbColor			ShieldGlowEndColor;			// Endfarbe für Schildleuchten

	float			fMaxSensorsDamage;			// Schadenskapazität der Sensoren
	float			fSensorsRepairRate;			// Reparierungsrate der Sensoren
	tbVector3		vSensorsPos;				// Relative Position der Sensoren

	int				iNumWeapons;				// Anzahl der Waffensysteme
	int				aiWeaponType[8];			// Waffentypen
	tbVector3		avWeaponPos[8];				// Relative Positionen der Waffensysteme
	SWeaponType*	apWeaponType[8];			// Zeiger auf die Waffentypen
	float			fMaxWeaponEnergy;			// Maximale Waffenenergie
	float			fWeaponEnergyReloadRate;	// Maximale Wiederaufladrate für die Waffenenergie

	tbVector3		vCockpitPos;				// Position des Cockpits
};

// __________________________________________________________________
// Klasse für ein Schiff
class CShip : public tbObject
{
public:
	// Variablen
	CGame*		m_pGame;					// Zeiger auf die Spielklasse

	BOOL		m_bExists;					// Existiert das Schiff?
	int			m_iIndex;					// Index im Array
	
	int			m_iTeam;					// Welches Team?
	SShipType*	m_pType;					// Zeiger auf Schiffstypstruktur
	int			m_iEngineSound;				// Soundpuffer für den Antrieb

	int			m_iTarget;					// Aktuelles Ziel
	BOOL		m_bAutoAim;					// Auto-Aim ein/aus
	float		m_fExplosionCountDown;		// Count-Down für Explosionen

	float		m_fThrottle;				// Antriebsstärke
	tbVector3	m_vSteering;				// Lenkung
	float		m_fEvasiveManeuvers;		// Ausweichmanöver?

	tbVector3	m_vCockpitShaking;			// Rütteln des Cockpits

	float		m_fHullDamage;				// Schaden der Hülle
	float		m_fEngineDamage;			// Schaden des Antriebs
	float		m_fShieldDamage;			// Schaden des Schildgenerators
	float		m_fSensorsDamage;			// Schaden der Sensoren
	float		m_afWeaponDamage[8];		// Schaden der Waffen
	float		m_fHullEfficiency;			// Effizienz der Hülle
	float		m_fEngineEfficiency;		// Effizienz des Antriebs
	float		m_fShieldEfficiency;		// Effizienz des Schildgenerators
	float		m_fSensorsEfficiency;		// Effizienz der Sensoren
	float		m_afWeaponEfficiency[8];	// Effizienz der Waffen

	float		m_fShieldEnergy;			// Energie der Schilde
	float		m_fWeaponEnergy;			// Waffenenergie
	int			m_aiWeaponAmmo[8];			// Munition der Waffen
	float		m_afWeaponReload[8];		// Nachladezeit der Waffen

	// Methoden
	tbResult	MoveShip(float fTime);					// Bewegt das Schiff
	tbResult	Render(float fTime);					// Rendert das Schiff
	tbResult	UpdateSound(float fTime);				// Aktualisiert den Sound des Schiffs
	tbResult	Fire(int iWeapon);						// Feuert eine Waffe ab
	tbResult	Control(float fTime);					// Steuerung des Schiffs
	void		DoDamage(tbVector3 vHit, float fPower);	// Fügt dem Schiff Schaden zu
	int			FindNextTarget(int iStart, int iTeam);	// Findet nächstes Ziel
	BOOL		IsVisible();							// Ist das Schiff sichtbar?
};

// __________________________________________________________________