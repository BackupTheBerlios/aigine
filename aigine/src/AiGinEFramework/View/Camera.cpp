/* Game Engine Design */

#include "Camera.h"
#include "../Model/Point3D.h"

Camera::Camera(){}

Point3D* Camera::getPosition(){ 
	return this->position; 
}

Point3D * Camera::getLookAtPosition() {
	return this->lookAtPosition;
}

void Camera::setLookAtPosition(Point3D * lookAtPosition){
	this->lookAtPosition= lookAtPosition;
}

Point3D * Camera::getNormalVector(){
	return this->normalVector;
}
void Camera::setNormalVector(Point3D * normal){
	this->normalVector = normal;
}
void Camera::moveRight(){}
void Camera::moveLeft(){}
void Camera::moveUp(){}
void Camera::moveDown(){
  this->position->setY(this->position->getY() - 1);
}
void Camera::turnUp(){}
void Camera::turnDown(){}
void Camera::turnRight(){}
void Camera::turnLeft(){}
	
void Camera::attachViewport(Viewport* param){}
void Camera::set(){}
Camera::Camera(Point3D* position, Point3D* lookAtPosition ,Point3D * normalVector){
	this->position = position;
	this->setLookAtPosition(lookAtPosition);
	this->setNormalVector(normalVector);
}
