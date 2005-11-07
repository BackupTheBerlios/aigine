/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbDirectInput.cpp
	=================
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Einfacher Umgang mit DirectInput

	Autor:
	David Scherfgen

********************************************************************/

#include <TriBase.h>

// ******************************************************************
BOOL				tbDirectInput::m_bInitialized = FALSE;
LPDIRECTINPUT8		tbDirectInput::m_pDirectInput = NULL;
DWORD				tbDirectInput::m_dwNumDevices = 0;
DWORD				tbDirectInput::m_dwNumButtons = 0;
tbInputDeviceInfo*	tbDirectInput::m_pDevices = NULL;
tbButtonInfo*		tbDirectInput::m_pButtons = NULL;
HWND				tbDirectInput::m_hWindow = NULL;
DWORD				tbDirectInput::m_dwCoopLevel = 0;

// ******************************************************************
// Globale Variablen
// Tastaturcode-Umwandlungstabelle
DWORD tb_g_adwKeyConv[108] = {DIK_ESCAPE,
							  DIK_F1,
							  DIK_F2,
							  DIK_F3,
							  DIK_F4,
							  DIK_F5,
							  DIK_F6,
							  DIK_F7,
							  DIK_F8,
							  DIK_F9,
							  DIK_F10,
							  DIK_F11,
							  DIK_F12,
							  DIK_F13,
							  DIK_F14,
							  DIK_F15,
							  DIK_SYSRQ,
							  DIK_SCROLL,
							  DIK_PAUSE,
							  DIK_1,
							  DIK_2,
							  DIK_3,
							  DIK_4,
							  DIK_5,
							  DIK_6,
							  DIK_7,
							  DIK_8,
							  DIK_9,
							  DIK_0,
							  DIK_MINUS,
							  DIK_EQUALS,
							  DIK_BACK,
							  DIK_INSERT,
							  DIK_HOME,
							  DIK_PRIOR,
							  DIK_NUMLOCK,
							  DIK_DIVIDE,
							  DIK_MULTIPLY,
							  DIK_SUBTRACT,
							  DIK_TAB,
							  DIK_A,
							  DIK_B,
							  DIK_C,
							  DIK_D,
							  DIK_E,
							  DIK_F,
							  DIK_G,
							  DIK_H,
							  DIK_I,
							  DIK_J,
							  DIK_K,
							  DIK_L,
							  DIK_M,
							  DIK_N,
							  DIK_O,
							  DIK_P,
							  DIK_Q,
							  DIK_R,
							  DIK_S,
							  DIK_T,
							  DIK_U,
							  DIK_V,
							  DIK_W,
							  DIK_X,
							  DIK_Z,
							  DIK_Y,
							  DIK_LBRACKET,
							  DIK_RBRACKET,
							  DIK_RETURN,
							  DIK_DELETE,
							  DIK_END,
							  DIK_NEXT,
							  DIK_NUMPAD0,
							  DIK_NUMPAD1,
							  DIK_NUMPAD2,
							  DIK_NUMPAD3,
							  DIK_NUMPAD4,
							  DIK_NUMPAD5,
							  DIK_NUMPAD6,
							  DIK_NUMPAD7,
							  DIK_NUMPAD8,
							  DIK_NUMPAD9,
							  DIK_ADD,
							  DIK_CAPITAL,
							  DIK_SEMICOLON,
							  DIK_APOSTROPHE,
							  DIK_BACKSLASH,
							  DIK_LSHIFT,
							  DIK_OEM_102,
							  DIK_COMMA,
							  DIK_PERIOD,
							  DIK_SLASH,
							  DIK_RSHIFT,
							  DIK_UP,
							  DIK_NUMPADENTER,
							  DIK_LCONTROL,
							  DIK_LWIN,
							  DIK_LALT,
							  DIK_SPACE,
							  DIK_RALT,
							  DIK_RWIN,
							  DIK_APPS,
							  DIK_RCONTROL,
							  DIK_LEFT,
							  DIK_DOWN,
							  DIK_RIGHT,
							  DIK_DECIMAL,
                              DIK_GRAVE};

// Tabelle zur Umwandlung einer Taste in ein druckbares Zeichen
//                                Normal  Shift  Alt Gr
char tb_g_acKeyToChar[108 * 3] = {0,      0,     0,		// TB_KEY_ESCAPE
                                  0,      0,     0,		// TB_KEY_F1
								  0,      0,     0,		// TB_KEY_F2
								  0,      0,     0,		// TB_KEY_F3
								  0,      0,     0,		// TB_KEY_F4
								  0,      0,     0,		// TB_KEY_F5
								  0,      0,     0,		// TB_KEY_F6
								  0,      0,     0,		// TB_KEY_F7
								  0,      0,     0,		// TB_KEY_F8
								  0,      0,     0,		// TB_KEY_F9
								  0,      0,     0,		// TB_KEY_F10
								  0,      0,     0,		// TB_KEY_F11
								  0,      0,     0,		// TB_KEY_F12
								  0,      0,     0,		// TB_KEY_F13
								  0,      0,     0,		// TB_KEY_F14
								  0,      0,     0,		// TB_KEY_F15
								  0,      0,     0,		// TB_KEY_SYSRQ
								  0,      0,     0,		// TB_KEY_SCROLL
								  0,      0,     0,		// TB_KEY_PAUSE
								  '1',    '!',   0,		// TB_KEY_1
								  '2',    '"',   '²',	// TB_KEY_2
								  '3',    '§',   '³',	// TB_KEY_3
								  '4',    '$',   0,		// TB_KEY_4
								  '5',    '%',   0,		// TB_KEY_5
								  '6',    '&',   0,		// TB_KEY_6
								  '7',    '/',   '{',	// TB_KEY_7
								  '8',    '(',   '[',	// TB_KEY_8
								  '9',    ')',   ']',	// TB_KEY_9
								  '0',    '=',   '}',	// TB_KEY_0
								  'ß',    '?',   '\\',	// TB_KEY_MINUS
								  '´',    '`',   0,		// TB_KEY_EQUALS
								  0,      0,     0,		// TB_KEY_BACK
								  0,      0,     0,		// TB_KEY_INSERT
								  0,      0,     0,		// TB_KEY_HOME
								  0,      0,     0,		// TB_KEY_PRIOR
								  0,      0,     0,		// TB_KEY_NUMLOCK
								  '/',    '/',   0,		// TB_KEY_DIVIDE
								  '*',    '*',   0,		// TB_KEY_MULTIPLY
								  '-',    '-',   0,		// TB_KEY_SUBTRACT
								  0,      0,     0,		// TB_KEY_TAB
								  'a',    'A',   0,		// TB_KEY_A
								  'b',    'B',   0,		// TB_KEY_B
								  'c',    'C',   0,		// TB_KEY_C
								  'd',    'D',   0,		// TB_KEY_D
								  'e',    'E',   '€',	// TB_KEY_E
								  'f',    'F',   0,		// TB_KEY_F
								  'g',    'G',   0,		// TB_KEY_G
								  'h',    'H',   0,		// TB_KEY_H
								  'i',    'I',   0,		// TB_KEY_I
								  'j',    'J',   0,		// TB_KEY_J
								  'k',    'K',   0,		// TB_KEY_K
								  'l',    'L',   0,		// TB_KEY_L
								  'm',    'M',   'µ',	// TB_KEY_M
								  'n',    'N',   0,		// TB_KEY_N
								  'o',    'O',   0,		// TB_KEY_O
								  'p',    'P',   0,		// TB_KEY_P
								  'q',    'Q',   '@',	// TB_KEY_Q
								  'r',    'R',   0,		// TB_KEY_R
								  's',    'S',   0,		// TB_KEY_S
								  't',    'T',   0,		// TB_KEY_T
								  'u',    'U',   0,		// TB_KEY_U
								  'v',    'V',   0,		// TB_KEY_V
								  'w',    'W',   0,		// TB_KEY_W
								  'x',    'X',   0,		// TB_KEY_X
								  'y',    'Y',   0,		// TB_KEY_Y
								  'z',    'Z',   0,		// TB_KEY_Z	
								  'ü',    'Ü',   0,		// TB_KEY_LBRACKET
								  '+',    '*',   '~',	// TB_KEY_RBRACKET
								  0,      0,     0,		// TB_KEY_RETURN
								  0,      0,     0,		// TB_KEY_DELETE
								  0,      0,     0,		// TB_KEY_END
								  0,      0,     0,		// TB_KEY_NEXT
								  0,      0,     0,		// TB_KEY_NUMPAD0
								  0,      0,     0,		// TB_KEY_NUMPAD1
								  0,      0,     0,		// TB_KEY_NUMPAD2
								  0,      0,     0,		// TB_KEY_NUMPAD3
								  0,      0,     0,		// TB_KEY_NUMPAD4
								  0,      0,     0,		// TB_KEY_NUMPAD5
								  0,      0,     0,		// TB_KEY_NUMPAD6
								  0,      0,     0,		// TB_KEY_NUMPAD7
								  0,      0,     0,		// TB_KEY_NUMPAD8
								  0,      0,     0,		// TB_KEY_NUMPAD9
								  '+',    '+',   0,		// TB_KEY_ADD
								  0,      0,     0,		// TB_KEY_CAPITAL
								  'ö',    'Ö',   0,		// TB_KEY_SEMICOLON
								  'ä',    'Ä',   0,		// TB_KEY_APOSTROPHE
								  '#',    '\'',  0,		// TB_KEY_BACKSLASH
								  0,      0,     0,		// TB_KEY_LSHIFT
								  '<',    '>',   '|',	// TB_KEY_OEM_102
								  ',',    ';',   0,		// TB_KEY_COMMA
								  '.',    ':',   0,		// TB_KEY_PERIOD
								  '-',    '_',   0,		// TB_KEY_SLASH
								  0,      0,     0,		// TB_KEY_RSHIFT
								  0,      0,     0,		// TB_KEY_UP
								  0,      0,     0,		// TB_KEY_NUMPADENTER
								  0,      0,     0,		// TB_KEY_LCONTROL
								  0,      0,     0,		// TB_KEY_LWIN
								  0,      0,     0,		// TB_KEY_LALT
								  ' ',    ' ',   0,		// TB_KEY_SPACE
								  0,      0,     0,		// TB_KEY_RALT
								  0,      0,     0,		// TB_KEY_RWIN
								  0,      0,     0,		// TB_KEY_APPS
								  0,      0,     0,		// TB_KEY_RCONTROL
								  0,      0,     0,		// TB_KEY_LEFT
								  0,      0,     0,		// TB_KEY_DOWN
								  0,      0,     0,		// TB_KEY_RIGHT
								  0,      0,     0,		// TB_KEY_DECIMAL
								  '^',    '°',   0};	// TB_KEY_CIRCONFLEX

// ******************************************************************
// Funktion zum Initialisieren der Tastatur
tbResult InitKeyboard()
{
	HRESULT					hResult;
	LPDIRECTINPUTDEVICE8	pDevice;


	// Geräteschnittstelle erstellen
	if(FAILED(hResult = tbDirectInput::m_pDirectInput->CreateDevice(GUID_SysKeyboard,
																	&pDevice,
																	NULL)))
	{
		// Fehler!
		TB_ERROR_DIRECTX("tbDirectInput::m_pDirectInput->CreateDevice", hResult, TB_ERROR);
	}

	// Datenformat setzen
	pDevice->SetDataFormat(&c_dfDIKeyboard);

	// Kooperationsebene setzen
	if(FAILED(hResult = pDevice->SetCooperativeLevel(tbDirectInput::m_hWindow,
											         tbDirectInput::m_dwCoopLevel)))
	{
		// Fehler!
		TB_ERROR_DIRECTX("pDevice->SetCooperativeLevel", hResult, TB_ERROR);
	}

	// Zugriff aktivieren
	pDevice->Acquire();

	// Name, Schnittstelle, GUID und sonstige Informationen eintragen
	strcpy(tbDirectInput::m_pDevices[0].acName, "Tastatur");
	tbDirectInput::m_pDevices[0].pDevice = pDevice;
	tbDirectInput::m_pDevices[0].DeviceGUID = GUID_SysKeyboard;
	tbDirectInput::m_pDevices[0].dwFirstButton = 0;
	tbDirectInput::m_pDevices[0].dwNumButtons = 108;

	// Die 108 Knöpfe in die Liste eintragen (Namen)
	strcpy(tbDirectInput::m_pButtons[TB_KEY_ESCAPE].acName, "Escape");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_F1].acName, "F1");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_F2].acName, "F2");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_F3].acName, "F3");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_F4].acName, "F4");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_F5].acName, "F5");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_F6].acName, "F6");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_F7].acName, "F7");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_F8].acName, "F8");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_F9].acName, "F9");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_F10].acName, "F10");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_F11].acName, "F11");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_F12].acName, "F12");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_F13].acName, "F13");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_F14].acName, "F14");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_F15].acName, "F15");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_SYSRQ].acName, "SysRq");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_SCROLL].acName, "Scroll Lock");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_PAUSE].acName, "Break");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_1].acName, "1");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_2].acName, "2");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_3].acName, "3");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_4].acName, "4");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_5].acName, "5");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_6].acName, "6");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_7].acName, "7");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_8].acName, "8");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_9].acName, "9");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_0].acName, "0");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_MINUS].acName, "ß");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_EQUALS].acName, "´ (Accent)");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_BACK].acName, "Rücktaste");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_INSERT].acName, "Einfg");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_HOME].acName, "Pos1");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_PRIOR].acName, "Bild hoch");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_NUMLOCK].acName, "Num Lock");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_DIVIDE].acName, "/ (NumPad)");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_MULTIPLY].acName, "* (NumPad)");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_SUBTRACT].acName, "- (NumPad)");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_TAB].acName, "Tab");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_A].acName, "A");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_B].acName, "B");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_C].acName, "C");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_D].acName, "D");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_E].acName, "E");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_F].acName, "F");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_G].acName, "G");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_H].acName, "H");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_I].acName, "I");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_J].acName, "J");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_K].acName, "K");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_L].acName, "L");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_M].acName, "M");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_N].acName, "N");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_O].acName, "O");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_P].acName, "P");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_Q].acName, "Q");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_R].acName, "R");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_S].acName, "S");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_T].acName, "T");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_U].acName, "U");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_V].acName, "V");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_W].acName, "W");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_X].acName, "X");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_Y].acName, "Y");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_Z].acName, "Z");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_LBRACKET].acName, "Ü");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_RBRACKET].acName, "+");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_RETURN].acName, "Return");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_DELETE].acName, "Entf");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_END].acName, "Ende");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_NEXT].acName, "Bild runter");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_NUMPAD0].acName, "0 (NumPad)");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_NUMPAD1].acName, "1 (NumPad)");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_NUMPAD2].acName, "2 (NumPad)");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_NUMPAD3].acName, "3 (NumPad)");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_NUMPAD4].acName, "4 (NumPad)");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_NUMPAD5].acName, "5 (NumPad)");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_NUMPAD6].acName, "6 (NumPad)");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_NUMPAD7].acName, "7 (NumPad)");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_NUMPAD8].acName, "8 (NumPad)");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_NUMPAD9].acName, "9 (NumPad)");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_ADD].acName, "+ (NumPad)");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_CAPITAL].acName, "Caps Lock");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_SEMICOLON].acName, "Ö");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_APOSTROPHE].acName, "Ä");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_BACKSLASH].acName, "#");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_LSHIFT].acName, "Shift links");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_OEM_102].acName, "<");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_COMMA].acName, ", (Komma)");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_PERIOD].acName, ". (Punkt)");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_SLASH].acName, "- (Strich)");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_RSHIFT].acName, "Shift rechts");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_UP].acName, "Pfeil hoch");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_NUMPADENTER].acName, "Enter (NumPad)");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_LCONTROL].acName, "Strg links");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_LWIN].acName, "Win links");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_LALT].acName, "Alt links");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_SPACE].acName, "Leertaste");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_RALT].acName, "Alt Gr");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_RWIN].acName, "Win rechts");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_APPS].acName, "Kontextmenü");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_RCONTROL].acName, "Strg rechts");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_LEFT].acName, "Pfeil links");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_DOWN].acName, "Pfeil runter");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_RIGHT].acName, "Pfeil rechts");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_DECIMAL].acName, ", (Komma, NumPad)");
	strcpy(tbDirectInput::m_pButtons[TB_KEY_CIRCONFLEX].acName, "^ (Circonflex)");

	// Geräte-IDs und Skalierung für die Knöpfe eintragen
	for(DWORD dwButton = 0; dwButton < 108; dwButton++)
	{
		tbDirectInput::m_pButtons[dwButton].dwDevice = 0;
		tbDirectInput::m_pButtons[dwButton].fScaling = 1.0f;
	}

	return TB_OK;
}

// ******************************************************************
// Funktion zum Initialisieren der Maus
tbResult InitMouse()
{
	HRESULT					hResult;
	LPDIRECTINPUTDEVICE8	pDevice;


	// Geräteschnittstelle erstellen
	if(FAILED(hResult = tbDirectInput::m_pDirectInput->CreateDevice(GUID_SysMouse,
																		&pDevice,
																		NULL)))
	{
		// Fehler!
		TB_ERROR_DIRECTX("tbDirectInput::m_pDirectInput->CreateDevice", hResult, TB_ERROR);
	}

	// Datenformat setzen
	pDevice->SetDataFormat(&c_dfDIMouse2);

	// Kooperationsebene setzen
	if(FAILED(hResult = pDevice->SetCooperativeLevel(tbDirectInput::m_hWindow,
											         tbDirectInput::m_dwCoopLevel)))
	{
		// Fehler!
		TB_ERROR_DIRECTX("pDevice->SetCooperativeLevel", hResult, TB_ERROR);
	}

	// Zugriff aktivieren
	pDevice->Acquire();

	// Name, Schnittstelle, GUID und sonstige Informationen eintragen
	strcpy(tbDirectInput::m_pDevices[1].acName, "Maus");
	tbDirectInput::m_pDevices[1].pDevice = pDevice;
	tbDirectInput::m_pDevices[1].DeviceGUID = GUID_SysMouse;
	tbDirectInput::m_pDevices[1].dwFirstButton = 108;
	tbDirectInput::m_pDevices[1].dwNumButtons = 14;

	// Die 14 Knöpfe erstellen
	strcpy(tbDirectInput::m_pButtons[TB_MOUSE_X_NEG].acName, "x-Achse negativ");
	strcpy(tbDirectInput::m_pButtons[TB_MOUSE_X_POS].acName, "x-Achse positiv");
	strcpy(tbDirectInput::m_pButtons[TB_MOUSE_Y_NEG].acName, "y-Achse negativ");
	strcpy(tbDirectInput::m_pButtons[TB_MOUSE_Y_POS].acName, "y-Achse positiv");
	strcpy(tbDirectInput::m_pButtons[TB_MOUSE_Z_NEG].acName, "z-Achse negativ");
	strcpy(tbDirectInput::m_pButtons[TB_MOUSE_Z_POS].acName, "z-Achse positiv");
	strcpy(tbDirectInput::m_pButtons[TB_MOUSE_BUTTON(0)].acName, "Knopf 1");
	strcpy(tbDirectInput::m_pButtons[TB_MOUSE_BUTTON(1)].acName, "Knopf 2");
	strcpy(tbDirectInput::m_pButtons[TB_MOUSE_BUTTON(2)].acName, "Knopf 3");
	strcpy(tbDirectInput::m_pButtons[TB_MOUSE_BUTTON(3)].acName, "Knopf 4");
	strcpy(tbDirectInput::m_pButtons[TB_MOUSE_BUTTON(4)].acName, "Knopf 5");
	strcpy(tbDirectInput::m_pButtons[TB_MOUSE_BUTTON(5)].acName, "Knopf 6");
	strcpy(tbDirectInput::m_pButtons[TB_MOUSE_BUTTON(6)].acName, "Knopf 7");
	strcpy(tbDirectInput::m_pButtons[TB_MOUSE_BUTTON(7)].acName, "Knopf 8");
	for(int iButton = 0; iButton < 14; iButton++)
	{
		tbDirectInput::m_pButtons[108 + iButton].dwDevice = 1;
		tbDirectInput::m_pButtons[108 + iButton].fScaling = 1.0f;
	}

	return TB_OK;
}

// ******************************************************************
// Funktion zum Initialisieren eines Joysticks
tbResult InitJoystick(LPCDIDEVICEINSTANCE pDeviceInstance,
					  DWORD dwJoystick)
{
	HRESULT					hResult;
	LPDIRECTINPUTDEVICE8	pDevice;
	DIPROPRANGE				Range;		// Struktur für Achsenskalierung
	DIPROPDWORD				DeadZone;	// Struktur für die tote Zone
	DIPROPDWORD				Saturation;	// Struktur für die Sättigung


	// Die Geräteliste und die Knopfliste erweitern
	tbDirectInput::m_dwNumDevices++;
	tbDirectInput::m_dwNumButtons += 66;
	tbDirectInput::m_pDevices = (tbInputDeviceInfo*)(tbMemReAlloc(tbDirectInput::m_pDevices, -((int)(sizeof(tbInputDeviceInfo)))));
	if(tbDirectInput::m_pDevices == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);
	tbDirectInput::m_pButtons = (tbButtonInfo*)(tbMemReAlloc(tbDirectInput::m_pButtons, -66 * ((int)(sizeof(tbButtonInfo)))));
	if(tbDirectInput::m_pButtons == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

	// Geräteschnittstelle erstellen
	if(FAILED(hResult = tbDirectInput::m_pDirectInput->CreateDevice(pDeviceInstance->guidInstance,
																		&pDevice,
																		NULL)))
	{
		// Fehler!
		TB_ERROR_DIRECTX("tbDirectInput::m_pDirectInput->CreateDevice", hResult, TB_ERROR);
	}

	// Datenformat setzen
	pDevice->SetDataFormat(&c_dfDIJoystick);

	// Kooperationsebene setzen
	if(FAILED(hResult = pDevice->SetCooperativeLevel(tbDirectInput::m_hWindow,
											         tbDirectInput::m_dwCoopLevel)))
	{
		// Fehler!
		TB_ERROR_DIRECTX("pDevice->SetCooperativeLevel", hResult, TB_ERROR);
	}

	// Zugriff aktivieren
	pDevice->Acquire();

	// Hier wird der Joystick kalibriert.
	// Als erstes wird die Achsenskalierung eingerichtet.
	Range.diph.dwHeaderSize	= sizeof(DIPROPHEADER);
	Range.diph.dwSize		= sizeof(DIPROPRANGE);
	Range.diph.dwObj		= 0;
	Range.diph.dwHow		= DIPH_DEVICE;
	Range.lMin				= -1000;
	Range.lMax				= +1000;
	if(FAILED(hResult = pDevice->SetProperty(DIPROP_RANGE,
		                                     &Range.diph)))
	{
		// Fehler!
		TB_ERROR_DIRECTX("pDevice->SetProperty", hResult, TB_ERROR);
	}

	// Nun die tote Zone
	DeadZone.diph.dwHeaderSize	= sizeof(DIPROPHEADER);
	DeadZone.diph.dwSize		= sizeof(DIPROPDWORD);
	DeadZone.diph.dwObj			= 0;
	DeadZone.diph.dwHow			= DIPH_DEVICE;
	DeadZone.dwData				= 1000;
	if(FAILED(hResult = pDevice->SetProperty(DIPROP_DEADZONE,
		                                     &DeadZone.diph)))
	{
		// Fehler!
		TB_ERROR_DIRECTX("pDevice->SetProperty", hResult, TB_ERROR);
	}

	// Sättigung
	Saturation.diph.dwHeaderSize	= sizeof(DIPROPHEADER);
	Saturation.diph.dwSize			= sizeof(DIPROPDWORD);
	Saturation.diph.dwObj			= 0;
	Saturation.diph.dwHow			= DIPH_DEVICE;
	Saturation.dwData				= 9000;
	if(FAILED(hResult = pDevice->SetProperty(DIPROP_SATURATION,
		                                     &Saturation.diph)))
	{
		// Fehler!
		TB_ERROR_DIRECTX("pDevice->SetProperty", hResult, TB_ERROR);
	}

	// Name, Schnittstelle, GUID und sonstige Informationen eintragen
	strcpy(tbDirectInput::m_pDevices[2 + dwJoystick].acName, pDeviceInstance->tszInstanceName);
	tbDirectInput::m_pDevices[2 + dwJoystick].pDevice = pDevice;
	tbDirectInput::m_pDevices[2 + dwJoystick].DeviceGUID = pDeviceInstance->guidInstance;
	tbDirectInput::m_pDevices[2 + dwJoystick].dwFirstButton = 122 + (dwJoystick * 66);
	tbDirectInput::m_pDevices[2 + dwJoystick].dwNumButtons = 66;

	// Die 66 Knöpfe erstellen (Namen)
	strcpy(tbDirectInput::m_pButtons[TB_JOY_X_NEG(dwJoystick)].acName, "x-Achse negativ");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_X_POS(dwJoystick)].acName, "x-Achse positiv");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_Y_NEG(dwJoystick)].acName, "y-Achse negativ");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_Y_POS(dwJoystick)].acName, "y-Achse positiv");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_Z_NEG(dwJoystick)].acName, "z-Achse negativ");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_Z_POS(dwJoystick)].acName, "z-Achse positiv");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_RX_NEG(dwJoystick)].acName, "Rx-Achse negativ");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_RX_POS(dwJoystick)].acName, "Rx-Achse positiv");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_RY_NEG(dwJoystick)].acName, "Ry-Achse negativ");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_RY_POS(dwJoystick)].acName, "Ry-Achse positiv");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_RZ_NEG(dwJoystick)].acName, "Rz-Achse negativ");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_RZ_POS(dwJoystick)].acName, "Rz-Achse positiv");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_BUTTON(dwJoystick, 0)].acName, "Knopf 1");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_BUTTON(dwJoystick, 1)].acName, "Knopf 2");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_BUTTON(dwJoystick, 2)].acName, "Knopf 3");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_BUTTON(dwJoystick, 3)].acName, "Knopf 4");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_BUTTON(dwJoystick, 4)].acName, "Knopf 5");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_BUTTON(dwJoystick, 5)].acName, "Knopf 6");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_BUTTON(dwJoystick, 6)].acName, "Knopf 7");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_BUTTON(dwJoystick, 7)].acName, "Knopf 8");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_BUTTON(dwJoystick, 8)].acName, "Knopf 9");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_BUTTON(dwJoystick, 9)].acName, "Knopf 10");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_BUTTON(dwJoystick, 10)].acName, "Knopf 11");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_BUTTON(dwJoystick, 11)].acName, "Knopf 12");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_BUTTON(dwJoystick, 12)].acName, "Knopf 13");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_BUTTON(dwJoystick, 13)].acName, "Knopf 14");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_BUTTON(dwJoystick, 14)].acName, "Knopf 15");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_BUTTON(dwJoystick, 15)].acName, "Knopf 16");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_BUTTON(dwJoystick, 16)].acName, "Knopf 17");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_BUTTON(dwJoystick, 17)].acName, "Knopf 18");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_BUTTON(dwJoystick, 18)].acName, "Knopf 19");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_BUTTON(dwJoystick, 19)].acName, "Knopf 20");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_BUTTON(dwJoystick, 20)].acName, "Knopf 21");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_BUTTON(dwJoystick, 21)].acName, "Knopf 22");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_BUTTON(dwJoystick, 22)].acName, "Knopf 23");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_BUTTON(dwJoystick, 23)].acName, "Knopf 24");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_BUTTON(dwJoystick, 24)].acName, "Knopf 25");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_BUTTON(dwJoystick, 25)].acName, "Knopf 26");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_BUTTON(dwJoystick, 26)].acName, "Knopf 27");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_BUTTON(dwJoystick, 27)].acName, "Knopf 28");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_BUTTON(dwJoystick, 28)].acName, "Knopf 29");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_BUTTON(dwJoystick, 29)].acName, "Knopf 30");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_BUTTON(dwJoystick, 30)].acName, "Knopf 31");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_BUTTON(dwJoystick, 31)].acName, "Knopf 32");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_SLIDER_NEG(dwJoystick, 0)].acName, "Slider 1 negativ");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_SLIDER_POS(dwJoystick, 0)].acName, "Slider 1 positiv");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_SLIDER_NEG(dwJoystick, 1)].acName, "Slider 2 negativ");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_SLIDER_POS(dwJoystick, 1)].acName, "Slider 2 positiv");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_SLIDER_ABS(dwJoystick, 0)].acName, "Slider 1 absolut");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_SLIDER_ABS(dwJoystick, 1)].acName, "Slider 2 absolut");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_POV_UP(dwJoystick, 0)].acName, "POV 1 hoch");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_POV_RIGHT(dwJoystick, 0)].acName, "POV 1 rechts");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_POV_DOWN(dwJoystick, 0)].acName, "POV 1 runter");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_POV_LEFT(dwJoystick, 0)].acName, "POV 1 links");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_POV_UP(dwJoystick, 1)].acName, "POV 2 hoch");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_POV_RIGHT(dwJoystick, 1)].acName, "POV 2 rechts");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_POV_DOWN(dwJoystick, 1)].acName, "POV 2 runter");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_POV_LEFT(dwJoystick, 1)].acName, "POV 2 links");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_POV_UP(dwJoystick, 2)].acName, "POV 3 hoch");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_POV_RIGHT(dwJoystick, 2)].acName, "POV 3 rechts");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_POV_DOWN(dwJoystick, 2)].acName, "POV 3 runter");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_POV_LEFT(dwJoystick, 2)].acName, "POV 3 links");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_POV_UP(dwJoystick, 3)].acName, "POV 4 hoch");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_POV_RIGHT(dwJoystick, 3)].acName, "POV 4 rechts");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_POV_DOWN(dwJoystick, 3)].acName, "POV 4 runter");
	strcpy(tbDirectInput::m_pButtons[TB_JOY_POV_LEFT(dwJoystick, 3)].acName, "POV 4 links");

	// Geräte-IDs und Skalierung für die Knöpfe eintragen
	for(int iButton = 0; iButton < 66; iButton++)
	{
		tbDirectInput::m_pButtons[122 + (dwJoystick * 66) + iButton].dwDevice = 2 + dwJoystick;
		tbDirectInput::m_pButtons[122 + (dwJoystick * 66) + iButton].fScaling = 1.0f;
	}

	// Alte Position der Schieberegler auf -10000 setzen.
	// Dadurch erkennt die Abfragefunktion, dass sie den Joystick zum ersten Mal
	// abfragt und sich daher nicht auf die alten Schiebereglerpositionen verlassen
	// darf.
	tbDirectInput::m_pDevices[2 + dwJoystick].alOldSliderPos[0] = -10000;
	tbDirectInput::m_pDevices[2 + dwJoystick].alOldSliderPos[1] = -10000;

	return TB_OK;
}

// ******************************************************************
// Rückruffunktion für die Eingabegeräte
BOOL CALLBACK EnumDevicesCallback(LPCDIDEVICEINSTANCE pDeviceInstance,
								  LPVOID pRef)
{
	// Prüfen, ob es sich um die Tastatur, die Maus oder einen Joystick handelt
	if(pDeviceInstance->guidInstance == GUID_SysKeyboard)
	{
		// Es ist die Tastatur!
		if(InitKeyboard()) {TB_WARNING("Fehler beim Initialisieren der Tastatur!");}
		else TB_INFO("Initialisierung der Tastatur komplett!");
	}
	else if(pDeviceInstance->guidInstance == GUID_SysMouse)
	{
		// Es ist die Maus!
		if(InitMouse()) {TB_WARNING("Fehler beim Initialisieren der Maus!");}
		else TB_INFO("Initialisierung der Maus komplett!");
	}
	else
	{
		// Es ist ein Joystick!
		if(InitJoystick(pDeviceInstance, *((DWORD*)(pRef)))) {TB_WARNING("Fehler beim Initialisieren des Joysticks!");}
		else
		{
			TB_INFO("Initialisierung des Joysticks komplett!");
			
			// Anzahl der Joysticks im Referenzparameter erhöhen
			(*((DWORD*)(pRef)))++;
		}
	}

	// Der Nächste bitte!
	return DIENUM_CONTINUE;
}

// ******************************************************************
// Initialisierung von DirectInput
tbResult tbDirectInput::Init(HWND hWindow,		// = NULL
							 DWORD dwCoopLevel)	// = DISCL_FOREGROUND | DISCL_NONEXCLUSIVE
{
	HRESULT	hResult;
	DWORD	dwNumJoysticks;


	TB_INFO("Die DirectInput-Komponente wird initialisiert...");

	// Direct3D-Fenster eintragen, falls NULL angegeben wurde
	if(hWindow == NULL)
	{
		if(!tbDirect3D::IsInitialized())
		{
			// Kein Fenster!
			TB_ERROR("Kein Fenster angegeben und Direct3D wurde noch nicht initialisiert!", TB_ERROR);
		}
		else hWindow = tbDirect3D::GetWindow();
	}

	// Angegebenes Fenster und die Kooperationsebene eintragen
	m_hWindow = hWindow;
	m_dwCoopLevel = dwCoopLevel;

	// IDirectInput8-Schnittstelle erstellen
	TB_INFO("DirectInput-Schnittstelle wird generiert...");
	if(FAILED(hResult = DirectInput8Create(TB_DLL_HANDLE,
		                                   0x0800,
										   IID_IDirectInput8,
										   (void**)(&m_pDirectInput),
										   NULL)))
	{
		// Fehler!
		TB_ERROR_DIRECTX("DirectInput8Create", hResult, TB_ERROR);
	}

	// Die Geräteliste initialisieren: zwei Einträge im Voraus erstellen
	m_dwNumDevices = 2;
	m_pDevices = (tbInputDeviceInfo*)(tbMemAlloc(2 * sizeof(tbInputDeviceInfo)));
	if(m_pDevices == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

	// 108 Knöpfe für die Tastatur und 14 für die Maus erstellen
	m_dwNumButtons = 122;
	m_pButtons = (tbButtonInfo*)(tbMemAlloc(122 * sizeof(tbButtonInfo)));
	if(m_pButtons == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

	// Alle Geräte aufzählen. Der Rückruffunktion wird ein Zeiger auf die
	// DWORD-Variable dwNumJoysticks übergeben. Diese wird dann bei jedem gefundenen
	// Joystick um eins erhöht - so weiß die Rückruffunktion immer, der wievielte
	// Joystick es gerade ist.
	dwNumJoysticks = 0;
	if(FAILED(hResult = m_pDirectInput->EnumDevices(DI8DEVCLASS_ALL, EnumDevicesCallback,
		                                            &dwNumJoysticks, DIEDFL_ALLDEVICES)))
	{
		// Fehler!
		TB_ERROR_DIRECTX("m_pDirectInput->EnumDevices", hResult, TB_ERROR);
	}

	// Damit sind wir fertig!
	TB_INFO("Die DirectInput-Komponente wurde erfolgreich initialisiert!");

	m_bInitialized = TRUE;

	return TB_OK;
}

// ******************************************************************
// Herunterfahren der tbDirectInput-Klasse
tbResult tbDirectInput::Exit()
{
	m_bInitialized = FALSE;

	// IDirectInput8-Schnittstelle freigeben
	TB_SAFE_RELEASE(m_pDirectInput);

	// Alle Geräteschnittstellen freigeben
	for(DWORD dwDevice = 0; dwDevice < m_dwNumDevices; dwDevice++)
	{
		if(m_pDevices[dwDevice].pDevice != NULL)
		{
			m_pDevices[dwDevice].pDevice->Unacquire();
			TB_SAFE_RELEASE(m_pDevices[dwDevice].pDevice);
		}
	}

	// Listen löschen
	TB_SAFE_MEMFREE(m_pDevices);
	TB_SAFE_MEMFREE(m_pButtons);

	TB_INFO("Die DirectInput-Komponente wurde heruntergefahren.");

	return TB_OK;
}

// ******************************************************************
// Fragt die Tastatur ab
tbResult GetKeyboardState(LPDIRECTINPUTDEVICE8 pDevice,
						  float* pfOut)
{
	HRESULT hResult;
	BYTE	aKey[256];


	// Die Tastatur abfragen
	pDevice->Poll();
	if(FAILED(pDevice->GetDeviceState(256, aKey)))
	{
		// Versuchen, den Zugriff auf das Gerät wieder zu aktivieren
		if(FAILED(pDevice->Acquire()))
		{
			// Es geht nicht! Die Anwendung ist wahrscheinlich inaktiv.
			TB_ERROR("Fehler beim Zugriff auf die Tastatur!", TB_ERROR);
		}
		else
		{
			// Noch einmal versuchen, die Daten abzufragen
			if(FAILED(hResult = pDevice->GetDeviceState(256, aKey)))
			{
				// Fehler!
				TB_ERROR_DIRECTX("pDevice->GetDeviceState", hResult, TB_ERROR);
			}
		}
	}

	// Die Daten der 108 analogen Knöpfe generieren
	for(DWORD dwButton = 0; dwButton < 108; dwButton++)
	{
		// Gedrückt: Wert = 1.0; Nicht gedrückt: Wert = 0.0
		pfOut[dwButton] = (aKey[tb_g_adwKeyConv[dwButton]] & 0x80) ? 1.0f : 0.0f;
	}

	return TB_OK;
}

// ******************************************************************
// Abfragen der Maus
tbResult GetMouseState(LPDIRECTINPUTDEVICE8 pDevice,
					   float* pfOut)
{
	HRESULT			hResult;
	DIMOUSESTATE2	MouseState;


	// Die Maus abfragen
	pDevice->Poll();
	if(FAILED(pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &MouseState)))
	{
		// Versuchen, den Zugriff auf das Gerät wieder zu aktivieren
		if(FAILED(pDevice->Acquire()))
		{
			// Es geht nicht! Die Anwendung ist wahrscheinlich inaktiv.
			TB_ERROR("Fehler beim Zugriff auf die Maus!", TB_ERROR);
		}
		else
		{
			// Noch einmal versuchen, die Daten abzufragen
			if(FAILED(hResult = pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &MouseState)))
			{
				// Fehler!
				TB_ERROR_DIRECTX("pDevice->GetDeviceState", hResult, TB_ERROR);
			}
		}
	}

	// Die drei Achsen (x, y, z) verarbeiten
	pfOut[TB_MOUSE_X_NEG] = (float)(MouseState.lX) / -250.0f;
	pfOut[TB_MOUSE_X_POS] = -pfOut[TB_MOUSE_X_NEG];
	pfOut[TB_MOUSE_Y_NEG] = (float)(MouseState.lY) / -250.0f;
	pfOut[TB_MOUSE_Y_POS] = -pfOut[TB_MOUSE_Y_NEG];
	pfOut[TB_MOUSE_Z_NEG] = (float)(MouseState.lZ) / -180.0f;
	pfOut[TB_MOUSE_Z_POS] = -pfOut[TB_MOUSE_Z_NEG];

	// Die 8 Knöpfe verarbeiten
	for(DWORD dwButton = 0; dwButton < 8; dwButton++)
	{
		// Gedrückt: 1.0; Nicht gedrückt: 0.0
		pfOut[TB_MOUSE_BUTTON(dwButton)] = (MouseState.rgbButtons[dwButton] & 0x80) ? 1.0f : 0.0f;
	}

	return TB_OK;
}

// ******************************************************************
// Abfragen des Joysticks
tbResult GetJoystickState(LPDIRECTINPUTDEVICE8 pDevice,
						  DWORD dwJoystick,
						  float* pfOut)
{
	HRESULT		hResult;
	DIJOYSTATE	JoyState;
	float		fAngle;
	float		fSin;
	float		fCos;


	// Den Joystick abfragen
	pDevice->Poll();
	if(FAILED(pDevice->GetDeviceState(sizeof(DIJOYSTATE), &JoyState)))
	{
		// Versuchen, den Zugriff auf das Gerät wieder zu aktivieren
		if(FAILED(pDevice->Acquire()))
		{
			// Es geht nicht! Die Anwendung ist wahrscheinlich inaktiv.
			TB_ERROR("Fehler beim Zugriff auf den Joystick!", TB_ERROR);
		}
		else
		{
			// Noch einmal versuchen, die Daten abzufragen
			if(FAILED(hResult = pDevice->GetDeviceState(sizeof(DIJOYSTATE), &JoyState)))
			{
				// Fehler!
				TB_ERROR_DIRECTX("pDevice->GetDeviceState", hResult, TB_ERROR);
			}
		}
	}

	// Die sechs Achsen (x, y, z, Rx, Ry, Rz) verarbeiten
	pfOut[TB_JOY_X_NEG(dwJoystick)] = (float)(JoyState.lX) / -1000.0f;
	pfOut[TB_JOY_X_POS(dwJoystick)] = -pfOut[TB_JOY_X_NEG(dwJoystick)];
	pfOut[TB_JOY_Y_NEG(dwJoystick)] = (float)(JoyState.lY) / -1000.0f;
	pfOut[TB_JOY_Y_POS(dwJoystick)] = -pfOut[TB_JOY_Y_NEG(dwJoystick)];
	pfOut[TB_JOY_Z_NEG(dwJoystick)] = (float)(JoyState.lZ) / -1000.0f;
	pfOut[TB_JOY_Z_POS(dwJoystick)] = -pfOut[TB_JOY_Z_NEG(dwJoystick)];
	pfOut[TB_JOY_RX_NEG(dwJoystick)] = (float)(JoyState.lRx) / -1000.0f;
	pfOut[TB_JOY_RX_POS(dwJoystick)] = -pfOut[TB_JOY_RX_NEG(dwJoystick)];
	pfOut[TB_JOY_RY_NEG(dwJoystick)] = (float)(JoyState.lRy) / -1000.0f;
	pfOut[TB_JOY_RY_POS(dwJoystick)] = -pfOut[TB_JOY_RY_NEG(dwJoystick)];
	pfOut[TB_JOY_RZ_NEG(dwJoystick)] = (float)(JoyState.lRz) / -1000.0f;
	pfOut[TB_JOY_RZ_POS(dwJoystick)] = -pfOut[TB_JOY_RZ_NEG(dwJoystick)];

	// Die beiden Schieberegler verarbeiten
	for(DWORD dwSlider = 0; dwSlider < 2; dwSlider++)
	{
		// Wenn die ehemalige Position des Schiebereglers auf -10000 steht,
		// dann ist das hier das erste Mal, dass wir den Joystick abfragen.
		if(tbDirectInput::m_pDevices[2 + dwJoystick].alOldSliderPos[dwSlider] == -10000)
		{
			// Der Startwert ist null.
			pfOut[TB_JOY_SLIDER_NEG(dwJoystick, dwSlider)] = 0.0f;
			pfOut[TB_JOY_SLIDER_POS(dwJoystick, dwSlider)] = 0.0f;
		}
		else
		{
			// Die Differenz zwischen alter und neuer Position bestimmt
			// den Wert der analogen Knöpfe.
			pfOut[TB_JOY_SLIDER_NEG(dwJoystick, dwSlider)] = (float)(tbDirectInput::m_pDevices[2 + dwJoystick].alOldSliderPos[dwSlider] - JoyState.rglSlider[dwSlider]) / -1.0f;
			pfOut[TB_JOY_SLIDER_POS(dwJoystick, dwSlider)] = -pfOut[TB_JOY_SLIDER_NEG(dwJoystick, dwSlider)];
		}

		// Knopf für absolute Schiebereglerposition verarbeiten
		pfOut[TB_JOY_SLIDER_ABS(dwJoystick, dwSlider)] = (float)(JoyState.rglSlider[dwSlider] - 1000) / -2000.0f;

		// Die aktuelle Schiebereglerposition als alte Position eintragen
		tbDirectInput::m_pDevices[2 + dwJoystick].alOldSliderPos[dwSlider] = JoyState.rglSlider[dwSlider];
	}

	// Die vier POV-Controller verarbeiten
	for(DWORD dwPOV = 0; dwPOV < 4; dwPOV++)
	{
		if(JoyState.rgdwPOV[dwPOV] == -1 ||
		   JoyState.rgdwPOV[dwPOV] == 65535)
		{
			// Alles null!
			pfOut[TB_JOY_POV_UP(dwJoystick, dwPOV)] = 0.0f;
			pfOut[TB_JOY_POV_RIGHT(dwJoystick, dwPOV)] = 0.0f;
			pfOut[TB_JOY_POV_DOWN(dwJoystick, dwPOV)] = 0.0f;
			pfOut[TB_JOY_POV_LEFT(dwJoystick, dwPOV)] = 0.0f;
		}
		else
		{
			// Winkel in Rad berechnen, dazu den Sinus und den Kosinus
			fAngle = TB_DEG_TO_RAD((float)(JoyState.rgdwPOV[dwPOV]) / 100.0f);
			fSin = sinf(fAngle);
			fCos = cosf(fAngle);

			// Der Wert des "Nach-Oben"-Knopfs ist der Kosinus des Winkels.
			// Um negative Werte kümmern wir uns nicht - die werden von der GetState-Methode
			// später sowieso korrigiert und auf null gesetzt.
			pfOut[TB_JOY_POV_UP(dwJoystick, dwPOV)] = fCos;

			// Der Wert des "Nach-Unten"-Knopfs ist der negative Kosinus.
			pfOut[TB_JOY_POV_DOWN(dwJoystick, dwPOV)] = -fCos;

			// Der Wert des "Nach-Rechts"-Knopfs ist der Sinus.
			pfOut[TB_JOY_POV_RIGHT(dwJoystick, dwPOV)] = fSin;

			// Der Wert des "Nach-Links"-Knopfs ist der negative Sinus.
			pfOut[TB_JOY_POV_LEFT(dwJoystick, dwPOV)] = -fSin;
		}
	}

	// Die 32 Knöpfe verarbeiten
	for(DWORD dwButton = 0; dwButton < 32; dwButton++)
	{
		// Gedrückt: 1.0; Nicht gedrückt: 0.0
		pfOut[TB_JOY_BUTTON(dwJoystick, dwButton)] = (JoyState.rgbButtons[dwButton] & 0x80) ? 1.0f : 0.0f;
	}

	return TB_OK;
}

// ******************************************************************
// Fragt alle Eingabegeräte ab und generiert analoge und digitale Knopfdaten
tbResult tbDirectInput::GetState(float* pfOut,
								 BOOL* pbOut) // = NULL
{
	// Parameter prüfen
	if(pfOut == NULL) TB_ERROR_NULL_POINTER("pfOut", TB_ERROR);


	// Jedes Gerät durchgehen und es abfragen
	for(DWORD dwDevice = 0; dwDevice < m_dwNumDevices; dwDevice++)
	{
		if(m_pDevices[dwDevice].pDevice != NULL)
		{
			if(m_pDevices[dwDevice].DeviceGUID == GUID_SysKeyboard)
			{
				// Es ist die Tastatur!
				if(GetKeyboardState(m_pDevices[dwDevice].pDevice,
					                pfOut))
				{
					// Fehler!
					TB_ERROR("Fehler beim Abfragen der Tastatur!", TB_ERROR);
				}
			}
			else if(m_pDevices[dwDevice].DeviceGUID == GUID_SysMouse)
			{
				// Es ist die Maus!
				if(GetMouseState(m_pDevices[dwDevice].pDevice,
					             pfOut))
				{
					// Fehler!
					TB_ERROR("Fehler beim Abfragen der Maus!", TB_ERROR);
				}
			}
			else
			{
				// Es ist ein Joystick!
				if(GetJoystickState(m_pDevices[dwDevice].pDevice,
					                dwDevice - 2,
									pfOut))
				{
					// Fehler!
					TB_ERROR("Fehler beim Abfragen des Joysticks!", TB_ERROR);
				}
			}
		}
	}

	// Die Knopfdaten skalieren und an die Grenzen anpassen
	for(DWORD dwButton = 0; dwButton < m_dwNumButtons; dwButton++)
	{
		pfOut[dwButton] *= m_pButtons[dwButton].fScaling;
		if(pfOut[dwButton] > 1.0f) pfOut[dwButton] = 1.0f;
		if(pfOut[dwButton] < 0.0f) pfOut[dwButton] = 0.0f;
	}

	// Die Knopfdaten digitalisieren (falls erwünscht)
	if(pbOut != NULL)
	{
		for(dwButton = 0; dwButton < m_dwNumButtons; dwButton++)
		{
			// Umwandeln in digitale Daten: >= 0.2: an; < 0.2: aus
			pbOut[dwButton] = pfOut[dwButton] >= 0.2f;
		}
	}

	return TB_OK;
}

// ******************************************************************
// Findet den ersten gerade aktiven analogen Knopf
int tbDirectInput::GetPressedButton()
{
	float* pfButtons;

	// Speicher reservieren
	pfButtons = (float*)(tbMemAlloc(m_dwNumButtons * sizeof(float)));
	if(pfButtons == NULL) TB_ERROR_OUT_OF_MEMORY(-1);

	// Zustände abfragen
	if(GetState(pfButtons))
	{
		// Fehler!
		TB_ERROR("Fehler beim Abfragen der Eingabegeräte!", -1);
	}

	// Den ersten gedrückten Knopf suchen
	for(DWORD dwButton = 0; dwButton < m_dwNumButtons; dwButton++)
	{
		if(dwButton != TB_JOY_SLIDER_ABS(0, 0) &&
		   dwButton != TB_JOY_SLIDER_ABS(0, 1) &&
		   dwButton != TB_JOY_SLIDER_ABS(1, 0) &&
		   dwButton != TB_JOY_SLIDER_ABS(1, 1) &&
		   dwButton != TB_JOY_SLIDER_ABS(2, 0) &&
		   dwButton != TB_JOY_SLIDER_ABS(2, 1) &&
		   dwButton != TB_JOY_SLIDER_ABS(3, 0) &&
		   dwButton != TB_JOY_SLIDER_ABS(3, 1) &&
		   pfButtons[dwButton] >= 0.2f)
		{
			// Knopf gefunden!
			TB_SAFE_MEMFREE(pfButtons);
			return (int)(dwButton);
		}
	}

	// Kein gedrückter Knopf!
	TB_SAFE_MEMFREE(pfButtons);
	return -1;
}

// ******************************************************************
// Wartet, bis der Benutzer einen Knopf drückt oder die Zeit abgelaufen ist
int tbDirectInput::WaitForButton(DWORD dwStartDelay,	// = 150
								 DWORD dwMaxTime)		// = 3000
{
	DWORD	dwEndTime;
	int		iButton;


	GetPressedButton();

	// Startverzögerung abwarten
	tbDelay(dwStartDelay);

	dwEndTime = timeGetTime() + dwMaxTime;
	while(timeGetTime() < dwEndTime)
	{
		// Gedrückten Knopf suchen
		iButton = GetPressedButton();
		if(iButton != -1) return iButton;
	}

	// Die Zeit ist abgelaufen!
	return -1;
}

// ******************************************************************
// Setzt die Skalierung eines analogen Knopfs
tbResult tbDirectInput::SetButtonScaling(DWORD dwButton,
										 float fScaling)
{
	// Parameter prüfen
	if(dwButton >= m_dwNumButtons) TB_ERROR_INVALID_VALUE("dwButton", TB_ERROR);

	
	// Skalierung einsetzen
	m_pButtons[dwButton].fScaling = fScaling;

	return TB_OK;
}

// ******************************************************************
// Fragt die Skalierung eines analogen Knopfs an
float tbDirectInput::GetButtonScaling(DWORD dwButton)
{
	// Parameter prüfen
	if(dwButton >= m_dwNumButtons) TB_ERROR_INVALID_VALUE("dwButton", 0.0f);

	
	// Skalierung zurückliefern
	return m_pButtons[dwButton].fScaling;
}

// ******************************************************************
// Umwandeln eines Knopfes in ein druckbares Zeichen
TRIBASE_API char tbKeyToChar(int iKey,
							 BOOL bShift,	// = FALSE
							 BOOL bAltCtrl)	// = FALSE
{
	if(iKey < 0 || iKey > 107) return 0;
	else return tb_g_acKeyToChar[iKey * 3 + (bShift ? 1 : (bAltCtrl ? 2 : 0))];
}

// ******************************************************************