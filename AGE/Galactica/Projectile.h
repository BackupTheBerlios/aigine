/*
**	GALACTICA
**	=========
**
**	Projectile.h
**	------------
**	Alles, was etwas mit den Projektilen (Raketen, Laserstrahlen) zu tun hat
*/

// __________________________________________________________________
// Klasse für ein Projektil
class CProjectile : public tbObject
{
public:
	// Variablen
	float			m_fLifetime;			// Verbleibende Lebenszeit
	SWeaponType*	m_pType;				// Zeiger auf die Waffentypstruktur
	int				m_iFiredBy;				// Nummer des Schiffs, das gefeuert hat
	CShip*			m_pTarget;				// Aktuelles Ziel

	// Methoden
	tbResult MoveProjectile(float fTime);	// Bewegt das Projektil
	tbResult Render(float fTime);			// Rendert das Projektil
};

// __________________________________________________________________