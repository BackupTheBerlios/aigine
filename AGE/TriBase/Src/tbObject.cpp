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

#include <TriBase.h>

// ******************************************************************
// Konstruktor der tbObject-Klasse
tbObject::tbObject()
{
	// Standardwerte einsetzen
	ZeroMemory(this, sizeof(tbObject));
	Reset();
	m_fMass = 100.0f;
	m_fMovementFriction = 1.0f;
	m_fRotationFriction = 1.0f;

	// Matrizen berechnen
	Update();
}

// ******************************************************************
// Setzt vektorielle Größen zurück
void tbObject::Reset()
{
	// Standardwerte
	m_vPosition = tbVector3(0.0f);
	m_vXAxis = tbVector3(1.0f, 0.0f, 0.0f);
	m_vYAxis = tbVector3(0.0f, 1.0f, 0.0f);
	m_vZAxis = tbVector3(0.0f, 0.0f, 1.0f);
	m_vVelocity = tbVector3(0.0f);
	m_vRotation = tbVector3(0.0f);
	m_vScaling = tbVector3(1.0f);

	// Matrizen berechnen
	Update();
}

// ******************************************************************
// Berechnung der Matrizen
void tbObject::Update()
{
	// Matrix = Skalierung * Rotation * Translation
	m_mMatrix = tbMatrixScaling(m_vScaling) *
                tbMatrixAxes(m_vXAxis, m_vYAxis, m_vZAxis) *
				tbMatrixTranslation(m_vPosition);

	// Inverse Matrix berechnen, die zum Beispiel gebraucht wird, um
	// eine absolute in eine zum Objekt relative Position oder Richtung
	// umzurechnen
	m_mInvMatrix = tbMatrixInvert(m_mMatrix);
}

// ******************************************************************
// Rotieren des Objekts (absolut)
void tbObject::RotateAbs(const tbVector3& vRotation)
{
	// Rotation um die x-Achse
	tbMatrix mRotation(tbMatrixRotationX(vRotation.x));
	m_vYAxis = tbVector3TransformNormal(m_vYAxis, mRotation);
	m_vZAxis = tbVector3Cross(m_vXAxis, m_vYAxis);

	// Rotation um die y-Achse
	mRotation = tbMatrixRotationY(vRotation.y);
	m_vXAxis = tbVector3TransformNormal(m_vXAxis, mRotation);
	m_vZAxis = tbVector3Cross(m_vXAxis, m_vYAxis);

	// Rotation um die z-Achse
	mRotation = tbMatrixRotationZ(vRotation.z);
	m_vXAxis = tbVector3TransformNormal(m_vXAxis, mRotation);
	m_vYAxis = tbVector3TransformNormal(m_vYAxis, mRotation);

	// Matrizen aktualisieren
	Update();
}

// ******************************************************************
// Rotieren des Objekts (relativ)
void tbObject::RotateRel(const tbVector3& vRotation)
{
	// Rotation um die x-Achse des Objekts
	tbMatrix mRotation(tbMatrixRotationAxis(m_vXAxis, vRotation.x));
	m_vYAxis = tbVector3TransformNormal(m_vYAxis, mRotation);
	m_vZAxis = tbVector3Cross(m_vXAxis, m_vYAxis);

	// Rotation um die y-Achse des Objekts
	mRotation = tbMatrixRotationAxis(m_vYAxis, vRotation.y);
	m_vXAxis = tbVector3TransformNormal(m_vXAxis, mRotation);
	m_vZAxis = tbVector3Cross(m_vXAxis, m_vYAxis);

	// Rotation um die z-Achse des Objekts
	mRotation = tbMatrixRotationAxis(m_vZAxis, vRotation.z);
	m_vXAxis = tbVector3TransformNormal(m_vXAxis, mRotation);
	m_vYAxis = tbVector3TransformNormal(m_vYAxis, mRotation);

	// Matrizen aktualisieren
	Update();
}

// ******************************************************************
// Richtet das Objekt entlang einer Achse aus
void tbObject::Align(const tbVector3& vZAxis,
					 const tbVector3& vUp) // = tbVector3(0.0f, 1.0f, 0.0f)
{
	// z-Achse normalisieren und kopieren
	m_vZAxis = tbVector3NormalizeEx(vZAxis);

	// Die x-Achse ist das Kreuzprodukt aus y- und z-Achse
	m_vXAxis = tbVector3NormalizeEx(tbVector3Cross(vUp, m_vZAxis));

	// y-Achse berechnen
	m_vYAxis = tbVector3NormalizeEx(tbVector3Cross(m_vZAxis, m_vXAxis));

	// Matrizen aktualisieren
	Update();
}

// ******************************************************************
// Richtet das Objekt auf einen bestimmten Punkt aus
void tbObject::LookAt(const tbVector3& vLookAt,
					  const tbVector3& vUp) // = tbVector3(0.0f, 1.0f, 0.0f)
{
	// Die z-Achse durch Differenz zwischen Blickpunkt und Position berechnen
	// und die Align-Methode aufrufen
	Align(vLookAt - m_vPosition, vUp);
}

// ******************************************************************
// Bewegen des Objekts
void tbObject::Move(const float fTime)
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
void tbObject::ApplyForce(const tbVector3& vAttack,
						  const tbVector3& vForce,
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
	float r = tbPointLineDistance(m_vPosition,
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
		tbVector3 vRelAttack(AbsToRelPos(vAttack) / m_fRadius);
		tbVector3 vRelForce(AbsToRelDir(vForce));

		// Rotation um die x-Achse
		AddRotationRel(tbVector3(vRelAttack.z * -vRelForce.y * fInvMass, 0.0f, 0.0f));
		AddRotationRel(tbVector3(vRelAttack.y * vRelForce.z * fInvMass, 0.0f, 0.0f));

		// Rotation um die y-Achse
		AddRotationRel(tbVector3(0.0f, vRelAttack.x * vRelForce.z * fInvMass, 0.0f));
		AddRotationRel(tbVector3(0.0f, vRelAttack.z * vRelForce.x * fInvMass, 0.0f));

		// Rotation um die z-Achse
		AddRotationRel(tbVector3(0.0f, 0.0f, vRelAttack.x * -vRelForce.y * fInvMass));
		AddRotationRel(tbVector3(0.0f, 0.0f, vRelAttack.y * vRelForce.x * fInvMass));
	}
}

// ******************************************************************