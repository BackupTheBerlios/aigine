// Template.h: Schnittstelle für die Klasse Template.
//
//////////////////////////////////////////////////////////////////////

/**
 * eine Schablone fuer eine doppelt verkette Liste
 * @autor Tobias Harpering @fhtw s0503936
 * @description: Doppelt verkettete Liste als Template
 * Handhabung wie bei einem Vector (nur einfacher vorallem beim debuggen)
 * Liste < Objekt jeglicher Art > 'bezeichner' = new Liste < Object >;

  * Index und benutzung laeuft nun 'edlich' ab 0
 */

#ifndef _LIST_H_
#define _LIST_H_

#include "../AiGinE.h"

// Template-Struktur für einen Eintrag einer verketteten Liste
template <typename Type> struct ListEntry
{
	ListEntry<Type>*	pPrevEntry;	// Zeiger auf den vorherigen Eintrag
	ListEntry<Type>*	pNextEntry;	// Zeiger auf den nächsten Eintrag
	Type				Data;		// Die eigentlichen Listendaten
};

// ******************************************************************
// Template-Klasse für eine verkettete Liste
template <typename Type> class List
{
private:
	// Variablen
	ListEntry<Type>*	m_pFirstEntry;	// Erster Listeneintrag
	ListEntry<Type>*	m_pLastEntry;	// Letzter Listeneintrag
	int					m_iNumEntries;	// Anzahl der Listeneinträge

public:
	// Konstruktur und Destruktor
	List();
	~List();

	// Methoden
	ListEntry<Type>*	AddEntry(Type* pData);									// Eintrag hinzufügen
	Result			FindEntry(Type* pData, ListEntry<Type>** ppOut);		// Eintrag suchen
	Result			DeleteEntry(ListEntry<Type>* pEntry);					// Eintrag löschen
	Result			Clear();												// Liste leeren
	Result			Traverse(Result (* pCallback)(ListEntry<Type>*));	// Liste durchlaufen

	// Inline-Methoden
	inline Type*				GetEntryData(ListEntry<Type>* pEntry)	{return &pEntry->Data;}
	inline ListEntry<Type>*	GetFirstEntry()							{return m_pFirstEntry;}
	inline ListEntry<Type>*	GetLastEntry()							{return m_pLastEntry;}
	inline int					GetNumEntries()							{return m_iNumEntries;}
};

// ******************************************************************
// Konstruktor der List-Klasse
template <typename Type> List<Type>::List()
{
	// Alles zurücksetzen
	ZeroMemory(this, sizeof(List<Type>));
}

// ******************************************************************
// Destruktor der List-Klasse
template <typename Type> List<Type>::~List()
{
	// Liste leeren
	Clear();
}

// ******************************************************************
// Diese Methode fügt einen neuen Eintrag zur Liste hinzu.
template <typename Type> ListEntry<Type>* List<Type>::AddEntry(Type* pData)
{
	ListEntry<Type>* pNewEntry;

	// Parameter prüfen
	if(pData == NULL) AGE_ERROR_NULL_POINTER("pData", NULL);


	// Speicher für die Listeneintragsstruktur reservieren
	pNewEntry = new ListEntry<Type>;
	if(pNewEntry == NULL) AGE_ERROR_OUT_OF_MEMORY(NULL);

	// Der neue Eintrag steht an letzter Stelle der Liste.
	// Daher gibt es keinen nächsten Eintrag und der vorherige Eintrag
	// ist der ursprüngliche letzte Listeneintrag.
	pNewEntry->pPrevEntry = m_pLastEntry;
	if(m_pLastEntry != NULL) m_pLastEntry->pNextEntry = pNewEntry;
	pNewEntry->pNextEntry = NULL;
	m_pLastEntry = pNewEntry;

	// Wenn die Liste noch ganz leer ist, dann ist dieser Eintrag auch
	// gleichzeitig der erste Eintrag.
	if(m_pFirstEntry == NULL) m_pFirstEntry = pNewEntry;

	// Daten kopieren
	memcpy(&pNewEntry->Data, pData, sizeof(Type));

	// Eintragszähler erhöhen
	m_iNumEntries++;

	// Zeiger auf die Listeneintragsstruktur zurückliefern
	return pNewEntry;
}

// ******************************************************************
// Diese Methode sucht einen Eintrag in der Liste mit den angegebenen Daten.
template <typename Type> Result List<Type>::FindEntry(Type* pData,
														  ListEntry<Type>** ppOut)
{
	ListEntry<Type>* pCurrentEntry;

	// Parameter prüfen
	if(pData == NULL) AGE_ERROR_NULL_POINTER("pData", AGE_ERROR);


	// Zuerst den allerersten Listeneintrag testen
	pCurrentEntry = m_pFirstEntry;

	while(pCurrentEntry != NULL)
	{
		// Die Daten des aktuellen Eintrags mit den angegebenen Daten
		// vergleichen. Falls sie übereinstimmen, ist die Suche beendet.
		if(!memcmp(&pCurrentEntry->Data, pData, sizeof(Type)))
		{
			// Falls gewünscht, wird der als Parameter angegebene Doppelzeiger
			// nun auf die Adresse der Listeneintragsstruktur gesetzt.
			if(ppOut != NULL) *ppOut = pCurrentEntry;
			return AGE_OK;
		}

		// Fortschreiten
		pCurrentEntry = pCurrentEntry->pNextEntry;
	}

	// Es wurde nichts gefunden!
	return AGE_NOT_FOUND;
}

// ******************************************************************
// Diese Methode löscht einen Eintrag in der Liste.
template <typename Type> Result List<Type>::DeleteEntry(ListEntry<Type>* pEntry)
{
	// Parameter prüfen
	if(pEntry == NULL) AGE_ERROR_NULL_POINTER("pEntry", AGE_ERROR);


	// Beim Löschen entsteht ein Loch in der Liste, welches nun "gestopft"
	// werden muss. Dabei spielt es eine Rolle, ob der Eintrag an erster
	// oder letzter Stelle oder irgendwo in der Mitte der Liste steht.
	if(pEntry == m_pFirstEntry &&
	   pEntry == m_pLastEntry)
	{
		// Der Eintrag ist der erste und einzige.
		m_pFirstEntry = NULL;
		m_pLastEntry = NULL;
	}
	else if(pEntry == m_pFirstEntry)
	{
		// Der Eintrag steht an erster Stelle.
		// Der neue erste Eintrag ist nun der Folgende des zu löschenden Eintrags.
		m_pFirstEntry = pEntry->pNextEntry;
		m_pFirstEntry->pPrevEntry = NULL;
	}
	else if(pEntry == m_pLastEntry)
	{
		// Der Eintrag steht an letzter Stelle.
		// Der neue letzte Eintrag ist nun der Vorherige des zu löschenden Eintrags.
		m_pLastEntry = pEntry->pPrevEntry;
		m_pLastEntry->pNextEntry = NULL;
	}
	else
	{
		// Der Eintrag steht irgendwo in der Mitte.
		// Der vorherige und der folgende Eintrag werden nun verknüpft.
		pEntry->pPrevEntry->pNextEntry = pEntry->pNextEntry;
		pEntry->pNextEntry->pPrevEntry = pEntry->pPrevEntry;
	}

	// Der Speicher für sie wurde beim Erstellen eines neuen Eintrags
	// reserviert und kann nun wieder freigegeben werden.
	AGE_SAFE_DELETE(pEntry);

	// Eintragszähler verringern
	m_iNumEntries--;

	return AGE_OK;
}

// ******************************************************************
// Diese Methode löscht die gesamte Liste.
template <typename Type> Result List<Type>::Clear()
{
	// Es wird so lange der erste Listeneintrag gelöscht, bis keiner mehr da ist.
	while(m_pFirstEntry != NULL) DeleteEntry(m_pFirstEntry);

	return AGE_OK;
}

// ******************************************************************
// Durchlaufen der Liste
template <typename Type> Result List<Type>::Traverse(Result (* pCallback)(ListEntry<Type>*))
{
	ListEntry<Type>* pCurrentEntry;

	// Parameter prüfen
	if(pCallback == NULL) AGE_ERROR_NULL_POINTER("pCallback", AGE_ERROR);


	// Die ganze Liste durchlaufen
	pCurrentEntry = m_pFirstEntry;

	while(pCurrentEntry != NULL)
	{
		// Rückruffunktion aufrufen
		if(pCallback(pCurrentEntry) == AGE_STOP) break;

		// Fortfahren
		pCurrentEntry = pCurrentEntry->pNextEntry;
	}

	return AGE_OK;
}

/*
template < class Type >
class List
{
public:
    ///////////////////////////////////////////////////////////////////////////////
    //Konstruktor
    ///////////////////////////////////////////////////////////////////////////////
    List()
    {
        _id = -1;
        _data = NULL;
        _tmp = NULL;
        _next = NULL;
        _before = NULL;
    }

    ///////////////////////////////////////////////////////////////////////////////
    //Methode anhaengen eines knotens an das 1. element
    ///////////////////////////////////////////////////////////////////////////////
    void addItem(Type input, int i)
    {
        _id = i;
        _tmp = this->getItem(i); // zum zwischenspeichern des vorhergehenden knotens
        addItem(input);
    }

    ///////////////////////////////////////////////////////////////////////////////
    //Methode zum anhaengen eines Knotens an alle weiteren
    ///////////////////////////////////////////////////////////////////////////////
    void addItem(Type input)
    {
        List < Type > * l = new List < Type >;
        //wenn die indexNummer von dem zwischengespeicherten aktuellen knotens(_tmp=this)
        // = 0 ist...
        if (_tmp == NULL)
        {
            //this->_next ist der neu auf dem heap angelegten knotens l
            _next = l;
            //next von dem neuen knoten wird mit 0 initialisiert, da
            //sonst in der methode length() die adresse des letzten
            //knoten->_next = 0xccccccccc != 0x000000000 (NULL)
            l->_next = NULL;
            //fuer die before-variable wird im 1. knoten NULL gesetzt
            _before = NULL;
            //die before-variable bekommt die adresse des 1.knotens
            l->_before = this;
            //index ist 0 (dadurch wird index 0 (this) nicht verwendet)
            l->_id = 0;
        }
        //bei allen weiteren knoten...
        else
        {
            
            //l->_next = NULL;
            //before bekommt den zuletzt erstellten knoten
            l->_before = _tmp;
            l->_id = (length()); //die index-variable von l bekommt den wert
            //der bis dahin berechneten leange (-1, da
            //1.knoten (_id 0) nicht belegt.
			//wird der zwischengespeicherten (am anfang 'this' ansonsten
            //der letzt erstellte) _next, der neue knoten zugewiesen
            _tmp->_next = l;

        }
        //der gerade erstellte knoten wird in this->_tmp zwischen-
        //gespeichert
        _tmp = l;
        //der neue knoten bekommt seine information
        l->_data = new Type(input);
    }

    ///////////////////////////////////////////////////////////////////////////////
    //Methode liefert die adresse der _data-vaialblen des n.ten knotens
    ///////////////////////////////////////////////////////////////////////////////
    Type * getData() { return _data; }

    Type * getData(int n)
    {
        //die variable tmp bekommt die adresse von dem n-ten knotens
        //aus getItem(int n)
        List < Type > * tmp = getItem(n);
        //und liefert aber die adresse auf seine enthaltene information
        return (tmp->_data);
    }

    ////////////////////////////////////////////////////////////////////
    //Methode liefert die adresse des n.ten knotens
    ////////////////////////////////////////////////////////////////////
    List< Type > * getItem(int n)
    {
        //neuen pointer mit der adresse des anfangs (this=dieser adresse)
        List < Type > * tmp = this;
        if (this == NULL)
        {
            tmp = NULL;
            return tmp;
        }
		
        //solange wie der index nicht = n wird...
        while (tmp->_id != n)
        {
            //wird in der liste ein schritt weiter gelaufen
            tmp = tmp->_next;

        }
        //ansonsten wird die adresse des gefundenen knotes zurueckgegeben
        return tmp;
    }

    ////////////////////////////////////////////////////////////////////
    //Methode liefert die ganszahlige laenge der listenstruktur
    ////////////////////////////////////////////////////////////////////
    int length()
    {
        //neuen pointer mit der adresse des anfangs (this=dieser adresse)
        List < Type > * tmp = this->_next;
        int i = 0;
        //solange tmp ungleich NULL wird...
        while (tmp != NULL)
        {
            //zaehle i ein hoch...
            i++;
            //und gehe in der liste einen schritt weiter
            tmp = tmp->_next;
        }
        //ansonsten wird i-1 zurueckgegeben
        return (i);
    }

    int reInitID()
    {
        //neuen pointer mit der adresse des anfangs (this=dieser adresse)
        List < Type > * tmp = this;
        int i = -1;
        //solange tmp ungleich NULL wird...
        while (tmp != NULL)
        {
            //nummerriert die id's noch mal durch
            tmp->_id = i;
            //zaehle i ein hoch...
            i++;
            //und gehe in der liste einen schritt weiter
            tmp = tmp->_next;
        }
        //ansonsten wird i zurueckgegeben
        return (i);
    }

    ////////////////////////////////////////////////////////////////////
    //Methode zur iterativen handhabung der liste
    ////////////////////////////////////////////////////////////////////
    List< Type > * nextItem() { return this->_next; }

    List< Type > * lastItem() { return this->_before; }

    ////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////
    void addList(List< Type > * list)
    {
        //for (int i=1;i<this->length();i++) {
        List < Type > * tmp = NULL;
        //if (this == NULL){this = list;}
        //else{
        tmp = this->getItem((this->reInitID()) );
        if (!(list->_data))
        {
            tmp->_next = list->_next;
        }
        else { tmp->_next = list; }
        //die liste wirde ohne das 1. element angeheangt. auf dem ruckweg ist es
        //aber da
        list->_before = tmp;
        //this->setItem((this->length())-1) = tmp;
        //}


    }

    ////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////
    int getID(int n) { return this->getItem(n)->_id; }

    ////////////////////////////////////////////////////////////////////
    //zwei elemente vertauschen
    ////////////////////////////////////////////////////////////////////
    void swapItem(char n)
    {
        List < Type > * tmpA = NULL;
        List < Type > * tmpB = NULL;
        List < Type > * tmpC = NULL;
        // List<Type> *nextTmp;
        switch (n)
        {

            case 'N':
                tmpA = _next;
                tmpB = _next->lastItem();
                _next = _next->nextItem();
                if (_before != NULL) { _before->_next = tmpA; }
                else { this->_next = tmpA; }
                if (!(_next == NULL)) { _next->_before = tmpB; }
                tmpC = _before;
                _before = tmpA;
                _before->_next = this;
                _before->_before = tmpC;
                // if(_next->_next = NULL){

                break;
            case 'L':
                if (this->_before != NULL)
                {

                    _before->swapItem('N');

                }
                break;
        }
    }

    void rebuild()
    {

        int n = this->reInitID();
        this->_tmp = this->getItem(n)->_next;
    }


    ~List() { }

private:
    //Variable zum speichern des knoten-indexes
    int _id;
    //Variable zum speichern des knoten-inhaltes
    Type * _data;
    //Variable zum zwischenspeichern des aktuellen knotens
    //beim erzeugen -> addItem();
    List< Type > * _tmp;
    //Adresse des nexten knotens
    List< Type > * _next;
    //Adresse des vorhergehenden knotens
    List< Type > * _before;
};
*/

/**
 * Die Objectorientierte Variante funktioniert leider aus irgendeinem
 * beschissenen grund nicht daher nur hier
 * ///////////////////////////////////////////////////////////////////////////////
 * //Konstruktor ///////////////////////////////////////////////////////////////////////////////
 * template <class Type> List<Type>::List(){
 * _id = 0; _data = NULL; _tmp = NULL; _next = NULL; _before = NULL; }
 * ///////////////////////////////////////////////////////////////////////////////
 * //Methode anhaengen eines knotens an das 1. element
 * ///////////////////////////////////////////////////////////////////////////////
 * template <class Type> void List<Type>::addItem(Type input, int i) {
 * _id = i; _tmp = this; // zum zwischenspeichern des vorhergehenden knotens
 * _data = NULL; addItem(input); }
 * ///////////////////////////////////////////////////////////////////////////////
 * ////Methode zum anhaengen eines Knotens an alle weiteren
 * ///////////////////////////////////////////////////////////////////////////////
 * template <class Type> void List<Type>::addItem(Type input) {
 * List<Type> *l = new List<Type>;
 * //wenn die indexNummer von dem zwischengespeicherten aktuellen
 * knotens(_tmp=this) // = 0 ist... if(_tmp==NULL){
 * //this->_next ist der neu auf dem heap angelegten knotens l _next = l ;
 * //next von dem neuen knoten wird mit 0 initialisiert, da //sonst in der
 * methode length() die adresse des letzten //knoten->_next = 0xccccccccc !=
 * 0x000000000 (NULL) l->_next = NULL;
 * //fuer die before-variable wird im 1. knoten NULL gesetzt _before = NULL;
 * //die before-variable bekommt die adresse des 1.knotens l->_before = this;
 * //index ist 1 (dadurch wird index 0 (this) nicht verwendet) l->_id = 1; }
 * //bei allen weiteren knoten... else{
 * //wird der zwischengespeicherten (am anfang 'this' ansonsten
 * //der letzt erstellte) _next, der neue knoten zugewiesen _tmp->_next = l;
 * l->_next = NULL; //before bekommt den zuletzt erstellten knoten
 * l->_before = _tmp; l->_id = (length()-1); //die index-variable von l
 * bekommt den wert //der bis dahin berechneten leange (-1, da //1.knoten (_id
 * 0) nicht belegt. } //der gerade erstellte knoten wird in this->_tmp zwischen-
 * //gespeichert _tmp = l; //der neue knoten bekommt seine information
 * l->_data = new Type(input); }
 * ///////////////////////////////////////////////////////////////////////////////
 * //Methode liefert die adresse der _data-vaialblen des aktuellen knotens
 * ///////////////////////////////////////////////////////////////////////////////
 * template <class Type> Type *List<Type>::getData() { return _data; }
 * ///////////////////////////////////////////////////////////////////////////////
 * //Methode liefert die adresse der _data-vaialblen des n.ten knotens
 * ///////////////////////////////////////////////////////////////////////////////
 * template <class Type> Type *List<Type>::getData(int n) {
 * //die variable tmp bekommt die adresse von dem n-ten knotens //aus
 * getItem(int n) List<Type>*tmp = getItem(n);
 * //und liefert aber die adresse auf seine enthaltene information
 * return (tmp->_data); } ///////////////////////////////////////////////////////////////////////////////
 * //Methode liefert die adresse des n.ten knotens
 * ///////////////////////////////////////////////////////////////////////////////
 * template <class Type> List<Type> *List<Type>::getItem(int n) {
 * //neuen pointer mit der adresse des anfangs (this=dieser adresse)
 * List<Type> *tmp = this; if (this == NULL) {tmp = NULL;return tmp;}
 * //solange wie der index nicht = n wird... while(tmp->_id != n) {
 * //wird in der List ein schritt weiter gelaufen tmp = tmp->_next; }
 * //ansonsten wird die adresse des gefundenen knotes zurueckgegeben
 * return tmp; } ///////////////////////////////////////////////////////////////////////////////
 * ////Methode liefert die ganszahlige laenge der Listnstruktur
 * ///////////////////////////////////////////////////////////////////////////////
 * template <class Type> int List<Type>::length() {
 * //neuen pointer mit der adresse des anfangs (this=dieser adresse)
 * List<Type> *tmp= this; int i =0; //solange tmp ungleich NULL wird...
 * while (tmp!=NULL) { //zaehle i ein hoch... i++;
 * //und gehe in der liste einen schritt weiter tmp = tmp->_next; }
 * //ansonsten wird i-1 zurueckgegeben return (i); }
 * template <class Type> int List<Type>::length(int n) {
 * //neuen pointer mit der adresse des anfangs (this=dieser adresse)
 * List<Type> *tmp= this; int i =0; //solange tmp ungleich NULL wird...
 * while (tmp!=NULL) { //nummerriert die id's noch mal durch tmp->_id=i;
 * //zaehle i ein hoch... i++; //und gehe in der liste einen schritt weiter
 * tmp = tmp->_next; } //ansonsten wird i-1 zurueckgegeben return (i); }
 * template <class Type>List<Type>* List<Type>::nextItem(){
 * return this->_next; } template <class Type>List<Type>*
 * List<Type>::lastItem(){ return this->_before; }
 * template <class Type>int List<Type>::getID(int n) {
 * return this->getItem(n)->_id; }
 * template <class Type>void List<Type>::addList(List<Type> *list) {
 * //for (int i=1;i<this->length();i++) { List<Type> *tmp=NULL;
 * //if (this == NULL){this = list;}
 * //else{      tmp = this->getItem((this->length(1))-1); if(!(list->_data)){
 * tmp->_next = list->_next; }else{tmp->_next = list;}
 * //die List wirde ohne das 1. element angeheangt. auf dem ruckweg ist es
 * //aber da list->_before = tmp; //this->setItem((this->length())-1) =
 * tmp; //} };
 */


#endif // !defined(AFX_TEMPLATE_H__C9B12662_C636_4257_B7BC_FF9737A99A91__INCLUDED_)
