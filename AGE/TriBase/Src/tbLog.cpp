/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbLog.cpp
	=========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Logbuchdateifunktionen

	Autor:
	David Scherfgen

********************************************************************/

#include <TriBase.h>

// ******************************************************************
// Globale Variablen
FILE* tb_g_pLogFile = NULL;	// Handle der Logbuchdatei

// ******************************************************************
// Initialisieren der Logbuchdatei
TRIBASE_API tbResult tbInitLog()
{
	// Logbuchdatei öffnen
	tb_g_pLogFile = fopen("Log.html", "wc");
	if(tb_g_pLogFile == NULL) return TB_ERROR;

	return TB_OK;
}

// ******************************************************************
// Schließen der Logbuchdatei
TRIBASE_API tbResult tbExitLog()
{
	// Datei schließen
	fflush(tb_g_pLogFile);
	fclose(tb_g_pLogFile);
	tb_g_pLogFile = NULL;

	return TB_OK;
}

// ******************************************************************
// Einen Text unverändert in die Logbuchdatei schreiben
TRIBASE_API tbResult tbWriteToLogDirect(char* pcFormat,
								        ...)
{
	char	acText[1024];	// Der zu schreibende String
	va_list	VAList;			// Variable-Argumente-Liste


	// Parameterliste anfertigen und den String erstellen
	va_start(VAList, pcFormat);
	vsprintf(acText, pcFormat, VAList);
	va_end(VAList);

	// String ins Logbuch schreiben
	if(tb_g_pLogFile != NULL)
	{
		fprintf(tb_g_pLogFile, "%s", acText);
#ifdef _DEBUG
		fflush(tb_g_pLogFile);
#endif
	}

	return TB_OK;
}

// ******************************************************************
// Entfernt HTML-Tags aus einem String
tbResult RemoveHTMLTags(char* pcIn,
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

	return TB_OK;
}

// ******************************************************************
// Einen Text an HTML anpassen und in die Logbuchdatei schreiben
TRIBASE_API tbResult tbWriteToLog(char* pcFormat,
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
	if(tb_g_pLogFile != NULL)
	{
		fprintf(tb_g_pLogFile, "<tr><td><font size=\"2\" color=\"#000080\">%s</font></td></tr>", acNewText);
#ifdef _DEBUG
		fflush(tb_g_pLogFile);
#endif
	}

	// Zusätzlich wird noch eine Debug-Ausgabe erzeugt.
	sprintf(acText, "%s\n", acText);
	RemoveHTMLTags(acText, acNewText, 256);
	OutputDebugString(acNewText);

	return TB_OK;
}

// ******************************************************************
// Einen 2D-Vektor in die Logbuchdatei schreiben
TRIBASE_API tbResult tbWriteVector2ToLog(tbVector2& v)
{
	// 2D-Vektor in die Logbuchdatei schreiben
	return tbWriteToLogDirect("<tr><td><font size=\"2\"><b><font color=\"#000080\">2D-Vektor:</font></b> x = <i>%.3f</i>, y = <i>%.3f</i>, Länge = <i>%.3f</i></td></tr>",
		                      v.x, v.y, tbVector2Length(v));
}

// ******************************************************************
// Einen 3D-Vektor in die Logbuchdatei schreiben
TRIBASE_API tbResult tbWriteVector3ToLog(tbVector3& v)
{
	// 3D-Vektor in die Logbuchdatei schreiben
	return tbWriteToLogDirect("<tr><td><font size=\"2\"><b><font color=\"#000080\">3D-Vektor:</font></b> x = <i>%.3f</i>, y = <i>%.3f</i>, z = <i>%.3f</i>, Länge = <i>%.3f</i></td></tr>",
		                      v.x, v.y, v.z, tbVector3Length(v));
}

// ******************************************************************
// Eine Matrix in die Logbuchdatei schreiben
TRIBASE_API tbResult tbWriteMatrixToLog(tbMatrix& m)
{
	// Matrix in die Logbuchdatei schreiben
	tbWriteToLogDirect("<tr><td><font size=\"2\"><b><font color=\"#000080\">Matrix:</font></b><table>");
	for(int iRow = 0; iRow < 4; iRow++)
	{
		tbWriteToLogDirect("<tr>");
		for(int iColumn = 0; iColumn < 4; iColumn++)
		{
			tbWriteToLogDirect("<td><font size=\"1\">");
			tbWriteToLogDirect("<i>%.3f</i>", m.m[iRow][iColumn]);
			tbWriteToLogDirect("</td>");
		}
		
		tbWriteToLogDirect("</tr>");
	}

	return tbWriteToLogDirect("</table>");
}

// ******************************************************************
// Eine Ebene in die Logbuchdatei schreiben
TRIBASE_API tbResult tbWritePlaneToLog(tbPlane& p)
{
	// Ebene in die Logbuchdatei schreiben
	return tbWriteToLogDirect("<tr><td><font size=\"2\"><b><font color=\"#000080\">Ebene:</font></b> a = <i>%.3f</i>, b = <i>%.3f</i>, c = <i>%.3f</i>, d = <i>%.3f</i>, Normalvektorlänge = <i>%.3f</i></td></tr>",
                              p.a, p.b, p.c, p.d, tbVector3Length(p.n));
}

// ******************************************************************
// Eine Farbe in die Logbuchdatei schreiben
TRIBASE_API tbResult tbWriteColorToLog(tbColor& c)
{
	char acHexColor[9];

	sprintf(acHexColor, "%x", (DWORD)(c) << 8);
	acHexColor[6] = 0;

	// Farbe in die Logbuchdatei schreiben
	return tbWriteToLogDirect("<tr><td><font size=\"2\"><b><font color=\"#000080\">Farbe:</font></b> a = <i>%.3f</i>, r = <i>%.3f</i>, g = <i>%.3f</i>, b = <i>%.3f</i>, Hexadezimal: <i>0x%x</i>, <font color=\"#%s\"><i>Probetext</i></font></td></tr>",
                              c.a, c.r, c.g, c.b, (DWORD)(c), acHexColor);
}

// ******************************************************************