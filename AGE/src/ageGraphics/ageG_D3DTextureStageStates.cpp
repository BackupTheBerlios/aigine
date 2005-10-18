/* Generated by Together */

#include "ageG_D3DTextureStageStates.h"
#include <TriBase.h>
#include "ageG_D3DRenderStates.h"
#include "ageG_D3DSamplerStates.h"
#include "AGE.h"
age_Result ageG_D3DTextureStageStates::SetTSS(DWORD dwStage,
							D3DTEXTURESTAGESTATETYPE TSS,
							DWORD dwValue)
{
	// In der Debug-Version wird gepr�ft, ob dwStage und TSS in Ordnung sind.
#ifdef _DEBUG
	if(dwStage < 0 || dwStage > 8 ||
	   TSS < 0 || TSS > 32)
	{
		// Texturschicht-State ist nicht g�ltig!
		TB_ERROR("Ung�ltiges Texturschicht-State!", TB_ERROR);
	}
#endif

	// Wenn das Texturschicht-State schon gesetzt ist, direkt abbrechen
	if(m_TSS.adwTSS[dwStage][TSS] == dwValue) return TB_OK;
	else
	{
		// Texturschicht-State setzen und in die Tabelle schreiben
		m_pD3DDevice->SetTextureStageState(dwStage, TSS, dwValue);
		m_TSS.adwTSS[dwStage][TSS] = dwValue;
	}

	return TB_OK;
}
age_Result ageG_D3DTextureStageStates::SetTSSF(DWORD dwStage,
							 D3DTEXTURESTAGESTATETYPE TSS,
							 float fValue)
{
	// In der Debug-Version wird gepr�ft, ob dwStage und TSS in Ordnung sind.
#ifdef _DEBUG
	if(dwStage < 0 || dwStage > 8 ||
	   TSS < 0 || TSS > 32)
	{
		// Texturschicht-State ist nicht g�ltig!
		TB_ERROR("Ung�ltiges Texturschicht-State!", TB_ERROR);
	}
#endif

	// Wenn das Texturschicht-State schon gesetzt ist, direkt abbrechen
	if(m_TSS.afTSS[dwStage][TSS] == fValue) return TB_OK;
	else
	{
		// Texturschicht-State setzen und in die Tabelle schreiben
		m_pD3DDevice->SetTextureStageState(dwStage, TSS, *((DWORD*)(&fValue)));
		m_TSS.afTSS[dwStage][TSS] = fValue;
	}

	return TB_OK;
}
DWORD ageG_D3DTextureStageStates::GetTSS(DWORD dwStage,
						 D3DTEXTURESTAGESTATETYPE TSS)
{
	// In der Debug-Version wird gepr�ft, ob dwStage und TSS in Ordnung sind.
#ifdef _DEBUG
	if(dwStage < 0 || dwStage > 8 ||
	   TSS < 0 || TSS > 32)
	{
		// Texturschicht-State ist nicht g�ltig!
		TB_ERROR("Ung�ltiges Texturschicht-State!", 0xEEEEEEEE);
	}
#endif

	// Auf die Tabelle zugreifen
	return m_TSS.adwTSS[dwStage][TSS];
}
float ageG_D3DTextureStageStates::GetTSSF(DWORD dwStage,
						  D3DTEXTURESTAGESTATETYPE TSS)
{
	// In der Debug-Version wird gepr�ft, ob dwStage und TSS in Ordnung sind.
#ifdef _DEBUG
	if(dwStage < 0 || dwStage > 8 ||
	   TSS < 0 || TSS > 32)
	{
		// Texturschicht-State ist nicht g�ltig!
		TB_ERROR("Ung�ltiges Texturschicht-State!", -10000000.0f);
	}
#endif

	// Auf die Tabelle zugreifen
	return m_TSS.afTSS[dwStage][TSS];
}
