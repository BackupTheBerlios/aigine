// File.cpp: implementation of the File class.
//
//////////////////////////////////////////////////////////////////////

#include "../AiGinE.h"

// ******************************************************************
// Struktur einer Datei eines ZIP-Archivs
struct ZIPHeader
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
DWORD	g_adwCRC32Table[256];		// CRC32-Tabelle für die Entschlüsselung
BOOL	g_bCRC32TableFilled = FALSE;	// Ist die Tabelle gefüllt?


// ******************************************************************
// CRC32-Wert ermitteln
DWORD CRC32(DWORD a,
			  BYTE b)
{
	// Liefert den CRC32-Wert zweier Zahlen zurück
	return g_adwCRC32Table[(a ^ b) & 0xFF] ^ (a >> 8);
}

// ******************************************************************
// CRC32-Tabelle füllen
Result FillCRC32Table()
{
	DWORD c;	// Temporäre Variable

	if(g_bCRC32TableFilled) return AGE_OK;

	// Tabelle mit speziellem Verfahren ausfüllen
	for(DWORD a = 0; a < 256; a++)
	{
		c = a;
		for(int b = 0; b < 8; b++)
		{
			c = c & 1 ? 0xEDB88320 ^ (c >> 1) : c >> 1;
		}

		// Eintrag speichern
		g_adwCRC32Table[a] = c;
	}

	// Tabelle gefüllt
	g_bCRC32TableFilled = TRUE;
	AGE_INFO("CRC32-Tabelle wurde generiert!");

	return AGE_OK;
}

// ******************************************************************
// ZIP-Schlüssel aktualisieren
Result UpdateZIPKeys(DWORD* pdwKey,
						 BYTE c)
{
	if(pdwKey == NULL) return AGE_ERROR;

	// Die drei Schlüssel aktualisieren
	pdwKey[0] = CRC32(pdwKey[0], c);
	pdwKey[1] += pdwKey[0] & 0x000000FF;
	pdwKey[1] = pdwKey[1] * 134775813 + 1;
	pdwKey[2] = CRC32(pdwKey[2], (BYTE)(pdwKey[1] >> 24));

	return AGE_OK;
}

// ******************************************************************
// Ein Byte einer ZIP-Datei entschlüsseln
BYTE DecryptZIPByte(DWORD* pdwKey)
{
	WORD wTemp;

	// BYTE entschlüsseln und zurückliefern
    wTemp = (WORD)(pdwKey[2] | 2);
	return (((wTemp * (wTemp ^ 1)) >> 8) & 0xFF);
}

// ******************************************************************
// Ein ganzes ZIP-Objekt entschlüsseln
Result DecryptZIPObject(DWORD dwObjectCRC32,
							BYTE* pEncryptionHeader,
							BYTE* pData,
							char* pcPassword)
{
	DWORD	adwKey[3];	// Die drei Schlüssel
	BYTE	Temp;		// Temporäre Variable
	int		iSize;		// Größe des Objekts


	// Parameter prüfen
	if(pcPassword == NULL)	AGE_ERROR_NULL_POINTER("pcPassword", AGE_ERROR);
	if(pData == NULL)		AGE_ERROR_NULL_POINTER("pData", AGE_ERROR);


	// Wenn die CRC32-Tabelle noch nicht ausgefüllt wurde, dann wird
	// das jetzt getan.
	if(!g_bCRC32TableFilled) FillCRC32Table();

	// Die drei Schlüssel initialisieren
	adwKey[0] = 305419896;
	adwKey[1] = 591751049;
	adwKey[2] = 878082192;

	// Schlüssel auf Passwort einstellen
	for(int a = 0; a < (int)(strlen(pcPassword)); a++) UpdateZIPKeys(adwKey, (BYTE)(pcPassword[a]));

	// Verschlüsselungsheader entschlüsseln
	for(a = 0; a < 12; a++)
	{
		Temp = *((BYTE*)(pEncryptionHeader) + a) ^ DecryptZIPByte(adwKey);
		pEncryptionHeader[a] = Temp;
		UpdateZIPKeys(adwKey, Temp);
	}

	// Testen, ob das Passwort in Ordnung war
	if((BYTE)((dwObjectCRC32 & 0xFF000000) >> 24) != pEncryptionHeader[11]) return AGE_ERROR;

	// Nun werden die Daten entschlüsselt.
	iSize = MemGetSize(pData);
	for(a = 0; a < iSize; a++)
	{
		Temp = pData[a] ^ DecryptZIPByte(adwKey);
		pData[a] = Temp;
		UpdateZIPKeys(adwKey, Temp);
	}

	return AGE_OK;
}

// ******************************************************************
// Passwort entschlüsseln
Result DecryptPassword(char* pcPassword)
{
	// Schlüsselwerte, die zum Entschlüsseln des Passworts benötigt werden
	int aiKey[50] = {0x02, 0x03, 0x05, 0x07, 0x11, 0x13, 0x17, 0x19, 0x23, 0x29,
				     0xA2, 0xB3, 0xC5, 0xD7, 0xE1, 0xF3, 0xA7, 0xB9, 0xC3, 0xD9,
	                 0x93, 0xA4, 0xB6, 0xC8, 0xD2, 0xE4, 0x98, 0xA8, 0xB4, 0xC8,
					 0x46, 0x58, 0x63, 0x67, 0x74, 0x78, 0x57, 0x57, 0x68, 0x67,
					 0xA9, 0xBC, 0xC9, 0xDF, 0xF6, 0x0C, 0xBF, 0xCF, 0xFC, 0xFF};

	int iHexToDec[256];		// Umrechungstabelle
	char acPassword[256];	// Neues Passwort


	// Parameter prüfen
	if(pcPassword == NULL) AGE_ERROR_NULL_POINTER("pcPassword", AGE_ERROR);


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
	
	// Das Passwort wird mit einer komplizierten Methode entschlüsselt.
	// Zuerst wird seine Länge verwendet, um den Zufallszahlen-
	// generator zu initialisieren. Das Initialisieren passiert
	// mehrfach, um eine höhere Sicherheit zu gewährleisten.
	srand(strlen(acPassword) * 17);
	for(a = 0; a < (int)(strlen(acPassword)); a++)
	{
		// Zufallszahlengenerator initialisieren
		srand(rand() + strlen(acPassword) * a);
		srand((rand() % (aiKey[a % 50])) + (rand() % (aiKey[(a * 23) % 50])));
		srand((rand() % (aiKey[(a + 2305) % 50])) + (rand() % (aiKey[(17 + a * 133) % 50])) * 177);

		// Entschlüsselung des Passworts vornehmen
		pcPassword[a] = acPassword[a] - (rand() % 256);

		// Zufallszahlengenerator erneut initialisieren
		srand(((BYTE)(acPassword[a]) + 1) * (a + 23) + (rand() % 1381));
	}

	return AGE_OK;
}

// ******************************************************************
// Eine Datei einlesen (alle Typen)
Result ReadFile(char* pcFilename,
								void** ppOut)
{
	char	acFilename[256];	// Kopie des Dateinamens (zum Basteln)
	char*	pcAt;				// Position des "@"-Zeichens im Dateinamen
	char*	pcLeft;				// Linker Teil des Dateinamen
	char*	pcRight;			// Linker Teil des Dateinamen
	char*	pcPassword;			// Passwort
	BOOL	bPasswordEncrypted;	// Passwort verschlüsselt?


	// Diese Funktione verarbeitet einen Dateinamen.
	// Möglich sind folgende Formate:
	// 1) DATEI.DAT
	// 2) DATEI.DAT@ZIPDATEI.ZIP
	// 3) DATEI.DAT@ZIPDATEI.ZIP#PASSWORT
	// 4) !DATEI.DAT@ZIPDATEI.ZIP#PASSWORT
	// Bei 4) bedeutet das "!", dass das angegebene Passwort
	// verschlüsselt ist. Dadurch kann es später nicht in der .EXE-
	// Datei wiedergefunden werden.


	// Parameter prüfen
	if(pcFilename == NULL) AGE_ERROR_NULL_POINTER("pcFilename", AGE_ERROR);


	// Dateiname kopieren
	strcpy(acFilename, pcFilename);
	pcFilename = acFilename;

	// Beginnt der Dateiname mit einem "!"?
	// Falls ja, wird es vorerst ignoriert, aber es wird gespeichert,
	// dass es sich um ein verschlüsseltes Passwort handelt.
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
		// eingelesen. Dazu rufen wir ReadRawFile auf.
		return ReadRawFile(pcFilename, ppOut);
	}

	// Es ist also ein "@"-Zeichen im Dateinamen.
	// Wir kennen seine Position, also können wir den Dateinamen in
	// zwei Strings aufteilen. Das "@"-Zeichen ersetzen wir einfach
	// durch eine binäre null, damit dort der erste String endet.
	*pcAt = 0;
	pcLeft = pcFilename;
	pcRight = pcAt + 1;

	// Falls versucht wird, eine verschlüsselte ZIP-Datei zu lesen,
	// muss zusätzlich ein Passwort angegeben werden. Es beginnt nach
	// dem "#"-Zeichen, was wir jetzt suchen.
	pcPassword = strrchr(pcRight, '#');
	if(pcPassword != NULL)
	{
		// Das "#"-Zeichen wurde gefunden.
		// Es wird durch eine binäre Null ersetzt, damit der String
		// des Dateinamen der ZIP-Datei dort endet.
		*pcPassword = 0;
		pcPassword++;
	}

	// Wenn das Passwort verschlüsselt ist, wird es jetzt entschlüsselt.
	if(bPasswordEncrypted) DecryptPassword(pcPassword);

	// Nun können wir die ZIP-Dateifunktion aufrufen.
	return ReadZIPFileObject(pcRight, pcLeft, pcPassword, ppOut);
}

// ******************************************************************
// ZIP-Objekt lesen
Result ReadZIPFileObject(char* pcZIPFilename,
										 char* pcObjectName,
										 char* pcPassword,
										 void** ppOut)
{
	FILE*		pZIPFile;				// Handle der geöffneten ZIP-Datei
	ZIPHeader	ZIPHeader;				// Header der ZIP-Datei
	char		acObjectName[256];		// Name des aktuellen Objekts
	BOOL		bFinished;				// Fertig mit dem Lesen?
	BOOL		bEncrypted;				// Sind die Daten verschlüsselt?
	BYTE		aEncryptionHeader[12];	// 12 Bytes großer Verschlüsselungsheader
	void*		pData;					// Daten des Objekts
	int			iSize;					// Größe des Objekts


	// Parameter prüfen
	if(pcZIPFilename == NULL)	AGE_ERROR_NULL_POINTER("pcZIPFilename", AGE_ERROR);
	if(pcObjectName == NULL)	AGE_ERROR_NULL_POINTER("pcObjectName", AGE_ERROR);


	// Datei binär zum Lesen öffnen
	pZIPFile = fopen(pcZIPFilename, "rb");
	if(pZIPFile == NULL) AGE_ERROR_FILE(pcZIPFilename, AGE_ERROR);

	// Die ZIP-Datei ist geöffnet.
	// Wir lesen jetzt so lange Objekte ein, bis wir das passende
	// gefunden haben.
	bFinished = FALSE;
	bEncrypted = FALSE;
	while(!bFinished)
	{
		// Den Header einlesen
		if(fread(&ZIPHeader, sizeof(ZIPHeader), 1, pZIPFile) != 1)
		{
			fclose(pZIPFile);
			AGE_ERROR("Fehler beim Lesen des Headers!", AGE_ERROR);
		}

		// Prüfen, ob die Signatur stimmt. Falls nicht, sind wohl
		// alle Dateien gelesen worden.
		if(ZIPHeader.dwSignature != 0x04034B50)
		{
			fclose(pZIPFile);
			AGE_ERROR("Das Objekt wurde nicht gefunden!", AGE_ERROR);
		}

		// Falls Bit Nr. 1 des wGeneralPurpose-Elements gesetzt ist,
		// sind die Daten dieses Objekts verschlüsselt.
		if(ZIPHeader.wGeneralPurpose & 0x0001) bEncrypted = TRUE;

		// Dateiname (Objektname) einlesen
		ZeroMemory(acObjectName, 256 * sizeof(char));
		if(fread(acObjectName, ZIPHeader.wFilenameLength, 1, pZIPFile) != 1)
		{
			fclose(pZIPFile);
			AGE_ERROR("Fehler beim Lesen des Dateinamens!", AGE_ERROR);
		}

		// Vergleichen
		if(!stricmp(pcObjectName, acObjectName))
		{
			// Das richtige Objekt wurde gefunden!
			bFinished = TRUE;
		}

		// Die Extradaten überspringen
		fseek(pZIPFile, ZIPHeader.wExtraLength, SEEK_CUR);

		if(!bFinished)
		{
			// Die Daten des Objekts überspringen, falls es sich nicht
			// um das gesuchte Objekt handelt.
			fseek(pZIPFile, ZIPHeader.dwCompressedSize, SEEK_CUR);

			// Falls Bit Nr. 4 des wGeneralPurpose-Elements des Headers
			// gesetzt ist, folgt ein weiterer Datenbereich, den wir aber
			// nicht brauchen und einfach überspringen können.
			if(ZIPHeader.wGeneralPurpose & 0x0008) fseek(pZIPFile, 12, SEEK_CUR);
		}
	}


	// Das Objekt wurde gefunden!
	// Es muss unkomprimiert sein, um gelesen zu werden.
	if(ZIPHeader.wCompressionMethod != 0)
	{
		fclose(pZIPFile);
		AGE_ERROR("Komprimiertes Objekt gefunden! Dekomprimierung wird nicht unterstützt!", AGE_ERROR);
	}

	// Falls das Objekt verschlüsselt ist, wird der 12 Bytes große
	// Verschlüsselungsheader, der sich vor den eigentlichen Daten
	// befindet, eingelesen werden.
	if(bEncrypted)
	{
		if(fread(aEncryptionHeader, 12, 1, pZIPFile) != 1)
		{
			fclose(pZIPFile);
			AGE_ERROR("Fehler beim Lesen des Verschlüsselungs-Headers!", AGE_ERROR);
		}
	}
	
	// Speicher wird für die Daten reserviert.
	pData = MemAlloc(ZIPHeader.dwCompressedSize - (bEncrypted ? 12 : 0));
	if(pData == NULL) AGE_ERROR_OUT_OF_MEMORY(AGE_ERROR);

	// Daten einlesen
	iSize = MemGetSize(pData);
	if(fread(pData, iSize, 1, pZIPFile) != 1)
	{
		MemFree(pData);
		fclose(pZIPFile);
		AGE_ERROR("Fehler beim Lesen der Objektdaten!", AGE_ERROR);
	}

	// Entschlüsseln, falls nötig
	if(bEncrypted)
	{
		if(DecryptZIPObject(ZIPHeader.dwCRC32, aEncryptionHeader,
		                      (BYTE*)(pData), pcPassword))
		{
			MemFree(pData);
			fclose(pZIPFile);
			AGE_ERROR("Entschlüsselung fehlgeschlagen!", AGE_ERROR);
		}
	}

	// Datei schließen und Pointer zurückliefern
	fclose(pZIPFile);

	// Zeiger liefern
	*ppOut = pData;

	return AGE_OK;
}

// ******************************************************************
// "Rohe" Datei lesen
Result ReadRawFile(char* pcFilename,
								   void** ppOut)
{
	struct stat FileStats;	// Statistiken der Datei
	void*		pData;		// Daten aus der Datei
	FILE*		pFile;		// Dateihandle


	// Parameter prüfen
	if(pcFilename == NULL) AGE_ERROR_NULL_POINTER("pcFilename", AGE_ERROR);


	// Statistiken der Datei abfragen (wir brauchen nur die Größe)
	ZeroMemory(&FileStats, sizeof(struct stat));
	stat(pcFilename, &FileStats);
	if(FileStats.st_size <= 0) AGE_ERROR_FILE(pcFilename, AGE_ERROR);

	// Speicherbereich reservieren, der groß genug ist
	pData = MemAlloc(FileStats.st_size);
	if(pData == NULL) AGE_ERROR_OUT_OF_MEMORY(AGE_ERROR);

	// Datei binär zum Lesen öffnen
	pFile = fopen(pcFilename, "rb");
	if(pFile == NULL)
	{
		MemFree(pData);
		AGE_ERROR_FILE(pcFilename, AGE_ERROR);
	}

	// Daten lesen
	if(fread(pData, FileStats.st_size, 1, pFile) != 1)
	{
		MemFree(pData);
		fclose(pFile);
		AGE_ERROR_FILE(pcFilename, AGE_ERROR);
	}

	// Datei schließen und Pointer zurückliefern
	fclose(pFile);

	// Zeiger liefern
	*ppOut = pData;

	return AGE_OK;
}

// ******************************************************************
// Daten in eine Datei schreiben
Result WriteFile(char* pcFilename,
								 void* pData,
								 int iNumBytes)
{
	FILE* pFile; // Dateihandle

	// Parameter prüfen
	if(pcFilename == NULL)	AGE_ERROR_NULL_POINTER("pcFilename", AGE_ERROR);
	if(pData == NULL)		AGE_ERROR_NULL_POINTER("pData", AGE_ERROR);
	if(iNumBytes <= 0)		AGE_ERROR_INVALID_VALUE("iNumBytes", AGE_ERROR);


	// Datei zum Schreiben im Binärmodus öffnen
	pFile = fopen(pcFilename, "wb");
	if(pFile == NULL) AGE_ERROR_FILE(pcFilename, AGE_ERROR);

	// Daten schreiben
	if(fwrite(pData, iNumBytes, 1, pFile) != 1)
	{
		fclose(pFile);
		AGE_ERROR_FILE(pcFilename, AGE_ERROR);
	}

	// Datei schließen und fertig
	fclose(pFile);

	return AGE_OK;
}
