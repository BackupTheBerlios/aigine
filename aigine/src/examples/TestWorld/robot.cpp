// robot.cpp: implementation of the robot class.
//
//////////////////////////////////////////////////////////////////////

#include "robot.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

robot::robot(SceneManagement* sm) {
	this->sceneMan = sm;
}

robot::~robot() {
}

void robot::drawRobot() {

		//Huefte
	AiGinEObject* huefte = this->sceneMan->addObjectPrim(new ageObjectPrim());
	((ageObjectPrim*)huefte)->setCube(0.5);
	((ageObjectPrim*)huefte)->setColor(255,0,0);
	huefte->setTranslation(new Translation3D(0,6,0));
	huefte->setScale(new Scale3D(6,1,1));
	this->Huefte = huefte;

	//Bein_Rechts
	//Gelenk_Oben_Rechts
	AiGinEObject* gelenk_o_r = this->sceneMan->addObjectPrim(new ageObjectPrim(),huefte,"child");
	((ageObjectPrim*)gelenk_o_r)->setSphere(0.25,50,50);
	((ageObjectPrim*)gelenk_o_r)->setColor(0,255,0);
	gelenk_o_r->setTranslation(new Translation3D(1.25,-0.5,0));
	gelenk_o_r->setScale(new Scale3D(1,1,1));
	gelenk_o_r->setRotation(new Rotation3D(-45,1,0,0));
	this->BeinR = gelenk_o_r;

	//Segment_Oben_Rechts
	AiGinEObject* seg_o_r = this->sceneMan->addObjectPrim(new ageObjectPrim(),gelenk_o_r,"child");
	((ageObjectPrim*)seg_o_r)->setCube(0.5);
	((ageObjectPrim*)seg_o_r)->setColor(255,0,0);
	seg_o_r->setTranslation(new Translation3D(0,-1.25,0));
	seg_o_r->setScale(new Scale3D(1,4,1));

	//Gelenk_Unten_Rechts
	AiGinEObject* gelenk_u_r = this->sceneMan->addObjectPrim(new ageObjectPrim(),seg_o_r,"child");
	((ageObjectPrim*)gelenk_u_r)->setSphere(0.25,50,50);
	((ageObjectPrim*)gelenk_u_r)->setColor(0,255,0);
	gelenk_u_r->setTranslation(new Translation3D(0,-1.25,0));
	gelenk_u_r->setScale(new Scale3D(1,1,1));
	gelenk_u_r->setRotation(new Rotation3D(45,1,0,0));
	this->KnieR = gelenk_u_r;

	//Segment_Unten_Rechts
	AiGinEObject* seg_u_r = this->sceneMan->addObjectPrim(new ageObjectPrim(),gelenk_u_r,"child");
	((ageObjectPrim*)seg_u_r)->setCube(0.5);
	((ageObjectPrim*)seg_u_r)->setColor(255,0,0);
	seg_u_r->setTranslation(new Translation3D(0,-1.25,0));
	seg_u_r->setScale(new Scale3D(1,4,1));



	//Bein_Links
	//Gelenk_Oben_Links	
	AiGinEObject* gelenk_o_l = this->sceneMan->addObjectPrim(new ageObjectPrim(),gelenk_o_r,"next");
	((ageObjectPrim*)gelenk_o_l)->setSphere(0.25,50,50);
	((ageObjectPrim*)gelenk_o_l)->setColor(0,255,0);
	gelenk_o_l->setTranslation(new Translation3D(-1.25,-0.5,0));
	gelenk_o_l->setScale(new Scale3D(1,1,1));
	gelenk_o_l->setRotation(new Rotation3D(0,1,0,0));
	this->BeinL = gelenk_o_l;

	//Segment_Oben_Links
	AiGinEObject* seg_o_l = this->sceneMan->addObjectPrim(new ageObjectPrim(),gelenk_o_l,"child");
	((ageObjectPrim*)seg_o_l)->setCube(0.5);
	((ageObjectPrim*)seg_o_l)->setColor(255,0,0);
	seg_o_l->setTranslation(new Translation3D(0,-1.25,0));
	seg_o_l->setScale(new Scale3D(1,4,1));

	//Gelenk_Unten_Links
	AiGinEObject* gelenk_u_l = this->sceneMan->addObjectPrim(new ageObjectPrim(),seg_o_l,"child");
	((ageObjectPrim*)gelenk_u_l)->setSphere(0.25,50,50);
	((ageObjectPrim*)gelenk_u_l)->setColor(0,255,0);
	gelenk_u_l->setTranslation(new Translation3D(0,-1.25,0));
	gelenk_u_l->setScale(new Scale3D(1,1,1));
	gelenk_u_l->setRotation(new Rotation3D(0,1,0,0));
	this->KnieL = gelenk_u_l;

	//Segment_Unten_Links
	AiGinEObject* seg_u_l = this->sceneMan->addObjectPrim(new ageObjectPrim(),gelenk_u_l,"child");
	((ageObjectPrim*)seg_u_l)->setCube(0.5);
	((ageObjectPrim*)seg_u_l)->setColor(255,0,0);
	seg_u_l->setTranslation(new Translation3D(0,-1.25,0));
	seg_u_l->setScale(new Scale3D(1,4,1));

}

void robot::animRobot() {
	this->Huefte->getRotation()->y = 1;
	this->Huefte->getRotation()->angle = ((int)this->Huefte->getRotation()->angle) % 360 + 1;
	this->sceneMan->display();

}
