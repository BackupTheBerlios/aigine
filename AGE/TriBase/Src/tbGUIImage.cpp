/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbGUIImage.cpp
	===============
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Bilder für die Benutzeroberfläche

	Autor:
	David Scherfgen

********************************************************************/

#include <TriBase.h>

// ******************************************************************
// Nachrichtenfunktion für ein Bild
tbResult tbGUIImage::ReceiveMessage(const tbGUIMessage* pMsg)
{
	tbGUIVertex aVertex[4];

	switch(pMsg->Type)
	{
	case TB_GMT_RENDER:
		// Wenn das Element unsichtbar ist, wird es nicht gerendert.
		if(!m_bVisible) break;

		if(pMsg->aiInt[0] == 2)
		{
			// Bild zeichnen
			m_pGUI->AddRect(m_vPosition * m_pGUI->m_vScaling, m_vSize * m_pGUI->m_vScaling,
				            m_Color, m_vTopLeftTex, m_vBottomRightTex - m_vTopLeftTex, aVertex);

			// Textur setzen
			tbDirect3D::SetTexture(0, m_pTexture);

			// Dreiecke zeichnen
			tbDirect3D::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, aVertex,
				                                     sizeof(tbGUIVertex));
		}
		break;
	}

	return TB_OK;
}

// ******************************************************************
// Erstellung eines Bildes
tbResult tbGUI::CreateImage(int iID,
							int iPage,
							tbVector2 vPosition,
							tbVector2 vSize,
							PDIRECT3DTEXTURE9 pTexture,
							tbVector2 vTopLeftTex,		// = tbVector2(0.0f, 0.0f)
							tbVector2 vBottomRightTex,	// = tbVector2(1.0f, 1.0f)
							tbColor Color)				// = tbColor(1.0f)
{
	tbGUIElement*	pElement;
	tbGUIImage*		pImage;


	// Image erstellen
	pElement = CreateElement(iID, TB_GET_IMAGE, iPage, vPosition, vSize);
	if(pElement == NULL) TB_ERROR("Fehler beim Erstellen des Bildes!", TB_ERROR);

	// Daten eintragen
	pImage = (tbGUIImage*)(pElement);
	pImage->m_pTexture = pTexture;
	pImage->m_vTopLeftTex = vTopLeftTex;
	pImage->m_vBottomRightTex = vBottomRightTex;
	pImage->m_Color = Color;

	return TB_OK;
}

// ******************************************************************