/* Game Engine Design 
 * Vector3D.cpp  
 */

#include "ageGM_Vector3.h"
#include <iostream>
#include <math.h>

using namespace std;

//____________________________________________________________________________
ageGM_Vector3::ageGM_Vector3(float x, float y, float z){
	this->x = x;
	this->y = y;
	this->z = z;
}

//____________________________________________________________________________
ageGM_Vector3 ageGM_Vector3::operator+(ageGM_Vector3 vVector)
{
	return Vector3D(vVector.x + this->x, vVector.y + this->y, vVector.z + this->z);
}

//____________________________________________________________________________
ageGM_Vector3  ageGM_Vector3::operator-(ageGM_Vector3 vVector)
{
	return Vector3D(this->x - vVector.x, this->y - vVector.y, this->z - vVector.z);
}
	
//____________________________________________________________________________
ageGM_Vector3  ageGM_Vector3::operator*(float num)
{
	return Vector3D(this->x * num, this->y * num, this->z * num);
}
/**/
//____________________________________________________________________________
float ageGM_Vector3::skalarProdukt(ageGM_Vector3 vector) {

  cout << this->x << " * " << vector.x << " + " << endl;
  cout << this->y << " * " << vector.y << " + " << endl;
  cout << this->z << " * " << vector.z << "  " << endl;
  return (this->x * vector.x) + (this->y * vector.y) + (this->z * vector.z);  
}

//____________________________________________________________________________
ageGM_Vector3  ageGM_Vector3::operator/(float num)
{
	return Vector3D(this->x / num, this->y / num, this->z / num);
}

//____________________________________________________________________________
ageGM_Vector3 ageGM_Vector3::cross(ageGM_Vector3 * vector1, ageGM_Vector3 * vector2)
{
  Vector3D temp = Vector3D(0.0, 0.0, 0.0);
  // bilde Vektorprodukt durch Berechnung der einzelnen Komponenten
  temp.x = ((vector1->y * vector2->z) - (vector1->z * vector2->y));
  temp.y = ((vector1->z * vector2->x) - (vector1->x * vector2->z));
  temp.z = ((vector1->x * vector2->y) - (vector1->y * vector2->x));
  return temp;
}

//____________________________________________________________________________
ageGM_Vector3 ageGM_Vector3::getNormalVector(ageGM_Vector3 vector)
{
    // Get the magnitude of our normal
    float magn = magnitude(vector);

    // Now that we have the magnitude, we can divide our vector by that magnitude.
    // That will make our vector a total length of 1.
    vector = vector / magn;

    // Finally, return our normalized vector
    return vector;
}

//____________________________________________________________________________
float ageGM_Vector3::magnitude(ageGM_Vector3 vector)
{
    // Here is the equation:  magnitude = sqrt(V.x^2 + V.y^2 + V.z^2) : Where V is the vector
    return (float)sqrt((vector.x * vector.x) + (vector.y * vector.y) +
       (vector.z * vector.z));
}


//____________________________________________________________________________
void ageGM_Vector3::print(){
	cout << "x: " << this->x << "   y: " << this->y << "   z: " << this->z  << endl;
}

//____________________________________________________________________________

//____________________________________________________________________________
ageGM_Vector3::ageGM_Vector3()
{
  this->x = 0.0;
  this->y = 0.0;
  this->z = 0.0;
}
void ageGM_Vector3::print(char* description){
	cout << description;
	print();
}
