/* Util Bin-Tree */

#include "Tree.h"
#include "windows.h"

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
void Tree::addToList(knot* k){}
