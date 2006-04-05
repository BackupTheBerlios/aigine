/*
**	Space Runner
**	=========
**
**	Ship.cpp
**	--------
**	Alles, was etwas mit den Schiffen zu tun hat
*/

#include "SpaceRunner.h"
//#define FF_DIVIDE 200

// __________________________________________________________________
// Bewegt ein Schiff
tbResult CShip::MoveShip(float fTime)
{
	int			iSound;
	tbVector3	vPosition;


	// Effizienz der Systeme berechnen: 1 - (Schaden / Maximaler Schaden)
	m_fHullEfficiency = 1.0f - (m_fHullDamage / m_pType->fMaxHullDamage);
	m_fEngineEfficiency = 1.0f - (m_fEngineDamage / m_pType->fMaxEngineDamage);
	m_fShieldEfficiency = 1.0f - (m_fShieldDamage / m_pType->fMaxShieldDamage);
	m_fSensorsEfficiency = 1.0f - (m_fSensorsDamage / m_pType->fMaxSensorsDamage);

	/* NETWORK
	if(tbServer::status == SERVER_GESTARTET) {
        Control(fTime);
	}
*/

	//Control(fTime);

	// Schub und Lenkung begrenzen
	if(m_fThrottle < -0.5f) m_fThrottle = -0.5f;
	else if(m_fThrottle > 1.0f) m_fThrottle = 1.0f;
	if(m_vSteering.x < -1.0f) m_vSteering.x = -1.0f;
	else if(m_vSteering.x > 1.0f) m_vSteering.x = 1.0f;
	if(m_vSteering.y < -1.0f) m_vSteering.y = -1.0f;
	else if(m_vSteering.y > 1.0f) m_vSteering.y = 1.0f;
	if(m_vSteering.z < -1.0f) m_vSteering.z = -1.0f;
	else if(m_vSteering.z > 1.0f) m_vSteering.z = 1.0f;

	// Antriebskraft nach vorne wirken lassen
	AddVelocityRel(tbVector3(0.0f, 0.0f, m_fThrottle * m_pType->fMaxAccel * m_fEngineEfficiency * fTime));

	// Schiff drehen
	AddRotationRel(m_vSteering * TB_DEG_TO_RAD(m_pType->fMaxAngularAccel) * m_fEngineEfficiency * fTime);
	// Bewegungen durchführen (tbObject)
	Move(fTime);

	// Rütteln des Cockpits abklingen lassen
	m_vCockpitShaking *= powf(0.25f, fTime);

	// Das Rütteln des Cockpits ist stärker, wenn der Schub stärker ist.
	if(m_fThrottle > 0.75f) m_vCockpitShaking += tbVector3Random() * (m_fThrottle - 0.75f) * 0.5f;

	// ------------------------------------------------------------------

	// Reparaturen durchführen
	if(m_fHullDamage < m_pType->fMaxHullDamage) m_fHullDamage -= m_pType->fHullRepairRate * fTime;
	if(m_fEngineDamage < m_pType->fMaxEngineDamage) m_fEngineDamage -= m_pType->fEngineRepairRate * fTime;
	if(m_fShieldDamage < m_pType->fMaxShieldDamage) m_fShieldDamage -= m_pType->fShieldRepairRate * fTime;
	if(m_fSensorsDamage < m_pType->fMaxSensorsDamage) m_fSensorsDamage -= m_pType->fSensorsRepairRate * fTime;

	// Schaden begrenzen - nach unten und nach oben hin
	if(m_fHullDamage < 0.0f) m_fHullDamage = 0.0f;
	if(m_fEngineDamage < 0.0f) m_fEngineDamage = 0.0f;
	if(m_fShieldDamage < 0.0f) m_fShieldDamage = 0.0f;
	if(m_fSensorsDamage < 0.0f) m_fSensorsDamage = 0.0f;
	if(m_fHullDamage > m_pType->fMaxHullDamage) m_fHullDamage = m_pType->fMaxHullDamage;
	if(m_fEngineDamage > m_pType->fMaxEngineDamage) m_fEngineDamage = m_pType->fMaxEngineDamage;
	if(m_fShieldDamage > m_pType->fMaxShieldDamage) m_fShieldDamage = m_pType->fMaxShieldDamage;
	if(m_fSensorsDamage > m_pType->fMaxSensorsDamage) m_fSensorsDamage = m_pType->fMaxSensorsDamage;

	// ------------------------------------------------------------------

	// Waffen aufladen und reparieren
	for(int iWeapon = 0; iWeapon < m_pType->iNumWeapons; iWeapon++)
	{
		// Waffeneffizienz berechnen
		m_afWeaponEfficiency[iWeapon] = 1.0f - (m_afWeaponDamage[iWeapon] / m_pType->apWeaponType[iWeapon]->fMaxDamage);
		if(m_afWeaponEfficiency[iWeapon] > 0.0f)
		{
			// Waffe aufladen
			m_afWeaponReload[iWeapon] -= fTime * m_afWeaponEfficiency[iWeapon];
			if(m_afWeaponReload[iWeapon] < 0.0f) m_afWeaponReload[iWeapon] = 0.0f;

			// Waffe reparieren
			m_afWeaponDamage[iWeapon] -= m_pType->apWeaponType[iWeapon]->fRepairRate * fTime;
		}

		// Den Waffenschaden begrenzen
		if(m_afWeaponDamage[iWeapon] < 0.0f) m_afWeaponDamage[iWeapon] = 0.0f;
		if(m_afWeaponDamage[iWeapon] > m_pType->apWeaponType[iWeapon]->fMaxDamage) m_afWeaponDamage[iWeapon] = m_pType->apWeaponType[iWeapon]->fMaxDamage;
	}

	// Waffenenergie aufladen
	m_fWeaponEnergy += m_pType->fWeaponEnergyReloadRate * fTime;
	if(m_fWeaponEnergy > m_pType->fMaxWeaponEnergy) m_fWeaponEnergy = m_pType->fMaxWeaponEnergy;

	// Schilde laden
	m_fShieldEnergy += m_fShieldEfficiency * m_pType->fMaxShieldReloadRate * fTime;
	if(m_fShieldEnergy < 0.0f) m_fShieldEnergy = 0.0f;
	if(m_fShieldEnergy >= m_pType->fMaxShieldEnergy) m_fShieldEnergy = m_pType->fMaxShieldEnergy;

	// ------------------------------------------------------------------

	if(m_fExplosionCountDown > 0.0f)
	{
		// Explosions-Count-Down aktualisieren
		m_fExplosionCountDown -= fTime;

		// Nach abgelaufenem Count-Down verschwindet das Schiff mit einer
		// großen Explosion.
		if(m_fExplosionCountDown <= 0.0f)
		{
			// Viele zufällige große Explosionen erzeugen
			for(int i = 0; i < tbIntRandom(20, 30); i++)
			{
				// Zufällige Position für die Explosion berechnen
				vPosition = m_vPosition + tbVector3Random() * tbFloatRandom(0.0f, m_fRadius * 0.75f);

				if(IsVisible() || this == m_pGame->m_pPlayer)
				{
					// Explosions-Sprite erstellen
					m_pGame->m_pPSystem->AddParticle(tbFloatRandom(1.5f, 3.0f), 8.0f, 23.0f,
													 vPosition,
													 m_vVelocity + tbVector3Random() * tbFloatRandom(0.0f, 5.0f),
													 1.0f,
													 tbFloatRandom(40.0f, 140.0f),
													 tbFloatRandom(160.0f, 260.0f),
													 tbColor(1.0f, 1.0f, 1.0f, tbFloatRandom(0.5f, 1.0f)),
													 tbColor(1.0f, 1.0f, 1.0f, 0.0f),
													 tbFloatRandom(0.0f, 2.0f),
													 tbFloatRandom(0.0f, 2.0f));

					// Trümmerpartikel erzeugen
					for(int j = 0; j < tbIntRandom(20, 40); j++)
					{
						m_pGame->m_pPSystem->AddParticle(tbFloatRandom(3.0f, 6.0f),
														 tbFloatRandom(32.0f, 47.0f),
														 tbFloatRandom(32.0f, 47.0f),
														 vPosition + tbVector3Random() * tbFloatRandom(0.0f, 5.0f),
														 m_vVelocity + tbVector3Random() * tbFloatRandom(0.0f, 50.0f),
														 1.0f,
														 tbFloatRandom(2.0f, 4.0f),
														 tbFloatRandom(2.0f, 4.0f),
														 m_pType->DebrisColor + tbColorRandom(-1.0f) * 0.1f,
														 tbColor(1.0f, 0.0f, 0.0f, 0.0f) + tbColorRandom(-1.0f) * 0.2f,
														 tbFloatRandom(0.0f, 2.0f), tbFloatRandom(0.0f, 2.0f));
					}
				}

				// 4 Explosionssounds abspielen
				if(i < 4)
				{
//					iSound = m_pGame->m_pExplosionSound->PlayAvailableBuffer();
					if(iSound != -1)
					{
						// Zufällige Frequenz und die 3D-Position setzen
//						m_pGame->m_pExplosionSound->SetFrequency((DWORD)(iSound), tbIntRandom(14100, 44100));
//						m_pGame->m_pExplosionSound->SetPosition((DWORD)(iSound), vPosition);
					}
				}
			}

			// Das Schiff hört auf zu existieren.
			// Wir stoppen auch seinen Antriebssound.
			m_bExists = FALSE;
			m_pType->pEngineSound->Stop((DWORD)(m_iEngineSound));
		}
		// In den letzten zwei Sekunden passiert nichts (die Stille vor dem Sturm).
		else if(m_fExplosionCountDown > 2.0f)
		{
			if(tbFloatRandom(0.0f, 1.0f + fTime) > 0.99f)
			{
				// Einige zufällige kleine Explosionen erzeugen
				for(int i = 0; i < tbIntRandom(4, 6); i++)
				{
					if(IsVisible() || this == m_pGame->m_pPlayer)
					{
						vPosition = m_vPosition + tbVector3Random() * tbFloatRandom(0.0f, m_fRadius);
						m_pGame->m_pPSystem->AddParticle(tbFloatRandom(0.5f, 1.5f), 8.0f, 23.0f,
														 vPosition,
														 m_vVelocity + tbVector3Random() * tbFloatRandom(0.0f, 5.0f),
														 1.0f,
														 tbFloatRandom(20.0f, 50.0f),
														 tbFloatRandom(60.0f, 120.0f),
														 tbColor(1.0f, 1.0f, 1.0f, tbFloatRandom(0.5f, 1.0f)),
														 tbColor(1.0f, 1.0f, 1.0f, 0.0f),
														 tbFloatRandom(0.0f, 2.0f),
														 tbFloatRandom(0.0f, 2.0f));

						// Trümmerpartikel erzeugen
						for(int j = 0; j < tbIntRandom(10, 20); j++)
						{
							m_pGame->m_pPSystem->AddParticle(tbFloatRandom(2.0f, 4.0f),
															 tbFloatRandom(32.0f, 47.0f),
															 tbFloatRandom(32.0f, 47.0f),
															 vPosition + tbVector3Random() * tbFloatRandom(0.0f, 5.0f),
															 m_vVelocity + tbVector3Random() * tbFloatRandom(0.0f, 50.0f),
															 1.0f,
															 tbFloatRandom(2.0f, 4.0f),
															 tbFloatRandom(2.0f, 4.0f),
															 m_pType->DebrisColor + tbColorRandom(-1.0f) * 0.1f,
															 tbColor(1.0f, 0.9f, 0.9f, 0.0f),
															 tbFloatRandom(0.0f, 2.0f), tbFloatRandom(0.0f, 2.0f));
						}
					}

					// Kraft auf das Schiff wirken lassen
					ApplyForce(vPosition, tbVector3Random() * tbFloatRandom(1.0f, 400.0f), 30.0f, 10.0f);

					// Cockpit rütteln lassen
					m_vCockpitShaking += tbVector3Random() * tbFloatRandom(2.5f, 5.0f);

					// 2 Explosionssounds abspielen
					if(i < 2)
					{
//						iSound = m_pGame->m_pExplosionSound->PlayAvailableBuffer();
						if(iSound != -1)
						{
							// Zufällige Frequenz und die 3D-Position setzen
//							m_pGame->m_pExplosionSound->SetFrequency((DWORD)(iSound), tbIntRandom(24100, 44100));
//							m_pGame->m_pExplosionSound->SetPosition((DWORD)(iSound), vPosition);
						}
					}
				}
			}
		}
	}

	// Wenn die Hülle zerstört ist, beginnt die Explosion des Schiffs.
	// Deren Länge ist von der Größe des Schiffs abhängig.
	if(m_fHullDamage >= m_pType->fMaxHullDamage &&
	   m_fExplosionCountDown == 0.0f)
	{
		// Count-Down beginnen!
		m_fExplosionCountDown = 4.0f + (m_fRadius / 10.0f);
	}

	return TB_OK;
}

// __________________________________________________________________
// Rendert ein Schiff
tbResult CShip::Render(float fTime)
{
	tbMatrix	mWeapon;
	float		fRandom;


	// Transformationsmatrix abfragen und einsetzen
	Update();
	tbDirect3D::SetTransform(D3DTS_WORLD, m_mMatrix);

	// Das Schiffmodell rendern
	m_pType->pModel->Render();

	// ------------------------------------------------------------------

	// Alle Waffensysteme durchgehen und rendern
	for(int iWeapon = 0; iWeapon < m_pType->iNumWeapons; iWeapon++)
	{
		if(m_pType->apWeaponType[iWeapon]->pLauncherModel != NULL)
		{
			// Transformationsmatrix erstellen und einsetzen
			mWeapon = tbMatrixTranslation(m_pType->avWeaponPos[iWeapon]) * m_mMatrix;
			tbDirect3D::SetTransform(D3DTS_WORLD, mWeapon);

			// Waffenmodell rendern
			m_pType->apWeaponType[iWeapon]->pLauncherModel->Render();
		}
	}

	// ------------------------------------------------------------------

	if(IsVisible() &&
	   !m_pGame->m_bPaused)
	{
		if(m_fThrottle > 0.0f)
		{
			// Alle Flammen durchgehen
			for(int iFlame = 0; iFlame < m_pType->iNumFlames; iFlame++)
			{
				// Flamme aus dem Antrieb schießen lassen - je nach Schub und Antriebsschaden
				fRandom = tbFloatRandom(m_fThrottle * m_fEngineEfficiency, 2.0f);
				if(fRandom > 1.0f)
				{
					m_pGame->m_pPSystem->AddParticle(m_pType->fFlameLifetime + tbFloatRandom(-0.5f, 0.5f),
													 (float)(m_pType->iFlameStartSprite), (float)(m_pType->iFlameEndSprite),
													 RelToAbsPos(m_pType->avFlamePos[iFlame]),
													 RelToAbsDir((tbVector3(0.0f, 0.0f, -1.0f) * m_fThrottle * m_fEngineEfficiency * m_pType->fFlameSpeed) + (tbVector3Random() * tbFloatRandom(0.0f, 0.1f))),
													 1.0f,
													 m_pType->fFlameStartSize + tbFloatRandom(-0.25f, 0.25f),
													 m_pType->fFlameEndSize + tbFloatRandom(-0.25f, 0.25f),
													 m_pType->FlameStartColor + (tbColorRandom(-1.0f) * 0.1f),
													 m_pType->FlameEndColor + (tbColorRandom(-1.0f) * 0.1f),
													 tbFloatRandom(0.0f, 2.0f), tbFloatRandom(0.0f, 2.0f));
				}
			}
		}
	}

	return TB_OK;
}

// __________________________________________________________________
// Aktualisiert den Sound des Schiffs
tbResult CShip::UpdateSound(float fTime)
{
	// Sicherstellen, dass der Antriebssound (noch) läuft
	if(m_iEngineSound == -1)
	{
		// Sound abspielen (mit Looping)
		m_iEngineSound = m_pType->pEngineSound->PlayAvailableBuffer(0, DSBPLAY_LOOPING);
	}
	else if(!(m_pType->pEngineSound->GetStatus((DWORD)(m_iEngineSound)) & DSBSTATUS_PLAYING))
	{
		// Sound erneut abspielen
		m_iEngineSound = m_pType->pEngineSound->PlayAvailableBuffer(0, DSBPLAY_LOOPING);
	}

	if(m_iEngineSound == -1) return TB_OK;

	// Aktuelle Position des Sounds setzen
	m_pType->pEngineSound->SetPosition((DWORD)(m_iEngineSound),
		                               RelToAbsPos(m_pType->vEnginePos),
									   1.0f);

	// Die Frequenz hängt von der Stärke und der Effizienz des Antriebs ab.
	m_pType->pEngineSound->SetFrequency((DWORD)(m_iEngineSound),
		                                22050 + (DWORD)(m_fThrottle * m_fEngineEfficiency * 22050.0f));

	return TB_OK;
}

// __________________________________________________________________
// Feuert eine Waffe des Schiffs ab
/*
tbResult CShip::Fire(int iWeapon)
{
	CProjectile*	pProjectile;
	float			fAngle;
	tbVector3		vAimAt;
	int				iSound;


	// Ist die Waffe voll aufgeladen?
	// Ist noch genug Munition da (nur bei Raketen)?
	// Ist noch genug Waffenenergie da?
	if(m_afWeaponReload[iWeapon] > 0.0f) return TB_OK;
	if(!m_pType->apWeaponType[iWeapon]->bIsLaserWeapon && m_aiWeaponAmmo[iWeapon] <= 0) return TB_OK;
	if(m_fWeaponEnergy < m_pType->apWeaponType[iWeapon]->fEnergyPerShot) return TB_OK;

	// Nach einem freien Projektil suchen
	for(int p = 0; p < 256; p++)
	{
		// Frei?
		pProjectile = &m_pGame->m_aProjectile[p];
		if(pProjectile->m_fLifetime > 0.0f) continue;

		// Ausfüllen!
		pProjectile->m_fLifetime = m_pType->apWeaponType[iWeapon]->fProjectileLifetime;
		pProjectile->m_pType = m_pType->apWeaponType[iWeapon];
		pProjectile->m_iFiredBy = m_iIndex;
		if(m_iTarget != -1) pProjectile->m_pTarget = &g_pSpaceRunner->m_pGame->m_aShip[m_iTarget];
		else pProjectile->m_pTarget = NULL;

		// Die Objektklasse ausfüllen.
		// Position, Skalierung und Achsen vom Schiff kopieren.
		pProjectile->Reset();
		pProjectile->m_vPosition = m_vPosition;
		pProjectile->m_vScaling = m_vScaling;
		pProjectile->m_vXAxis = m_vXAxis;
		pProjectile->m_vYAxis = m_vYAxis;
		pProjectile->m_vZAxis = m_vZAxis;
		pProjectile->Update();

		// Masse und und Rotation setzen, Radius auf null setzen
		pProjectile->m_fMass = m_pType->apWeaponType[iWeapon]->fMissileMass;
		pProjectile->m_vRotation = tbVector3(0.0f);
		pProjectile->m_fRadius = 0.0f;

		if(!m_pType->apWeaponType[iWeapon]->bIsLaserWeapon)
		{
			// Reibung gibt es nur bei Raketen.
			pProjectile->m_fMovementFriction = m_pType->apWeaponType[iWeapon]->fMissileMovementFriction;
			pProjectile->m_fRotationFriction = m_pType->apWeaponType[iWeapon]->fMissileRotationFriction;
		}
		else
		{
			// Laser fliegen ungebremst.
			pProjectile->m_fMovementFriction = 1.0f;
			pProjectile->m_fRotationFriction = 1.0f;
		}

		// Das Geschoss an seine Position bringen: Waffenposition + Abschussposition
		pProjectile->TranslateRel(m_pType->avWeaponPos[iWeapon]);
		pProjectile->TranslateRel(m_pType->apWeaponType[iWeapon]->vProjectileStartPos);

		if(m_pType->apWeaponType[iWeapon]->bIsLaserWeapon)
		{
			// Für Laserstrahlen:
			// Wenn Auto-Aim eingeschaltet ist und die Waffe es unterstützt, wird
			// der Laserstrahl auf das Ziel ausgerichtet.
			if(m_bAutoAim && m_pType->apWeaponType[iWeapon]->bAutoAim)
			{
				if(m_iTarget != -1)
				{
					// Zielvektor berechnen
					vAimAt = tbComputeAimingVector(m_vPosition,
								                   m_pGame->m_aShip[m_iTarget].m_vPosition,
												   m_pGame->m_aShip[m_iTarget].m_vVelocity,
												   m_pType->apWeaponType[iWeapon]->fLaserSpeed);

					// Je nach Sensorenschaden wird diese Position leicht geändert.
					vAimAt += tbVector3Random() * tbFloatRandom(0.0f, 100.0f * (1.0f - m_fSensorsEfficiency));

					// Den Winkel zwischen der z-Achse des Schiffs und dem Zielpunkt berechnen
					fAngle = tbVector3Angle(tbVector3Normalize(vAimAt - m_vPosition), m_vZAxis);

					// Wenn der Winkel zu groß ist, funktioniert Auto-Aim nicht.
					if(fabsf(fAngle) <= TB_DEG_TO_RAD(m_pType->apWeaponType[iWeapon]->fMaxAutoAimAngle))
					{
						// Den Laserstrahl auf den neuen Zielpunkt ausrichten
						pProjectile->LookAt(vAimAt);
					}
				}
			}

			// Den Startgeschwindigkeitsvektor setzen
			pProjectile->m_vVelocity = pProjectile->m_vZAxis *
									   m_pType->apWeaponType[iWeapon]->fLaserSpeed;
		}
		else
		{
			// Für Raketen:
			// Den Startgeschwindigkeitsvektor setzen
			pProjectile->m_vVelocity = m_vVelocity +
				                       (m_pType->apWeaponType[iWeapon]->vMissileStartVector *
									    m_pType->apWeaponType[iWeapon]->fMissileStartSpeed);

			// Die Rakete wird entlang ihres Abschussvektors ausgerichtet -
			// relativ zum Schiff.
			pProjectile->Align(RelToAbsDir(pProjectile->m_pType->vMissileStartVector));
		}

		// Je nach Schaden an der Waffe wird der Bewegungsvektor leicht verändert.
		pProjectile->m_vVelocity += tbVector3Random() * tbFloatRandom(0.0f, 1.0f - m_afWeaponEfficiency[iWeapon]);

		// Abschusssound abspielen
		iSound = pProjectile->m_pType->pLauncherSound->PlayAvailableBuffer(0, 0, m_vVelocity);
		if(iSound != -1)
		{
			// Frequenz zufällig setzen
			pProjectile->m_pType->pLauncherSound->SetFrequency((DWORD)(iSound),
						                                       tbIntRandom(36100, 44100));

			// 3D-Position setzen.
			// Der Sound wird ein Stück nach vorne versetzt, damit er nicht
			// gedämpft wird, wenn das Cockpit vor den Kanonen ist.
			pProjectile->m_pType->pLauncherSound->SetPosition((DWORD)(iSound),
						                                      pProjectile->m_vPosition + m_vZAxis * 30.0f);
		}

		// Waffenaufladung zurücksetzen und Munition verringern
		m_afWeaponReload[iWeapon] = m_pType->apWeaponType[iWeapon]->fMinReloadTime;
		m_aiWeaponAmmo[iWeapon]--;

		// Waffenenergie abziehen
		m_fWeaponEnergy -= m_pType->apWeaponType[iWeapon]->fEnergyPerShot;

		// Fertig!
		return TB_OK;
	}

	return TB_OK;
}
*/
// __________________________________________________________________
// Steuert das Schiff
tbResult CShip::Control(float fTime)
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
	CShip*		pTarget;
	CShip*		pShip;

	g_pSpaceRunner->message_control.msgid = MSG_CONTROL;


	// Wenn das Ziel des Schiffs ungültig ist, zerstört wurde oder es gerade explodiert,
	// wird ein neues gewählt.
	if(m_iTarget == -1 || m_iTarget == m_iIndex) m_iTarget = FindNextTarget(-1, m_iTeam);
	else if(this != m_pGame->m_pPlayer && m_pGame->m_aShip[m_iTarget].m_iTeam == m_iTeam) m_iTarget = FindNextTarget(-1, m_iTeam);
	else if(!m_pGame->m_aShip[m_iTarget].m_bExists || m_pGame->m_aShip[m_iTarget].m_fExplosionCountDown != 0.0f) m_iTarget = FindNextTarget(m_iTarget, this == m_pGame->m_pPlayer ? -1 : m_iTeam);

	// Spieler oder Computer?
	if(m_pGame->m_pPlayer == this && m_pGame->m_CameraMode != CM_FREE)
	{
		if(m_pGame->m_bUseJoystick)
		{
			// Joystick in Tastatur umwandeln
			g_pfButtons[TB_KEY_LEFT] = g_pfButtons[TB_JOY_X_NEG(0)];
			g_pfButtons[TB_KEY_RIGHT] = g_pfButtons[TB_JOY_X_POS(0)];
			g_pfButtons[TB_KEY_UP] = g_pfButtons[TB_JOY_Y_NEG(0)];
			g_pfButtons[TB_KEY_DOWN] = g_pfButtons[TB_JOY_Y_POS(0)];
			g_pfButtons[TB_KEY_Q] = g_pfButtons[TB_JOY_RZ_NEG(0)];
			g_pfButtons[TB_KEY_W] = g_pfButtons[TB_JOY_RZ_POS(0)];
			g_pbButtons[TB_KEY_LSHIFT] = g_pbButtons[TB_JOY_BUTTON(0, 0)];
			g_pbButtons[TB_KEY_LCONTROL] = g_pbButtons[TB_JOY_BUTTON(0, 1)];
			g_pbButtons[TB_KEY_T] = g_pbButtons[TB_JOY_BUTTON(0, 2)];

			// Absolute Schubkontrolle
			if(fabsf(g_pfOldButtons[TB_JOY_SLIDER_ABS(0, 0)] - g_pfButtons[TB_JOY_SLIDER_ABS(0, 0)]) > 0.005f)
			{
				// Der Schieberegler wurde verändert.
				// Wir setzen den Schub nun absolut.
				m_fThrottle = (g_pfButtons[TB_JOY_SLIDER_ABS(0, 0)] - 0.5f) * 2.0f;
			}
		}

		// Relative Schubkontrolle
		m_fThrottle += g_pfButtons[TB_KEY_ADD] * 0.5f * fTime;
		m_fThrottle -= g_pfButtons[TB_KEY_SUBTRACT] * 0.5f * fTime;
		if(g_pbButtons[TB_KEY_BACK]) m_fThrottle = 0.0f;

		g_pSpaceRunner->message_control.throttle = m_fThrottle;

		// Lenkung
		m_vSteering.x = g_pfButtons[TB_KEY_UP] - g_pfButtons[TB_KEY_DOWN];
		m_vSteering.y = g_pfButtons[TB_KEY_RIGHT] - g_pfButtons[TB_KEY_LEFT];
		m_vSteering.z = g_pfButtons[TB_KEY_Q] - g_pfButtons[TB_KEY_W];

		g_pSpaceRunner->message_control.steering = m_vSteering;

		// Waffen
		if(g_pbButtons[TB_KEY_LSHIFT])
		{
			// Alle Laserwaffen abfeuern
			for(int w = 0; w < m_pType->iNumWeapons; w++)
			{
				if(m_pType->apWeaponType[w]->bIsLaserWeapon)
				{
					// Feuer!
//					Fire(w);
				}
			}
		}

		if(g_pbButtons[TB_KEY_LCONTROL])
		{
			// Raketenwaffen abfeuern
			for(int w = 0; w < m_pType->iNumWeapons; w++)
			{
				if(!m_pType->apWeaponType[w]->bIsLaserWeapon)
				{
					// Feuer!
//					Fire(w);
				}
			}
		}

		// Zielerfassung
		if(WasButtonPressed(TB_KEY_T))
		{
			// Nächstes Schiff in der Liste suchen (beliebiges Team)
			m_iTarget = FindNextTarget(m_iTarget, -1);
		}

		// Auto-Aim ein/aus
		if(WasButtonPressed(TB_KEY_A)) m_bAutoAim = !m_bAutoAim;

		// Selbstzerstörung
		if(g_pbButtons[TB_KEY_S] && g_pbButtons[TB_KEY_D]) m_fHullDamage = m_pType->fMaxHullDamage;

		// Radarreichweite
		if(g_pbButtons[TB_KEY_PRIOR]) m_pGame->m_fRadarRange += fTime * 2000.0f;
		if(g_pbButtons[TB_KEY_NEXT]) m_pGame->m_fRadarRange -= fTime * 2000.0f;
		if(m_pGame->m_fRadarRange > 32000.0f) m_pGame->m_fRadarRange = 32000.0f;
		if(m_pGame->m_fRadarRange < 100.0f) m_pGame->m_fRadarRange = 100.0f;

		g_pSpaceRunner->send_control();
	}
	else
	{
		// Zeiger auf das aktuelle Zielschiff bestimmen.
		// Ist kein Ziel ausgewählt, so bewegt sich das Schiff nicht.
		if(m_iTarget == -1)
		{
			m_vSteering = tbVector3(0.0f, 0.0f, 0.0f);
			m_fThrottle = 0.0f;

			return TB_OK;
		}

		pTarget = &m_pGame->m_aShip[m_iTarget];

		// Mittelwert der Waffenpositionen berechnen, außerdem den
		// Mittelwert der Geschossgeschwindigkeiten
		vWeapons = tbVector3(0.0f);
		fWeaponSpeed = 0.0f;
		iNumWeapons = 0;
		for(int i = 0; i < m_pType->iNumWeapons; i++)
		{
			if(m_pType->apWeaponType[i]->bIsLaserWeapon &&
			   !m_pType->apWeaponType[i]->bAutoAim)
			{
				vWeapons += m_pType->avWeaponPos[i];
				fWeaponSpeed += m_pType->apWeaponType[i]->fLaserSpeed;
				iNumWeapons++;
			}
		}

		// Keine Waffen gefunden?
		if(iNumWeapons == 0)
		{
			for(int i = 0; i < m_pType->iNumWeapons; i++)
			{
				// Wir beziehen jetzt alle Auto-Aiming-Waffen ein
				if(m_pType->apWeaponType[i]->bAutoAim)
				{
					vWeapons += m_pType->avWeaponPos[i];
					fWeaponSpeed += m_pType->apWeaponType[i]->fLaserSpeed;
					iNumWeapons++;
				}
			}
		}

		if(iNumWeapons == 0)
		{
			// Immer noch keine Waffen gefunden!
			// Standardwerte einsetzen.
			vWeapons = tbVector3(0.0f);
			fWeaponSpeed = 100.0f;
			iNumWeapons = 1;
		}

		// Mittelwerte berechnen
		vWeapons /= (float)(iNumWeapons);
		fWeaponSpeed /= (float)(iNumWeapons);

		// Punkt berechnen, auf den gezielt werden muss, um das gegnerische
		// Schiff treffen zu können
		vAimAt = tbComputeAimingVector(RelToAbsPos(vWeapons),
			                           pTarget->m_vPosition,
									   pTarget->m_vVelocity,
									   fWeaponSpeed);

		// Die Position der Waffen mit einrechnen
		vAimAt -= RelToAbsDir(vWeapons);

		// Schaden an den Sensoren führen zu einem ungenauen Zielpunkt.
		vAimAt += tbVector3Random() * tbFloatRandom(0.0f, 100.0f * (1.0f - m_fSensorsEfficiency));

		// Relative Position des Zielpunkts bestimmen
		vAimAtRel = AbsToRelPos(vAimAt);

		// Lenkung...
		m_vSteering = tbVector3(0.0f);
		if(vAimAtRel.x < 0.0f) m_vSteering.y = -1.0f;
		if(vAimAtRel.x > 0.0f) m_vSteering.y = 1.0f;
		if(vAimAtRel.y < 0.0f) m_vSteering.x = 1.0f;
		if(vAimAtRel.y > 0.0f) m_vSteering.x = -1.0f;
		m_vSteering.x += tbFloatRandom(-0.5f, 0.5f);
		m_vSteering.y += tbFloatRandom(-0.5f, 0.5f);
		m_vSteering.z += tbFloatRandom(-0.5f, 0.5f);

		// Kosinus des Winkels zum Zielpunkt bestimmen
		// (Winkel zwischen Verbindungslinie zum Gegner und z-Achse des Schiffs)
		fDot = tbVector3Dot(tbVector3Normalize(vAimAt - m_vPosition), m_vZAxis);
		fAngle = acosf(fDot);

		// Der Schub hängt vom Winkel zwischen diesem Schiff und dem Ziel ab.
		// Ist das Ziel direkt voraus, gibt es vollen Schub.
		// Ab einer gewissen Entfernung ebenso.
		m_fThrottle = fDot;
		fDistance = tbVector3Length(vAimAt - m_vPosition);
		if(fDistance > 800.0f) m_fThrottle = 1.0f;
		
		// Verhindern, dass die Raumschiffe sich nicht mehr von der Stelle bewegen
		// und nur noch umeinander herumfliegen
		if(fabsf(fAngle) > TB_DEG_TO_RAD(90.0f) &&
		   fDistance < 1200.0f)
		{
			m_vSteering = tbVector3(0.0f);
			m_fThrottle = 1.0f;
		}

		// Kollisionen vermeiden
		for(int iShip = 0; iShip < 32; iShip++)
		{
			pShip = &m_pGame->m_aShip[iShip];
			if(!pShip->m_bExists) continue;
			if(pShip == this) continue;

			// Die aktuelle Distanz zum Schiff und die in zwei Sekunden berechnen
			fDistance = tbVector3Length(pShip->m_vPosition - m_vPosition);
			fNextDistance = tbVector3Length((pShip->m_vPosition + pShip->m_vVelocity * 2.0f) -
				                            (m_vPosition + m_vVelocity * 2.0f));

			// Wenn die Distanz zu klein ist und die Distanz in einer kurzen
			// Zeit kleiner ist als die jetzige, dann besteht die Gefahr einer
			// Kollision.
			if(fNextDistance < (m_pType->pModel->GetBoundingSphereRadius() +
							   pShip->m_pType->pModel->GetBoundingSphereRadius()) * 2.0f
			   && fNextDistance < fDistance)
			{
				// Ausweichmanöver!
				m_fEvasiveManeuvers = tbFloatRandom(2.0f, 3.0f);
			}
		}

		// Wenn das Schiff Ausweichmanöver durchführt, lenkt es wild.
		if(m_fEvasiveManeuvers > 0.0f)
		{
			m_vSteering.x += 2.0f * sinf(m_pGame->m_fTime * 0.25f + tbFloatRandom(-0.1f, 0.1f));
			m_vSteering.y += 2.0f * cosf(m_pGame->m_fTime * 0.25f + tbFloatRandom(-0.1f, 0.1f));
			m_vSteering.z += tbFloatRandom(-1.0f, 1.0f);
			m_fThrottle = sinf(m_pGame->m_fTime);

			// Count-Down...
			m_fEvasiveManeuvers -= fTime;
		}

		// Ab 2000 Einheiten fängt das Schiff mit dem Feuern an.
		// Aber wenn das Ziel gerade schon am explodieren ist, ist das natürlich
		// nicht mehr nötig.
		fDistance = tbVector3Length(vAimAt - m_vPosition);

		if(fDistance <= 2500.0f &&
		   pTarget->m_fExplosionCountDown == 0.0f)
		{
			// Jede Waffe durchgehen...
			for(int w = 0; w < m_pType->iNumWeapons; w++)
			{
				if(m_pType->apWeaponType[w]->bIsLaserWeapon)
				{
					// Unterstützt die Waffe Auto-Aiming und ist der Winkel klein genug?
					// Bei normalen Waffen muss der Winkel schon ein wenig kleiner sein.
					// Wenn er klein genug ist: feuern!
					if((m_pType->apWeaponType[w]->bAutoAim && m_bAutoAim && fabsf(fAngle) <= TB_DEG_TO_RAD(m_pType->apWeaponType[w]->fMaxAutoAimAngle)) ||
					   (!m_pType->apWeaponType[w]->bAutoAim && fabsf(fAngle) <= TB_DEG_TO_RAD(10.0f)))
					{
//						Fire(w);
					}
				}
				else
				{
					// Raketen werden abgefeuert, wenn das Ziel eine Mindestentfernung
					// von 300 Einheiten hat oder der Winkel sehr klein ist.
//					if(fDistance > 300.0f || fabsf(fAngle) <= TB_DEG_TO_RAD(10.0f)) Fire(w);
				}
			}
		}
	}

	return TB_OK;
}

// __________________________________________________________________
// Fügt dem Schiff Schaden zu
void CShip::DoDamage(tbVector3 vHit,
					 float fPower)
{
	float fDistance;
#ifdef FF
	HRESULT hr;
	DWORD   dwCurrentTime;
#endif

	// Die Trefferposition in das Koordinatensystem des Schiffs umrechnen
	vHit = AbsToRelPos(vHit);
#ifdef FF
	// Force Feedback effekt wird gestartet
	if(m_pEffect && fPower > 0 ) {
		
		// Make sure the device is acquired, if we are gaining focus.
        this->m_pJoyDevice->Acquire();
		if (FAILED(hr = m_pEffect->Start(1, 0)))
		{
			// Schade !
			TB_WARNING("Fehler beim Starten des ForceFeedback Effectes");
		}
		
		dwCurrentTime = timeGetTime();
        
		if( dwCurrentTime - g_dwLastEffectSet > 100 )
		{
			// Don't allow setting effect more often than
			// 100ms since every time an effect is set, the
			// device will jerk.
			//
			// Note: This is not neccessary, and is specific to this sample


			g_dwLastEffectSet = dwCurrentTime;
			int y = vHit.x;
			int x = vHit.y;
			//x -= FEEDBACK_WINDOW_X;
			//y -= FEEDBACK_WINDOW_Y;

			g_nXForce = CoordToForce( x );
			g_nYForce = CoordToForce( y );

			//hr = SetDeviceForcesXY();
			hr = this->SetDeviceForcesXY(vHit, fPower);
			if( hr == TB_NOT_FOUND ){
				TB_WARNING( "Fehler beim initialisieren, des ForceFeedback Effectes ( NOT_FOUND ) ");
			} else if (hr == TB_OK ) {
				g_dwLastEffectSet = dwCurrentTime;
			}else {
				TB_WARNING( "Fehler beim initialisieren, des ForceFeedback Effectes ( ELSE )");
			}
		}		
	}
#endif
	// Dem Schiff Schaden hinzufügen.
	// Die Hülle bekommt den vollen Anteil ab.
	m_fHullDamage += tbFloatRandom(0.9f, 1.1f) * fPower;

	// Bei den anderen Systemen kommt es auf die Distanz zum Trefferpunkt an.
	// Erst nehmen wir uns den Antrieb vor.
	fDistance = tbVector3Length(vHit - m_pType->vEnginePos) / m_fRadius * 4.0f;
	if(fDistance < 1.0f) fDistance = 1.0f;
	m_fEngineDamage += tbFloatRandom(0.9f, 1.1f) * (fPower / fDistance);

	// Schildgenerator
	fDistance = tbVector3Length(vHit - m_pType->vShieldPos) / m_fRadius * 4.0f;
	if(fDistance < 1.0f) fDistance = 1.0f;
	m_fShieldDamage += tbFloatRandom(0.9f, 1.1f) * (fPower / fDistance);

	// Sensoren
	fDistance = tbVector3Length(vHit - m_pType->vSensorsPos) / m_fRadius * 4.0f;
	if(fDistance < 1.0f) fDistance = 1.0f;
	m_fSensorsDamage += tbFloatRandom(0.9f, 1.1f) * (fPower / fDistance);

	// Nun noch alle Waffensysteme
	for(int w = 0; w < m_pType->iNumWeapons; w++)
	{
		fDistance = tbVector3Length(vHit - m_pType->avWeaponPos[w]) / m_fRadius * 4.0f;
		if(fDistance < 1.0f) fDistance = 1.0f;
		m_afWeaponDamage[w] += tbFloatRandom(0.9f, 1.1f) * (fPower / fDistance);
	}
}

#ifdef FF
//-----------------------------------------------------------------------------
// Name: SetDeviceForcesXY()
// setzt die ForceFeedback Kraft für x und y achsen
// by mrnice
//-----------------------------------------------------------------------------
HRESULT CShip::SetDeviceForcesXY(tbVector3 vHit, float fPower)
{
    // Modifying an effect is basically the same as creating a new one, except
    // you need only specify the parameters you are modifying
    LONG rglDirection[2] = { 0, 0 };
	DWORD NumFFAxis = 2;
    DICONSTANTFORCE cf;
	DIPERIODIC diPeriodic;      // type-specific parameters
	
	//if( tbDirectInput::GetNumForceFeedbackAxis() == 1 )
 //   {
 //       // If only one force feedback axis, then apply only one direction and 
 //       // keep the direction at zero
 //       cf.lMagnitude = LONG(-vHit.x);
 //       rglDirection[0] = 0;
 //   }
 //   else
    
        // If two force feedback axis, then apply magnitude from both directions 
	rglDirection[0] = LONG(vHit.x);//LONG(this->g_nXForce);
	rglDirection[1] = LONG(vHit.y);
	diPeriodic.dwMagnitude = fPower * 100;
	/*cf.lMagnitude = (DWORD)sqrt( (double)g_nXForce * (double)g_nXForce +
                                 (double)g_nYForce * (double)g_nYForce );
    */

    DIEFFECT eff;
    ZeroMemory( &eff, sizeof(eff) );
    eff.dwSize                = sizeof(DIEFFECT);
    eff.dwFlags               = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
	eff.cAxes                 = NumFFAxis;//tbDirectInput::GetNumForceFeedbackAxis();
    eff.rglDirection          = rglDirection;

    //eff.lpEnvelope            = 0;
    eff.cbTypeSpecificParams  = sizeof(diPeriodic);
    eff.lpvTypeSpecificParams = &diPeriodic;
    eff.dwStartDelay            = 0;

    // Now set the new parameters and start the effect immediately.
	return m_pEffect->SetParameters( &eff, DIEP_DIRECTION |
                                           DIEP_TYPESPECIFICPARAMS |
                                           DIEP_START );
	
}

//-----------------------------------------------------------------------------
// Name: CoordToForce()
// Desc: Convert a coordinate 0 <= nCoord <= FEEDBACK_WINDOW_WIDTH 
//       to a force value in the range -DI_FFNOMINALMAX to +DI_FFNOMINALMAX.
//-----------------------------------------------------------------------------
INT CShip::CoordToForce( INT nCoord )
{
    INT nForce = MulDiv( nCoord, 2 * DI_FFNOMINALMAX, FF_DIVIDE )
                 - DI_FFNOMINALMAX;

    // Keep force within bounds
    if( nForce < -DI_FFNOMINALMAX ) 
        nForce = -DI_FFNOMINALMAX;

    if( nForce > +DI_FFNOMINALMAX ) 
        nForce = +DI_FFNOMINALMAX;

    return nForce;
}

#endif
// __________________________________________________________________
// Findet nächstes Ziel
int CShip::FindNextTarget(int iStart,
						  int iTeam)
{
	// Nächstes Schiff in der Liste suchen
	for(int s = iStart + 1; s < 32; s++)
	{
		// Das Schiff muss exisitieren, es darf nicht das eigene Schiff sein
		// und es darf dem angegebenen Team NICHT angehören.
		if(m_pGame->m_aShip[s].m_bExists &&
		   &m_pGame->m_aShip[s] != this &&
		   m_pGame->m_aShip[s].m_iTeam != iTeam)
		{
			// Dieses Schiff ist das neue Ziel!
			return s;
		}
	}

	// Kein Schiff gefunden? Nochmal von vorne suchen.
	for(s = 0; s < 32; s++)
	{
		if(m_pGame->m_aShip[s].m_bExists &&
		   &m_pGame->m_aShip[s] != this &&
		   m_pGame->m_aShip[s].m_iTeam != iTeam)
		{
			// Dieses Schiff ist das neue Ziel!
			return s;
		}
	}

	// Immer noch keins? Dann gibt es keins.
	return -1;
}

// __________________________________________________________________
// Ist das Schiff sichtbar?
BOOL CShip::IsVisible()
{
	// Wenn der umgebende Quader des Modells innerhalb des
	// View-Frustums liegt, dann gilt das Schiff als sichtbar.
	Update();
	return tbBoxVisible_Inv(m_pType->pModel->GetBoundingBoxMin(),
							m_pType->pModel->GetBoundingBoxMax(),
							m_mInvMatrix,
							m_pGame->m_aViewFrustum);
}

// __________________________________________________________________