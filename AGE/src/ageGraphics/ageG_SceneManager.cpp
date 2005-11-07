/* Generated by Together */

#include "ageG_SceneManager.h"
#include "ageG_Object.h"
#include "ageG_Object.h"
/**********************C O N S T R U C T O R**********************/
ageG_SceneManager::ageG_SceneManager(): SceneGraph(), GraphicPipeline(){	
}
ageG_Object * ageG_SceneManager::addObjectPrim(ageG_Object* obj)
{
	this->addRoot(obj);
	return obj;
}
ageG_Object * ageG_SceneManager::addObjectPrim(ageG_Object* obj, ageG_Object* parent, string kind) {
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
/**********************display**********************/
void ageG_SceneManager::display() {
	this->initDisplay(); // von Klasse GraphicPipeline
	this->draw(); // von Klasse SceneGraph
	//this->ImageMask();//hier wir die Image mask geladen !!! macht sie aber noch nicht so richtig
	glutSwapBuffers(); // Tauschen der Bildspeicher - for double buffer

}