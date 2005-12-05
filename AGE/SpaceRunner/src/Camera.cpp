/*
**	Space Runner
**	=========
**
**	Caméra.cpp
**	--------
**	kamera-Objekt
*/

#include "SpaceRunner.h"

// __________________________________________________________________
// Bewegt ein Schiff
tbResult CCamera::Move(float fTime)
{
	tbVector3	vPosition;

	Control(fTime);

	// Antriebskraft nach vorne wirken lassen
	//AddVelocityRel(tbVector3(0.0f, 0.0f, 0.0f));

	// Schiff drehen
	//AddRotationRel(0.0f);

	// Bewegungen durchführen (tbObject)
	Move(fTime);

	return TB_OK;
}

// Steuert das Schiff
tbResult CCamera::Control(float fTime)
{
	tbVector3	vRotation;
	tbVector3	vAimAt;
	tbVector3	vAimAtRel;
	tbVector3	vWeapons;
	float		fWeaponSpeed;
	float		fDistance;
	float		fNextDistance;
	float		fDot;
	float		fAngle;
	int			iNumWeapons;



	// Lenkung
	m_vSteering.x = g_pfButtons[TB_KEY_UP] - g_pfButtons[TB_KEY_DOWN];
	m_vSteering.y = g_pfButtons[TB_KEY_RIGHT] - g_pfButtons[TB_KEY_LEFT];
	m_vSteering.z = g_pfButtons[TB_KEY_Q] - g_pfButtons[TB_KEY_W];


	// Lenkung...
	m_vSteering = tbVector3(0.0f);
	if(vAimAtRel.x < 0.0f) m_vSteering.y = -1.0f;
	if(vAimAtRel.x > 0.0f) m_vSteering.y = 1.0f;
	if(vAimAtRel.y < 0.0f) m_vSteering.x = 1.0f;
	if(vAimAtRel.y > 0.0f) m_vSteering.x = -1.0f;
	m_vSteering.x += tbFloatRandom(-0.5f, 0.5f);
	m_vSteering.y += tbFloatRandom(-0.5f, 0.5f);
	m_vSteering.z += tbFloatRandom(-0.5f, 0.5f);


	return TB_OK;
}