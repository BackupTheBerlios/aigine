/* Generated by Together */

#include "ageG_Light.h"
#include "ageG_Translation3D.h"
#include "ageG_Vector3.h"
#include "ageG_GraphicPipeline.h"
#include "ageG_Translation3D.h"
#include "ageG_Vector3.h"
/**Std - Constructor
*@param GraphicPipeline um auf deren funktion f�r das setzten des lichtes zureuck zu greifen
*/
ageG_Light::ageG_Light(ageG_GraphicPipeline * lnkPipe) {
	
	this->lnkPipeline = lnkPipe;
	//this->attachViewport( new Viewport());
	this->setLookAtPosition(new Vector3D(0.0,0.0,0.0));
	this->setTranslation(new Translation3D(0.0,0.0,0.0));
	
	/**die GLUT - werte laufen nach den  verschiedenen licht-arten:
	*
	* GLenum light - GL_LIGHT0 bis GL_LIGHT7
	* GLenum pname - GL_AMBIENT, GL_DIFFUSE, GL_SPECULAR
	*/
	this->diffuse = new float[4];
	this->specular= new float[4];
	this->ambient= new float[4];
	this->pos_direct = 1;	// Positional light by default, if you not understand: don�n panic :-) ! tobi
}
/** Constructor with specialized param for position and LookAt
* @param GraphicPipeline um auf deren funktion f�r das setzten des lichtes zureuck zu greifen
*/
ageG_Light::ageG_Light(ageG_GraphicPipeline * lnkPipe,ageG_Translation3D* position, ageG_Vector3 * lookAt) {
	
	this->lnkPipeline = lnkPipe;

	this->setLookAtPosition(lookAt);

    //this->attachViewport(new Viewport());	
	this->setTranslation(position);

	
		
}
/**setzt und gibt die Art des Lichtes: 0 - directional ; 1 - positional */
int ageG_Light::getPos_direct() { return this->pos_direct;}
void ageG_Light::setPos_direct(int art) {
	this->pos_direct = art;
}
float * ageG_Light::getDiffuse()	{
	return diffuse; 
}
void ageG_Light::setDiffuse(float * value)	{
	this->diffuse = value;
}
/**
* Getter und Setter f�r die Attribute
*/ 
	
float ageG_Light::getCone(){ 
	return cone; 
}
void ageG_Light::setCone(float cone){ 
	this->cone = cone; 
}
float * ageG_Light::getSpecular()	{
	return specular; 
}
void ageG_Light::setSpecular(float * specular) {
	this->specular = specular; 
}
float * ageG_Light::getAmbient(){ 
	return ambient; 
}
void ageG_Light::setAmbient(float * ambient) { 
	this->ambient = ambient; 
}
/** Display f�r eine einheitliche handhabung*/
void ageG_Light::display(int index, string kind) {	
	lnkPipeline->enableLight(this, index);	
}
void ageG_Light::display() {	

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
ageG_Light::~ageG_Light() {
}
