/* Game Engine Design 
 * Vector3D.cpp  
 */

#include "Vector3D.h"
#include <iostream>
#include <math.h>

using namespace std;

//____________________________________________________________________________
Vector3D::Vector3D(float x, float y, float z){
	this->x = x;
	this->y = y;
	this->z = z;
}

//____________________________________________________________________________
Vector3D Vector3D::operator+(Vector3D vVector)
{
	return Vector3D(vVector.x + this->x, vVector.y + this->y, vVector.z + this->z);
}

//____________________________________________________________________________
Vector3D  Vector3D::operator-(Vector3D vVector)
{
	return Vector3D(this->x - vVector.x, this->y - vVector.y, this->z - vVector.z);
}
	
//____________________________________________________________________________
Vector3D  Vector3D::operator*(float num)
{
	return Vector3D(this->x * num, this->y * num, this->z * num);
}

//____________________________________________________________________________
Vector3D  Vector3D::operator/(float num)
{
	return Vector3D(this->x / num, this->y / num, this->z / num);
}

//____________________________________________________________________________
Vector3D Vector3D::cross(Vector3D * vector1, Vector3D * vector2)
{
  Vector3D temp = Vector3D(0.0, 0.0, 0.0);
  // bilde Vektorprodukt durch Berechnung der einzelnen Komponenten
  temp.x = ((vector1->y * vector2->z) - (vector1->z * vector2->y));
  temp.y = ((vector1->z * vector2->x) - (vector1->x * vector2->z));
  temp.z = ((vector1->x * vector2->y) - (vector1->y * vector2->x));
  return temp;
}

//____________________________________________________________________________
Vector3D Vector3D::getNormalVector(Vector3D vector)
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
float Vector3D::magnitude(Vector3D vector)
{
    // Here is the equation:  magnitude = sqrt(V.x^2 + V.y^2 + V.z^2) : Where V is the vector
    return (float)sqrt((vector.x * vector.x) + (vector.y * vector.y) +
       (vector.z * vector.z));
}


//____________________________________________________________________________
void Vector3D::print(){
	cout << "x: " << this->x << "   y: " << this->y << "   z: " << this->z  << endl;
}

//____________________________________________________________________________
void Vector3D::print(char* description){
	cout << description;
	print();
}

//____________________________________________________________________________
Vector3D::Vector3D()
{
  this->x = 0.0;
  this->y = 0.0;
  this->z = 0.0;
}
