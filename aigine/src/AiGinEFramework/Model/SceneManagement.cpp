/****************************************************************
*						S C E N E M E N A G E M E N T . C P P
*	@author
*	@description
*
*
***************************************************************/


/**********************I N C L U D E*********************************/
#include "SceneManagement.h"
#include "Renderer.h"
#include "GraphicPipeline.h"
#include "ageObject3DS.h"
#include "../Util/loader3DS/Object3DS.h"


/*********************P U B L I C***************************************/


/**********************C O N S T R U C T O R**********************/
SceneManagement::SceneManagement(): SceneGraph() {	
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
void SceneManagement::display() {
	this->initDisplay(); // von Klasse GraphicPipeline
	this->draw(); // von Klasse SceneGraph
}

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
