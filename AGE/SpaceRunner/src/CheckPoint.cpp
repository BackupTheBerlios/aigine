/*
**	Space Runner
**	=========
**
**	CheckPoint.cpp
**	--------
**	Sblaum
*/

#include "SpaceRunner.h"

// __________________________________________________________________
// Rendert ein Schiff
tbResult CCheckPoint::Render(float fTime)
{
	// Transformationsmatrix abfragen und einsetzen
	Update();
	tbDirect3D::SetTransform(D3DTS_WORLD, m_mMatrix);
	
	// Das Schiffmodell rendern
	m_pType->pModel->Render();
	// ------------------------------------------------------------------

	return TB_OK;
}

// __________________________________________________________________
// Steuert das Schiff
BOOL CCheckPoint::IsVisible()
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