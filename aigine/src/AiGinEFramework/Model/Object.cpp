
#include "../AiGinE.h"

// ******************************************************************
// Konstruktor der Object-Klasse
Object::Object()
{
	// Standardwerte einsetzen
	ZeroMemory(this, sizeof(Object));
	Reset();
	m_fMass = 100.0f;
	m_fMovementFriction = 1.0f;
	m_fRotationFriction = 1.0f;

	// Matrizen berechnen
	Update();
}

// ******************************************************************
// Setzt vektorielle Größen zurück
void Object::Reset()
{
	// Standardwerte
	m_vPosition = Vector3D(0.0f);
	m_vXAxis = Vector3D(1.0f, 0.0f, 0.0f);
	m_vYAxis = Vector3D(0.0f, 1.0f, 0.0f);
	m_vZAxis = Vector3D(0.0f, 0.0f, 1.0f);
	m_vVelocity = Vector3D(0.0f);
	m_vRotation = Vector3D(0.0f);
	m_vScaling = Vector3D(1.0f);

	// Matrizen berechnen
	Update();
}

// ******************************************************************
// Berechnung der Matrizen
void Object::Update()
{
	// Matrix = Skalierung * Rotation * Translation
	m_mMatrix = Matrix4x4::Scaling(m_vScaling) *
		Matrix4x4::Axes(m_vXAxis, m_vYAxis, m_vZAxis) *
		Matrix4x4::Translation(m_vPosition);

	// Inverse Matrix berechnen, die zum Beispiel gebraucht wird, um
	// eine absolute in eine zum Objekt relative Position oder Richtung
	// umzurechnen
	m_mInvMatrix = Matrix4x4::Invert(m_mMatrix);
}

// ******************************************************************
// Rotieren des Objekts (absolut)
void Object::RotateAbs(const Vector3D& vRotation)
{
	// Rotation um die x-Achse
	Matrix4x4 mRotation(Matrix4x4::RotationX(vRotation.x));
	m_vYAxis = Vector3DTransformNormal(m_vYAxis, mRotation);
	m_vZAxis = Vector3D::Cross(m_vXAxis, m_vYAxis);

	// Rotation um die y-Achse
	mRotation = Matrix4x4::RotationY(vRotation.y);
	m_vXAxis = Vector3DTransformNormal(m_vXAxis, mRotation);
	m_vZAxis = Vector3D::Cross(m_vXAxis, m_vYAxis);

	// Rotation um die z-Achse
	mRotation = Matrix4x4::RotationZ(vRotation.z);
	m_vXAxis = Vector3DTransformNormal(m_vXAxis, mRotation);
	m_vYAxis = Vector3DTransformNormal(m_vYAxis, mRotation);

	// Matrizen aktualisieren
	Update();
}

// ******************************************************************
// Rotieren des Objekts (relativ)
void Object::RotateRel(const Vector3D& vRotation)
{
	// Rotation um die x-Achse des Objekts
	Matrix4x4 mRotation(Matrix4x4::RotationAxis(m_vXAxis, vRotation.x));
	m_vYAxis = Vector3DTransformNormal(m_vYAxis, mRotation);
	m_vZAxis = Vector3D::Cross(m_vXAxis, m_vYAxis);

	// Rotation um die y-Achse des Objekts
	mRotation = Matrix4x4::RotationAxis(m_vYAxis, vRotation.y);
	m_vXAxis = Vector3DTransformNormal(m_vXAxis, mRotation);
	m_vZAxis = Vector3D::Cross(m_vXAxis, m_vYAxis);

	// Rotation um die z-Achse des Objekts
	mRotation = Matrix4x4::RotationAxis(m_vZAxis, vRotation.z);
	m_vXAxis = Vector3DTransformNormal(m_vXAxis, mRotation);
	m_vYAxis = Vector3DTransformNormal(m_vYAxis, mRotation);

	// Matrizen aktualisieren
	Update();
}

// ******************************************************************
// Richtet das Objekt entlang einer Achse aus
void Object::Align(const Vector3D& vZAxis,
					 const Vector3D& vUp) // = Vector3D(0.0f, 1.0f, 0.0f)
{
	// z-Achse normalisieren und kopieren
	m_vZAxis = Vector3D::NormalizeEx(vZAxis);

	// Die x-Achse ist das Kreuzprodukt aus y- und z-Achse
	m_vXAxis = Vector3D::NormalizeEx(Vector3D::Cross(vUp, m_vZAxis));

	// y-Achse berechnen
	m_vYAxis = Vector3D::NormalizeEx(Vector3D::Cross(m_vZAxis, m_vXAxis));

	// Matrizen aktualisieren
	Update();
}

// ******************************************************************
// Richtet das Objekt auf einen bestimmten Punkt aus
void Object::LookAt(const Vector3D& vLookAt,
					  const Vector3D& vUp) // = Vector3D(0.0f, 1.0f, 0.0f)
{
	// Die z-Achse durch Differenz zwischen Blickpunkt und Position berechnen
	// und die Align-Methode aufrufen
	Align(vLookAt - m_vPosition, vUp);
}

// ******************************************************************
// Bewegen des Objekts
void Object::Move(const float fTime)
{
	// Objekt verschieben: s = v * t
	m_vPosition += m_vVelocity * fTime;

	// Objekt rotieren
	RotateRel(m_vRotation * fTime);

	// Bewegungs- und Rotationsreibung anwenden
	if(m_fMovementFriction != 1.0f) m_vVelocity *= powf(m_fMovementFriction, fTime);
	if(m_fRotationFriction != 1.0f) m_vRotation *= powf(m_fRotationFriction, fTime);

	// Matrizen aktualisieren
	Update();
}

// ******************************************************************
// Kraft einwirken lassen
void Object::ApplyForce(const Vector3D& vAttack,
						  const Vector3D& vForce,
						  const float fMovementExaggeration,	// = 1.0f
						  const float fRotationExaggeration)	// = 1.0f
{
	// Diese Funktion stellt den Kern der gesamten Physikeinheit dar.
	// Sie berechnet die Wirkungen der Kraft mit der Richtung und
	// Stärke vForce, die am Punkt vAttack angreift.
	// Beides sind absolute Angaben.

	// Rotations- und Bewegungsfaktor werden berechnet.
	// Sie bestimmen, welcher Anteil der Kraft in eine Rotation und
	// welcher in eine Bewegung umgewandelt wird.
	// Ihre Summe ergibt immer 1, daher braucht man nur einen der
	// beiden Faktoren zu berechnen.
	// Der Rotationsfaktor ist das Verhältnis zwischen dem Quadrat der
	// kürzesten Distanz zwischen dem Schwerpunkt und der Linie
	// mit der Richtung der Kraft, die durch den Angriffspunkt geht,
	// und dem Quadrat des Radius.
	float r = PointLineDistance(m_vPosition,
								  vAttack - (10000.0f * vForce),
								  vAttack + (10000.0f * vForce));
	float fRotationFactor = (r * r) / (m_fRadius * m_fRadius);
	if(fRotationFactor > 1.0f) fRotationFactor = 1.0f;

	// Der Bewegungsfaktor kann nun ganz leicht berechnet werden.
	float fMovementFactor = 1.0f - fRotationFactor;

	// Kehrwert der Masse bestimmen (für schnellere Division)
	float fInvMass = 1.0f / m_fMass;

	if(fMovementFactor >= 0.0001f)
	{
		// Dem Objekt Bewegung hinzufügen. Man erhält sie, indem man
		// die Richtung der Kraft mit dem Bewegungsfaktor multipliziert
		// und das Produkt durch die Masse des Objekts teilt.
		// Alles wird noch mit einem bestimmten Faktor multipliziert,
		// um den Effekt ein wenig zu übertreiben.
		AddVelocityAbs(fMovementFactor * fMovementExaggeration * vForce * fInvMass);
	}

	if(fRotationFactor >= 0.0001f)
	{
		// Rotation hinzufügen. Erst rechnen wir die relative Position des Angriffspunkts
		// aus und teilen sie durch den Radius. Die Richtung der Kraft wird ebenfalls
		// in eine relative Richtung umgerechnet.
		Vector3D vRelAttack(AbsToRelPos(vAttack) / m_fRadius);
		Vector3D vRelForce(AbsToRelDir(vForce));

		// Rotation um die x-Achse
		AddRotationRel(Vector3D(vRelAttack.z * -vRelForce.y * fInvMass, 0.0f, 0.0f));
		AddRotationRel(Vector3D(vRelAttack.y * vRelForce.z * fInvMass, 0.0f, 0.0f));

		// Rotation um die y-Achse
		AddRotationRel(Vector3D(0.0f, vRelAttack.x * vRelForce.z * fInvMass, 0.0f));
		AddRotationRel(Vector3D(0.0f, vRelAttack.z * vRelForce.x * fInvMass, 0.0f));

		// Rotation um die z-Achse
		AddRotationRel(Vector3D(0.0f, 0.0f, vRelAttack.x * -vRelForce.y * fInvMass));
		AddRotationRel(Vector3D(0.0f, 0.0f, vRelAttack.y * vRelForce.x * fInvMass));
	}
}

// ******************************************************************