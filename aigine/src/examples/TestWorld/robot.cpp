// robot.cpp: implementation of the robot class.
//
//////////////////////////////////////////////////////////////////////

#include "robot.h"
#include "../../AiGinEFramework/Model/AiGinEObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

robot::robot(SceneManagement* sm) {
	this->sceneMan = sm;
}

robot::~robot() {
}


//=========================== Huefte ===========================
AiGinEObject* robot::drawBaseLegs(AiGinEObject* parent, string kind) {
	//Huefte
	AiGinEObject* base;
	if(parent != NULL) {
		base = this->sceneMan->addObjectPrim(new ageObjectPrim(),parent,kind);
	} else {
		base = this->sceneMan->addObjectPrim(new ageObjectPrim());
	}
	((ageObjectPrim*)base)->setCube(1.0);
	((ageObjectPrim*)base)->setColor(255,0,0);
	base->setScale(new Scale3D(BASE_WIDTH, BASE_HEIGHT, TORSO));

	//Torso
	AiGinEObject* torso = this->sceneMan->addObjectPrim(new ageObjectPrim(),base,"child");
	((ageObjectPrim*)torso)->setCube(1.0);
	((ageObjectPrim*)torso)->setColor(255,0,0);
	torso->setScale(new Scale3D(TORSO_WIDTH,TORSO_HEIGHT,TORSO));
	torso->setTranslation(new Translation3D(0.0, TORSO_HEIGHT / 2.0, 0.0));

	//Beine
	AiGinEObject* right_leg = drawLeg(torso,"next","right");
	AiGinEObject* left_leg = drawLeg(right_leg, "next", "left");
	right_leg->setTranslation(new Translation3D(TORSO_WIDTH * 0.33 , - (BASE_HEIGHT), 0.0));
	left_leg->setTranslation(new Translation3D(- TORSO_WIDTH * 0.33 , - (BASE_HEIGHT), 0.0));

	this->BeinR = right_leg;
	this->BeinL = left_leg;

	//Kopf
	AiGinEObject* head_join = this->sceneMan->addObjectPrim(new ageObjectPrim(),torso,"child");
	((ageObjectPrim*)head_join)->setSphere(1.0,8,8);
	((ageObjectPrim*)head_join)->setColor(0,255,0);
	head_join->setScale(new Scale3D(HEAD_JOINT_SIZE,HEAD_JOINT_SIZE,HEAD_JOINT_SIZE));
	head_join->setTranslation(new Translation3D(0.0, TORSO_HEIGHT + (HEAD_HEIGHT/2.0) +HEAD_JOINT_SIZE * 2.0, 0.0));
	
	AiGinEObject* head = this->sceneMan->addObjectPrim(new ageObjectPrim(),head_join,"child");
	((ageObjectPrim*)head)->setCube(1.0);
	((ageObjectPrim*)head)->setColor(255,0,0);
	head->setScale(new Scale3D(HEAD_WIDTH,HEAD_HEIGHT, TORSO));
	head->setTranslation(new Translation3D(0.0,-HEAD_HEIGHT * 0.66,0.0));

	//Arme
	AiGinEObject* right_arm = drawArm(head,"next","right");
	AiGinEObject* left_arm = drawArm(right_arm, "next", "left");
	right_arm->setTranslation(new Translation3D(TORSO_WIDTH * 0.66, TORSO_HEIGHT * 0.875,0.0));
	left_arm->setTranslation(new Translation3D(- (TORSO_WIDTH * 0.66), TORSO_HEIGHT * 0.875,0.0));

	return base;
}

//=========================== Bein ===========================
AiGinEObject* robot::drawLeg(AiGinEObject* parent, string kind, string side) {
	//Bein_Oben_Gelenk
	AiGinEObject* upper_leg_join = this->sceneMan->addObjectPrim(new ageObjectPrim(),parent,kind);
	((ageObjectPrim*)upper_leg_join)->setSphere(1.0,8,8);
	((ageObjectPrim*)upper_leg_join)->setColor(0,255,0);
	upper_leg_join->setScale(new Scale3D(UP_LEG_JOINT_SIZE,UP_LEG_JOINT_SIZE,UP_LEG_JOINT_SIZE));

	//Bein_Oben
	AiGinEObject* upper_leg = this->sceneMan->addObjectPrim(new ageObjectPrim(),upper_leg_join,"child");
	((ageObjectPrim*)upper_leg)->setCube(1.0);
	((ageObjectPrim*)upper_leg)->setColor(255,0,0);
	upper_leg->setScale(new Scale3D(UP_LEG_WIDTH, UP_LEG_HEIGHT, UP_LEG_WIDTH));
	upper_leg->setTranslation(new Translation3D(0.0,- UP_LEG_HEIGHT * 0.75,0.0));


	//Bein_Unten_Gelenk
	AiGinEObject* lower_leg_join = this->sceneMan->addObjectPrim(new ageObjectPrim(),upper_leg,"child");
	((ageObjectPrim*)lower_leg_join)->setSphere(1.0,8,8);
	((ageObjectPrim*)lower_leg_join)->setColor(0,255,0);
	lower_leg_join->setScale(new Scale3D(LO_LEG_JOINT_SIZE,LO_LEG_JOINT_SIZE,LO_LEG_JOINT_SIZE));
	lower_leg_join->setTranslation(new Translation3D(0.0,- UP_LEG_HEIGHT * 0.75,0.0));
	
	//Bein_Unten
	AiGinEObject* lower_leg = this->sceneMan->addObjectPrim(new ageObjectPrim(),lower_leg_join,"child");
	((ageObjectPrim*)lower_leg)->setCube(1.0);
	((ageObjectPrim*)lower_leg)->setColor(255,0,0);
	lower_leg->setScale(new Scale3D(LO_LEG_WIDTH,LO_LEG_HEIGHT,LO_LEG_WIDTH));
	lower_leg->setTranslation(new Translation3D(0.0,- LO_LEG_HEIGHT * 0.75,0.0));

	//Fuss-Gelenk
	AiGinEObject* foot_join = this->sceneMan->addObjectPrim(new ageObjectPrim(),lower_leg,"child");
	((ageObjectPrim*)foot_join)->setCube(1.0);
	((ageObjectPrim*)foot_join)->setColor(0,255,0);
	foot_join->setScale(new Scale3D(FOOT_JOINT_SIZE,FOOT_JOINT_SIZE,FOOT_JOINT_SIZE));
	foot_join->setTranslation(new Translation3D(0.0,- LO_LEG_HEIGHT * 0.625, 0.0));
	
	//Fuss
	AiGinEObject* foot = this->sceneMan->addObjectPrim(new ageObjectPrim(),foot_join,"child");
	((ageObjectPrim*)foot)->setCube(1.0);
	((ageObjectPrim*)foot)->setColor(255,0,0);
	foot->setScale(new Scale3D(FOOT_WIDTH, FOOT_HEIGHT, FOOT));
	foot->setTranslation(new Translation3D(0.0,- FOOT_JOINT_SIZE * 2.0, FOOT_WIDTH * 0.5));
	
	if(side == "right") {
		this->KnieR = lower_leg_join;
		this->FussR = foot_join;
	} else if(side == "left") {
		this->KnieL = lower_leg_join;
		this->FussL = foot_join;
	} else {
		//Fehler
	}
	return upper_leg_join;
}

//=========================== Arm ============================
AiGinEObject* robot::drawArm(AiGinEObject* parent,  string kind, string side) {

  	//Arm_Oben_Gelenk
	AiGinEObject* upper_arm_join = this->sceneMan->addObjectPrim(new ageObjectPrim(),parent,kind);
	((ageObjectPrim*)upper_arm_join)->setSphere(1.0,8,8);
	((ageObjectPrim*)upper_arm_join)->setColor(0,255,0);
	upper_arm_join->setScale(new Scale3D(UP_ARM_JOINT_SIZE,UP_ARM_JOINT_SIZE,UP_ARM_JOINT_SIZE));

	//Arm_Oben
	AiGinEObject* upper_arm = this->sceneMan->addObjectPrim(new ageObjectPrim(),upper_arm_join,"child");
	((ageObjectPrim*)upper_arm)->setCube(1.0);
	((ageObjectPrim*)upper_arm)->setColor(255,0,0);
	upper_arm->setScale(new Scale3D(UP_ARM_WIDTH,UP_ARM_HEIGHT,UP_ARM_WIDTH));
	upper_arm->setTranslation(new Translation3D(0.0,- UP_ARM_HEIGHT * 0.7, 0.0));

	//Arm_Unten_Gelenk
	AiGinEObject* lower_arm_join = this->sceneMan->addObjectPrim(new ageObjectPrim(),upper_arm,"child");
	((ageObjectPrim*)lower_arm_join)->setSphere(1.0,8,8);
	((ageObjectPrim*)lower_arm_join)->setColor(0,255,0);
	lower_arm_join->setScale(new Scale3D(LO_ARM_JOINT_SIZE,LO_ARM_JOINT_SIZE,LO_ARM_JOINT_SIZE));
	lower_arm_join->setTranslation(new Translation3D(0.0,- UP_ARM_HEIGHT * 0.6,0.0));

	//Arm_Unten
	AiGinEObject* lower_arm = this->sceneMan->addObjectPrim(new ageObjectPrim(),lower_arm_join,"child");
	((ageObjectPrim*)lower_arm)->setCube(1.0);
	((ageObjectPrim*)lower_arm)->setColor(255,0,0);
	lower_arm->setScale(new Scale3D(LO_ARM_WIDTH,LO_ARM_HEIGHT,LO_ARM_WIDTH));
	lower_arm->setTranslation(new Translation3D(0.0,- LO_ARM_HEIGHT * 0.625,0.0));
	
	//Hand
	AiGinEObject* hand = this->sceneMan->addObjectPrim(new ageObjectPrim(),lower_arm,"child");
	((ageObjectPrim*)hand)->setCube(1.0);
	((ageObjectPrim*)hand)->setColor(255,0,0);
	hand->setScale(new Scale3D(HAND_WIDTH,HAND_HEIGHT,HAND));
	hand->setTranslation(new Translation3D(0.0,- (LO_ARM_HEIGHT * 0.75), 0.0));
	
	if(side == "right") {
		this->EllbogenR = lower_arm_join;
		this->HandR = hand;
	} else if(side == "left") {
		this->EllbogenL = lower_arm_join;
		this->HandL = hand;
	} else {
		//Fehler
	}
	return upper_arm_join;

/*
   glTranslatef(0.0,  HAND_HEIGHT , 0.0) ;
   glTranslatef(0.0,- HAND_HEIGHT , 0.0) ;
*/
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
	this->BeinL->getRotation()->angle += 2;

	this->KnieR->getRotation()->z = 1;
	this->KnieR->getRotation()->angle += 2;

	this->FussL->getRotation()->y = 1;
	this->FussL->getRotation()->angle += 4;
	
//	this->sceneMan->display();
	
}
