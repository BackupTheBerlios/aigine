/* Game Engine Design */

#include "ageGM_Light.h"
class ageGV_GraphicPipeline;
class ageGM_Translation3D;
class ageGM_Vector3;

#define	BASEPOINT 0.0,0.0,0.0



/**Std - Constructor
*@param GraphicPipeline um auf deren funktion für das setzten des lichtes zureuck zu greifen
*/
ageGM_Light::ageGM_Light(ageGV_GraphicPipeline * lnkPipe) {
	
	this->lnkPipeline = lnkPipe;
	//this->attachViewport( new Viewport());
	this->setLookAtPosition(new Vector3D(BASEPOINT));
	this->setTranslation(new Translation3D(BASEPOINT));
	
	/**die GLUT - werte laufen nach den  verschiedenen licht-arten:
	*
	* GLenum light - GL_LIGHT0 bis GL_LIGHT7
	* GLenum pname - GL_AMBIENT, GL_DIFFUSE, GL_SPECULAR
	*/
	this->diffuse = new float[4];
	this->specular= new float[4];
	this->ambient= new float[4];
	this->pos_direct = 1;	// Positional light by default, if you not understand: don´n panic :-) ! tobi
}

/** Constructor with specialized param for position and LookAt
* @param GraphicPipeline um auf deren funktion für das setzten des lichtes zureuck zu greifen
*/
ageGM_Light::ageGM_Light(ageGV_GraphicPipeline * lnkPipe,ageGM_Translation3D* position, ageGM_Vector3 * lookAt) {
	
	this->lnkPipeline = lnkPipe;

	this->setLookAtPosition(lookAt);

    //this->attachViewport(new Viewport());	
	this->setTranslation(position);

	
		
}

/**
* Getter und Setter für die Attribute
*/ 
	
float ageGM_Light::getCone(){ 
	return cone; 
}

void ageGM_Light::setCone(float cone){ 
	this->cone = cone; 
}

float * ageGM_Light::getDiffuse()	{
	return diffuse; 
}


void ageGM_Light::setDiffuse(float * value)	{
	this->diffuse = value;
}

float * ageGM_Light::getSpecular()	{
	return specular; 
}

void ageGM_Light::setSpecular(float * specular) {
	this->specular = specular; 
}

float * ageGM_Light::getAmbient(){ 
	return ambient; 
}

void ageGM_Light::setAmbient(float * ambient) { 
	this->ambient = ambient; 
}

/**setzt und gibt die Art des Lichtes: 0 - directional ; 1 - positional */
int ageGM_Light::getPos_direct() { return this->pos_direct;}
void ageGM_Light::setPos_direct(int art) {
	this->pos_direct = art;
}

/** Display für eine einheitliche handhabung*/
void ageGM_Light::display(int index, string kind) {	
	lnkPipeline->enableLight(this, index);	
}

void ageGM_Light::display() {	

	float * lightGreen_defuse = new float[4];
	lightGreen_defuse[0] = 1.0;
	lightGreen_defuse[1] = 1.0;
	lightGreen_defuse[2] = 1.0;
	lightGreen_defuse[3] = 1.0;        // leichtes gruen	
	this->setDiffuse(lightGreen_defuse);  // leichtes gruen	

	float * ambient = new float[4];
	ambient[0] = 0.7;
	ambient[1] = 0.7;
	ambient[2] = 0.7;
	ambient[3] = 1.0; 
	this->setAmbient(ambient);
	this->display(1,"spot"); // spot leider noch nicht implementiert ..:-(

}

//Destructor
ageGM_Light::~ageGM_Light() {
}