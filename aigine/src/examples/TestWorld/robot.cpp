// robot.cpp: implementation of the robot class.
//
//////////////////////////////////////////////////////////////////////

#include "robot.h"
#include "../../AiGinEFramework/Utils/AiGinEObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

robot::robot(SceneManagement* sm) {
	this->sceneMan = sm;
}

robot::~robot() {
}


//================= Base(frame) ======================

AiGinEObject* robot::drawBaseLegs(AiGinEObject* parent, string kind) {
AiGinEObject* cube;
	if(parent != NULL) {
		cube = this->sceneMan->addObjectPrim(new ageObjectPrim(),parent,kind);
	} else {
		cube = this->sceneMan->addObjectPrim(new ageObjectPrim());
	}
	((ageObjectPrim*)cube)->setCube(1.0);
	((ageObjectPrim*)cube)->setColor(255,0,0);
	cube->setScale(new Scale3D(BASE_WIDTH, BASE_HEIGHT, TORSO));

	AiGinEObject* right_leg = drawLeg(cube,"child");
	right_leg->setTranslation(new Translation3D(TORSO_WIDTH * 0.33 , - (BASE_HEIGHT), 0.0));
	this->BeinR = right_leg;
	AiGinEObject* left_leg = drawLeg(right_leg, "next");
	left_leg->setTranslation(new Translation3D(- TORSO_WIDTH * 0.33 , - (BASE_HEIGHT), 0.0));
	this->BeinL = left_leg;

	return cube;
}

AiGinEObject* robot::drawLeg(AiGinEObject* parent, string kind) {

	AiGinEObject* upperLeg = drawUpperLeg(parent, kind);
	return upperLeg;
}

AiGinEObject* robot::drawUpperLeg(AiGinEObject* parent, string kind) {
	//Gelenk_Oben
	AiGinEObject* sphere = this->sceneMan->addObjectPrim(new ageObjectPrim(),parent,kind);
	((ageObjectPrim*)sphere)->setSphere(1.0,8,8);
	((ageObjectPrim*)sphere)->setColor(0,255,0);
	sphere->setScale(new Scale3D(UP_LEG_JOINT_SIZE,UP_LEG_JOINT_SIZE,UP_LEG_JOINT_SIZE));

	//Segment_Oben
	AiGinEObject* cube = this->sceneMan->addObjectPrim(new ageObjectPrim(),sphere,"child");
	((ageObjectPrim*)cube)->setCube(1.0);
	((ageObjectPrim*)cube)->setColor(255,0,0);
	cube->setScale(new Scale3D(UP_LEG_WIDTH, UP_LEG_HEIGHT, UP_LEG_WIDTH));
	cube->setTranslation(new Translation3D(0.0,- UP_LEG_HEIGHT * 0.75,0.0));

	AiGinEObject* lowerLeg = drawLowerLeg(cube, "child");
	lowerLeg->setTranslation(new Translation3D(0.0,- UP_LEG_HEIGHT * 0.75,0.0));
	if(kind == "child") {
		this->KnieR = lowerLeg;
	} else {
		this->KnieL = lowerLeg;
	}
	return sphere;
}

AiGinEObject* robot::drawLowerLeg(AiGinEObject* parent, string kind) {
	//Gelenk_Oben
	AiGinEObject* sphere = this->sceneMan->addObjectPrim(new ageObjectPrim(),parent,kind);
	((ageObjectPrim*)sphere)->setSphere(1.0,8,8);
	((ageObjectPrim*)sphere)->setColor(0,255,0);
	sphere->setScale(new Scale3D(LO_LEG_JOINT_SIZE,LO_LEG_JOINT_SIZE,LO_LEG_JOINT_SIZE));
	
	//Segment_Oben
	AiGinEObject* cube = this->sceneMan->addObjectPrim(new ageObjectPrim(),sphere,"child");
	((ageObjectPrim*)cube)->setCube(1.0);
	((ageObjectPrim*)cube)->setColor(255,0,0);
	cube->setScale(new Scale3D(LO_LEG_WIDTH,LO_LEG_HEIGHT,LO_LEG_WIDTH));
	cube->setTranslation(new Translation3D(0.0,- LO_LEG_HEIGHT * 0.75,0.0));

	AiGinEObject* foot = drawFoot(cube, "child");
	foot->setTranslation(new Translation3D(0.0,- LO_LEG_HEIGHT * 0.625, 0.0));
	return sphere;
}

AiGinEObject* robot::drawFoot(AiGinEObject* parent, string kind) {
	//Fuss-Gelenk
	AiGinEObject* cube1 = this->sceneMan->addObjectPrim(new ageObjectPrim(),parent,kind);
	((ageObjectPrim*)cube1)->setCube(1.0);
	((ageObjectPrim*)cube1)->setColor(0,255,0);
	cube1->setScale(new Scale3D(FOOT_JOINT_SIZE,FOOT_JOINT_SIZE,FOOT_JOINT_SIZE));
	
	//Fuss
	AiGinEObject* cube2 = this->sceneMan->addObjectPrim(new ageObjectPrim(),cube1,"child");
	((ageObjectPrim*)cube2)->setCube(1.0);
	((ageObjectPrim*)cube2)->setColor(255,0,0);
	cube2->setScale(new Scale3D(FOOT_WIDTH, FOOT_HEIGHT, FOOT));
	cube2->setTranslation(new Translation3D(0.0,- FOOT_JOINT_SIZE * 2.0, FOOT_WIDTH * 0.5));
	return cube1;
}


//-------------------------------------------------------------------
void robot::drawRobot() {

	AiGinEObject* base = drawBaseLegs(NULL,"");
	this->Huefte = base;

}

void robot::animRobot() {
	this->Huefte->getRotation()->y = 1;
	this->Huefte->getRotation()->angle = ((int)this->Huefte->getRotation()->angle) % 360 + 1;
	
	this->BeinL->getRotation()->x = 1;
	this->KnieR->getRotation()->z = 1;
	this->BeinL->getRotation()->angle += 2;
	this->KnieR->getRotation()->angle += 2;
	this->sceneMan->display();
	
}
