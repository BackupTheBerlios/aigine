/* Game Engine Design */

#include <math.h>

#include "Camera.h"
#include "../Input/Mouse.h"
#include "../Model/Point3D.h"
#include "../Model/Vector3D.h"

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

Point3D * Camera::getRotation(){ return rotation; }

/**
*	Set the Rotation of the Camera controled with the Mouse (Shooter Style)
*/
void Camera::setRotation(Point3D * rotation, int mouseX, int mouseY, int* CurrentWinSize)	{
	//Mouse * mouse = new Mouse();
	POINT mousePos;
	
	int middleX = CurrentWinSize[0]  >> 1;				// This is a binary shift to get half the width
	int middleY = CurrentWinSize[1]  >> 1;				// This is a binary shift to get half the height
	float angleY = 0.0f;							// This is the direction for looking up or down
	float angleZ = 0.0f;							// This will be the value we need to rotate around the Y axis (Left and Right)
	static float currentRotX = 0.0f;
	

	// Get the mouse's current X,Y position
	GetCursorPos(&mousePos);
	// If our cursor is still in the middle, we never moved... so don't update the screen
	if( (mousePos.x == middleX) && (mousePos.y == middleY) ) return;
	

	cout<< "-------------------------------------------------------------------------------------"<<endl;
	cout <<"current mouse : "<< mousePos.x<<" | "<<mousePos.y<<endl;
	

	// Set the mouse position to the middle of our window
	SetCursorPos(middleX, middleY);


	// Get the direction the mouse moved in, but bring the number down to a reasonable amount
	angleY = (float)( (middleX - mousePos.x) ) / 1000.0f;		
	angleZ = (float)( (middleY - mousePos.y) ) / 1000.0f;	

	// Here we keep track of the current rotation (for up and down) so that
	// we can restrict the camera from doing a full 360 loop.
	currentRotX -= angleZ;


	// If the current rotation (in radians) is greater than 1.0, we want to cap it.
	if(currentRotX > 1.0f)
		currentRotX = 1.0f;
	// Check if the rotation is below -1.0, if so we want to make sure it doesn't continue
	else if(currentRotX < -1.0f)
		currentRotX = -1.0f;
	// Otherwise, we can rotate the view around our position
	else
	{
		// To find the axis we need to rotate around for up and down
		// movements, we need to get a perpendicular vector from the
		// camera's view vector and up vector.  This will be the axis.
		//Point3D vAxis = Cross(m_vView - m_vPosition, m_vUpVector);
		Point3D normal = Point3D(rotation->getX() , rotation->getY(), rotation->getZ());
		Point3D temp = *lookAtPosition - *position;	
		normal = cross(&temp, rotation);
			
		normal = getNormalVector(normal);

		// Rotate around our perpendicular axis and along the y-axis
		RotateView(angleZ, normal.getX(), normal.getY(), normal.getZ());
		RotateView(angleY, 0, 1, 0);
		rotation->print("rotation_tobi : ");
	}



	// aus den Winkeln eine Vektor machen
	//Point3D * vAxis = cross(m_vView - m_vPosition, m_vUpVector);
	//Point3D normal = Point3D(rotation->getX() , rotation->getY(), rotation->getZ());
	//Point3D temp = Point3D(rotation->getX() - 1 , rotation->getY() - 1, rotation->getZ() - 1);
	
	// operatoren test
	/////////////////////////////////////////////
	/*	normal = normal + temp;
		normal.print("addtion = ");
		normal = normal - temp;
		normal.print("subtraktion = ");
		normal = normal * 5;
		normal.print("multiplikation = ");
		normal = normal / 10;
		normal.print("division = ");
	/////////////////////////////////////////////

	temp = *lookAtPosition - *position;	
	normal = cross(&temp, rotation);
	temp.print("temp :   ");
	rotation->print("rotation :   ");
	normal.print("cross (temp, rotation:  ");
	normal = getNormalVector(normal);
	normal.print("normalize:  ");

	this->rotation->setX(normal.getZ());
	this->rotation->setY(normal.getX());
	this->rotation->setZ(normal.getY());
*/
}



/////////////////////////////////////// CROSS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns a perpendicular vector from 2 given vectors by taking the cross product.
/////
/////////////////////////////////////// CROSS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
												
Point3D Camera::cross(Point3D * vVector1, Point3D * vVector2)
{
	Point3D vNormal = Point3D(0.0, 0.0, 0.0);	

	// Calculate the cross product with the non communitive equation
	vNormal.x = ((vVector1->y * vVector2->z) - (vVector1->z * vVector2->y));
	vNormal.y = ((vVector1->z * vVector2->x) - (vVector1->x * vVector2->z));
	vNormal.z = ((vVector1->x * vVector2->y) - (vVector1->y * vVector2->x));

	// Return the cross product
	return vNormal;										 
}
  


  
/////////////////////////////////////// MAGNITUDE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns the magnitude of a vector
/////
/////////////////////////////////////// MAGNITUDE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

float Camera::magnitude(Point3D vNormal)
{
	// Here is the equation:  magnitude = sqrt(V.x^2 + V.y^2 + V.z^2) : Where V is the vector
	return (float)sqrt( (vNormal.x * vNormal.x) + 
						(vNormal.y * vNormal.y) + 
						(vNormal.z * vNormal.z) );
}



/////////////////////////////////////// NORMALIZE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns a normalize vector (A vector exactly of length 1)
/////
/////////////////////////////////////// NORMALIZE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

Point3D Camera::getNormalVector(Point3D vVector)
{
	// Get the magnitude of our normal
	float magn = magnitude(vVector);				

	// Now that we have the magnitude, we can divide our vector by that magnitude.
	// That will make our vector a total length of 1.  
	vVector = vVector / magn;		
	
	// Finally, return our normalized vector
	return vVector;										
}


///////////////////////////////// ROTATE VIEW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This rotates the view around the position using an axis-angle rotation
/////
///////////////////////////////// ROTATE VIEW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Camera::RotateView(float angle, float x, float y, float z)
{
	Point3D vNewView = Point3D(0.0,0.0,0.0);;

	// Get the view vector (The direction we are facing)
	//Point3D vView = m_vView - m_vPosition;		
	Point3D vView  = *lookAtPosition - *position;
	// Calculate the sine and cosine of the angle once
	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);

	// Find the new x position for the new rotated point
	vNewView.x  = (cosTheta + (1 - cosTheta) * x * x)		* vView.x;
	vNewView.x += ((1 - cosTheta) * x * y - z * sinTheta)	* vView.y;
	vNewView.x += ((1 - cosTheta) * x * z + y * sinTheta)	* vView.z;

	// Find the new y position for the new rotated point
	vNewView.y  = ((1 - cosTheta) * x * y + z * sinTheta)	* vView.x;
	vNewView.y += (cosTheta + (1 - cosTheta) * y * y)		* vView.y;
	vNewView.y += ((1 - cosTheta) * y * z - x * sinTheta)	* vView.z;

	// Find the new z position for the new rotated point
	vNewView.z  = ((1 - cosTheta) * x * z - y * sinTheta)	* vView.x;
	vNewView.z += ((1 - cosTheta) * y * z + x * sinTheta)	* vView.y;
	vNewView.z += (cosTheta + (1 - cosTheta) * z * z)		* vView.z;

	// Now we just add the newly rotated vector to our position to set
	// our new rotated view of our camera.
	Point3D point_tmp = Point3D(this->getPosition()->x, this->getPosition()->y,this->getPosition()->z);
	point_tmp = point_tmp + vNewView;
	this->setLookAtPosition(&point_tmp);
}





void Camera::moveRight(){}
void Camera::moveLeft(){}
void Camera::moveUp(){}
void Camera::moveDown(){
  this->position->setY(this->position->getY() - 1);
}
void Camera::moveForward(){
this->position->setZ(this->position->getZ() +1);
}
void Camera::moveBack(){}
void Camera::turnUp(){}
void Camera::turnDown(){}
void Camera::turnRight(){}
void Camera::turnLeft(){}
	
void Camera::attachViewport(Viewport* param){}
void Camera::set(){}
Camera::Camera(Point3D* position, Point3D* lookAtPosition ,Point3D * rotation){
	this->position = position;
	this->lookAtPosition = lookAtPosition;
	this->rotation = rotation;	
}
