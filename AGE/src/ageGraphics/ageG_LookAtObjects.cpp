/* Generated by Together */

#include "ageG_LookAtObjects.h"
#include <math.h>
#include "ageG_Vector3.h"
#include "ageG_Vector3.h"
ageG_LookAtObjects::ageG_LookAtObjects() : AiGinEObject() {

}
ageG_LookAtObjects::ageG_LookAtObjects(ageG_Vector3 * lookAt) : AiGinEObject() {
	this->lookAtPosition = lookAt;

}
/**
*/
ageG_Vector3 * ageG_LookAtObjects::getLookAtPosition() {
	return lookAtPosition; 
}
/**
*/
void ageG_LookAtObjects::setLookAtPosition(ageG_Vector3 * lookAtPosition){

	this->lookAtPosition = lookAtPosition; 
}
/**
* Rotation des gesamten Objectes in Bezug auf sein LookAt. D.h. es wird 
* rund um eine logische 'BoundigsSphere gedreht super h�ftig Vectorrechnung
*/

void ageG_LookAtObjects::rotateView(float angle, float x, float y, float z)  {
    Vector3D vNewView = Vector3D(0.0, 0.0, 0.0);
    // Get the view vector (The direction we are facing)	
    Vector3D vView = * lookAtPosition - * this->getTranslation();
    // Calculate the sine and cosine of the angle once
    float cosTheta = (float)cos(angle);
    float sinTheta = (float)sin(angle);

    // Find the new x position for the new rotated point
    vNewView.x = (cosTheta + (1 - cosTheta) * x * x) * vView.x;
    vNewView.x += ((1 - cosTheta) * x * y - z * sinTheta) * vView.y;
    vNewView.x += ((1 - cosTheta) * x * z + y * sinTheta) * vView.z;

    // Find the new y position for the new rotated point
    vNewView.y = ((1 - cosTheta) * x * y + z * sinTheta) * vView.x;
    vNewView.y += (cosTheta + (1 - cosTheta) * y * y) * vView.y;
    vNewView.y += ((1 - cosTheta) * y * z - x * sinTheta) * vView.z;

    // Find the new z position for the new rotated point
    vNewView.z = ((1 - cosTheta) * x * z - y * sinTheta) * vView.x;
    vNewView.z += ((1 - cosTheta) * y * z + x * sinTheta) * vView.y;
    vNewView.z += (cosTheta + (1 - cosTheta) * z * z) * vView.z;

    // Now we just add the newly rotated vector to our position to set
    // our new rotated view of our camera.
    * lookAtPosition = * this->getTranslation() + vNewView;
}