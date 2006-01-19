/*
**	Space Runner
**	=========
**
**	CheckPoint.h
**	------
**	SBlaum
*/

class CGame;

// __________________________________________________________________
// Struktur für einen CheckPoint
struct SCheckPointType
{
	int				iIndex;						// Index in der Liste
	char			acModel[256];				// Name der Modelldatei
	char			acCollisionModel[256];		// Name des Kollisionsmodells
	tbModel*		pModel;						// Modell des CheckPointTyps
	tbModel*		pCollisionModel;			// Kollisionsmodell
};

// __________________________________________________________________
// Klasse für ein Schiff
class CCheckPoint : public tbObject
{
public:
	// Variablen
	CGame*		m_pGame;					// Zeiger auf die Spielklasse

	BOOL		m_bExists;					// Existiert dder CheckPoint?
	int			m_iIndex;					// Index im Array
	
	SCheckPointType*	m_pType;					// Zeiger auf CheckPointTypstruktur

	// Methoden
	tbResult	Render(float fTime);					// Rendert den CheckPoint
	BOOL		IsVisible();							// Ist der CheckPoint sichtbar?
};

// __________________________________________________________________