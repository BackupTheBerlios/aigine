/*
**	Space Runner
**	=========
**
**	Camera.cpp
**	--------
**	kamera-Objekt
**	SBlaum
*/

#include "SpaceRunner.h"

// __________________________________________________________________
// Bewegt ein Schiff
tbResult CCamera::MoveCamera(float fTime)
{
	tbVector3	vPosition;
	

	//SetRotation(tbVector3(0.0f, 0.0f, 0.0f));
	//SetVelocity(tbVector3(0.0f, 0.0f, 0.0f));

	Control(fTime);

	if(m_fThrottle < -1.0f) m_fThrottle = -1.0f;
	else if(m_fThrottle > 1.0f) m_fThrottle = 1.0f;
	if(m_vSteering.x < -1.0f) m_vSteering.x = -1.0f;
	else if(m_vSteering.x > 1.0f) m_vSteering.x = 1.0f;
	if(m_vSteering.y < -1.0f) m_vSteering.y = -1.0f;
	else if(m_vSteering.y > 1.0f) m_vSteering.y = 1.0f;
	if(m_vSteering.z < -1.0f) m_vSteering.z = -1.0f;
	else if(m_vSteering.z > 1.0f) m_vSteering.z = 1.0f;

	// Antriebskraft nach vorne wirken lassen
//	AddVelocityAbs(tbVector3(0.0f, 0.0f, m_fThrottle * 400.0f));
	AddVelocityRel(tbVector3(0.0f, 0.0f, m_fThrottle * 400.0f));

	// Schiff drehen
//	AddRotationAbs(m_vSteering * TB_DEG_TO_RAD(45.0f));
	AddRotationRel(m_vSteering * TB_DEG_TO_RAD(45.0f));

	// Bewegungen durchführen (tbObject)
	Move(fTime);
	Update();
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


	// Relative Schubkontrolle
	m_fThrottle = g_pfButtons[TB_KEY_ADD] - g_pfButtons[TB_KEY_SUBTRACT];
	if(g_pbButtons[TB_KEY_BACK]) m_fThrottle = 0.0f;

	// Lenkung
	m_vSteering.x = g_pfButtons[TB_KEY_UP] - g_pfButtons[TB_KEY_DOWN];
	m_vSteering.y = g_pfButtons[TB_KEY_RIGHT] - g_pfButtons[TB_KEY_LEFT];
	m_vSteering.z = g_pfButtons[TB_KEY_Q] - g_pfButtons[TB_KEY_W];


	return TB_OK;
}