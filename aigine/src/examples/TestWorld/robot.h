// robot.h: interface for the robot class.
//
//////////////////////////////////////////////////////////////////////

#include "../../AiGinEFramework/Model/SceneManagement.h"
#include <iostream>
using namespace std;

#ifndef ROBOT_H
#define ROBOT_H

#ifndef PI
#define PI 3.1415
#endif

#define FOOT_JOINT_SIZE			HEAD_JOINT_SIZE
#define FOOT_HEIGHT				FOOT_JOINT_SIZE * 2.0
#define FOOT_WIDTH				LO_LEG_WIDTH
#define FOOT					FOOT_WIDTH * 2.0
#define UP_ARM_HEIGHT			TORSO_HEIGHT * 0.625
#define UP_ARM_WIDTH			TORSO_WIDTH/4.0
#define UP_ARM_JOINT_SIZE		HEAD_JOINT_SIZE * 2.0
#define LO_ARM_HEIGHT			TORSO_HEIGHT * 0.5
#define LO_ARM_WIDTH			UP_ARM_WIDTH
#define LO_ARM_JOINT_SIZE		UP_ARM_JOINT_SIZE * 0.75
#define HAND_HEIGHT				LO_ARM_HEIGHT / 2.0
#define HAND_WIDTH				LO_ARM_WIDTH
#define HAND					LO_ARM_WIDTH / 2.0
#define FINGER_SIZE				0.1
#define TORSO_WIDTH				TORSO_HEIGHT * 0.75
#define TORSO_HEIGHT			8
#define TORSO					TORSO_WIDTH / 3.0
#define HEAD_WIDTH				HEAD_HEIGHT * 0.93
#define HEAD_HEIGHT 			TORSO_HEIGHT * 0.375
#define HEAD_JOINT_SIZE			HEAD_HEIGHT/6
#define BASE_WIDTH				TORSO_WIDTH
#define BASE_HEIGHT				TORSO_HEIGHT / 4.0
#define UP_LEG_HEIGHT			LO_ARM_HEIGHT
#define UP_LEG_JOINT_SIZE		UP_ARM_JOINT_SIZE
#define UP_LEG_WIDTH			UP_LEG_JOINT_SIZE * 2.0
#define LO_LEG_HEIGHT			UP_LEG_HEIGHT
#define LO_LEG_WIDTH			UP_LEG_WIDTH
#define LO_LEG_JOINT_SIZE		UP_LEG_JOINT_SIZE
#define LEG_HEIGHT				UP_LEG_HEIGHT + LO_LEG_HEIGHT + FOOT_HEIGHT + 2*(FOOT_JOINT_SIZE+UP_LEG_JOINT_SIZE+LO_LEG_JOINT_SIZE)

#define FRAMES 20

#define SOLID 1
#define WIRE  2

#define LEFT  0
#define RIGHT 1
#define RADIUS 2.4
/**/

class robot  
{
public:
	robot(SceneManagement* sm);
	virtual ~robot();
	void drawRobot();
	void animRobot();
private:
	SceneManagement* sceneMan;
	AiGinEObject* Huefte;
	AiGinEObject* BeinR;
	AiGinEObject* BeinL;
	AiGinEObject* KnieR;
	AiGinEObject* KnieL;
	AiGinEObject* FussR;
	AiGinEObject* FussL;
	
	AiGinEObject* drawBaseLegs(AiGinEObject* parent, string kind);
	AiGinEObject* drawUpperLeg(AiGinEObject* parent, string kind);
	AiGinEObject* drawLowerLeg(AiGinEObject* parent, string kind);
	AiGinEObject* drawFoot(AiGinEObject* parent, string kind);
	AiGinEObject* drawLeg(AiGinEObject* parent, string kind);

};

#endif //ROBOT_H
