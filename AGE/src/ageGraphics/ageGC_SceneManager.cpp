/* Generated by Together */

#include "ageGC_SceneManager.h"
#include "ageGraphics/View/ageGV_GraphicPipeline.h"
#include "ageGraphics/ageGC_Object.h"
/**********************C O N S T R U C T O R**********************/
ageGC_SceneManager::ageGC_SceneManager(): SceneGraph(), GraphicPipeline(){	
}
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
/**********************display**********************/
void ageGC_SceneManager::display() {
	this->initDisplay(); // von Klasse GraphicPipeline
	this->draw(); // von Klasse SceneGraph
	//this->ImageMask();//hier wir die Image mask geladen !!! macht sie aber noch nicht so richtig
	glutSwapBuffers(); // Tauschen der Bildspeicher - for double buffer

}
