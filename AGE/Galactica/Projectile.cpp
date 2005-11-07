/*
**	GALACTICA
**	=========
**
**	Projectile.cpp
**	--------------
**	Alles, was etwas mit den Projektilen (Raketen, Laserstrahlen) zu tun hat
*/

#include "Galactica.h"

// __________________________________________________________________
// Bewegt ein Projektil
tbResult CProjectile::MoveProjectile(float fTime)
{
	static CGame*		pGame;
	static tbVector3	vTarget;
	static tbVector3	vOldPos;
	static tbVector3	vCollisionEnd;
	static tbVector3	vCollision;
	static CShip*		pShip;
	static SShipType*	pShipType;
	static float		fTolerance;
	static int			iSound;
	static CShip*		pHitShip;

	pGame = g_pGalactica->m_pGame;

	// Rakete? Wenn ja, dann steuern!
	if(!m_pType->bIsLaserWeapon)
	{
		// Die Rakete nach vorne beschleunigen
		AddVelocityRel(tbVector3(0.0f, 0.0f, 1.0f) *
		               m_pType->fMissileAccel *
		               fTime);

		// Rakete langsam um ihre eigene z-Achse drehen
		m_vRotation.z = TB_DEG_TO_RAD(22.5f);

		// Wenn die Rakete ein Ziel hat, steuert sie darauf zu.
		// Außerdem fängt sie erst nach einer halben Sekunde an zu lenken.
		if(m_pTarget != NULL &&
		   m_fLifetime <= m_pType->fProjectileLifetime - 0.5f)
		{
			if(m_pType->iMissileAIProc == 1)
			{
				// Erstes KI-Programm für Raketen.
				// Die Koordinaten des Ziels in Koordinaten relativ zur Rakete umrechnen
				vTarget = AbsToRelPos(m_pTarget->m_vPosition);

				// Ist das Ziel links von der Rakete, lenkt sie nach links.
				if(vTarget.x < -0.5f) AddRotationRel(tbVector3(0.0f, -m_pType->fMissileAngularAccel * fTime, 0.0f));
				else if(vTarget.x > 0.5f) AddRotationRel(tbVector3(0.0f, m_pType->fMissileAngularAccel * fTime, 0.0f));

				// Gleiches für hoch/runter
				vTarget = AbsToRelPos(m_pTarget->m_vPosition);
				if(vTarget.y < -0.5f) AddRotationRel(tbVector3(m_pType->fMissileAngularAccel * fTime, 0.0f, 0.0f));
				else if(vTarget.y > 0.5f) AddRotationRel(tbVector3(-m_pType->fMissileAngularAccel * fTime, 0.0f, 0.0f));
			}
			else if(m_pType->iMissileAIProc == 2)
			{
				// Zweites KI-Programm für Raketen.
				// Es wird versucht, die Position des Ziels zum Zeitpunkt des Treffers
				// vorherzusagen, so dass die Rakete genauer treffen kann.
				vTarget = AbsToRelPos(tbComputeAimingVector(m_vPosition,
															m_pTarget->m_vPosition,
															m_pTarget->m_vVelocity,
															tbVector3Length(m_vVelocity)));

				// Ist das Ziel links von der Rakete, lenkt sie nach links.
				if(vTarget.x < 0.0f) AddRotationRel(tbVector3(0.0f, -m_pType->fMissileAngularAccel * fTime, 0.0f));
				if(vTarget.x > 0.0f) AddRotationRel(tbVector3(0.0f, m_pType->fMissileAngularAccel * fTime, 0.0f));

				// Gleiches für hoch/runter.
				if(vTarget.y < 0.0f) AddRotationRel(tbVector3(m_pType->fMissileAngularAccel * fTime, 0.0f, 0.0f));
				if(vTarget.y > 0.0f) AddRotationRel(tbVector3(-m_pType->fMissileAngularAccel * fTime, 0.0f, 0.0f));
			}
		}
	}

	// Bewegungen durchführen (tbObject), alte Position speichern
	vOldPos = m_vPosition;
	Move(fTime);

	// Kollision mit Schiffen überprüfen
	pHitShip = NULL;
	for(int s = 0; s < 32; s++)
	{
		pShip = &pGame->m_aShip[s];
		pShipType = pShip->m_pType;

		// Existiert das Schiff und ist es nicht das Schiff, das die Rakete
		// abgefeuert hat?
		if(!pShip->m_bExists || s == m_iFiredBy) continue;

		// Prüfen, ob die Linie zwischen alter und neuer Position das
		// Modell des Schiffs schneidet. Die Toleranz hängt davon ab,
		// ob die Waffe eine Laserwaffe ist oder nicht und ob das Schiff
		// noch genug Schutzschildenergie hat.
		if(pShip->m_fShieldEnergy >= m_pType->fDamageToShields) fTolerance = 2.0f;
		else fTolerance = 0.0f;

		vCollisionEnd = m_vPosition;
		if(m_pType->bIsLaserWeapon) vCollisionEnd += m_vZAxis * m_pType->fLaserLength;

		// Trifft die Linie das Schiff?
		if(tbLineHitsModel(vOldPos, vCollisionEnd,
				           pShip->m_pType->pCollisionModel,
						   pShip->m_mMatrix,
						   pShip->m_mInvMatrix,
						   fTolerance,
						   &vCollision))
		{
			// Ja, es gab eine Kollision!
			// Das Projektil muss nun verschwinden bzw. explodieren.
			m_fLifetime = 0.0f;
			pHitShip = pShip;

			// Hat das Schiff noch genug Schildenergie?
			if(pShip->m_fShieldEnergy >= m_pType->fDamageToShields)
			{
				// Die Schildenergie verringern
				pShip->m_fShieldEnergy -= m_pType->fDamageToShields;

				// Schildleuchtpartikel erstellen
				pGame->m_pPSystem->AddParticle(pShipType->fShieldGlowLifetime,
						                       pShipType->fShieldGlowStartSprite,
											   pShipType->fShieldGlowEndSprite,
											   vCollision,
											   pShip->m_vVelocity,
											   1.0f,
											   pShipType->fShieldGlowStartSize,
											   pShipType->fShieldGlowEndSize,
											   pShipType->ShieldGlowStartColor,
											   pShipType->ShieldGlowEndColor,
											   tbFloatRandom(0.0f, 2.0f),
											   tbFloatRandom(0.0f, 2.0f));

				// Schutzschildsound abspielen
				iSound = pShipType->pShieldSound->PlayAvailableBuffer();
				if(iSound != -1)
				{
					// Zufällige Frequenz und die 3D-Position setzen
					pShipType->pShieldSound->SetFrequency((DWORD)(iSound), tbIntRandom(36100, 44100));
					pShipType->pShieldSound->SetPosition((DWORD)(iSound), vCollision);
				}

				// Das getroffene Schiff beginnt mit Ausweichmanövern.
				if(pShip != pGame->m_pPlayer) pShip->m_fEvasiveManeuvers = tbFloatRandom(0.1f, 0.2f);

				// Wenn das Schiff ein vom Computer gesteuertes Schiff ist, dann
				// wird per Zufall entschieden, ob es das Ziel auf das Schiff ändert,
				// von dem der Schuss kommt.
				if(pShip != pGame->m_pPlayer &&
				   tbIntRandom(1, 20) >= 5)
				{
					// Neues Ziel setzen!
					pShip->m_iTarget = m_iFiredBy;
				}

				// Cockpit leicht rütteln lassen
				pShip->m_vCockpitShaking += tbVector3Random() * tbFloatRandom(0.5f, 1.0f);
			}
			else
			{
				if(pShip->IsVisible())
				{
					// Der Schuss trifft die Hülle!
					if(tbVector3LengthSq(g_pGalactica->m_pGame->m_vCameraPos - pShip->m_vPosition) <= 1000000.0f)
					{
						// Eine kleine Explosion wird erzeugt.
						pGame->m_pPSystem->AddParticle(tbFloatRandom(0.5f, 0.75f), 8.0f, 23.0f,
													   vCollision, pShip->m_vVelocity,
													   1.0f, 2.0f, 5.0f + m_pType->fDamageToHull * 0.1f, tbColor(1.0f), tbColor(0.5f),
													   tbFloatRandom(0.0f, 2.0f), tbFloatRandom(0.0f, 2.0f));

						// Trümmerpartikel erzeugen
						for(int i = 0;
						    i < (m_pType->bIsLaserWeapon ? tbIntRandom(5, 10) :
						                                   tbIntRandom(20, 40));
							i++)
						{
							pGame->m_pPSystem->AddParticle(tbFloatRandom(0.5f, 1.0f),
														   tbFloatRandom(32.0f, 47.0f),
														   tbFloatRandom(32.0f, 47.0f),
														   vCollision,
														   pShip->m_vVelocity + tbVector3Random() * tbFloatRandom(0.0f, 50.0f),
														   1.0f,
														   tbFloatRandom(1.0f, 2.5f),
														   tbFloatRandom(1.0f, 2.5f),
														   pShipType->DebrisColor + tbColorRandom(-1.0f) * 0.1f,
														   tbColor(0.0f),
														   tbFloatRandom(0.0f, 2.0f), tbFloatRandom(0.0f, 2.0f));
						}
					}
				}

				// Das getroffene Schiff beginnt mit Ausweichmanövern.
				if(pShip != pGame->m_pPlayer) pShip->m_fEvasiveManeuvers = tbFloatRandom(0.5f, 1.0f);

				// Wenn das Schiff ein vom Computer gesteuertes Schiff ist, dann
				// wird per Zufall entschieden, ob es das Ziel auf das Schiff ändert,
				// das das Projektil abgefeuert hat.
				if(pShip != pGame->m_pPlayer &&
				   tbIntRandom(1, 10) >= 5)
				{
					// Neues Ziel setzen!
					pShip->m_iTarget = m_iFiredBy;
				}

				// Hüllentreffersound abspielen
				iSound = pGame->m_pHullHitSound->PlayAvailableBuffer();
				if(iSound != -1)
				{
					// Zufällige Frequenz und die 3D-Position setzen
					pGame->m_pHullHitSound->SetFrequency((DWORD)(iSound), tbIntRandom(36100, 44100));
					pGame->m_pHullHitSound->SetPosition((DWORD)(iSound), vCollision);
				}

				// Dem Schiff schaden hinzufügen
				pShip->DoDamage(vCollision,
					            m_pType->fDamageToHull * tbFloatRandom(0.9f, 1.1f));

				// Es wirkt nun Kraft auf das Schiff.
				// Wir übertreiben die Bewegungswirkung dieser Kraft mit dem Faktor 30,
				// da man es sonst überhaupt nicht merken würde (auf Grund der Reibung,
				// die die Schiffe gleich wieder zum Stillstand bringen würde).
				pShip->ApplyForce(vCollision,
						          tbVector3NormalizeEx(m_vVelocity) * m_pType->fHitForce,
								  30.0f, 10.0f);

				// Cockpit rütteln lassen
				pShip->m_vCockpitShaking += tbVector3Random() * m_pType->fHitForce * 0.01f * tbFloatRandom(0.8f, 1.25f);
			}

			// Nach weiteren Schiffen, die im Weg des Projektils sein könnten,
			// muss nicht mehr gesucht werden.
			break;
		}
	}

	// Lebenszeit verringern
	m_fLifetime -= fTime;

	// Wenn die Lebenszeit kleiner gleich null ist und wir es mit einer
	// Rakete zu tun haben, dann gibt es eine schöne Explosion.
	if(m_fLifetime <= 0.0f &&
	   !m_pType->bIsLaserWeapon)
	{
		// Fünf zufällige Explosionen erzeugen
		for(int i = 0; i < 5; i++)
		{
			pGame->m_pPSystem->AddParticle(m_pType->fMissileExplosionLifetime + tbFloatRandom(-0.25f, 0.25f),
										   m_pType->fMissileExplosionStartSprite,
										   m_pType->fMissileExplosionEndSprite,
										   m_vPosition + tbVector3Random() * tbFloatRandom(0.0f, 20.0f),
										   pHitShip == NULL ? m_vVelocity + tbVector3Random() * tbFloatRandom(0.0f, 5.0f)
										                    : pHitShip->m_vVelocity + tbVector3Random() * tbFloatRandom(0.0f, 5.0f),
										   1.0f,
										   m_pType->fMissileExplosionStartSize + tbFloatRandom(-5.0f, 5.0f),
										   m_pType->fMissileExplosionEndSize + tbFloatRandom(-5.0f, 5.0f),
										   m_pType->MissileExplosionStartColor + tbColorRandom(-1.0f) * tbFloatRandom(0.0f, 0.5f),
										   m_pType->MissileExplosionEndColor + tbColorRandom(-1.0f) * tbFloatRandom(0.0f, 0.5f),
										   tbFloatRandom(0.0f, 2.0f),
										   tbFloatRandom(0.0f, 2.0f));
		}

		// Explosionssound abspielen
		iSound = m_pType->pExplosionSound->PlayAvailableBuffer();
		if(iSound != -1)
		{
			// Zufällige Frequenz und die 3D-Position setzen
			m_pType->pExplosionSound->SetFrequency((DWORD)(iSound), tbIntRandom(36100, 44100));
			m_pType->pExplosionSound->SetPosition((DWORD)(iSound), m_vPosition);
		}
	}

	return TB_OK;
}

// __________________________________________________________________
// Rendert ein Projektil
tbResult CProjectile::Render(float fTime)
{
	static tbVector3 vGlowPosition;

	if(m_pType->bIsLaserWeapon)
	{
		// Es ist ein Laserstrahl!
		// Wir fügen nun das Laser-Sprite zur Sprite-Engine hinzu - wenn
		// das Projektil nicht als gewöhnliches Sprite gerendert werden soll.
		if(m_pType->bRenderAsSprite)
		{
			// Normales Sprite
			g_pGalactica->m_pGame->m_pSprites->AddSprite(m_pType->iLaserSprite,
				                                         m_vPosition,
														 m_pType->LaserStartColor,
														 tbVector2(m_pType->fLaserRadius),
														 m_fLifetime * -2.0f);

			vGlowPosition = m_vPosition;
		}
		else
		{
			// Laser-Sprite
			g_pGalactica->m_pGame->m_pSprites->AddLaserSprite(m_pType->iLaserSprite,
				                                              m_vPosition,
															  m_vPosition + (m_vZAxis * m_pType->fLaserLength),
															  m_pType->fLaserRadius,
															  m_pType->LaserStartColor,
															  m_pType->LaserEndColor);

			vGlowPosition = 0.5f * (m_vPosition +
				                    m_vPosition + m_vZAxis * m_pType->fLaserLength);
		}
	}
	else
	{
		// Es ist kein Laserstrahl, hat also ein Modell!
		// Transformationsmatrix abfragen und einsetzen.
		Update();
		tbDirect3D::SetTransform(D3DTS_WORLD, m_mMatrix);

		// Das Modell rendern
		m_pType->pProjectileModel->Render();

		// Flamme aus der Rakete schießen lassen
		if(tbFloatRandom(0.0f, 1.0f) > 0.1f &&
		   !g_pGalactica->m_pGame->m_bPaused)
		{
			g_pGalactica->m_pGame->m_pPSystem->AddParticle(m_pType->fMissileSmokeTrailLifetime + tbFloatRandom(-0.5f, 0.5f),
														   (float)(m_pType->iMissileSmokeTrailStartSprite), (float)(m_pType->iMissileSmokeTrailStartSprite),
														   RelToAbsPos(m_pType->vMissileSmokeTrailPos),
											 			   RelToAbsDir((tbVector3(0.0f, 0.0f, -1.0f) * m_pType->fMissileSmokeTrailSpeed) + (tbVector3Random() * tbFloatRandom(0.0f, 0.1f))),
											 			   1.0f,
											 			   m_pType->fMissileSmokeTrailStartSize + tbFloatRandom(-0.25f, 0.25f),
											 			   m_pType->fMissileSmokeTrailEndSize + tbFloatRandom(-0.25f, 0.25f),
											 			   m_pType->MissileSmokeTrailStartColor + (tbColorRandom(-1.0f) * 0.1f),
											 			   m_pType->MissileSmokeTrailEndColor + (tbColorRandom(-1.0f) * 0.1f),
											 			   tbFloatRandom(0.0f, 2.0f), tbFloatRandom(0.0f, 2.0f));
		}

		vGlowPosition = m_vPosition;
	}

	// Leucht-Sprite zeichnen
	g_pGalactica->m_pGame->m_pSprites->AddSprite(m_pType->iGlowSprite,
				                                 vGlowPosition,
												 m_pType->GlowColor,
												 tbVector2(m_pType->fGlowSize));

	return TB_OK;
}

// __________________________________________________________________