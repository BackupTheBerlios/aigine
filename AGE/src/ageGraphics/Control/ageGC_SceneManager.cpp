/****************************************************************
*						S C E N E M E N A G E M E N T . C P P
*	@author
*	@description
*
*
***************************************************************/


/**********************I N C L U D E*********************************/
#include "ageGC_SceneManager.h"
#include "ageGraphics/View/ageGV_GraphicPipeline.h"
#include "../Model/AiGinEObject.h"
class ageGC_Object;


/*********************P U B L I C***************************************/


/**********************C O N S T R U C T O R**********************/
ageGC_SceneManager::ageGC_SceneManager(): SceneGraph(), GraphicPipeline(){	
}

/********load(String fileType, String fileName, object insertAt)**********/
/*void SceneManagement::load(string fileType, string fileName) {
	Object3DS* my3DSObject = new Object3DS();
	char* name = (char*)fileName.c_str();
	my3DSObject->loadObject(name);
	my3DSObject->toString();
	// speichern im TestObjekt des SceneGraph => durch Baum ersetzen
	this->setTmpTestObject(my3DSObject);
}*/

/**********************display**********************/
void ageGC_SceneManager::display() {
	this->initDisplay(); // von Klasse GraphicPipeline
	this->draw(); // von Klasse SceneGraph
	//this->ImageMask();//hier wir die Image mask geladen !!! macht sie aber noch nicht so richtig
	glutSwapBuffers(); // Tauschen der Bildspeicher - for double buffer

}
/*
AiGinEObject * SceneManagement::addObject3DS(string fileName)
{
	ageObject3DS* my3DSObject = new ageObject3DS(fileName);
	this->addRoot(my3DSObject);
	return my3DSObject;
}

AiGinEObject * SceneManagement::addObject3DS(string fileName, AiGinEObject* parent, string kind) {
	ageObject3DS* my3DSObject = new ageObject3DS(fileName);

	if(kind == "child") {
		this->addChild(parent, my3DSObject);
	}
	else if(kind == "next") {
		this->addNext(parent, my3DSObject);
	}
	else {
		//error-ausnahme
	}
	return my3DSObject;
}
*/
ageGC_Object * ageGC_SceneManager::addObjectPrim(ageGC_Object* obj)
{
	this->addRoot(obj);
	return obj;
}

ageGC_Object * ageGC_SceneManager::addObjectPrim(ageGC_Object* obj, ageGC_Object* parent, string kind) {
	if(kind == "child") {
		this->addChild(parent, obj);
	}
	else if(kind == "next") {
		this->addNext(parent, obj);
	}
	else {
		//error-ausnahme
	}
	return obj;
}


