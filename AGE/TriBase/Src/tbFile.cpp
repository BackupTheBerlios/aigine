/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`�__\ /\ \ \ \  __\ /\ __\_ /\  __\   /�__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbFile.cpp
	==========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Umgang mit verschiedenen Dateien

	Autor:
	David Scherfgen
	_________________________________________________
	|												|
	|	ACHTUNG:									|
	|	========									|
	|	DIESE DATEI MUSS MIT DER COMPILER-OPTION	|
	|	"STRUKTURMITGLIED AUF 1 BYTE AUSRICHTEN"	|
	|	KOMPILIERT WERDEN!							|
	|												|
********************************************************************/

#include <TriBase.h>

// ******************************************************************
// Struktur einer Datei eines ZIP-Archivs
struct tbZIPHeader
{
	DWORD	dwSignature;
	WORD	wVersionNeeded;
	WORD	wGeneralPurpose;
	WORD	wCompressionMethod;
	WORD	wLastModTime;
	WORD	wLastModDate;
	DWORD	dwCRC32;
	DWORD	dwCompressedSize;
	DWORD	dwUncompressedSize;
	WORD	wFilenameLength;
	WORD	wExtraLength;
};

// ******************************************************************
// Interne Variablen
DWORD	tb_g_adwCRC32Table[256];		// CRC32-Tabelle f�r die Entschl�sselung
BOOL	tb_g_bCRC32TableFilled = FALSE;	// Ist die Tabelle gef�llt?


// ******************************************************************
// CRC32-Wert ermitteln
DWORD tbCRC32(DWORD a,
			  BYTE b)
{
	// Liefert den CRC32-Wert zweier Zahlen zur�ck
	return tb_g_adwCRC32Table[(a ^ b) & 0xFF] ^ (a >> 8);
}

// ******************************************************************
// CRC32-Tabelle f�llen
tbResult tbFillCRC32Table()
{
	DWORD c;	// Tempor�re Variable

	if(tb_g_bCRC32TableFilled) return TB_OK;

	// Tabelle mit speziellem Verfahren ausf�llen
	for(DWORD a = 0; a < 256; a++)
	{
		c = a;
		for(int b = 0; b < 8; b++)
		{
			c = c & 1 ? 0xEDB88320 ^ (c >> 1) : c >> 1;
		}

		// Eintrag speichern
		tb_g_adwCRC32Table[a] = c;
	}

	// Tabelle gef�llt
	tb_g_bCRC32TableFilled = TRUE;
	TB_INFO("CRC32-Tabelle wurde generiert!");

	return TB_OK;
}

// ******************************************************************
// ZIP-Schl�ssel aktualisieren
tbResult tbUpdateZIPKeys(DWORD* pdwKey,
						 BYTE c)
{
	if(pdwKey == NULL) return TB_ERROR;

	// Die drei Schl�ssel aktualisieren
	pdwKey[0] = tbCRC32(pdwKey[0], c);
	pdwKey[1] += pdwKey[0] & 0x000000FF;
	pdwKey[1] = pdwKey[1] * 134775813 + 1;
	pdwKey[2] = tbCRC32(pdwKey[2], (BYTE)(pdwKey[1] >> 24));

	return TB_OK;
}

// ******************************************************************
// Ein Byte einer ZIP-Datei entschl�sseln
BYTE tbDecryptZIPByte(DWORD* pdwKey)
{
	WORD wTemp;

	// BYTE entschl�sseln und zur�ckliefern
    wTemp = (WORD)(pdwKey[2] | 2);
	return (((wTemp * (wTemp ^ 1)) >> 8) & 0xFF);
}

// ******************************************************************
// Ein ganzes ZIP-Objekt entschl�sseln
tbResult tbDecryptZIPObject(DWORD dwObjectCRC32,
							BYTE* pEncryptionHeader,
							BYTE* pData,
							char* pcPassword)
{
	DWORD	adwKey[3];	// Die drei Schl�ssel
	BYTE	Temp;		// Tempor�re Variable
	int		iSize;		// Gr��e des Objekts


	// Parameter pr�fen
	if(pcPassword == NULL)	TB_ERROR_NULL_POINTER("pcPassword", TB_ERROR);
	if(pData == NULL)		TB_ERROR_NULL_POINTER("pData", TB_ERROR);


	// Wenn die CRC32-Tabelle noch nicht ausgef�llt wurde, dann wird
	// das jetzt getan.
	if(!tb_g_bCRC32TableFilled) tbFillCRC32Table();

	// Die drei Schl�ssel initialisieren
	adwKey[0] = 305419896;
	adwKey[1] = 591751049;
	adwKey[2] = 878082192;

	// Schl�ssel auf Passwort einstellen
	for(int a = 0; a < (int)(strlen(pcPassword)); a++) tbUpdateZIPKeys(adwKey, (BYTE)(pcPassword[a]));

	// Verschl�sselungsheader entschl�sseln
	for(a = 0; a < 12; a++)
	{
		Temp = *((BYTE*)(pEncryptionHeader) + a) ^ tbDecryptZIPByte(adwKey);
		pEncryptionHeader[a] = Temp;
		tbUpdateZIPKeys(adwKey, Temp);
	}

	// Testen, ob das Passwort in Ordnung war
	if((BYTE)((dwObjectCRC32 & 0xFF000000) >> 24) != pEncryptionHeader[11]) return TB_ERROR;

	// Nun werden die Daten entschl�sselt.
	iSize = tbMemGetSize(pData);
	for(a = 0; a < iSize; a++)
	{
		Temp = pData[a] ^ tbDecryptZIPByte(adwKey);
		pData[a] = Temp;
		tbUpdateZIPKeys(adwKey, Temp);
	}

	return TB_OK;
}

// ******************************************************************
// Passwort entschl�sseln
tbResult tbDecryptPassword(char* pcPassword)
{
	// Schl�sselwerte, die zum Entschl�sseln des Passworts ben�tigt werden
	int aiKey[50] = {0x02, 0x03, 0x05, 0x07, 0x11, 0x13, 0x17, 0x19, 0x23, 0x29,
				     0xA2, 0xB3, 0xC5, 0xD7, 0xE1, 0xF3, 0xA7, 0xB9, 0xC3, 0xD9,
	                 0x93, 0xA4, 0xB6, 0xC8, 0xD2, 0xE4, 0x98, 0xA8, 0xB4, 0xC8,
					 0x46, 0x58, 0x63, 0x67, 0x74, 0x78, 0x57, 0x57, 0x68, 0x67,
					 0xA9, 0xBC, 0xC9, 0xDF, 0xF6, 0x0C, 0xBF, 0xCF, 0xFC, 0xFF};

	int iHexToDec[256];		// Umrechungstabelle
	char acPassword[256];	// Neues Passwort


	// Parameter pr�fen
	if(pcPassword == NULL) TB_ERROR_NULL_POINTER("pcPassword", TB_ERROR);


	// Hexadezimaltabelle initialisieren
	ZeroMemory(iHexToDec, 256 * sizeof(int));
	iHexToDec['0'] = 0;
	iHexToDec['1'] = 1;
	iHexToDec['2'] = 2;
	iHexToDec['3'] = 3;
	iHexToDec['4'] = 4;
	iHexToDec['5'] = 5;
	iHexToDec['6'] = 6;
	iHexToDec['7'] = 7;
	iHexToDec['8'] = 8;
	iHexToDec['9'] = 9;
	iHexToDec['A'] = iHexToDec['a'] = 10;
	iHexToDec['B'] = iHexToDec['b'] = 11;
	iHexToDec['C'] = iHexToDec['c'] = 12;
	iHexToDec['D'] = iHexToDec['d'] = 13;
	iHexToDec['E'] = iHexToDec['e'] = 14;
	iHexToDec['F'] = iHexToDec['f'] = 15;

	// Das Passwort liegt im Hexadezimalformat vor. Dieses wandeln
	// wir jetzt in Bytefolgen um.
	ZeroMemory(acPassword, 256 * sizeof(char));
	for(int a = 0; a < (int)(strlen(pcPassword)); a += 2)
	{
		acPassword[a / 2] = iHexToDec[pcPassword[a]] * 16 +
			                iHexToDec[pcPassword[a + 1]];
	}

	// Das Hexadezimalpasswort brauchen wir nicht mehr.
	ZeroMemory(pcPassword, strlen(pcPassword) * sizeof(char));
	
	// Das Passwort wird mit einer komplizierten Methode entschl�sselt.
	// Zuerst wird seine L�nge verwendet, um den Zufallszahlen-
	// generator zu initialisieren. Das Initialisieren passiert
	// mehrfach, um eine h�here Sicherheit zu gew�hrleisten.
	srand(strlen(acPassword) * 17);
	for(a = 0; a < (int)(strlen(acPassword)); a++)
	{
		// Zufallszahlengenerator initialisieren
		srand(rand() + strlen(acPassword) * a);
		srand((rand() % (aiKey[a % 50])) + (rand() % (aiKey[(a * 23) % 50])));
		srand((rand() % (aiKey[(a + 2305) % 50])) + (rand() % (aiKey[(17 + a * 133) % 50])) * 177);

		// Entschl�sselung des Passworts vornehmen
		pcPassword[a] = acPassword[a] - (rand() % 256);

		// Zufallszahlengenerator erneut initialisieren
		srand(((BYTE)(acPassword[a]) + 1) * (a + 23) + (rand() % 1381));
	}

	return TB_OK;
}

// ******************************************************************
// Eine Datei einlesen (alle Typen)
TRIBASE_API tbResult tbReadFile(char* pcFilename,
								void** ppOut)
{
	char	acFilename[256];	// Kopie des Dateinamens (zum Basteln)
	char*	pcAt;				// Position des "@"-Zeichens im Dateinamen
	char*	pcLeft;				// Linker Teil des Dateinamen
	char*	pcRight;			// Linker Teil des Dateinamen
	char*	pcPassword;			// Passwort
	BOOL	bPasswordEncrypted;	// Passwort verschl�sselt?


	// Diese Funktione verarbeitet einen Dateinamen.
	// M�glich sind folgende Formate:
	// 1) DATEI.DAT
	// 2) DATEI.DAT@ZIPDATEI.ZIP
	// 3) DATEI.DAT@ZIPDATEI.ZIP#PASSWORT
	// 4) !DATEI.DAT@ZIPDATEI.ZIP#PASSWORT
	// Bei 4) bedeutet das "!", dass das angegebene Passwort
	// verschl�sselt ist. Dadurch kann es sp�ter nicht in der .EXE-
	// Datei wiedergefunden werden.


	// Parameter pr�fen
	if(pcFilename == NULL) TB_ERROR_NULL_POINTER("pcFilename", TB_ERROR);


	// Dateiname kopieren
	strcpy(acFilename, pcFilename);
	pcFilename = acFilename;

	// Beginnt der Dateiname mit einem "!"?
	// Falls ja, wird es vorerst ignoriert, aber es wird gespeichert,
	// dass es sich um ein verschl�sseltes Passwort handelt.
	bPasswordEncrypted = pcFilename[0] == '!';
	if(bPasswordEncrypted) pcFilename++;

	// Nach einem "@"-Zeichen im Dateinamen suchen.
	// Falls eines gefunden wird, wird der Dateiname an dieser Stelle
	// geteilt - der vordere Teil ist der Objektname und der hintere
	// der Dateiname der enthaltenden Datei.
	pcAt = strrchr(pcFilename, '@');
	if(pcAt == NULL)
	{
		// Kein "@"-Zeichen gefunden! Die Datei wird ganz normal
		// eingelesen. Dazu rufen wir tbReadRawFile auf.
		return tbReadRawFile(pcFilename, ppOut);
	}

	// Es ist also ein "@"-Zeichen im Dateinamen.
	// Wir kennen seine Position, also k�nnen wir den Dateinamen in
	// zwei Strings aufteilen. Das "@"-Zeichen ersetzen wir einfach
	// durch eine bin�re null, damit dort der erste String endet.
	*pcAt = 0;
	pcLeft = pcFilename;
	pcRight = pcAt + 1;

	// Falls versucht wird, eine verschl�sselte ZIP-Datei zu lesen,
	// muss zus�tzlich ein Passwort angegeben werden. Es beginnt nach
	// dem "#"-Zeichen, was wir jetzt suchen.
	pcPassword = strrchr(pcRight, '#');
	if(pcPassword != NULL)
	{
		// Das "#"-Zeichen wurde gefunden.
		// Es wird durch eine bin�re Null ersetzt, damit der String
		// des Dateinamen der ZIP-Datei dort endet.
		*pcPassword = 0;
		pcPassword++;
	}

	// Wenn das Passwort verschl�sselt ist, wird es jetzt entschl�sselt.
	if(bPasswordEncrypted) tbDecryptPassword(pcPassword);

	// Nun k�nnen wir die ZIP-Dateifunktion aufrufen.
	return tbReadZIPFileObject(pcRight, pcLeft, pcPassword, ppOut);
}

// ******************************************************************
// ZIP-Objekt lesen
TRIBASE_API tbResult tbReadZIPFileObject(char* pcZIPFilename,
										 char* pcObjectName,
										 char* pcPassword,
										 void** ppOut)
{
	FILE*		pZIPFile;				// Handle der ge�ffneten ZIP-Datei
	tbZIPHeader	ZIPHeader;				// Header der ZIP-Datei
	char		acObjectName[256];		// Name des aktuellen Objekts
	BOOL		bFinished;				// Fertig mit dem Lesen?
	BOOL		bEncrypted;				// Sind die Daten verschl�sselt?
	BYTE		aEncryptionHeader[12];	// 12 Bytes gro�er Verschl�sselungsheader
	void*		pData;					// Daten des Objekts
	int			iSize;					// Gr��e des Objekts


	// Parameter pr�fen
	if(pcZIPFilename == NULL)	TB_ERROR_NULL_POINTER("pcZIPFilename", TB_ERROR);
	if(pcObjectName == NULL)	TB_ERROR_NULL_POINTER("pcObjectName", TB_ERROR);


	// Datei bin�r zum Lesen �ffnen
	pZIPFile = fopen(pcZIPFilename, "rb");
	if(pZIPFile == NULL) TB_ERROR_FILE(pcZIPFilename, TB_ERROR);

	// Die ZIP-Datei ist ge�ffnet.
	// Wir lesen jetzt so lange Objekte ein, bis wir das passende
	// gefunden haben.
	bFinished = FALSE;
	bEncrypted = FALSE;
	while(!bFinished)
	{
		// Den Header einlesen
		if(fread(&ZIPHeader, sizeof(tbZIPHeader), 1, pZIPFile) != 1)
		{
			fclose(pZIPFile);
			TB_ERROR("Fehler beim Lesen des Headers!", TB_ERROR);
		}

		// Pr�fen, ob die Signatur stimmt. Falls nicht, sind wohl
		// alle Dateien gelesen worden.
		if(ZIPHeader.dwSignature != 0x04034B50)
		{
			fclose(pZIPFile);
			TB_ERROR("Das Objekt wurde nicht gefunden!", TB_ERROR);
		}

		// Falls Bit Nr. 1 des wGeneralPurpose-Elements gesetzt ist,
		// sind die Daten dieses Objekts verschl�sselt.
		if(ZIPHeader.wGeneralPurpose & 0x0001) bEncrypted = TRUE;

		// Dateiname (Objektname) einlesen
		ZeroMemory(acObjectName, 256 * sizeof(char));
		if(fread(acObjectName, ZIPHeader.wFilenameLength, 1, pZIPFile) != 1)
		{
			fclose(pZIPFile);
			TB_ERROR("Fehler beim Lesen des Dateinamens!", TB_ERROR);
		}

		// Vergleichen
		if(!stricmp(pcObjectName, acObjectName))
		{
			// Das richtige Objekt wurde gefunden!
			bFinished = TRUE;
		}

		// Die Extradaten �berspringen
		fseek(pZIPFile, ZIPHeader.wExtraLength, SEEK_CUR);

		if(!bFinished)
		{
			// Die Daten des Objekts �berspringen, falls es sich nicht
			// um das gesuchte Objekt handelt.
			fseek(pZIPFile, ZIPHeader.dwCompressedSize, SEEK_CUR);

			// Falls Bit Nr. 4 des wGeneralPurpose-Elements des Headers
			// gesetzt ist, folgt ein weiterer Datenbereich, den wir aber
			// nicht brauchen und einfach �berspringen k�nnen.
			if(ZIPHeader.wGeneralPurpose & 0x0008) fseek(pZIPFile, 12, SEEK_CUR);
		}
	}


	// Das Objekt wurde gefunden!
	// Es muss unkomprimiert sein, um gelesen zu werden.
	if(ZIPHeader.wCompressionMethod != 0)
	{
		fclose(pZIPFile);
		TB_ERROR("Komprimiertes Objekt gefunden! Dekomprimierung wird nicht unterst�tzt!", TB_ERROR);
	}

	// Falls das Objekt verschl�sselt ist, wird der 12 Bytes gro�e
	// Verschl�sselungsheader, der sich vor den eigentlichen Daten
	// befindet, eingelesen werden.
	if(bEncrypted)
	{
		if(fread(aEncryptionHeader, 12, 1, pZIPFile) != 1)
		{
			fclose(pZIPFile);
			TB_ERROR("Fehler beim Lesen des Verschl�sselungs-Headers!", TB_ERROR);
		}
	}
	
	// Speicher wird f�r die Daten reserviert.
	pData = tbMemAlloc(ZIPHeader.dwCompressedSize - (bEncrypted ? 12 : 0));
	if(pData == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

	// Daten einlesen
	iSize = tbMemGetSize(pData);
	if(fread(pData, iSize, 1, pZIPFile) != 1)
	{
		tbMemFree(pData);
		fclose(pZIPFile);
		TB_ERROR("Fehler beim Lesen der Objektdaten!", TB_ERROR);
	}

	// Entschl�sseln, falls n�tig
	if(bEncrypted)
	{
		if(tbDecryptZIPObject(ZIPHeader.dwCRC32, aEncryptionHeader,
		                      (BYTE*)(pData), pcPassword))
		{
			tbMemFree(pData);
			fclose(pZIPFile);
			TB_ERROR("Entschl�sselung fehlgeschlagen!", TB_ERROR);
		}
	}

	// Datei schlie�en und Pointer zur�ckliefern
	fclose(pZIPFile);

	// Zeiger liefern
	*ppOut = pData;

	return TB_OK;
}

// ******************************************************************
// "Rohe" Datei lesen
TRIBASE_API tbResult tbReadRawFile(char* pcFilename,
								   void** ppOut)
{
	struct stat FileStats;	// Statistiken der Datei
	void*		pData;		// Daten aus der Datei
	FILE*		pFile;		// Dateihandle


	// Parameter pr�fen
	if(pcFilename == NULL) TB_ERROR_NULL_POINTER("pcFilename", TB_ERROR);


	// Statistiken der Datei abfragen (wir brauchen nur die Gr��e)
	ZeroMemory(&FileStats, sizeof(struct stat));
	stat(pcFilename, &FileStats);
	if(FileStats.st_size <= 0) TB_ERROR_FILE(pcFilename, TB_ERROR);

	// Speicherbereich reservieren, der gro� genug ist
	pData = tbMemAlloc(FileStats.st_size);
	if(pData == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

	// Datei bin�r zum Lesen �ffnen
	pFile = fopen(pcFilename, "rb");
	if(pFile == NULL)
	{
		tbMemFree(pData);
		TB_ERROR_FILE(pcFilename, TB_ERROR);
	}

	// Daten lesen
	if(fread(pData, FileStats.st_size, 1, pFile) != 1)
	{
		tbMemFree(pData);
		fclose(pFile);
		TB_ERROR_FILE(pcFilename, TB_ERROR);
	}

	// Datei schlie�en und Pointer zur�ckliefern
	fclose(pFile);

	// Zeiger liefern
	*ppOut = pData;

	return TB_OK;
}

// ******************************************************************
// Daten in eine Datei schreiben
TRIBASE_API tbResult tbWriteFile(char* pcFilename,
								 void* pData,
								 int iNumBytes)
{
	FILE* pFile; // Dateihandle

	// Parameter pr�fen
	if(pcFilename == NULL)	TB_ERROR_NULL_POINTER("pcFilename", TB_ERROR);
	if(pData == NULL)		TB_ERROR_NULL_POINTER("pData", TB_ERROR);
	if(iNumBytes <= 0)		TB_ERROR_INVALID_VALUE("iNumBytes", TB_ERROR);


	// Datei zum Schreiben im Bin�rmodus �ffnen
	pFile = fopen(pcFilename, "wb");
	if(pFile == NULL) TB_ERROR_FILE(pcFilename, TB_ERROR);

	// Daten schreiben
	if(fwrite(pData, iNumBytes, 1, pFile) != 1)
	{
		fclose(pFile);
		TB_ERROR_FILE(pcFilename, TB_ERROR);
	}

	// Datei schlie�en und fertig
	fclose(pFile);

	return TB_OK;
}

// ******************************************************************