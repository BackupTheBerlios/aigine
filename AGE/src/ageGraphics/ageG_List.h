// Template.h: Schnittstelle f�r die Klasse Template.
//
//////////////////////////////////////////////////////////////////////

/**
 * eine Schablone fuer eine doppelt verkette Liste
 * @autor Tobias Harpering @fhtw s0503936
 * @description: Doppelt verkettete Liste als Template
 * Handhabung wie bei einem Vector (nur einfacher vorallem beim debuggen)
 * Liste < Objekt jeglicher Art > 'bezeichner' = new Liste < Object >;
  * 
  * Index und benutzung laeuft nun 'edlich' ab 0
 */

#ifndef _LIST_H_
#define _LIST_H_

#include <iostream>
#include "ageG_List.h"
using namespace std;

template < class Type >
class ageG_List
{
public:
    ///////////////////////////////////////////////////////////////////////////////
    //Konstruktor
    ///////////////////////////////////////////////////////////////////////////////
    ageG_List()
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
    ageG_List< Type > * getItem(int n)
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
    ageG_List< Type > * nextItem() { return this->_next; }

    ageG_List< Type > * lastItem() { return this->_before; }

    ////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////
    void addList(ageG_List< Type > * list)
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


    ~ageG_List() { }

private:
    //Variable zum speichern des knoten-indexes
    int _id;
    //Variable zum speichern des knoten-inhaltes
    Type * _data;
    //Variable zum zwischenspeichern des aktuellen knotens
    //beim erzeugen -> addItem();
    ageG_List< Type > * _tmp;
    //Adresse des nexten knotens
    ageG_List< Type > * _next;
    //Adresse des vorhergehenden knotens
    ageG_List< Type > * _before;
};

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
