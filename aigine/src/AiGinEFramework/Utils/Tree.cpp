/* Util Bin-Tree */
////////////////////////////////////////////////////////////////////////
//						T R E E . C P P
//--------------------------------------------------------------------//
// @description
// @author Sebastian Blaum
// @modified by
// @date
//
////////////////////////////////////////////////////////////////////////
#include "Tree.h"
#include "windows.h"
#include <iostream>
#include <string>
using namespace std;

Tree::Tree() {
	this->knotListPreorder = new Liste<knot*>;
	this->knotListInorder = new Liste<knot*>;
	this->knotListPostorder = new Liste<knot*>;
	this->knotListLevelorder = new Liste<knot*>;
	this->actualListElement = 1;

	this->makeTest();
}

/**
* @description 
* @param 
* @return	
* @modified by 
* note: 
*/

void Tree::makeTest() {
	string p = "p";
	string m = "m";
	string l = "l";
	string s = "s";
	string e = "e";
	string a = "a";
	string r = "r";
	string t = "t";
	
	knot* newKnot;
	
	//level 1
	this->rootKnot = this->initKnot(new knot);
	this->rootKnot->data = &p;
	
	//level 2 - links
	newKnot = this->initKnot(new knot);
	newKnot->data = &m;
	this->addKnotLeft(this->rootKnot, newKnot);
	
	//level 2 - rechts
	newKnot = this->initKnot(new knot);
	newKnot->data = &l;
	this->addKnotRight(this->rootKnot, newKnot);
	
	//level 3 - links - links
	newKnot = this->initKnot(new knot);
	newKnot->data = &s;
	this->addKnotLeft(this->rootKnot->left, newKnot);
	
	//level 3 - rechts - rechts
	newKnot = this->initKnot(new knot);
	newKnot->data = &e;
	this->addKnotRight(this->rootKnot->right, newKnot);
	
	//level 4 - links - links - links
	newKnot = this->initKnot(new knot);
	newKnot->data = &a;
	this->addKnotLeft(this->rootKnot->left->left, newKnot);
	
	//level 4 - links - links - rechts
	newKnot = this->initKnot(new knot);
	newKnot->data = &a;
	this->addKnotRight(this->rootKnot->left->left, newKnot);
	
	//level 4 - rechts - rechts - rechts
	newKnot = this->initKnot(new knot);
	newKnot->data = &r;
	this->addKnotRight(this->rootKnot->right->right, newKnot);
	
	//level 5 - rechts - rechts - rechts - links
	newKnot = this->initKnot(new knot);
	newKnot->data = &t;
	this->addKnotLeft(this->rootKnot->right->right->right, newKnot);
	
	//level 5 - rechts - rechts - rechts - rechts
	newKnot = this->initKnot(new knot);
	newKnot->data = &e;
	this->addKnotRight(this->rootKnot->right->right->right, newKnot);
	
	//level 6 - rechts - rechts - rechts - rechts - links
	newKnot = this->initKnot(new knot);
	newKnot->data = &e;
	this->addKnotLeft(this->rootKnot->right->right->right->right, newKnot);
	
	
	cout << endl << "Preorder-Traversierung:" << endl;
	this->buildPreorder(this->rootKnot);
	knot* tmp;
	while((tmp = this->getKnotPreorder()) != NULL) {
		cout << *(string*)(tmp->data) << " ";
	}
	
	cout << endl << "Inorder-Traversierung:" << endl;
	this->buildInorder(this->rootKnot);
	this->rewindLists();
	while((tmp = this->getKnotInorder()) != NULL) {
		cout << *(string*)(tmp->data) << " ";
	}
	
	cout << endl << "Postorder-Traversierung:" << endl;
	this->buildPostorder(this->rootKnot);
	this->rewindLists();
	while((tmp = this->getKnotPostorder()) != NULL) {
		cout << *(string*)(tmp->data) << " ";
	}
	cout << endl;
}

/**
* @description 
* @param 
* @return	
* note: 
*/

knot* Tree::initKnot(knot* k) {
	k->data = NULL;
	k->left = NULL;
	k->right = NULL;
	return k;
}

/**
* @description 
* @param 
* @return	
* @modified by 
* note: 
*/

bool Tree::addKnotLeft(knot* paren, knot* child) {
	if(paren != NULL) {
		paren->left = child;
		return true;
	} else {
		return false;
	}
}

/**
* @description 
* @param 
* @return	
* @modified by 
* note: 
*/

bool Tree::addKnotRight(knot* paren, knot* child) {
	if(paren != NULL) {
		paren->right = child;
		return true;
	} else {
		return false;
	}
}

/**
* @description 
* @param 
* @return	
* @modified by 
* note: 
*/

knot* Tree::getRootKnot() {
	return this->rootKnot;
}

/**
* @description 
* @param 
* @return	
* @modified by 
* note: 
*/

knot* Tree::getTemporaryKnot() {
	return this->temporaryKnot;
}

/**
* @description 
* @param 
* @return	
* @modified by 
* note: 
*/

void Tree::rewindLists() {
	this->actualListElement = 1;
}

/**
* @description 
* @param 
* @return	
* @modified by 
* note: 
*/

knot* Tree::getKnotPreorder() {
	if(this->actualListElement < this->knotListPreorder->length()) {
		knot* tmp = *this->knotListPreorder->getData(this->actualListElement);
		this->actualListElement++;
		return tmp;
	} else {
		this->actualListElement = 1;
		return NULL;
	}
}

/**
* @description 
* @param 
* @return	
* @modified by 
* note: 
*/

knot* Tree::getKnotInorder() {
	if(this->actualListElement < this->knotListInorder->length()) {
		knot* tmp = *this->knotListInorder->getData(this->actualListElement);
		this->actualListElement++;
		return tmp;
	} else {
		this->actualListElement = 1;
		return NULL;
	}
}

	/**
	* @description 
	* @param 
	* @return	
	* @modified by 
	* note: 
	*/
knot* Tree::getKnotPostorder() {
	if(this->actualListElement < this->knotListPostorder->length()) {
		knot* tmp = *this->knotListPostorder->getData(this->actualListElement);
		this->actualListElement++;
		return tmp;
	} else {
		this->actualListElement = 1;
		return NULL;
	}
}

	/**
	* @description 
	* @param 
	* @return	
	* @modified by 
	* note: 
	*/
knot* Tree::getKnotLevelorder() {
	return NULL;
}

	/**
	* @description 
	* @param 
	* @return	
	* @modified by 
	* note: 
	*/
void Tree::buildPreorder(knot* k) {
	this->knotListPreorder->addItem(k);
//	cout << *(string*)(k->data) << endl;
	if(k->left != NULL) this->buildPreorder(k->left);
	if(k->right!= NULL) this->buildPreorder(k->right);

}

void Tree::buildInorder(knot* k) {
	if(k->left != NULL) this->buildInorder(k->left);
	this->knotListInorder->addItem(k);
//	cout << *(string*)(k->data) << endl;
	if(k->right!= NULL) this->buildInorder(k->right);
}
void Tree::buildPostorder(knot* k) {
	if(k->left != NULL) this->buildPostorder(k->left);
	if(k->right!= NULL) this->buildPostorder(k->right);
	this->knotListPostorder->addItem(k);
//	cout << *(string*)(k->data) << endl;
}
void Tree::buildLevelorder(knot* k) {
}
