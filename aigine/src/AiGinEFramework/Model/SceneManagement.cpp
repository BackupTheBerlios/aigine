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
#include "../Util/loader3DS/Object3DS.h"


/*********************P U B L I C***************************************/


/**********************C O N S T R U C T O R**********************/
SceneManagement::SceneManagenent() {	
}

/********load(String fileType, String fileName, object insertAt)**********/
void SceneManagement::load(string fileType, string fileName) {
	Object3DS* my3DSObject = new Object3DS();
	char* name = (char*)fileName.c_str();
	my3DSObject->loadObject(name);
	my3DSObject->toString();
	// speichern im TestObjekt des SceneGraph => durch Baum ersetzen
	this->setTmpTestObject(my3DSObject);
}

/**********************display**********************/
void SceneManagement::display() {
	this->initDisplay(); // von Klasse GraphicPipeline
	this->draw(); // von Klasse SceneGraph
}