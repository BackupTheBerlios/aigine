/*
**	Space Runner
**	=========
**
**	Camera.h
**	------
**	Camera-Objekt f�r die freie Kamera
*/

class CGame;

// __________________________________________________________________

// Klasse f�r die Kamera
class CCamera : public tbObject
{
public:
	// Variablen
	CGame*		m_pGame;					// Zeiger auf die Spielklasse
	float		m_fThrottle;				// Antriebsst�rke
	tbVector3	m_vSteering;				// Lenkung

	// Methoden
	tbResult	MoveCamera(float fTime);					// Bewegt die Camera
	tbResult	Control(float fTime);				// Steuerung der Kamera
};

// __________________________________________________________________