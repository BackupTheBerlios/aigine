///////////////////////////////////////////////////////////////////////////////////
//						D R I F T E L E M E N T . H								 //
//-------------------------------------------------------------------------------//
// @description: Mainclass for the gineric level-element (tunnel: strait/left/right)

#ifndef DRIFTELEMENT_H
#define DRIFTELEMENT_H

#include <iostream>
using namespace std;



// __________________________________________________________________
// Struktur für einen Straeckentype
struct SRoadElementType	{
	int				iIndex;						// Index in der Liste
	int				iRoadType;					// Gerade (0), nach rechts(1) oder nach links(-1) gekrümmtes Element
	char			acName[256];				// Name des Straeckentyps
	char			acDesc[1024];				// Beschreibungstext
	char			acModel[256];				// Name der Modelldatei
	char			acCollisionModel[256];		// Name des Kollisionsmodells
	tbModel*		pModel;						// Modell des Straeckentyps
	tbModel*		pCollisionModel;			// Kollisionsmodell

	float			fMass;						// Masse
	float			fMovementFriction;			// Reibung bei der Bewegung
	//float			fRotationFriction;			// Reibung bei der Rotation

	tbVector3		vDirection;					// Ausrichtungs Vektor im Raum
};

class CDriftElement : public tbObject {
	
public :
	//Variablen
	CGame*		m_pGame;					// Zeiger auf die Spielklasse
	BOOL		m_bExists;					// Existiert bereits ein Straeckenelement in der naehe?
	int			m_iIndex;					// Index im Array
	SRoadElementType* m_pType;				// Zeiger auf die Teilstreckenstruktur

	
	CDriftElement();							//Konstruktor
	~CDriftElement();						//Destruktor
	// Methoden
	tbResult	MoveElement(tbVector3 vAlign, tbVector3 vPos);		// Ausrichten des Streckenabschnittes
	tbResult	MoveElement(CDriftElement* pLastElement );		// Ausrichten des Streckenabschnittes an einem bereits Vorhandenem
	tbResult	Render(float fTime);					// Rendert den Streckenabschnitt
	
};


#endif
