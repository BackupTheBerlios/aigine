/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbINIReader.h
	=============
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Umgang mit INI-Dateien

	Autor:
	Snorky
	[24.9.2003]

********************************************************************/

#ifndef __TBINIREADER__
#define __TBINIREADER__

#pragma pack(1)


// ******************************************************************
// Klasse eines INI-Files
class TRIBASE_API tbINIReader
{
private:
	char pcFilePath[256];	// Pfad und Dateiname des INI-Files
public:
	// Konstruktor und Destruktor
	tbINIReader();
	~tbINIReader();

	// Methoden
	tbResult	SetFilePath(char* pcINIFile);		// setzen des zu lesenden Ini-Files
	
	// Liest einen String aus der INI-Datei
	tbResult ReadINIString(char* pcSection,
						   char* pcKey,
						   char* pcOut,
						   int iBufferSize);
									
	// Liest einen tbColor-Wert aus der INI-Datei
	tbColor ReadINIColor(char* pcSection,
					     char* pcKey);

	// Liest einen tbVector2-Wert aus der INI-Datei
	tbVector2 ReadINIVector2(char* pcSection,
						     char* pcKey);
	
	// Liest einen tbVector3-Wert aus der INI-Datei
	tbVector3 ReadINIVector3(char* pcSection,
						     char* pcKey);
	
	// Liest einen tbVector4-Wert aus der INI-Datei
	tbVector4 ReadINIVector4(char* pcSection,
							 char* pcKey);

	// Liest einen float-Wert aus der INI-Datei
	float ReadINIFloat(char* pcSection,
					   char* pcKey);

	// Liest einen int-Wert aus der INI-Datei
	int ReadINIInt(char* pcSection,
				   char* pcKey);

	// Schreibt einen String in die INI-Datei
	tbResult WriteINIString(char* pcSection,
							char* pcKey,
							char* pcString);

	// Inline-Methoden
	char* GetFilePath()	{return pcFilePath;}
};

// ******************************************************************

#endif	__TBINIREADER__

