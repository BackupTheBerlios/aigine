// robot.cpp: implementation of the robot class.
//
//////////////////////////////////////////////////////////////////////

#include "robot.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

robot::robot(SceneManagement* sm) {
	this->sceneMan = sm;
	this->myAngle = 0;
	this->direction = 1;
	this->vel = 1;
	this->frames = FRAMES;
	this->flag = 1;

	this->langle_count = 0.0;
	this->langle_count2 = 0.0;
	this->rangle_count = 0.0;
	this->rangle_count2 = 0.0;
	

}

robot::~robot() {
}

AiGinEObject* robot::getModel() {
	return this->Huefte;
}

/*
//=========================== Huefte ===========================
AiGinEObject* robot::drawModel(AiGinEObject* parent, string kind) {
	//Huefte
	AiGinEObject* base;
	if(parent != NULL) {
		base = this->sceneMan->addObjectPrim(new ageObjectPrim("data\\huefte.txt"),parent,kind);
	} else {
		base = this->sceneMan->addObjectPrim(new ageObjectPrim());
	}
	((ageObjectPrim*)base)->setCube(1.0);
	((ageObjectPrim*)base)->setColor(255,0,0);
	base->setScale(new Scale3D(BASE_WIDTH, BASE_HEIGHT, TORSO));
	((ageObjectPrim*)base)->writeData();
	
	//Torso
	AiGinEObject* torso = this->sceneMan->addObjectPrim(new ageObjectPrim("data\\torso.txt"),base,"child");
	((ageObjectPrim*)torso)->setCube(1.0);
	((ageObjectPrim*)torso)->setColor(255,0,0);
	torso->setScale(new Scale3D(TORSO_WIDTH,TORSO_HEIGHT,TORSO));
	torso->setTranslation(new Translation3D(0.0, TORSO_HEIGHT / 2.0 + BASE_HEIGHT / 2.0, 0.0));
	this->Torso = torso;
	((ageObjectPrim*)torso)->writeData();

	//Beine
	AiGinEObject* right_leg = drawLeg(torso,"next","right");
	AiGinEObject* left_leg = drawLeg(right_leg, "next", "left");

	this->BeinR = right_leg;
	this->BeinL = left_leg;

	//Kopf
	AiGinEObject* head_join = this->sceneMan->addObjectPrim(new ageObjectPrim("data\\headJoin.txt"),torso,"child");
	((ageObjectPrim*)head_join)->setSphere(1.0,8,8);
	((ageObjectPrim*)head_join)->setColor(0,255,0);
	head_join->setScale(new Scale3D(HEAD_JOINT_SIZE,HEAD_JOINT_SIZE,HEAD_JOINT_SIZE));
	head_join->setTranslation(new Translation3D(0.0,TORSO_HEIGHT/2 + HEAD_JOINT_SIZE,0.0));
	((ageObjectPrim*)head_join)->writeData();
	this->Kopf = head_join;

	AiGinEObject* head = this->sceneMan->addObjectPrim(new ageObjectPrim("data\\head.txt"),head_join,"child");
	((ageObjectPrim*)head)->setCube(1.0);
	((ageObjectPrim*)head)->setColor(255,0,0);
	head->setScale(new Scale3D(HEAD_WIDTH,HEAD_HEIGHT, TORSO));
	head->setTranslation(new Translation3D(0.0, (HEAD_HEIGHT/2.0) + HEAD_JOINT_SIZE, 0.0));
	((ageObjectPrim*)head)->writeData();

	//Arme
	AiGinEObject* right_arm = drawArm(head_join,"next","right");
	AiGinEObject* left_arm = drawArm(right_arm, "next", "left");
	this->ArmR = right_arm;
	this->ArmL = left_arm;

	return base;
}

//=========================== Bein ===========================
AiGinEObject* robot::drawLeg(AiGinEObject* parent, string kind, string side) {
	//Bein_Oben_Gelenk
	string str = "data\\upperLegJoin" + side + ".txt";
	AiGinEObject* upper_leg_join = this->sceneMan->addObjectPrim(new ageObjectPrim(str.c_str()),parent,kind);
	((ageObjectPrim*)upper_leg_join)->setSphere(1.0,8,8);
	((ageObjectPrim*)upper_leg_join)->setColor(0,255,0);
	upper_leg_join->setScale(new Scale3D(UP_LEG_JOINT_SIZE,UP_LEG_JOINT_SIZE,UP_LEG_JOINT_SIZE));

	if(side == "right") {
		upper_leg_join->setTranslation(new Translation3D(TORSO_WIDTH * 0.33 , - (BASE_HEIGHT), 0.0));
	} else {
	upper_leg_join->setTranslation(new Translation3D(- TORSO_WIDTH * 0.33 , - (BASE_HEIGHT), 0.0));
	}
	((ageObjectPrim*)upper_leg_join)->writeData();

	//Bein_Oben
	str = "data\\upperLeg" + side + ".txt";
	AiGinEObject* upper_leg = this->sceneMan->addObjectPrim(new ageObjectPrim(str.c_str()),upper_leg_join,"child");
	((ageObjectPrim*)upper_leg)->setCube(1.0);
	((ageObjectPrim*)upper_leg)->setColor(255,0,0);
	upper_leg->setScale(new Scale3D(UP_LEG_WIDTH, UP_LEG_HEIGHT, UP_LEG_WIDTH));
	upper_leg->setTranslation(new Translation3D(0.0,- UP_LEG_HEIGHT * 0.75,0.0));
	((ageObjectPrim*)upper_leg)->writeData();

	//Bein_Unten_Gelenk
	str = "data\\lowerLegJoin" + side + ".txt";
	AiGinEObject* lower_leg_join = this->sceneMan->addObjectPrim(new ageObjectPrim(str.c_str()),upper_leg,"child");
	((ageObjectPrim*)lower_leg_join)->setSphere(1.0,8,8);
	((ageObjectPrim*)lower_leg_join)->setColor(0,255,0);
	lower_leg_join->setScale(new Scale3D(LO_LEG_JOINT_SIZE,LO_LEG_JOINT_SIZE,LO_LEG_JOINT_SIZE));
	lower_leg_join->setTranslation(new Translation3D(0.0,- UP_LEG_HEIGHT * 0.75,0.0));
	((ageObjectPrim*)lower_leg_join)->writeData();

	//Bein_Unten
	str = "data\\lowerLeg" + side + ".txt";
	AiGinEObject* lower_leg = this->sceneMan->addObjectPrim(new ageObjectPrim(str.c_str()),lower_leg_join,"child");
	((ageObjectPrim*)lower_leg)->setCube(1.0);
	((ageObjectPrim*)lower_leg)->setColor(255,0,0);
	lower_leg->setScale(new Scale3D(LO_LEG_WIDTH,LO_LEG_HEIGHT,LO_LEG_WIDTH));
	lower_leg->setTranslation(new Translation3D(0.0,- LO_LEG_HEIGHT * 0.75,0.0));
	((ageObjectPrim*)lower_leg)->writeData();

	//Fuss-Gelenk
	str = "data\\footJoin" + side + ".txt";
	AiGinEObject* foot_join = this->sceneMan->addObjectPrim(new ageObjectPrim(str.c_str()),lower_leg,"child");
	((ageObjectPrim*)foot_join)->setCube(1.0);
	((ageObjectPrim*)foot_join)->setColor(0,255,0);
	foot_join->setScale(new Scale3D(FOOT_JOINT_SIZE,FOOT_JOINT_SIZE,FOOT_JOINT_SIZE));
	foot_join->setTranslation(new Translation3D(0.0,- LO_LEG_HEIGHT * 0.625, 0.0));
	((ageObjectPrim*)foot_join)->writeData();

	//Fuss
	str = "data\\foot" + side + ".txt";
	AiGinEObject* foot = this->sceneMan->addObjectPrim(new ageObjectPrim(str.c_str()),foot_join,"child");
	((ageObjectPrim*)foot)->setCube(1.0);
	((ageObjectPrim*)foot)->setColor(255,0,0);
	foot->setScale(new Scale3D(FOOT_WIDTH, FOOT_HEIGHT, FOOT));
	foot->setTranslation(new Translation3D(0.0,- FOOT_JOINT_SIZE * 2.0, FOOT_WIDTH * 0.5));
	((ageObjectPrim*)foot)->writeData();

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
	string str = "data\\upperArmJoin" + side + ".txt";
	AiGinEObject* upper_arm_join = this->sceneMan->addObjectPrim(new ageObjectPrim(str.c_str()),parent,kind);
	((ageObjectPrim*)upper_arm_join)->setSphere(1.0,8,8);
	((ageObjectPrim*)upper_arm_join)->setColor(0,255,0);
	upper_arm_join->setScale(new Scale3D(UP_ARM_JOINT_SIZE,UP_ARM_JOINT_SIZE,UP_ARM_JOINT_SIZE));
	if(side == "right") {
	upper_arm_join->setTranslation(new Translation3D(TORSO_WIDTH * 0.66, TORSO_HEIGHT/2 - UP_ARM_JOINT_SIZE,0.0));
	} else {
	upper_arm_join->setTranslation(new Translation3D(- (TORSO_WIDTH * 0.66), TORSO_HEIGHT/2 - UP_ARM_JOINT_SIZE,0.0));
	}
	
	((ageObjectPrim*)upper_arm_join)->writeData();

	//Arm_Oben
	str = "data\\upperArm" + side + ".txt";
	AiGinEObject* upper_arm = this->sceneMan->addObjectPrim(new ageObjectPrim(str.c_str()),upper_arm_join,"child");
	((ageObjectPrim*)upper_arm)->setCube(1.0);
	((ageObjectPrim*)upper_arm)->setColor(255,0,0);
	upper_arm->setScale(new Scale3D(UP_ARM_WIDTH,UP_ARM_HEIGHT,UP_ARM_WIDTH));
	upper_arm->setTranslation(new Translation3D(0.0,- UP_ARM_HEIGHT * 0.7, 0.0));
	((ageObjectPrim*)upper_arm)->writeData();

	//Arm_Unten_Gelenk
	str = "data\\lowerLegJoin" + side + ".txt";
	AiGinEObject* lower_arm_join = this->sceneMan->addObjectPrim(new ageObjectPrim(str.c_str()),upper_arm,"child");
	((ageObjectPrim*)lower_arm_join)->setSphere(1.0,8,8);
	((ageObjectPrim*)lower_arm_join)->setColor(0,255,0);
	lower_arm_join->setScale(new Scale3D(LO_ARM_JOINT_SIZE,LO_ARM_JOINT_SIZE,LO_ARM_JOINT_SIZE));
	lower_arm_join->setTranslation(new Translation3D(0.0,- UP_ARM_HEIGHT * 0.6,0.0));
	((ageObjectPrim*)lower_arm_join)->writeData();

	//Arm_Unten
	str = "data\\lowerArm" + side + ".txt";
	AiGinEObject* lower_arm = this->sceneMan->addObjectPrim(new ageObjectPrim(str.c_str()),lower_arm_join,"child");
	((ageObjectPrim*)lower_arm)->setCube(1.0);
	((ageObjectPrim*)lower_arm)->setColor(255,0,0);
	lower_arm->setScale(new Scale3D(LO_ARM_WIDTH,LO_ARM_HEIGHT,LO_ARM_WIDTH));
	lower_arm->setTranslation(new Translation3D(0.0,- LO_ARM_HEIGHT * 0.625,0.0));
	((ageObjectPrim*)lower_arm)->writeData();
	
	//Hand
	str = "data\\hand" + side + ".txt";
	AiGinEObject* hand = this->sceneMan->addObjectPrim(new ageObjectPrim(str.c_str()),lower_arm,"child");
	((ageObjectPrim*)hand)->setCube(1.0);
	((ageObjectPrim*)hand)->setColor(0,255,0);
	hand->setScale(new Scale3D(HAND_WIDTH,HAND_HEIGHT,HAND));
	hand->setTranslation(new Translation3D(0.0,- (LO_ARM_HEIGHT * 0.75), 0.0));
	((ageObjectPrim*)hand)->writeData();

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

}
*/
//-------------------------------------------------------------------
AiGinEObject* robot::drawRobot(AiGinEObject* parent, string kind) {

	AiGinEObject* base = drawModel2(parent,kind);
//	this->sceneMan->printGraph();
	this->Huefte = base;
	this->origPos = this->Huefte->getTranslation()->y;	
	this->readDataFromFile();
	langle_count = this->BeinL->getRotation()->angle;
	langle_count2 = this->KnieL->getRotation()->angle;
	rangle_count = this->BeinR->getRotation()->angle;
	rangle_count2 = this->KnieR->getRotation()->angle;

	return this->Huefte;
}

//=========================== Huefte ===========================
AiGinEObject* robot::drawModel2(AiGinEObject* parent, string kind) {
	//Huefte
	AiGinEObject* base;
	if(parent != NULL) {
		base = this->sceneMan->addObjectPrim(new ageObjectPrim("data\\huefte.txt"),parent,kind);
	} else {
		base = this->sceneMan->addObjectPrim(new ageObjectPrim("data\\huefte.txt"));
	}
	((ageObjectPrim*)base)->readData();

	//Torso
	this->Torso = this->sceneMan->addObjectPrim(new ageObjectPrim("data\\torso.txt"),base,"child");
	((ageObjectPrim*)this->Torso)->readData();

	//Beine
	this->BeinR = drawLeg2(this->Torso,"next","right");
	this->BeinL = drawLeg2(this->BeinR, "next", "left");

	//Kopf
	this->Kopf = this->sceneMan->addObjectPrim(new ageObjectPrim("data\\headJoin.txt"),this->Torso,"child");
	((ageObjectPrim*)this->Kopf)->readData();
	AiGinEObject* head = this->sceneMan->addObjectPrim(new ageObjectPrim("data\\head.txt"),this->Kopf,"child");
	((ageObjectPrim*)head)->readData();

	//Arme
	this->ArmR = drawArm2(this->Kopf,"next","right");
	this->ArmL = drawArm2(this->ArmR, "next", "left");

	return base;
}

//=========================== Bein ===========================
AiGinEObject* robot::drawLeg2(AiGinEObject* parent, string kind, string side) {
	//Bein_Oben_Gelenk
	string str = "data\\upperLegJoin" + side + ".txt";
	AiGinEObject* upper_leg_join = this->sceneMan->addObjectPrim(new ageObjectPrim(str.c_str()),parent,kind);
	((ageObjectPrim*)upper_leg_join)->readData();

	//Bein_Oben
	str = "data\\upperLeg" + side + ".txt";
	AiGinEObject* upper_leg = this->sceneMan->addObjectPrim(new ageObjectPrim(str.c_str()),upper_leg_join,"child");
	((ageObjectPrim*)upper_leg)->readData();

	//Bein_Unten_Gelenk
	str = "data\\lowerLegJoin" + side + ".txt";
	AiGinEObject* lower_leg_join = this->sceneMan->addObjectPrim(new ageObjectPrim(str.c_str()),upper_leg,"child");
	((ageObjectPrim*)lower_leg_join)->readData();

	//Bein_Unten
	str = "data\\lowerLeg" + side + ".txt";
	AiGinEObject* lower_leg = this->sceneMan->addObjectPrim(new ageObjectPrim(str.c_str()),lower_leg_join,"child");
	((ageObjectPrim*)lower_leg)->readData();

	//Fuss-Gelenk
	str = "data\\footJoin" + side + ".txt";
	AiGinEObject* foot_join = this->sceneMan->addObjectPrim(new ageObjectPrim(str.c_str()),lower_leg,"child");
	((ageObjectPrim*)foot_join)->readData();

	//Fuss
	str = "data\\foot" + side + ".txt";
	AiGinEObject* foot = this->sceneMan->addObjectPrim(new ageObjectPrim(str.c_str()),foot_join,"child");
	((ageObjectPrim*)foot)->readData();

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
AiGinEObject* robot::drawArm2(AiGinEObject* parent,  string kind, string side) {

  	//Arm_Oben_Gelenk
	string str = "data\\upperArmJoin" + side + ".txt";
	AiGinEObject* upper_arm_join = this->sceneMan->addObjectPrim(new ageObjectPrim(str.c_str()),parent,kind);
	((ageObjectPrim*)upper_arm_join)->readData();

	//Arm_Oben
	str = "data\\upperArm" + side + ".txt";
	AiGinEObject* upper_arm = this->sceneMan->addObjectPrim(new ageObjectPrim(str.c_str()),upper_arm_join,"child");
	((ageObjectPrim*)upper_arm)->readData();

	//Arm_Unten_Gelenk
	str = "data\\lowerLegJoin" + side + ".txt";
	AiGinEObject* lower_arm_join = this->sceneMan->addObjectPrim(new ageObjectPrim(str.c_str()),upper_arm,"child");
	((ageObjectPrim*)lower_arm_join)->readData();

	//Arm_Unten
	str = "data\\lowerArm" + side + ".txt";
	AiGinEObject* lower_arm = this->sceneMan->addObjectPrim(new ageObjectPrim(str.c_str()),lower_arm_join,"child");
	((ageObjectPrim*)lower_arm)->readData();
	
	//Hand
	str = "data\\hand" + side + ".txt";
	AiGinEObject* hand = this->sceneMan->addObjectPrim(new ageObjectPrim(str.c_str()),lower_arm,"child");
	((ageObjectPrim*)hand)->readData();

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
}

void robot::animRobot() {
	this->animateBody();
}


FILE* robot::openFiles(char r_w_a) {
	FILE *file ;

   if (r_w_a == 'r')
   {
 	   file  = fopen("data.txt","r") ;
   }
   else if (r_w_a == 'w')
   {
      file  = fopen("test.txt","w") ;
   }
   else
   {
      printf("Wrong use of 'Open_Files' procedure, exiting...\n") ;
      printf("Please press enter to finish\n") ;
      getchar() ;
      exit(0) ;
   }
   return file ;
}

void robot::readDataFromFile() {
   int counter, scan_counter ;
   FILE *working_file ;

   working_file = openFiles('r') ;

   if (working_file == NULL)
   {
      printf("Searched the following directories and could not find the file\n") ;
      printf("'data.txt'\ne:\\bp\\chapter2\\model2\\\ng:\\data\\\na:\\\n") ;
      printf("Exiting, please press enter...\n\n") ;
      getchar() ;
      exit(0) ;
   }
   else
   {
      printf("Data input file opened!\n\n") ;

	  this->Kopf->getRotation()->x = 1;
      this->Torso->getRotation()->x = 1 ;
      this->Huefte->getRotation()->x = 1 ;
      this->ArmL->getRotation()->x = 1 ;
      this->EllbogenL->getRotation()->x = 1 ;
      this->HandL->getRotation()->x = 1 ;
      this->BeinL->getRotation()->x = 1 ;
      this->KnieL->getRotation()->x = 1 ;
      this->FussL->getRotation()->x = 1 ;
      this->ArmR->getRotation()->x = 1 ;
      this->EllbogenR->getRotation()->x = 1 ;
      this->HandR->getRotation()->x = 1 ;
      this->BeinR->getRotation()->x = 1 ;
      this->KnieR->getRotation()->x = 1 ;
      this->FussR->getRotation()->x = 1 ;

      scan_counter =  fscanf(working_file,"%f",&this->Kopf->getRotation()->angle) ;
      scan_counter += fscanf(working_file,"%f",&this->Torso->getRotation()->angle) ;
      scan_counter += fscanf(working_file,"%f",&this->Huefte->getRotation()->angle) ;
      scan_counter += fscanf(working_file,"%f",&this->ArmL->getRotation()->angle) ;
      scan_counter += fscanf(working_file,"%f",&this->EllbogenL->getRotation()->angle) ;
      scan_counter += fscanf(working_file,"%f",&this->HandL->getRotation()->angle) ;
      scan_counter += fscanf(working_file,"%f",&this->BeinL->getRotation()->angle) ;
      scan_counter += fscanf(working_file,"%f",&this->KnieL->getRotation()->angle) ;
      scan_counter += fscanf(working_file,"%f",&this->FussL->getRotation()->angle) ;
      scan_counter += fscanf(working_file,"%f",&this->ArmR->getRotation()->angle) ;
      scan_counter += fscanf(working_file,"%f",&this->EllbogenR->getRotation()->angle) ;
      scan_counter += fscanf(working_file,"%f",&this->HandR->getRotation()->angle) ;
      scan_counter += fscanf(working_file,"%f",&this->BeinR->getRotation()->angle) ;
      scan_counter += fscanf(working_file,"%f",&this->KnieR->getRotation()->angle) ;
      scan_counter += fscanf(working_file,"%f",&this->FussR->getRotation()->angle) ;

      for (counter = 0 ; counter < 4 ; counter++)
      {
      	scan_counter +=  fscanf(working_file,"%f",&this->array[counter].head) ;
      	scan_counter += fscanf(working_file,"%f",&this->array[counter].upbody) ;
      	scan_counter += fscanf(working_file,"%f",&this->array[counter].lobody) ;
      	scan_counter += fscanf(working_file,"%f",&this->array[counter].l_uparm) ;
      	scan_counter += fscanf(working_file,"%f",&this->array[counter].l_loarm) ;
      	scan_counter += fscanf(working_file,"%f",&this->array[counter].l_hand) ;
      	scan_counter += fscanf(working_file,"%f",&this->array[counter].l_upleg) ;
      	scan_counter += fscanf(working_file,"%f",&this->array[counter].l_loleg) ;
      	scan_counter += fscanf(working_file,"%f",&this->array[counter].l_foot) ;
      	scan_counter += fscanf(working_file,"%f",&this->array[counter].r_uparm) ;
      	scan_counter += fscanf(working_file,"%f",&this->array[counter].r_loarm) ;
      	scan_counter += fscanf(working_file,"%f",&this->array[counter].r_hand) ;
      	scan_counter += fscanf(working_file,"%f",&this->array[counter].r_upleg) ;
      	scan_counter += fscanf(working_file,"%f",&this->array[counter].r_loleg) ;
      	scan_counter += fscanf(working_file,"%f",&this->array[counter].r_foot) ;
		}

      printf("%d values read.\n\n",scan_counter) ;
   }

   fclose(working_file) ;
}





double robot::findBaseMove(double langle_up, double langle_lo, double rangle_up, double rangle_lo) {

//	cout << langle_up << " , " << langle_lo << " , " << rangle_up << " , " << rangle_lo << endl;
   double result1, result2, first_result, second_result, radians_up, radians_lo ;

   radians_up = (PI*langle_up)/180.0 ;
   radians_lo = (PI*langle_lo-langle_up)/180.0 ;
   result1 = (UP_LEG_HEIGHT + 2*UP_LEG_JOINT_SIZE) * cos(radians_up) ;
   result2 = (LO_LEG_HEIGHT + 2*(LO_LEG_JOINT_SIZE+FOOT_JOINT_SIZE)+FOOT_HEIGHT)
             * cos(radians_lo) ;
   first_result = LEG_HEIGHT - (result1 + result2) ;

   radians_up = (PI*rangle_up)/180.0 ;
   radians_lo = (PI*rangle_lo-rangle_up)/180.0 ;
   result1 = (UP_LEG_HEIGHT + 2*UP_LEG_JOINT_SIZE) * cos(radians_up) ;
   result2 = (LO_LEG_HEIGHT + 2*(LO_LEG_JOINT_SIZE+FOOT_JOINT_SIZE)+FOOT_HEIGHT)
             * cos(radians_lo) ;
   second_result = LEG_HEIGHT - (result1 + result2) ;

   if (first_result <= second_result) {
      return (-first_result);
	  } else {
      return (-second_result);
	  }
}

void robot::animateBody() {

	float l_upleg_add, l_loleg_add, r_upleg_add, r_loleg_add,
         l_uparm_add, l_loarm_add, l_hand_add,
         r_uparm_add, r_loarm_add, r_hand_add = 0.0;

   switch (flag)
   {
      case 1 :

         l_upleg_add = this->array[0].l_upleg / FRAMES ;
       	r_upleg_add = this->array[0].r_upleg / FRAMES ;
         l_loleg_add = this->array[0].l_loleg / FRAMES ;
       	r_loleg_add = this->array[0].r_loleg / FRAMES ;

         l_uparm_add = this->array[0].l_uparm / FRAMES ;
       	l_loarm_add = this->array[0].l_loarm / FRAMES ;
         l_hand_add  = this->array[0].l_hand / FRAMES ;
       	r_uparm_add = this->array[0].r_uparm / FRAMES ;
         r_hand_add  = this->array[0].r_hand / FRAMES ;
         r_loarm_add = this->array[0].r_loarm / FRAMES ;


		 this->BeinL->getRotation()->angle += r_upleg_add;
		 this->BeinR->getRotation()->angle += l_upleg_add;
		 this->KnieL->getRotation()->angle += r_loleg_add;
		 this->KnieR->getRotation()->angle += l_loleg_add;

		 this->ArmL->getRotation()->angle += l_uparm_add;
		 this->EllbogenL->getRotation()->angle += l_loarm_add;
		 this->HandL->getRotation()->angle += l_hand_add;

 		 this->ArmR->getRotation()->angle += r_uparm_add;
		 this->EllbogenR->getRotation()->angle += r_loarm_add ;
		 this->HandR->getRotation()->angle += r_hand_add ;

		 
         langle_count  -= l_upleg_add ;
         langle_count2 -= l_loleg_add ;
         rangle_count  -= r_upleg_add ;
         rangle_count2 -= r_loleg_add ;
		 this->Huefte->getTranslation()->y = this->origPos + findBaseMove(langle_count,langle_count2,rangle_count,rangle_count2) ;
         frames-- ;
         if (frames == 0)
         {
            flag = 2 ;
            frames = FRAMES ;
         }
         break ;

      case 2 :

         l_upleg_add = this->array[1].l_upleg / FRAMES ;
        	r_upleg_add = this->array[1].r_upleg / FRAMES ;
         l_loleg_add = this->array[1].l_loleg / FRAMES ;
       	r_loleg_add = this->array[1].r_loleg / FRAMES ;

         l_uparm_add = this->array[1].l_uparm / FRAMES ;
       	l_loarm_add = this->array[1].l_loarm / FRAMES ;
         l_hand_add  = this->array[1].l_hand / FRAMES ;
       	r_uparm_add = this->array[1].r_uparm / FRAMES ;
         r_hand_add  = this->array[1].r_hand / FRAMES ;
         r_loarm_add = this->array[1].r_loarm / FRAMES ;


		 this->BeinL->getRotation()->angle += r_upleg_add;
		 this->BeinR->getRotation()->angle += l_upleg_add;
		 this->KnieL->getRotation()->angle += r_loleg_add;
		 this->KnieR->getRotation()->angle += l_loleg_add;

		 this->ArmL->getRotation()->angle += l_uparm_add;
		 this->EllbogenL->getRotation()->angle += l_loarm_add;
		 this->HandL->getRotation()->angle += l_hand_add;

 		 this->ArmR->getRotation()->angle += r_uparm_add;
		 this->EllbogenR->getRotation()->angle += r_loarm_add ;
		 this->HandR->getRotation()->angle += r_hand_add ;

		 
         langle_count  -= l_upleg_add ;
         langle_count2 -= l_loleg_add ;
         rangle_count  -= r_upleg_add ;
         rangle_count2 -= r_loleg_add ;
		 this->Huefte->getTranslation()->y = this->origPos + findBaseMove(langle_count,langle_count2,rangle_count,rangle_count2) ;
         frames-- ;
         if (frames == 0)
         {
            flag = 3 ;
            frames = FRAMES ;
         }
         break ;

      case 3 :

         l_upleg_add = this->array[2].l_upleg / FRAMES ;
       	r_upleg_add = this->array[2].r_upleg / FRAMES ;
         l_loleg_add = this->array[2].l_loleg / FRAMES ;
       	r_loleg_add = this->array[2].r_loleg / FRAMES ;

         l_uparm_add = this->array[2].l_uparm / FRAMES ;
       	l_loarm_add = this->array[2].l_loarm / FRAMES ;
         l_hand_add  = this->array[2].l_hand / FRAMES ;
       	r_uparm_add = this->array[2].r_uparm / FRAMES ;
         r_hand_add  = this->array[2].r_hand / FRAMES ;
         r_loarm_add = this->array[2].r_loarm / FRAMES ;

		 this->BeinL->getRotation()->angle += r_upleg_add;
		 this->BeinR->getRotation()->angle += l_upleg_add;
		 this->KnieL->getRotation()->angle += r_loleg_add;
		 this->KnieR->getRotation()->angle += l_loleg_add;

		 this->ArmL->getRotation()->angle += l_uparm_add;
		 this->EllbogenL->getRotation()->angle += l_loarm_add;
		 this->HandL->getRotation()->angle += l_hand_add;

 		 this->ArmR->getRotation()->angle += r_uparm_add;
		 this->EllbogenR->getRotation()->angle += r_loarm_add ;
		 this->HandR->getRotation()->angle += r_hand_add ;


		 
         langle_count  -= l_upleg_add ;
         langle_count2 -= l_loleg_add ;
         rangle_count  -= r_upleg_add ;
         rangle_count2 -= r_loleg_add ;
		 this->Huefte->getTranslation()->y = this->origPos + findBaseMove(langle_count,langle_count2,rangle_count,rangle_count2) ;
         frames-- ;
         if (frames == 0)
         {
            flag = 4 ;
            frames = FRAMES ;
         }
         break ;

      case 4 :

         l_upleg_add = this->array[3].l_upleg / FRAMES ;
       	r_upleg_add = this->array[3].r_upleg / FRAMES ;
         l_loleg_add = this->array[3].l_loleg / FRAMES ;
       	r_loleg_add = this->array[3].r_loleg / FRAMES ;

         l_uparm_add = this->array[3].l_uparm / FRAMES ;
       	l_loarm_add = this->array[3].l_loarm / FRAMES ;
         l_hand_add  = this->array[3].l_hand / FRAMES ;
       	r_uparm_add = this->array[3].r_uparm / FRAMES ;
         r_hand_add  = this->array[3].r_hand / FRAMES ;
         r_loarm_add = this->array[3].r_loarm / FRAMES ;

		 this->BeinL->getRotation()->angle += r_upleg_add;
		 this->BeinR->getRotation()->angle += l_upleg_add;
		 this->KnieL->getRotation()->angle += r_loleg_add;
		 this->KnieR->getRotation()->angle += l_loleg_add;

		 this->ArmL->getRotation()->angle += l_uparm_add;
		 this->EllbogenL->getRotation()->angle += l_loarm_add;
		 this->HandL->getRotation()->angle += l_hand_add;

 		 this->ArmR->getRotation()->angle += r_uparm_add;
		 this->EllbogenR->getRotation()->angle += r_loarm_add ;
		 this->HandR->getRotation()->angle += r_hand_add ;

		 
         langle_count  -= l_upleg_add ;
         langle_count2 -= l_loleg_add ;
         rangle_count  -= r_upleg_add ;
         rangle_count2 -= r_loleg_add ;
		 this->Huefte->getTranslation()->y = this->origPos + findBaseMove(langle_count,langle_count2,rangle_count,rangle_count2) ;
         frames-- ;
         if (frames == 0)
         {
            flag = 5 ;
            frames = FRAMES ;
         }
         break ;

		case 5 :

         l_upleg_add = this->array[0].r_upleg / FRAMES ;
       	r_upleg_add = this->array[0].l_upleg / FRAMES ;
         l_loleg_add = this->array[0].r_loleg / FRAMES ;
       	r_loleg_add = this->array[0].l_loleg / FRAMES ;

         l_uparm_add = this->array[0].r_uparm / FRAMES ;
       	l_loarm_add = this->array[0].r_loarm / FRAMES ;
         l_hand_add  = this->array[0].r_hand / FRAMES ;
       	r_uparm_add = this->array[0].l_uparm / FRAMES ;
         r_hand_add  = this->array[0].l_hand / FRAMES ;
         r_loarm_add = this->array[0].l_loarm / FRAMES ;

		 this->BeinL->getRotation()->angle += r_upleg_add;
		 this->BeinR->getRotation()->angle += l_upleg_add;
		 this->KnieL->getRotation()->angle += r_loleg_add;
		 this->KnieR->getRotation()->angle += l_loleg_add;

		 this->ArmL->getRotation()->angle += l_uparm_add;
		 this->EllbogenL->getRotation()->angle += l_loarm_add;
		 this->HandL->getRotation()->angle += l_hand_add;

 		 this->ArmR->getRotation()->angle += r_uparm_add;
		 this->EllbogenR->getRotation()->angle += r_loarm_add ;
		 this->HandR->getRotation()->angle += r_hand_add ;

		 
         langle_count  -= l_upleg_add ;
         langle_count2 -= l_loleg_add ;
         rangle_count  -= r_upleg_add ;
         rangle_count2 -= r_loleg_add ;
		 this->Huefte->getTranslation()->y = this->origPos + findBaseMove(langle_count,langle_count2,rangle_count,rangle_count2) ;
         frames-- ;
         if (frames == 0)
         {
            flag = 6 ;
            frames = FRAMES ;
         }
         break ;

      case 6 :

         l_upleg_add = this->array[1].r_upleg / FRAMES ;
       	r_upleg_add = this->array[1].l_upleg / FRAMES ;
         l_loleg_add = this->array[1].r_loleg / FRAMES ;
       	r_loleg_add = this->array[1].l_loleg / FRAMES ;

         l_uparm_add = this->array[1].r_uparm / FRAMES ;
       	l_loarm_add = this->array[1].r_loarm / FRAMES ;
         l_hand_add  = this->array[1].r_hand / FRAMES ;
       	r_uparm_add = this->array[1].l_uparm / FRAMES ;
         r_hand_add  = this->array[1].l_hand / FRAMES ;
         r_loarm_add = this->array[1].l_loarm / FRAMES ;

		 this->BeinL->getRotation()->angle += r_upleg_add;
		 this->BeinR->getRotation()->angle += l_upleg_add;
		 this->KnieL->getRotation()->angle += r_loleg_add;
		 this->KnieR->getRotation()->angle += l_loleg_add;

		 this->ArmL->getRotation()->angle += l_uparm_add;
		 this->EllbogenL->getRotation()->angle += l_loarm_add;
		 this->HandL->getRotation()->angle += l_hand_add;

 		 this->ArmR->getRotation()->angle += r_uparm_add;
		 this->EllbogenR->getRotation()->angle += r_loarm_add ;
		 this->HandR->getRotation()->angle += r_hand_add ;
	
		 
         langle_count  -= l_upleg_add ;
         langle_count2 -= l_loleg_add ;
         rangle_count  -= r_upleg_add ;
         rangle_count2 -= r_loleg_add ;
		 this->Huefte->getTranslation()->y = this->origPos + findBaseMove(langle_count,langle_count2,rangle_count,rangle_count2) ;
         frames-- ;
         if (frames == 0)
         {
            flag = 7 ;
            frames = FRAMES ;
         }
         break ;

      case 7 :

         l_upleg_add = this->array[2].r_upleg / FRAMES ;
       	r_upleg_add = this->array[2].l_upleg / FRAMES ;
         l_loleg_add = this->array[2].r_loleg / FRAMES ;
       	r_loleg_add = this->array[2].l_loleg / FRAMES ;

         l_uparm_add = this->array[2].r_uparm / FRAMES ;
       	l_loarm_add = this->array[2].r_loarm / FRAMES ;
         l_hand_add  = this->array[2].r_hand / FRAMES ;
       	r_uparm_add = this->array[2].l_uparm / FRAMES ;
         r_hand_add  = this->array[2].l_hand / FRAMES ;
         r_loarm_add = this->array[2].l_loarm / FRAMES ;

		 this->BeinL->getRotation()->angle += r_upleg_add;
		 this->BeinR->getRotation()->angle += l_upleg_add;
		 this->KnieL->getRotation()->angle += r_loleg_add;
		 this->KnieR->getRotation()->angle += l_loleg_add;

		 this->ArmL->getRotation()->angle += l_uparm_add;
		 this->EllbogenL->getRotation()->angle += l_loarm_add;
		 this->HandL->getRotation()->angle += l_hand_add;

 		 this->ArmR->getRotation()->angle += r_uparm_add;
		 this->EllbogenR->getRotation()->angle += r_loarm_add ;
		 this->HandR->getRotation()->angle += r_hand_add ;

		 
         langle_count  -= l_upleg_add ;
         langle_count2 -= l_loleg_add ;
         rangle_count  -= r_upleg_add ;
         rangle_count2 -= r_loleg_add ;
		 this->Huefte->getTranslation()->y = this->origPos + findBaseMove(langle_count,langle_count2,rangle_count,rangle_count2) ;
         frames-- ;
         if (frames == 0)
         {
            flag = 8 ;
            frames = FRAMES ;
         }
         break ;

      case 8 :

         l_upleg_add = this->array[3].r_upleg / FRAMES ;
       	r_upleg_add = this->array[3].l_upleg / FRAMES ;
         l_loleg_add = this->array[3].r_loleg / FRAMES ;
       	r_loleg_add = this->array[3].l_loleg / FRAMES ;

         l_uparm_add = this->array[3].r_uparm / FRAMES ;
       	l_loarm_add = this->array[3].r_loarm / FRAMES ;
         l_hand_add  = this->array[3].r_hand / FRAMES ;
       	r_uparm_add = this->array[3].l_uparm / FRAMES ;
         r_hand_add  = this->array[3].l_hand / FRAMES ;
         r_loarm_add = this->array[3].l_loarm / FRAMES ;

		 this->BeinL->getRotation()->angle += r_upleg_add;
		 this->BeinR->getRotation()->angle += l_upleg_add;
		 this->KnieL->getRotation()->angle += r_loleg_add;
		 this->KnieR->getRotation()->angle += l_loleg_add;

		 this->ArmL->getRotation()->angle += l_uparm_add;
		 this->EllbogenL->getRotation()->angle += l_loarm_add;
		 this->HandL->getRotation()->angle += l_hand_add;

 		 this->ArmR->getRotation()->angle += r_uparm_add;
		 this->EllbogenR->getRotation()->angle += r_loarm_add ;
		 this->HandR->getRotation()->angle += r_hand_add ;

		 
         langle_count  -= l_upleg_add ;
         langle_count2 -= l_loleg_add ;
         rangle_count  -= r_upleg_add ;
         rangle_count2 -= r_loleg_add ;
		 this->Huefte->getTranslation()->y = this->origPos + findBaseMove(langle_count,langle_count2,rangle_count,rangle_count2) ;
         frames-- ;
         if (frames == 0)
         {
            flag = 1 ;
            frames = FRAMES ;
         }
         break ;
      default :
         break ;
   }
}
/**/


