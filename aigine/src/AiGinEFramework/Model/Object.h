// Object.h: interface for the Object class.
//
//////////////////////////////////////////////////////////////////////

#ifndef OBJECT_H
#define OBJECT_H

#include "../AiGinE.h"

class Object {
public:
	// Variablen
	Vector3D	m_vXAxis;				// x-Achse des Objekts (absolut)
	Vector3D	m_vYAxis;				// y-Achse des Objekts (absolut)
	Vector3D	m_vZAxis;				// z-Achse des Objekts (absolut)
	Vector3D	m_vPosition;			// Position des Objekts (absolut)
	Vector3D	m_vScaling;				// Skalierung auf allen drei Achsen (absolut)
	Vector3D	m_vVelocity;			// Bewegungsvektor mit Geschwindigkeit (absolut)
	Vector3D	m_vRotation;			// Rotation um alle drei Achsen des Objekts (relativ)
	Matrix4x4	m_mMatrix;				// Transformationsmatrix
	Matrix4x4	m_mInvMatrix;			// Inverse Transformationsmatrix
	float		m_fMass;				// Masse des Objekts
	float		m_fRadius;				// Radius des Objekts
	float		m_fMovementFriction;	// Reibung bei der Bewegung
	float		m_fRotationFriction;	// Reibung bei der Rotation

	// Konstruktor
	Object();

	// Methoden
	void Reset();																																			// Setzt vektorielle Größen zurück
	void Update();																																			// Aktualisiert die Matrizen
	void RotateAbs(const Vector3D& vRotation);																												// Rotiert das Objekt absolut
	void RotateRel(const Vector3D& vRotation);																												// Rotiert das Objekt relativ
	void Align(const Vector3D& vZAxis, const Vector3D& vUp = Vector3D(0.0f, 1.0f, 0.0f));																// Richtet das Objekt entlang einer Achse aus
	void LookAt(const Vector3D& vLookAt, const Vector3D& vUp = Vector3D(0.0f, 1.0f, 0.0f));																// Richtet das Objekt auf einen bestimmten Punkt aus
	void Move(const float fTime);																															// Bewegt das Objekt
	void ApplyForce(const Vector3D& vAttack, const Vector3D& vForce, const float fMovementExaggeration = 1.0f, const float fRotationExaggeration = 1.0f);	// Lässt eine Kraft wirken

	// Inline-Methoden
	inline Vector3D		AbsToRelPos(const Vector3D& v)			{return Vector3DTransformCoords(v, m_mInvMatrix);}
	inline Vector3D		RelToAbsPos(const Vector3D& v)			{return Vector3DTransformCoords(v, m_mMatrix);}
	inline Vector3D		AbsToRelDir(const Vector3D& v)			{return Vector3DTransformNormal(v, m_mInvMatrix);}
	inline Vector3D		RelToAbsDir(const Vector3D& v)			{return Vector3DTransformNormal(v, m_mMatrix);}
	inline void			TranslateAbs(const Vector3D& vAdd)		{m_vPosition += vAdd; Update();}
	inline void			TranslateRel(const Vector3D& vAdd)		{m_vPosition += Vector3DTransformNormal(vAdd, m_mMatrix); Update();}
	inline void			AddVelocityAbs(const Vector3D& vAdd)	{m_vVelocity += vAdd;}
	inline void			AddVelocityRel(const Vector3D& vAdd)	{m_vVelocity += Vector3DTransformNormal(vAdd, m_mMatrix);}
	inline void			AddRotationAbs(const Vector3D& vAdd)	{m_vRotation += Vector3DTransformNormal(vAdd, m_mMatrix);}
	inline void			AddRotationRel(const Vector3D& vAdd)	{m_vRotation += vAdd;}
	inline void			SetPosition(const Vector3D& vPosition)	{m_vPosition = vPosition; Update();}
	inline Vector3D		GetPosition()							{return m_vPosition;}
	inline float		GetDistance(const Vector3D& vPoint)	{return Vector3D::Length(vPoint - m_vPosition);}
	inline Vector3D		GetXAxis()								{return m_vXAxis;}
	inline Vector3D		GetYAxis()								{return m_vYAxis;}
	inline Vector3D		GetZAxis()								{return m_vZAxis;}
	inline void			SetVelocity(const Vector3D& vVelocity)	{m_vVelocity = vVelocity;}
	inline Vector3D		GetVelocity()							{return m_vVelocity;}
	inline float		GetSpeed()								{return Vector3D::Length(m_vVelocity);}
	inline void			SetRotation(const Vector3D& vRotation)	{m_vRotation = vRotation;}
	inline Vector3D		GetRotation()							{return m_vRotation;}
	inline void			SetScaling(const Vector3D& vScaling)	{m_vScaling = vScaling; Update();}
	inline Vector3D		GetScaling()							{return m_vScaling;}
	inline Matrix4x4	GetMatrix()								{Update(); return m_mMatrix;}
	inline Matrix4x4	GetInvMatrix()							{Update(); return m_mInvMatrix;}
	inline void			SetMass(const float fMass)				{m_fMass = fMass;}
	inline float		GetMass()								{return m_fMass;}
	inline void			SetMovementFriction(const float f)		{m_fMovementFriction = f;}
	inline float		GetMovementFriction()					{return m_fMovementFriction;}
	inline void			SetRotationFriction(const float f)		{m_fRotationFriction = f;}
	inline float		GetRotationFriction()					{return m_fRotationFriction;}
};
#endif //OBJECT_H
