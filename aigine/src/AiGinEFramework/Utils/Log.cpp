// Log.cpp: implementation of the Log class.
//
//////////////////////////////////////////////////////////////////////

#include "../AiGinE.h"

// Globale Variablen
FILE* g_pLogFile = NULL;	// Handle der Logbuchdatei

// ******************************************************************
// Initialisieren der Logbuchdatei
Result InitLog()
{
	// Logbuchdatei öffnen
	g_pLogFile = fopen("Log.html", "wc");
	if(g_pLogFile == NULL) return AGE_ERROR;

	return AGE_OK;
}

// ******************************************************************
// Schließen der Logbuchdatei
Result ExitLog()
{
	// Datei schließen
	fflush(g_pLogFile);
	fclose(g_pLogFile);
	g_pLogFile = NULL;

	return AGE_OK;
}

// ******************************************************************
// Einen Text unverändert in die Logbuchdatei schreiben
Result WriteToLogDirect(char* pcFormat,
								        ...)
{
	char	acText[1024];	// Der zu schreibende String
	va_list	VAList;			// Variable-Argumente-Liste


	// Parameterliste anfertigen und den String erstellen
	va_start(VAList, pcFormat);
	vsprintf(acText, pcFormat, VAList);
	va_end(VAList);

	// String ins Logbuch schreiben
	if(g_pLogFile != NULL)
	{
		fprintf(g_pLogFile, "%s", acText);
#ifdef _DEBUG
		fflush(g_pLogFile);
#endif
	}

	return AGE_OK;
}

// ******************************************************************
// Entfernt HTML-Tags aus einem String
Result RemoveHTMLTags(char* pcIn,
						char* pcOut,
						DWORD dwOutLength)
{
	DWORD dwInLength = strlen(pcIn);
	DWORD dwOut = 0;


	ZeroMemory(pcOut, dwOutLength);
	for(DWORD dwIn = 0; dwIn < dwInLength; dwIn++)
	{
		if(pcIn[dwIn] != '<') {pcOut[dwOut] = pcIn[dwIn]; dwOut++;}
		else
		{
			while(pcIn[dwIn] != '>') dwIn++;
		}
	}

	return AGE_OK;
}

// ******************************************************************
// Einen Text an HTML anpassen und in die Logbuchdatei schreiben
Result WriteToLog(char* pcFormat,
								  ...)
{
	char	acText[1024];		// Temporärer String
	char	acNewText[2048];	// Der zu schreibende String
	va_list	VAList;				// Variable-Argumente-Liste
	int		iCursor;


	// Parameterliste anfertigen und den String erstellen
	va_start(VAList, pcFormat);
	vsprintf(acText, pcFormat, VAList);
	va_end(VAList);

	// Alle "\n"s durch "<br>" ersetzen
	iCursor = 0;
	ZeroMemory(acNewText, 2048 * sizeof(char));
	for(int iChar = 0; iChar < (int)(strlen(acText)); iChar++)
	{
		if(acText[iChar] == '\n')
		{
			acNewText[iCursor++] = '<';
			acNewText[iCursor++] = 'b';
			acNewText[iCursor++] = 'r';
			acNewText[iCursor++] = '>';
		}
		else
		{
			acNewText[iCursor] = acText[iChar];
			iCursor++;
		}
	}

	// String ins Logbuch schreiben
	if(g_pLogFile != NULL)
	{
		fprintf(g_pLogFile, "<tr><td><font size=\"2\" color=\"#000080\">%s</font></td></tr>", acNewText);
#ifdef _DEBUG
		fflush(g_pLogFile);
#endif
	}

	// Zusätzlich wird noch eine Debug-Ausgabe erzeugt.
	sprintf(acText, "%s\n", acText);
	RemoveHTMLTags(acText, acNewText, 256);
	OutputDebugString(acNewText);

	return AGE_OK;
}

// ******************************************************************
// Einen 2D-Vektor in die Logbuchdatei schreiben
Result WriteVector2ToLog(Vector2D& v)
{
	// 2D-Vektor in die Logbuchdatei schreiben
	return WriteToLogDirect("<tr><td><font size=\"2\"><b><font color=\"#000080\">2D-Vektor:</font></b> x = <i>%.3f</i>, y = <i>%.3f</i>, Länge = <i>%.3f</i></td></tr>",
		v.x, v.y, Vector2D::Length(v));
}

// ******************************************************************
// Einen 3D-Vektor in die Logbuchdatei schreiben
Result WriteVector3ToLog(Vector3D& v)
{
	// 3D-Vektor in die Logbuchdatei schreiben
	return WriteToLogDirect("<tr><td><font size=\"2\"><b><font color=\"#000080\">3D-Vektor:</font></b> x = <i>%.3f</i>, y = <i>%.3f</i>, z = <i>%.3f</i>, Länge = <i>%.3f</i></td></tr>",
		v.x, v.y, v.z, Vector3D::Length(v));
}

// ******************************************************************
// Eine Matrix in die Logbuchdatei schreiben
Result WriteMatrixToLog(Matrix4x4& m)
{
	// Matrix in die Logbuchdatei schreiben
	WriteToLogDirect("<tr><td><font size=\"2\"><b><font color=\"#000080\">Matrix:</font></b><table>");
	for(int iRow = 0; iRow < 4; iRow++)
	{
		WriteToLogDirect("<tr>");
		for(int iColumn = 0; iColumn < 4; iColumn++)
		{
			WriteToLogDirect("<td><font size=\"1\">");
			WriteToLogDirect("<i>%.3f</i>", m.m[iRow][iColumn]);
			WriteToLogDirect("</td>");
		}
		
		WriteToLogDirect("</tr>");
	}

	return WriteToLogDirect("</table>");
}

// ******************************************************************
// Eine Ebene in die Logbuchdatei schreiben
Result WritePlaneToLog(Plane& p)
{
	// Ebene in die Logbuchdatei schreiben
	return WriteToLogDirect("<tr><td><font size=\"2\"><b><font color=\"#000080\">Ebene:</font></b> a = <i>%.3f</i>, b = <i>%.3f</i>, c = <i>%.3f</i>, d = <i>%.3f</i>, Normalvektorlänge = <i>%.3f</i></td></tr>",
		p.a, p.b, p.c, p.d, Vector3D::Length(p.n));
}

// ******************************************************************
// Eine Farbe in die Logbuchdatei schreiben
Result WriteColorToLog(Color& c)
{
	char acHexColor[9];

	sprintf(acHexColor, "%x", (DWORD)(c) << 8);
	acHexColor[6] = 0;

	// Farbe in die Logbuchdatei schreiben
	return WriteToLogDirect("<tr><td><font size=\"2\"><b><font color=\"#000080\">Farbe:</font></b> a = <i>%.3f</i>, r = <i>%.3f</i>, g = <i>%.3f</i>, b = <i>%.3f</i>, Hexadezimal: <i>0x%x</i>, <font color=\"#%s\"><i>Probetext</i></font></td></tr>",
                              c.a, c.r, c.g, c.b, (DWORD)(c), acHexColor);
}

// ******************************************************************