/* Util Bin-Tree */

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

	this->makeTest();
}

void Tree::makeTest() {
	string a = "a";
	string b = "b";
	string c = "c";
	string d = "d";
	string e = "e";
	string f = "f";
	string g = "g";

	this->rootKnot = new knot;
	this->initKnot(this->rootKnot);
	this->rootKnot->data = &a;

	this->rootKnot->left = new knot;
	this->initKnot(this->rootKnot->left);
	this->rootKnot->left->data = &b;
	
	this->rootKnot->right = new knot;
	this->initKnot(this->rootKnot->right);
	this->rootKnot->right->data = &c;


	this->rootKnot->left->left = new knot;
	this->initKnot(this->rootKnot->left->left);
	this->rootKnot->left->left->data = &d;

	this->rootKnot->left->right = new knot;
	this->initKnot(this->rootKnot->left->right);
	this->rootKnot->left->right->data = &e;

	this->rootKnot->right->left = new knot;
	this->initKnot(this->rootKnot->right->left);
	this->rootKnot->right->left->data = &f;

	this->rootKnot->right->right = new knot;
	this->initKnot(this->rootKnot->right->right);
	this->rootKnot->right->right->data = &g;


	cout << "TestTree:" << endl;
	cout << *(string*)(this->rootKnot->data) << endl;
	cout << *(string*)(this->rootKnot->left->data) << " , " << *(string*)(this->rootKnot->right->data) << endl;
	cout << *(string*)(this->rootKnot->left->left->data) << " , " << *(string*)(this->rootKnot->left->right->data) << " , ";
	cout << *(string*)(this->rootKnot->right->left->data) << " , " << *(string*)(this->rootKnot->right->right->data);
	cout << endl;

	cout << "Preorder-Traversierung:" << endl;
	this->buildPreorder(this->rootKnot);
	for(int i = 1; i<this->knotListPreorder->length();i++) {
		knot* tmp = *this->knotListPreorder->getData(i);
		cout << *(string*)(tmp->data) << endl;
	}

	cout << "Inorder-Traversierung:" << endl;
	this->buildInorder(this->rootKnot);
	for(int j = 1; j<this->knotListInorder->length();j++) {
		knot* tmp = *this->knotListInorder->getData(j);
		cout << *(string*)(tmp->data) << endl;
	}

	cout << "Postorder-Traversierung:" << endl;
	this->buildPostorder(this->rootKnot);
	for(int k = 1; k<this->knotListPostorder->length();k++) {
		knot* tmp = *this->knotListPostorder->getData(k);
		cout << *(string*)(tmp->data) << endl;
	}
}

void Tree::initKnot(knot* k) {
	k->data = NULL;
	k->left = NULL;
	k->right = NULL;
}


bool Tree::addKnotLeft(knot* paren, knot* child) {
	if(paren != NULL) {
		paren->left = child;
		return true;
	} else {
		return false;
	}
}

bool Tree::addKnotRight(knot* paren, knot* child) {
	if(paren != NULL) {
		paren->right = child;
		return true;
	} else {
		return false;
	}
}

knot* Tree::getRootKnot() {
	return this->rootKnot;
}
knot* Tree::getTemporaryKnot() {
	return this->temporaryKnot;
}

knot* Tree::getKnotPreorder() {
	return NULL;
}
knot* Tree::getKnotInorder() {
	return NULL;
}
knot* Tree::getKnotPostorder() {
	return NULL;
}
knot* Tree::getKnotLevelorder() {
	return NULL;
}

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
void Tree::addToList(knot* k){}

/*
void main(int argc, char** argv) {
	Tree* t = new Tree();
}
*/