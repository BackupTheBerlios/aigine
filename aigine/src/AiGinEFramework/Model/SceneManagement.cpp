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

}



/*********************P R I V A T E***************************************/
