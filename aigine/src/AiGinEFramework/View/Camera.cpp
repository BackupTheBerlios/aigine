/* Game Engine Design */

#include "Camera.h"
#include "../Model/Point3D.h"

Camera::Camera(){}

Point3D* Camera::getPosition(){ 
	return this->position; 
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
Camera::Camera(Point3D* position){
	this->position = position;
}
