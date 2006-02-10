///////////////////////////////////////////////////////////////////////////////////
//						D R I F T E L E M E N T . C P P							 //
//-------------------------------------------------------------------------------//
// @description: Mainclass for the gineric level-element (tunnel: strait/left/right)


#include "SpaceRunner.h"
#include "DriftElement.h"

/**
 * Konsturktor
 */
CDriftElement::CDriftElement() {}

/**
 * Bewegen und Ausrichten eins Streckenabschnittes im 3D-Raum
 * @param: tbVector3 - Richtungsvektor, tbVector - Positionsvektor
 * @return: tbResult
 */
tbResult CDriftElement::MoveElement(tbVector3 vAlign, tbVector3 vPos) {
	
	tbVector3	vNewDirection;
	
	this->Align(vAlign);
	this->SetPosition(vPos);
	//SetRotation(tbVector3(0.0f, 0.0f, 0.0f));
	return TB_OK;
}

/**
 * Ausrichten eines Streckenabschnittes an einem bereits vorhandenem
 * @param: CDriftElement* - ein bereits geladenes 3D Modell
 * @return: tbResult
 */
tbResult CDriftElement::MoveElement(CDriftElement* pLastElement ) {
	tbVector3	vTmpPos;
	tbVector3	vNewDirection;
	
	//ermitteln der actuellen Position des vorhergehenden Objektes
	vTmpPos = pLastElement->GetPosition();
	//TODO: laenge des anderen objectes ermitteln um die beiden nahtlos aneinander zu fügen
	//this->SetPosition( vTmpPos + tbVector3(0.0f, 2.0f, 0.0f));
	//this->Align(pLastElement->GetXAxis);

	//SetRotation(tbVector3(0.0f, 0.0f, 0.0f));
	return TB_OK;
}

tbResult CDriftElement::Render(float fTime) {
	
	// Transformationsmatrix abfragen und einsetzen
	Update();
	tbDirect3D::SetTransform(D3DTS_WORLD, m_mMatrix);

	// Das Modell rendern
	m_pType->pModel->Render();
	return TB_OK;
}

/**
 *	Destruktor
 */
CDriftElement::~CDriftElement() {
	//ich weiss nicht ob das nötig ist...
	//TB_SAFE_DELETE(m_pType) ;
}
