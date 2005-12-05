/*
**	Space Runner
**	=========
**
**	Camera.h
**	------
**	Camera-Objekt für die freie Kamera
*/

class CGame;

// __________________________________________________________________

// Klasse für die Kamera
class CCamera : public tbObject
{
public:
	// Variablen
	CGame*		m_pGame;					// Zeiger auf die Spielklasse
	tbVector3	m_vSteering;				// Lenkung

	// Methoden
	tbResult	Move(float fTime);					// Bewegt die Camera
	tbResult	Control(float fTime);				// Steuerung der Kamera
};

// __________________________________________________________________