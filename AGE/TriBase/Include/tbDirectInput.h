/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbDirectInput.h
	===============
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Einfaches Abfragen von Eingabegeräten mit DirectInput

	Autor:
	David Scherfgen

********************************************************************/
//#define FF
// Tastaturdefinitionen
#define TB_KEY_ESCAPE		(0)
#define TB_KEY_F1			(1)
#define TB_KEY_F2			(2)
#define TB_KEY_F3			(3)
#define TB_KEY_F4			(4)
#define TB_KEY_F5			(5)
#define TB_KEY_F6			(6)
#define TB_KEY_F7			(7)
#define TB_KEY_F8			(8)
#define TB_KEY_F9			(9)
#define TB_KEY_F10			(10)
#define TB_KEY_F11			(11)
#define TB_KEY_F12			(12)
#define TB_KEY_F13			(13)
#define TB_KEY_F14			(14)
#define TB_KEY_F15			(15)
#define TB_KEY_SYSRQ		(16)
#define TB_KEY_SCROLL		(17)
#define TB_KEY_PAUSE		(18)
#define TB_KEY_1			(19)
#define TB_KEY_2			(20)
#define TB_KEY_3			(21)
#define TB_KEY_4			(22)
#define TB_KEY_5			(23)
#define TB_KEY_6			(24)
#define TB_KEY_7			(25)
#define TB_KEY_8			(26)
#define TB_KEY_9			(27)
#define TB_KEY_0			(28)
#define TB_KEY_MINUS		(29)
#define TB_KEY_EQUALS		(30)
#define TB_KEY_BACK			(31)
#define TB_KEY_INSERT		(32)
#define TB_KEY_HOME			(33)
#define TB_KEY_PRIOR		(34)
#define TB_KEY_NUMLOCK		(35)
#define TB_KEY_DIVIDE		(36)
#define TB_KEY_MULTIPLY		(37)
#define TB_KEY_SUBTRACT		(38)
#define TB_KEY_TAB			(39)
#define TB_KEY_A			(40)
#define TB_KEY_B			(41)
#define TB_KEY_C			(42)
#define TB_KEY_D			(43)
#define TB_KEY_E			(44)
#define TB_KEY_F			(45)
#define TB_KEY_G			(46)
#define TB_KEY_H			(47)
#define TB_KEY_I			(48)
#define TB_KEY_J			(49)
#define TB_KEY_K			(50)
#define TB_KEY_L			(51)
#define TB_KEY_M			(52)
#define TB_KEY_N			(53)
#define TB_KEY_O			(54)
#define TB_KEY_P			(55)
#define TB_KEY_Q			(56)
#define TB_KEY_R			(57)
#define TB_KEY_S			(58)
#define TB_KEY_T			(59)
#define TB_KEY_U			(60)
#define TB_KEY_V			(61)
#define TB_KEY_W			(62)
#define TB_KEY_X			(63)
#define TB_KEY_Y			(64)
#define TB_KEY_Z			(65)
#define TB_KEY_LBRACKET		(66)
#define TB_KEY_RBRACKET		(67)
#define TB_KEY_RETURN		(68)
#define TB_KEY_DELETE		(69)
#define TB_KEY_END			(70)
#define TB_KEY_NEXT			(71)
#define TB_KEY_NUMPAD0		(72)
#define TB_KEY_NUMPAD1		(73)
#define TB_KEY_NUMPAD2		(74)
#define TB_KEY_NUMPAD3		(75)
#define TB_KEY_NUMPAD4		(76)
#define TB_KEY_NUMPAD5		(77)
#define TB_KEY_NUMPAD6		(78)
#define TB_KEY_NUMPAD7		(79)
#define TB_KEY_NUMPAD8		(80)
#define TB_KEY_NUMPAD9		(81)
#define TB_KEY_ADD			(82)
#define TB_KEY_CAPITAL		(83)
#define TB_KEY_SEMICOLON	(84)
#define TB_KEY_APOSTROPHE	(85)
#define TB_KEY_BACKSLASH	(86)
#define TB_KEY_LSHIFT		(87)
#define TB_KEY_OEM_102		(88)
#define TB_KEY_COMMA		(89)
#define TB_KEY_PERIOD		(90)
#define TB_KEY_SLASH		(91)
#define TB_KEY_RSHIFT		(92)
#define TB_KEY_UP			(93)
#define TB_KEY_NUMPADENTER	(94)
#define TB_KEY_LCONTROL		(95)
#define TB_KEY_LWIN			(96)
#define TB_KEY_LALT			(97)
#define TB_KEY_SPACE		(98)
#define TB_KEY_RALT			(99)
#define TB_KEY_RWIN			(100)
#define TB_KEY_APPS			(101)
#define TB_KEY_RCONTROL		(102)
#define TB_KEY_LEFT			(103)
#define TB_KEY_DOWN			(104)
#define TB_KEY_RIGHT		(105)
#define TB_KEY_DECIMAL		(106)
#define TB_KEY_CIRCONFLEX	(107)

// Mausknopfdefinitionen
#define TB_MOUSE_X_NEG		(108 + 0)		// x-Achse negativ
#define TB_MOUSE_X_POS		(108 + 1)		// x-Achse positiv
#define TB_MOUSE_Y_NEG		(108 + 2)		// ...
#define TB_MOUSE_Y_POS		(108 + 3)		// ...
#define TB_MOUSE_Z_NEG		(108 + 4)		// ...
#define TB_MOUSE_Z_POS		(108 + 5)		// ...
#define TB_MOUSE_BUTTON(i)	(108 + 6 + (i))	// Mausknopf Nr. i (0 bis 7)

// Joystickknopfdefinitionen
#define TB_JOY_X_NEG(j)			(122 + (j) * 66 + 0)			// x-Achse negativ
#define TB_JOY_X_POS(j)			(122 + (j) * 66 + 1)			// x-Achse positiv
#define TB_JOY_Y_NEG(j)			(122 + (j) * 66 + 2)			// ...
#define TB_JOY_Y_POS(j)			(122 + (j) * 66 + 3)			// ...
#define TB_JOY_Z_NEG(j)			(122 + (j) * 66 + 4)			// ...
#define TB_JOY_Z_POS(j)			(122 + (j) * 66 + 5)			// ...
#define TB_JOY_RX_NEG(j)		(122 + (j) * 66 + 6)			// Rx-Achse negativ
#define TB_JOY_RX_POS(j)		(122 + (j) * 66 + 7)			// Rx-Achse positiv
#define TB_JOY_RY_NEG(j)		(122 + (j) * 66 + 8)			// ...
#define TB_JOY_RY_POS(j)		(122 + (j) * 66 + 9)			// ...
#define TB_JOY_RZ_NEG(j)		(122 + (j) * 66 + 10)			// ...
#define TB_JOY_RZ_POS(j)		(122 + (j) * 66 + 11)			// ...
#define TB_JOY_SLIDER_NEG(j, i)	(122 + (j) * 66 + 12 + (i * 3))	// Schieberegler Nr. i negativ (0 bis 1)
#define TB_JOY_SLIDER_POS(j, i)	(122 + (j) * 66 + 13 + (i * 3))	// Schieberegler Nr. i positiv (0 bis 1)
#define TB_JOY_SLIDER_ABS(j, i)	(122 + (j) * 66 + 14 + (i * 3))	// Schieberegler Nr. i positiv (0 bis 1)
#define TB_JOY_POV_UP(j, i)		(122 + (j) * 66 + 18 + (i * 4))	// POV-Controller Nr. i hoch (0 bis 3)
#define TB_JOY_POV_RIGHT(j, i)	(122 + (j) * 66 + 19 + (i * 4))	// ...
#define TB_JOY_POV_DOWN(j, i)	(122 + (j) * 66 + 20 + (i * 4))	// ...
#define TB_JOY_POV_LEFT(j, i)	(122 + (j) * 66 + 21 + (i * 4))	// ...
#define TB_JOY_BUTTON(j, i)		(122 + (j) * 66 + 34 + (i))     // Joystickknopf Nr. i (0 bis 31)

// ******************************************************************
// Struktur für Informationen über einen analogen Knopf
struct TRIBASE_API tbButtonInfo
{
	char	acName[64];	// Name des Knopfs (z.B. "Return")
	DWORD	dwDevice;	// Zu welchem Gerät gehört der Knopf?
	float	fScaling;	// Werteskalierung für diesen Knopf
};

// ******************************************************************
// Struktur für Informationen über ein Eingabegerät
struct TRIBASE_API tbInputDeviceInfo
{
	char					acName[64];			// Name des Geräts (z.B. "Tastatur")
	LPDIRECTINPUTDEVICE8	pDevice;			// Geräteschnittstelle
	GUID					DeviceGUID;			// GUID des Geräts
	DWORD					dwFirstButton;		// Index des ersten Knopfs
	DWORD					dwNumButtons;		// Anzahl der analogen Knöpfe
	LONG					alOldSliderPos[2];	// Alte Schiebereglerpositionen
};

// ******************************************************************
// Klasse für DirectInput
class TRIBASE_API tbDirectInput
{
private:
	// Variablen
	static BOOL					m_bInitialized;	// Initialisiert?
	static LPDIRECTINPUT8		m_pDirectInput;	// IDirectInput8-Schnittstelle
	static DWORD				m_dwNumDevices;	// Anzahl der Eingabegeräte
	static DWORD				m_dwNumButtons;	// Anzahl der gesamten Knöpfe
	static tbInputDeviceInfo*	m_pDevices;		// Die Liste der Eingabegeräte
	static tbButtonInfo*		m_pButtons;		// Liste der analogen Knöpfe
	static HWND					m_hWindow;		// Verwendetes Fenster
	static DWORD				m_dwCoopLevel;	// Kooperationsebene
	
#ifdef FF
	static BOOL					m_dwEffectFound;// Ist ein Effekt gefunden worden?
	static DWORD				m_dwNumFFAxis;  // Anzahl der Achxen auf den der Effekt wirken kann
	static LPDIRECTINPUTEFFECT	m_pEffect;		// Pointer auf den FFEffekt (struct)
#endif

public:
	// Methoden
	static tbResult	Init(HWND hWindow = NULL, DWORD dwCoopLevel = DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);	// Initialisierung
	static tbResult Exit();																					// Herunterfahren
	static tbResult	GetState(float* pfOut, BOOL* pbOut = NULL);												// Abfragen aller Eingabegeräte
	static int		GetPressedButton();																		// Findet den ersten gedrückten Knopf
	static int		WaitForButton(DWORD dwStartDelay = 150, DWORD dwMaxTime = 3000);						// Wartet, bis ein Knopf gedrückt wird
	static tbResult	SetButtonScaling(DWORD dwButton, float fScaling);										// Setzt die Skalierung eines Knopfs
	static float	GetButtonScaling(DWORD dwButton);														// Fragt die Skalierung eines Knopfs ab

	// Inline-Methoden
	static inline BOOL					IsInitialized()	{return m_bInitialized;}
	static inline LPDIRECTINPUT8		GetDInput()		{return m_pDirectInput;}
	static inline DWORD					GetNumDevices()	{return m_dwNumDevices;}
	static inline DWORD					GetNumButtons()	{return m_dwNumButtons;}
	static inline tbInputDeviceInfo*	GetDevices()	{return m_pDevices;}
	static inline tbButtonInfo*			GetButtons()	{return m_pButtons;}
	static inline HWND					GetWindow()		{return m_hWindow;}
	static inline DWORD					GetCoopLevel()	{return m_dwCoopLevel;}
	
#ifdef FF
	static inline LPDIRECTINPUTEFFECT	tbDirectInput::GetFFEffect() {return m_pEffect;}
	static inline void					SetEffectFound(BOOL effFound)	{m_dwEffectFound = effFound;}
#endif

	// friend-Funktionen
	friend tbResult InitKeyboard();
	friend tbResult InitMouse();
	friend tbResult InitJoystick(LPCDIDEVICEINSTANCE pDeviceInstance, DWORD dwJoystick);
	friend tbResult GetJoystickState(LPDIRECTINPUTDEVICE8 pDevice, DWORD dwJoystick, float* pfOut);
};

// ******************************************************************
// Funktionen
TRIBASE_API tbDirectInput*	tbGetDirectInput();
TRIBASE_API char			tbKeyToChar(int iKey, BOOL bShift = FALSE, BOOL bAltCtrl = FALSE);

// ******************************************************************