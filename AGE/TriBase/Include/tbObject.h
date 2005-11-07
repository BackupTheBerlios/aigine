/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbObject.h
	==========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Kontrolle von Objekten (Achsenausrichtung, Bewegung...)

	Autor:
	David Scherfgen

********************************************************************/

// Die Objektklasse
class TRIBASE_API tbObject
{
public:
	// Variablen
	tbVector3	m_vXAxis;				// x-Achse des Objekts (absolut)
	tbVector3	m_vYAxis;				// y-Achse des Objekts (absolut)
	tbVector3	m_vZAxis;				// z-Achse des Objekts (absolut)
	tbVector3	m_vPosition;			// Position des Objekts (absolut)
	tbVector3	m_vScaling;				// Skalierung auf allen drei Achsen (absolut)
	tbVector3	m_vVelocity;			// Bewegungsvektor mit Geschwindigkeit (absolut)
	tbVector3	m_vRotation;			// Rotation um alle drei Achsen des Objekts (relativ)
	tbMatrix	m_mMatrix;				// Transformationsmatrix
	tbMatrix	m_mInvMatrix;			// Inverse Transformationsmatrix
	float		m_fMass;				// Masse des Objekts
	float		m_fRadius;				// Radius des Objekts
	float		m_fMovementFriction;	// Reibung bei der Bewegung
	float		m_fRotationFriction;	// Reibung bei der Rotation

	// Konstruktor
	tbObject();

	// Methoden
	void Reset();																																			// Setzt vektorielle Größen zurück
	void Update();																																			// Aktualisiert die Matrizen
	void RotateAbs(const tbVector3& vRotation);																												// Rotiert das Objekt absolut
	void RotateRel(const tbVector3& vRotation);																												// Rotiert das Objekt relativ
	void Align(const tbVector3& vZAxis, const tbVector3& vUp = tbVector3(0.0f, 1.0f, 0.0f));																// Richtet das Objekt entlang einer Achse aus
	void LookAt(const tbVector3& vLookAt, const tbVector3& vUp = tbVector3(0.0f, 1.0f, 0.0f));																// Richtet das Objekt auf einen bestimmten Punkt aus
	void Move(const float fTime);																															// Bewegt das Objekt
	void ApplyForce(const tbVector3& vAttack, const tbVector3& vForce, const float fMovementExaggeration = 1.0f, const float fRotationExaggeration = 1.0f);	// Lässt eine Kraft wirken

	// Inline-Methoden
	inline tbVector3	AbsToRelPos(const tbVector3& v)			{return tbVector3TransformCoords(v, m_mInvMatrix);}
	inline tbVector3	RelToAbsPos(const tbVector3& v)			{return tbVector3TransformCoords(v, m_mMatrix);}
	inline tbVector3	AbsToRelDir(const tbVector3& v)			{return tbVector3TransformNormal(v, m_mInvMatrix);}
	inline tbVector3	RelToAbsDir(const tbVector3& v)			{return tbVector3TransformNormal(v, m_mMatrix);}
	inline void			TranslateAbs(const tbVector3& vAdd)		{m_vPosition += vAdd; Update();}
	inline void			TranslateRel(const tbVector3& vAdd)		{m_vPosition += tbVector3TransformNormal(vAdd, m_mMatrix); Update();}
	inline void			AddVelocityAbs(const tbVector3& vAdd)	{m_vVelocity += vAdd;}
	inline void			AddVelocityRel(const tbVector3& vAdd)	{m_vVelocity += tbVector3TransformNormal(vAdd, m_mMatrix);}
	inline void			AddRotationAbs(const tbVector3& vAdd)	{m_vRotation += tbVector3TransformNormal(vAdd, m_mMatrix);}
	inline void			AddRotationRel(const tbVector3& vAdd)	{m_vRotation += vAdd;}
	inline void			SetPosition(const tbVector3& vPosition)	{m_vPosition = vPosition; Update();}
	inline tbVector3	GetPosition()							{return m_vPosition;}
	inline float		GetDistance(const tbVector3& vPoint)	{return tbVector3Length(vPoint - m_vPosition);}
	inline tbVector3	GetXAxis()								{return m_vXAxis;}
	inline tbVector3	GetYAxis()								{return m_vYAxis;}
	inline tbVector3	GetZAxis()								{return m_vZAxis;}
	inline void			SetVelocity(const tbVector3& vVelocity)	{m_vVelocity = vVelocity;}
	inline tbVector3	GetVelocity()							{return m_vVelocity;}
	inline float		GetSpeed()								{return tbVector3Length(m_vVelocity);}
	inline void			SetRotation(const tbVector3& vRotation)	{m_vRotation = vRotation;}
	inline tbVector3	GetRotation()							{return m_vRotation;}
	inline void			SetScaling(const tbVector3& vScaling)	{m_vScaling = vScaling; Update();}
	inline tbVector3	GetScaling()							{return m_vScaling;}
	inline tbMatrix		GetMatrix()								{Update(); return m_mMatrix;}
	inline tbMatrix		GetInvMatrix()							{Update(); return m_mInvMatrix;}
	inline void			SetMass(const float fMass)				{m_fMass = fMass;}
	inline float		GetMass()								{return m_fMass;}
	inline void			SetMovementFriction(const float f)		{m_fMovementFriction = f;}
	inline float		GetMovementFriction()					{return m_fMovementFriction;}
	inline void			SetRotationFriction(const float f)		{m_fRotationFriction = f;}
	inline float		GetRotationFriction()					{return m_fRotationFriction;}
};

// ******************************************************************