/* Game Engine Design */

#include "Light.h"

#define	BASEPOINT 0.0,0.0,0.0



/**Std - Constructor
*@param GraphicPipeline um auf deren funktion für das setzten des lichtes zureuck zu greifen
*/
Light::Light(GraphicPipeline * lnkPipe) {
	
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
Light::Light(GraphicPipeline * lnkPipe,Translation3D* position, Vector3D * lookAt) {
	
	this->lnkPipeline = lnkPipe;

	this->setLookAtPosition(lookAt);

    //this->attachViewport(new Viewport());	
	this->setTranslation(position);

	
		
}

/**
* Getter und Setter für die Attribute
*/ 
	
float Light::getCone(){ 
	return cone; 
}

void Light::setCone(float cone){ 
	this->cone = cone; 
}

float * Light::getDiffuse()	{
	return diffuse; 
}


void Light::setDiffuse(float * value)	{
	this->diffuse = value;
}

float * Light::getSpecular()	{
	return specular; 
}

void Light::setSpecular(float * specular) {
	this->specular = specular; 
}

float * Light::getAmbient(){ 
	return ambient; 
}

void Light::setAmbient(float * ambient) { 
	this->ambient = ambient; 
}

/**setzt und gibt die Art des Lichtes: 0 - directional ; 1 - positional */
int Light::getPos_direct() { return this->pos_direct;}
void Light::setPos_direct(int art) {
	this->pos_direct = art;
}

/** Display für eine einheitliche handhabung*/
void Light::display(int index, string kind) {	
	lnkPipeline->enableLight(this, index);	
}

void Light::display() {	

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
Light::~Light() {
}