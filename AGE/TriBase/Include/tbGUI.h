/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`�__\ /\ \ \ \  __\ /\ __\_ /\  __\   /�__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbGUI.h
	=======
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Eine grafische Benutzeroberfl�che

	Autor:
	David Scherfgen

********************************************************************/

// Verwendete Symbole vordefinieren
class tbGUI;

// ******************************************************************
// Vertexformat
#define TB_GUI_FVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
struct TRIBASE_API tbGUIVertex
{
	tbVector3	vPosition;	// Positionsangabe
	float		fRHW;		// 1/w
	D3DCOLOR	Diffuse;	// Streufarbe
	tbVector2	vTexture;	// Texturkoordinaten
};

// ******************************************************************
// Aufz�hlung f�r Nachrichtentypen
enum TRIBASE_API tbGUIMessageType
{
	TB_GMT_CREATE,			// Das Element wurde erstellt.
	TB_GMT_DELETE,			// Das Element wird gel�scht.
	TB_GMT_ENTERPAGE,		// Betreten einer Seite
	TB_GMT_MOVE,			// Das Element soll bewegt werden.
	TB_GMT_RENDER,			// Das Element soll gerendert werden.
	TB_GMT_ELEMENTCLICKED,	// Ein Element wurde angeklickt.
	TB_GMT_LBUTTONDOWN,		// Linker Mausknopf gedr�ckt
	TB_GMT_LBUTTONUP,		// Linker Mausknopf losgelassen
	TB_GMT_RBUTTONDOWN,		// Rechter Mausknopf gedr�ckt
	TB_GMT_RBUTTONUP,		// Rechter Mausknopf losgelassen
	TB_GMT_KEYDOWN,			// Tastaturtaste gedr�ckt
	TB_GMT_KEYUP,			// Tastaturtaste losgelassen
	TB_GMT_KEYREPEAT,		// Wiederholter Knopfdruck (Taste bleibt gedr�ckt)
	TB_GMT_SELECTION		// Auswahl bei einer Liste wurde ge�ndert
};

// ******************************************************************
// Struktur f�r eine Nachricht
struct TRIBASE_API tbGUIMessage
{
	tbGUIMessageType	Type;			// Typ der Nachricht
	int					aiInt[8];		// 8 Integer
	float				afFloat[8];		// 8 Flie�kommazahlen
	void*				apPointer[8];	// 8 Zeiger
};

// ******************************************************************
// Aufz�hlung f�r Oberfl�chenelementtypen
enum TRIBASE_API tbGUIElementType
{
	TB_GET_FRAME,		// Rahmen
	TB_GET_BUTTON,		// Knopf
	TB_GET_TEXT,		// Text
	TB_GET_IMAGE,		// Bild
	TB_GET_CHECKBOX,	// Check-Box
	TB_GET_RADIOBOX,	// Radio-Box
	TB_GET_INPUT,		// Eingabefeld
	TB_GET_LIST			// Liste
};

// ******************************************************************
// Klasse f�r ein Oberfl�chenelement
class TRIBASE_API tbGUIElement
{
	friend class tbGUI;
	friend class tbGUIFrame;
	friend class tbGUIButton;
	friend class tbGUIText;
	friend class tbGUIImage;
	friend class tbGUICheckBox;
	friend class tbGUIRadioBox;
	friend class tbGUIInput;
	friend class tbGUIList;

private:
	// Variablen
	int					m_iID;			// ID des Elements
	tbGUIElementType	m_Type;			// Typ des Elements
	int					m_iPage;		// Auf welcher Seite ist das Element?
	tbGUI*				m_pGUI;			// Zu welcher Oberfl�che das Element geh�rt
	tbVector2			m_vPosition;	// Position der linken oberen Ecke
	tbVector2			m_vSize;		// Breite und H�he
	BOOL				m_bVisible;		// Sichtbar?
	BOOL				m_bEnabled;		// Aktiviert?
	BOOL				m_bPressed;		// Wird das Element gerade gedr�ckt?

public:
	// Methoden
	virtual tbResult ReceiveMessage(const tbGUIMessage* pMsg) = 0; // Das Element erh�lt eine Nachricht.

	// Inline-Methoden
	inline int				GetID()						{return m_iID;}
	inline tbGUIElementType	GetType()					{return m_Type;}
	inline int				GetPage()					{return m_iPage;}
	inline tbGUI*			GetGUI()					{return m_pGUI;}
	inline tbVector2		GetPosition()				{return m_vPosition;}
	inline tbVector2		GetSize()					{return m_vSize;}
	inline BOOL				IsVisible()					{return m_bVisible;}
	inline BOOL				IsEnabled()					{return m_bEnabled;}
	inline BOOL				IsPressed()					{return m_bPressed;}
	inline void				SetPage(int iPage)			{m_iPage = iPage;}
	inline void				SetPosition(tbVector2 vPos)	{m_vPosition = vPos;}
	inline void				SetSize(tbVector2 vSize)	{m_vSize = vSize;}
	inline void				Show(BOOL bShow)			{m_bVisible = bShow;}
	inline void				Enable(BOOL bEnable)		{m_bEnabled = bEnable;}
};

// ******************************************************************
// Klasse f�r einen Rahmen
class TRIBASE_API tbGUIFrame : public tbGUIElement
{
	friend class tbGUI;

public:
	// Methoden
	tbResult ReceiveMessage(const tbGUIMessage* pMsg); // Das Element erh�lt eine Nachricht.
};

// ******************************************************************
// Klasse f�r einen Knopf
class TRIBASE_API tbGUIButton : public tbGUIElement
{
	friend class tbGUI;

private:
	// Variablen
	char	m_acText[256];		// Text
	BOOL	m_bIsListScrollBtn;	// Ist es ein Knopf einer Liste (zum Scrollen)?
	BOOL	m_bIsScrollUpBtn;	// Ist es ein Knopf zum Hochscrollen?
	int		m_iList;			// ID der zugeh�rigen Liste

public:
	// Methoden
	tbResult ReceiveMessage(const tbGUIMessage* pMsg); // Das Element erh�lt eine Nachricht.

	// Inline-Methoden
	inline char*	GetText()				{return m_acText;}
	inline void		SetText(char* pcText)	{strcpy(m_acText, pcText);}
};

// ******************************************************************
// Klasse f�r einen Text
class TRIBASE_API tbGUIText : public tbGUIElement
{
	friend class tbGUI;

private:
	// Variablen
	char		m_acText[4096];	// Text
	tbColor		m_Color;		// Farbe
	tbVector2	m_vTextSize;	// Gr��e
	DWORD		m_dwTextFlags;	// Flags f�r die Textdarstellung

public:
	// Methoden
	tbResult ReceiveMessage(const tbGUIMessage* pMsg); // Das Element erh�lt eine Nachricht.

	// Inline-Methoden
	inline char*		GetText()					{return m_acText;}
	inline tbColor		GetColor()					{return m_Color;}
	inline tbVector2	GetTextSize()				{return m_vTextSize;}
	inline DWORD		GetTextFlags()				{return m_dwTextFlags;}
	inline void			SetText(char* pcText)		{strcpy(m_acText, pcText);}
	inline void			SetColor(tbColor c)			{m_Color = c;}
	inline void			SetTextSize(tbVector2 s)	{m_vTextSize = s;}
	inline void			SetTextFlags(DWORD f)		{m_dwTextFlags = f;}
};

// ******************************************************************
// Klasse f�r ein Bild
class TRIBASE_API tbGUIImage : public tbGUIElement
{
	friend class tbGUI;

private:
	// Variablen
	PDIRECT3DTEXTURE9	m_pTexture;			// Die Textur
	tbVector2			m_vTopLeftTex;		// Linke obere Texturkoordinaten
	tbVector2			m_vBottomRightTex;	// Rechte untere Texturkoordinaten
	tbColor				m_Color;			// Farbe

public:
	// Methoden
	tbResult ReceiveMessage(const tbGUIMessage* pMsg); // Das Element erh�lt eine Nachricht.

	// Inline-Methoden
	inline PDIRECT3DTEXTURE9	GetTexture()					{return m_pTexture;}
	inline tbVector2			GetTopLeftTex()					{return m_vTopLeftTex;}
	inline tbVector2			GetBottomRightTex()				{return m_vBottomRightTex;}
	inline tbColor				GetColor()						{return m_Color;}
	inline void					SetTexture(PDIRECT3DTEXTURE9 t)	{m_pTexture = t;}
	inline void					SetTopLeftTex(tbVector2 v)		{m_vTopLeftTex = v;}
	inline void					SetBottomRightTex(tbVector2 v)	{m_vBottomRightTex = v;}
	inline void					SetColor(tbColor c)				{m_Color = c;}
};

// ******************************************************************
// Klasse f�r eine Check-Box
class TRIBASE_API tbGUICheckBox : public tbGUIElement
{
	friend class tbGUI;

private:
	// Variablen
	char m_acText[256];	// Text
	BOOL m_bChecked;	// Angekreuzt?

public:
	// Methoden
	tbResult ReceiveMessage(const tbGUIMessage* pMsg); // Das Element erh�lt eine Nachricht.

	// Inline-Methoden
	inline char*	GetText()				{return m_acText;}
	inline BOOL		IsChecked()				{return m_bChecked;}
	inline void		SetText(char* pcText)	{strcpy(m_acText, pcText);}
	inline void		Check(BOOL bCheck)		{m_bChecked = bCheck;}
};

// ******************************************************************
// Klasse f�r eine Radio-Box
class TRIBASE_API tbGUIRadioBox : public tbGUIElement
{
	friend class tbGUI;

private:
	// Variablen
	char	m_acText[256];	// Text
	int		m_iGroup;		// Gruppen-ID
	BOOL	m_bChecked;		// Angekreuzt?

public:
	// Methoden
	tbResult ReceiveMessage(const tbGUIMessage* pMsg);	// Das Element erh�lt eine Nachricht.
	tbResult Check(BOOL bCheck);						// Status �ndern

	// Inline-Methoden
	inline char*	GetText()				{return m_acText;}
	inline BOOL		IsChecked()				{return m_bChecked;}
	inline int		GetGroup()				{return m_iGroup;}
	inline void		SetText(char* pcText)	{strcpy(m_acText, pcText);}
	inline void		SetGroup(int iGroup)	{m_iGroup = iGroup; Check(m_bChecked);}
};

// ******************************************************************
// Klasse f�r ein Eingabefeld
class TRIBASE_API tbGUIInput : public tbGUIElement
{
	friend class tbGUI;

private:
	// Variablen
	char	m_acText[4096];	// Text
	BOOL	m_bPassword;	// Passwortfeld?
	char	m_PasswordChar;	// Zeichen f�r Passwort
	int		m_iCursor;		// Wo der Cursor gerade ist
	int		m_iScrolling;	// Ab wo der Text angezeigt wird

public:
	// Methoden
	tbResult ReceiveMessage(const tbGUIMessage* pMsg); // Das Element erh�lt eine Nachricht.

	// Inline-Methoden
	inline char*	GetText()				{return m_acText;}
	inline BOOL		IsPassword()			{return m_bPassword;}
	inline char		GetPasswordChar()		{return m_PasswordChar;}
	inline int		GetCursor()				{return m_iCursor;}
	inline void		SetText(char* pcText)	{strcpy(m_acText, pcText); m_iCursor = strlen(m_acText);}
	inline void		SetPassword(BOOL b)		{m_bPassword = b;}
	inline void		SetPasswordChar(char c)	{m_PasswordChar = c;}
	inline void		SetCursor(int iCursor)	{m_iCursor = iCursor;}
};

// ******************************************************************
// Struktur f�r einen Listeneintrag
struct TRIBASE_API tbGUIListEntry
{
	int		iID;			// ID dieses Eintrags
	char	acText[256];	// Text
	void*	pData;			// Daten
};

// ******************************************************************
// Klasse f�r eine Liste
class TRIBASE_API tbGUIList : public tbGUIElement
{
	friend class tbGUI;

private:
	// Variablen
	tbList<tbGUIListEntry>*	m_pList;		// Die Liste der Eintr�ge
	int						m_iCursor;		// ID des angew�hlten Listeneintrags
	int						m_iScrolling;	// ID des ersten sichtbaren Listeneintrags
	int						m_iNextEntryID;	// ID des n�chsten Eintrags, der erstellt wird
	int						m_iScrollUpBtn;	// ID des Knopfes zum Hochscrollen
	int						m_iScrollDnBtn;	// ID des Knopfes zum Runterscrollen
	float					m_fLineHeight;	// Zeilenh�he
	int						m_iOldCursor;	// Alter Cursor

public:
	// Methoden
	tbResult						ReceiveMessage(const tbGUIMessage* pMsg);	// Das Element erh�lt eine Nachricht.
	tbGUIListEntry*					GetEntry(int iID);							// Liefert einen Listeneintrag
	tbListEntry<tbGUIListEntry>*	GetEntryListEntry(int iID);					// Liefert einen Listeneintrag
	int								AddEntry(char* pcText, void* pData);		// F�gt einen Eintrag hinzu
	tbResult						DeleteEntry(int iID);						// L�scht einen Eintrag
	int								GetEntryOrder(int iID);						// Liefert die Ordnung eines Eintrags
	tbGUIListEntry*					GetEntryByOrder(int iOrder);				// Liefert den Eintrag mit einer bestimmten Ordnung
	int								GetPrevEntry(int iID);						// Liefert vorherigen Eintrag
	int								GetNextEntry(int iID);						// Liefert n�chsten Eintrag
	tbResult						Correct();									// Korrigiert eventuelle Fehler

	// Inline-Methoden
	inline int				GetCursor()				{return m_iCursor;}
	inline float			GetLineHeight()			{return m_fLineHeight;}
	inline int				GetNumEntries()			{return m_pList->GetNumEntries();}
	inline tbGUIListEntry*	GetSelectedEntry()		{return GetEntry(m_iCursor);}
	inline void				SetCursor(int i)		{m_iCursor = i;}
	inline void				SetLineHeight(float f)	{m_fLineHeight = f;}
};

// ******************************************************************
// Struktur f�r einen Skin
struct TRIBASE_API tbGUISkin
{
	PDIRECT3DTEXTURE9	pSkinTexture;		// Textur f�r den Skin
	tbEffect*			pEffect;			// Effekt f�r die Oberfl�chenelemente
	tbFont*				pFont;				// Schriftart f�r alle Texte
	tbVector2			vCursorSize;		// Gr��e des Cursors
	tbColor				CursorColor1;		// Farbe 1 des Cursors
	tbColor				CursorColor2;		// Farbe 2 des Cursors
	tbVector2			vCursorHotSpot;		// Hot-Spot des Cursors
	float				fCursorSpeed;		// Geschwindigkeit des Cursors
	tbColor				EnabledColor;		// Farbe f�r aktivierte Elemente
	tbColor				DisabledColor;		// Farbe f�r deaktivierte Elemente
	tbColor				HighlightColor;		// Farbe f�r hervorgehobene Elemente
	tbVector2			vTextSize;			// Gr��e der Texte
	tbColor				EnabledTextColor;	// Farbe f�r Texte aktivierter Elemente
	tbColor				DisabledTextColor;	// Farbe f�r Texte deaktivierter Elemente
	tbColor				HighlightTextColor;	// Farbe f�r Texte hervorgehobener Elemente
};

// ******************************************************************
// Klasse f�r die Benutzeroberfl�che
class TRIBASE_API tbGUI
{
	friend class tbGUIElement;
	friend class tbGUIFrame;
	friend class tbGUIText;
	friend class tbGUIImage;
	friend class tbGUIButton;
	friend class tbGUICheckBox;
	friend class tbGUIRadioBox;
	friend class tbGUIInput;
	friend class tbGUIList;

private:
	// Variablen
	tbList<tbGUIElement*>*	m_pElements;		// Liste der Elemente
	int						m_iFocus;			// Das Element, das den Fokus hat (ID)
	tbVertexBuffer*			m_pVertexBuffer;	// Der Vertex-Buffer
	BOOL*					m_pbButtons;		// Zustand der analogen Kn�pfe (digital)
	BOOL*					m_pbOldButtons;		// Ehemaliger Zustand der analogen Kn�pfe (digital)
	float*					m_pfButtons;		// Zustand der analogen Kn�pfe
	tbGUISkin				m_Skin;				// Skin f�r die Benutzeroberfl�che
	int						m_iCurrentPage;		// Aktive Seite
	tbVector2				m_vCursorPos;		// Cursorposition
	tbVector2				m_vSize;			// Gr��e
	tbVector2				m_vScaling;			// Skalierung
	void*					m_pMsgProc;			// Funktion f�r Kommandonachrichten
	int						m_aiKeyRepeat[108];	// Array f�r wiederholten Tastendruck
	DWORD					m_dwOldTime;		// Alte Zeit

public:
	// Konstruktor und Destruktor
	tbGUI();
	~tbGUI();

	// Methoden
	tbResult					Init(tbGUISkin* pSkin, void* pMsgProc, tbVector2 vSize, tbVector2 vScaling = tbVector2(1.0f));	// Initialisierung
	tbResult					Move(const float fTime, BOOL* pbButtons, float* pfButtons);										// Bewegen
	tbResult					Render(const float fTime);																		// Rendern
	tbResult					RenderCursor(const float fTime);																// Mauszeiger rendern
	tbResult					PostMessage(int iID, tbGUIMessage* pMsg);														// Sendet eine Nachricht an ein Element
	tbResult					BroadcastMessage(tbGUIMessage* pMsg, int iPage = -1);											// Schickt eine Nachricht an alle Elemente
	tbListEntry<tbGUIElement*>*	GetElementListEntry(int iID);																	// Liefert den Listeneintrag eines Elements
	tbGUIElement*				GetElement(int iID);																			// Liefert Zeiger auf ein Element
	tbResult					DeleteElement(int iID);																			// L�scht ein Element
	tbResult					SetCurrentPage(int iPage);																		// Setzt die aktive Seite

	// F�gt ein Rechteck zu den Vertizes hinzu
	tbResult AddRect(const tbVector2& vTopLeft,
		             const tbVector2& vSize,
					 const tbColor& Color,
					 const tbVector2& vTopLeftTex,
					 const tbVector2& vSizeOnTex,
					 tbGUIVertex* pOut = NULL);

	// F�gt mehrere Rechtecke in einer Reihe hinzu
	tbResult AddRects1D(const tbVector2& vTopLeft,
						const tbVector2& vSize,
						float fNumRects,
						const tbVector2& vTranslation,
						const tbColor& Color,
						const tbVector2& vTopLeftTex,
						const tbVector2& vSizeOnTex);

	// F�gt mehrere Rechtecke zweidimensional hinzu
	tbResult AddRects2D(tbVector2 vTopLeft,
						const tbVector2& vSize,
						const float fNumColumns,
						float fNumRows,
						const tbColor& Color,
						const tbVector2& vTopLeftTex,
						const tbVector2& vSizeOnTex);

	// F�gt einen ganzen Rahmen hinzu
	tbResult AddFrame(const tbVector2& vTopLeft,
		              const tbVector2& vSize,
					  const tbColor& Color,
					  const tbVector2& vTopLeftOnTex);

	// Erstellt ein neues Element
	tbGUIElement* CreateElement(int iID,
								tbGUIElementType Type,
								int iPage,
								tbVector2 vPosition,
								tbVector2 vSize);

	// Erstellt einen neuen Rahmen
	tbResult CreateFrame(int iID,
						 int iPage,
		                 tbVector2 vPosition,
						 tbVector2 vSize);

	// Erstellt einen neuen Knopf
	tbResult CreateButton(int iID,
		                  int iPage,
		                  tbVector2 vPosition,
					      tbVector2 vSize,
						  char* pcText);

	// Erstellt einen neuen Text
	tbResult CreateText(int iID,
		                int iPage,
		                tbVector2 vPosition,
						char* pcText,
						tbColor Color = tbColor(1.0f),
						tbVector2 vTextSize = tbVector2(1.0f),
						DWORD dwTextFlags = TB_FF_RELATIVESCALING);

	// Erstellt ein neues Bild
	tbResult CreateImage(int iID,
		                 int iPage,
		                 tbVector2 vPosition,
					     tbVector2 vSize,
						 PDIRECT3DTEXTURE9 pTexture,
						 tbVector2 vTopLeftTex = tbVector2(0.0f, 0.0f),
						 tbVector2 vBottomRightTex = tbVector2(1.0f, 1.0f),
						 tbColor Color = tbColor(1.0f));

	// Erstellt eine neue Check-Box
	tbResult CreateCheckBox(int iID,
							int iPage,
							tbVector2 vPosition,
							tbVector2 vSize,
							char* pcText,
							BOOL bChecked = FALSE);

	// Erstellt eine neue Radio-Box
	tbResult CreateRadioBox(int iID,
							int iPage,
							tbVector2 vPosition,
							tbVector2 vSize,
							char* pcText,
							int iGroup = 0,
							BOOL bChecked = FALSE);

	// Erstellt ein neues Eingabefeld
	tbResult CreateInput(int iID,
						 int iPage,
		                 tbVector2 vPosition,
 						 tbVector2 vSize,
						 char* pcText = "",
						 BOOL bPassword = FALSE,
						 char PasswordChar = '*');

	// Erstellt eine neue Liste
	tbResult CreateList(int iID,
		                int iPage,
						tbVector2 vPosition,
						tbVector2 vSize,
						float fLineHeight);

	// Inline-Methoden
	inline BOOL			WasButtonPressed(int b)		{return !m_pbOldButtons[b] && m_pbButtons[b];}
	inline BOOL			WasButtonReleased(int b)	{return m_pbOldButtons[b] && !m_pbButtons[b];}
	inline tbGUISkin	GetSkin()					{return m_Skin;}
	inline int			GetCurrentPage()			{return m_iCurrentPage;}
	inline tbVector2	GetCursorPos()				{return m_vCursorPos;}
	inline void*		GetMessageProc()			{return m_pMsgProc;}
	inline void			SetSkin(tbGUISkin* pSkin)	{m_Skin = *pSkin;}
	inline void			SetCursorPos(tbVector2 v)	{m_vCursorPos = v;}
	inline void			SetMessageProc(void *pProc)	{m_pMsgProc = pProc;}
};

// ******************************************************************