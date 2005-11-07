/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbINIReader.cpp
	===============
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Umgang mit INI-Dateien

	Autor:
	Snorky
	[24.9.2003]

********************************************************************/

#include <TriBase.h>


// ******************************************************************
// Konstruktor der tb2DSprite-Klasse
tbINIReader::tbINIReader()
{
	// Alles zurücksetzen
	ZeroMemory(this, sizeof(tbINIReader));
}

// ******************************************************************
// Destruktor der tb2DSprite-Klasse
tbINIReader::~tbINIReader()
{
}

// ******************************************************************
// Angabe des zu lesenden Ini-Files
tbResult tbINIReader::SetFilePath(char* pcINIFile)
{
	strncpy(pcFilePath, pcINIFile, sizeof(pcFilePath));
	
	return TB_OK;
}

// ******************************************************************
// Liest einen int-Wert aus der INI-Datei
int tbINIReader::ReadINIInt(char* pcSection,
							char* pcKey)
{
	char acString[256];
	ZeroMemory(acString, sizeof(acString));

	// String lesen
	ReadINIString(pcSection, pcKey, acString, 256);
	if(!strcmp(acString, "[NOT FOUND]")) return 12345678;

	// In int-Wert umwandeln
	return atoi(acString);
}

// ******************************************************************
// Liest einen float-Wert aus der INI-Datei
float tbINIReader::ReadINIFloat(char* pcSection,
								char* pcKey)
{
	float	fValue;
	char	acString[256];
	ZeroMemory(acString, sizeof(acString));


	// String lesen
	ReadINIString(pcSection, pcKey, acString, 256);
	if(!strcmp(acString, "[NOT FOUND]")) return 12345678.0f;

	// In float-Wert umwandeln
	sscanf(acString, "%f", &fValue);

	return fValue;
}

// ******************************************************************
// Liest einen tbVector2-Wert aus der INI-Datei
tbVector2 tbINIReader::ReadINIVector2(char* pcSection,
									  char* pcKey)
{
	tbVector2	vValue;
	char		acString[256];
	ZeroMemory(acString, sizeof(acString));


	// String lesen
	ReadINIString(pcSection, pcKey, acString, 256);
	if(!strcmp(acString, "[NOT FOUND]")) return tbVector2(12345678.0f, 12345678.0f);

	// Die Vektorkomponenten extrahieren
	sscanf(acString, "%f, %f", &vValue.x, &vValue.y);

	return vValue;
}

// ******************************************************************
// Liest einen tbVector3-Wert aus der INI-Datei
tbVector3 tbINIReader::ReadINIVector3(char* pcSection,
									  char* pcKey)
{
	tbVector3	vValue;
	char		acString[256];
	ZeroMemory(acString, sizeof(acString));


	// String lesen
	ReadINIString(pcSection, pcKey, acString, 256);
	if(!strcmp(acString, "[NOT FOUND]")) return tbVector3(12345678.0f, 12345678.0f, 12345678.0f);

	// Die Vektorkomponenten extrahieren
	sscanf(acString, "%f, %f, %f", &vValue.x, &vValue.y, &vValue.z);

	return vValue;
}

// ******************************************************************
// Liest einen tbVector4-Wert aus der INI-Datei
tbVector4 tbINIReader::ReadINIVector4(char* pcSection,
									  char* pcKey)
{
	tbVector4	vValue;
	char		acString[256];
	ZeroMemory(acString, sizeof(acString));


	// String lesen
	ReadINIString(pcSection, pcKey, acString, 256);
	if(!strcmp(acString, "[NOT FOUND]")) return tbVector4(12345678.0f, 12345678.0f, 12345678.0f, 12345678.0f);

	// Die Vektorkomponenten extrahieren
	sscanf(acString, "%f, %f, %f, %f", &vValue.x1, &vValue.y1, &vValue.x2, &vValue.y2);

	return vValue;
}

// ******************************************************************
// Liest einen tbColor-Wert aus der INI-Datei
tbColor tbINIReader::ReadINIColor(char* pcSection,
								  char* pcKey)
{
	tbColor	Value;
	char	acString[256];
	ZeroMemory(acString, sizeof(acString));


	// String lesen
	ReadINIString(pcSection, pcKey, acString, 256);
	if(!strcmp(acString, "[NOT FOUND]")) return tbColor(12345678.0f, 12345678.0f, 12345678.0f, 12345678.0f);

	// Die Farbkomponenten extrahieren
	sscanf(acString, "%f, %f, %f, %f", &Value.r, &Value.g, &Value.b, &Value.a);

	return Value;
}

// ******************************************************************
// Liest einen String aus der INI-Datei
tbResult tbINIReader::ReadINIString(char* pcSection,
									char* pcKey,
									char* pcOut,
									int iBufferSize)
{
	// String lesen
	GetPrivateProfileString(pcSection, pcKey, "[NOT FOUND]",
		                    pcOut, iBufferSize,
							pcFilePath);

	return TB_OK;
}

// ******************************************************************
// Schreibt einen String in die INI-Datei
tbResult tbINIReader::WriteINIString(char* pcSection,
									 char* pcKey,
									 char* pcString)
{
	// String schreiben
	WritePrivateProfileString(pcSection, pcKey, pcString, pcFilePath);

	return TB_OK;
}

// ******************************************************************
