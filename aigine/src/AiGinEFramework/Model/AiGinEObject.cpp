/* Game Engine Design */

#include "AiGinEObject.h"
class Rotation3D;
class Translation3D;

void AiGinEObject::setTranslation(Translation3D translation){ this->translation = translation; }
Translation3D& AiGinEObject::getTranslation(){ return translation; }
void AiGinEObject::setRotation(Rotation3D rotation){ this->rotation = rotation; }
Rotation3D& AiGinEObject::getRotation(){ return rotation; }
