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


/**********************display**********************/
void SceneManagement::display() {
	this->initDisplay(); // von Klasse GraphicPipeline
	this->draw(); // von Klasse SceneGraph
}

/********load(String fileType, String fileName, object insertAt)**********/
void SceneManagement::load(string fileType, string fileName) {
	Object3DS* my3DSObject = new Object3DS();
	char* name = (char*)fileName.c_str();
	my3DSObject->loadObject(name);
	my3DSObject->toString();
	this->setTmpTestObject(my3DSObject); // speichern im TestObjekt des
															// SceneGraph => durch Baum ersetzen
}




/**********************getWindowSizt**********************/
//int * SceneManagement::getWindowSize() {
	
//}

/*********************P R I V A T E***************************************/
