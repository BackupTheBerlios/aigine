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
	AiGinEObject* left_leg = drawLeg(right_leg, "next");

	right_leg->setTranslation(new Translation3D(TORSO_WIDTH * 0.33 , - (BASE_HEIGHT), 0.0));
	left_leg->setTranslation(new Translation3D(- TORSO_WIDTH * 0.33 , - (BASE_HEIGHT), 0.0));

	this->BeinL = right_leg;
	this->BeinR = left_leg;

	return cube;
}

//=========================== Bein ===========================
AiGinEObject* robot::drawLeg(AiGinEObject* parent, string kind) {
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
	
	if(kind == "next") {
		this->KnieR = lower_leg_join;
		this->FussR = foot_join;
	} else {
		this->KnieL = lower_leg_join;
		this->FussL = foot_join;
	}
	return upper_leg_join;
}

//================= Draw_Head(frame) =================
/*
void Draw_Head(int frame) {
   glScalef(HEAD_WIDTH,HEAD_HEIGHT, TORSO) ;
   	glutSolidCube(1.0) ;
   glTranslatef(0.0,-HEAD_HEIGHT * 0.66,0.0) ;
   glScalef(HEAD_JOINT_SIZE,HEAD_JOINT_SIZE,HEAD_JOINT_SIZE) ;
   glutWireSphere(1.0,8,8) ;
}

//================= Draw_Torso(frame) ================

void Draw_Torso(int frame)
{
   glScalef(TORSO_WIDTH,TORSO_HEIGHT,TORSO) ;
   	glutSolidCube(1.0) ;
}

//================== Draw_Arm (side, frame) ====================

void Draw_Upper_Arm(int frame)
{
   glScalef(UP_ARM_JOINT_SIZE,UP_ARM_JOINT_SIZE,UP_ARM_JOINT_SIZE) ;
   glutWireSphere(1.0,8,8) ;
   glTranslatef(0.0,- UP_ARM_HEIGHT * 0.7, 0.0) ;
   glScalef(UP_ARM_WIDTH,UP_ARM_HEIGHT,UP_ARM_WIDTH) ;
   glutSolidCube(1.0) ;
}

void Draw_Lower_Arm(int frame) {
   glScalef(LO_ARM_JOINT_SIZE,LO_ARM_JOINT_SIZE,LO_ARM_JOINT_SIZE) ;
   glutWireSphere(1.0,8,8) ;
   glTranslatef(0.0,- LO_ARM_HEIGHT * 0.625,0.0) ;
   glScalef(LO_ARM_WIDTH,LO_ARM_HEIGHT,LO_ARM_WIDTH) ;
   glutSolidCube(1.0) ;
}

void Draw_Hand(int frame) {
   glScalef(HAND_WIDTH,HAND_HEIGHT,HAND) ;
   glutSolidCube(1.0) ;
}

void Draw_Arm(int side, int frame) {
	glPushMatrix() ;

   glRotatef(walking_angles[side][0],1.0,0.0,0.0) ;
   Draw_Upper_Arm(frame) ;
   glTranslatef(0.0,- UP_ARM_HEIGHT * 0.6,0.0) ;
   glRotatef(walking_angles[side][1],1.0,0.0,0.0) ;
   Draw_Lower_Arm(frame) ;

   glTranslatef(0.0,- (LO_ARM_HEIGHT * 0.75), 0.0) ;

   glTranslatef(0.0,  HAND_HEIGHT , 0.0) ;
   glRotatef(walking_angles[side][2],1.0,0.0,0.0) ;
   glTranslatef(0.0,- HAND_HEIGHT , 0.0) ;
   Draw_Hand(frame) ;

}
/**/

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

	this->FussL->getRotation()->y = 1;
	this->FussL->getRotation()->angle += 4;
	
	this->sceneMan->display();
	
}
