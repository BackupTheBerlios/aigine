/* Game Engine Design */

#include "ageGM_Vector2.h"








//____________________________________________________________________________
/**
* Constructor
*/
ageGM_Vector2::ageGM_Vector2()
{
  this->x = 0.0;
  this->y = 0.0;
  
}

//____________________________________________________________________________
/**
* Constructor
*/
ageGM_Vector2::ageGM_Vector2(float x, float y  ){
	this->x = x;
	this->y = y;
	
	
}


ageGM_Vector2 ageGM_Vector2::operator/(float num)
{
	return Vector2D(this->x / num, this->y / num);
}


//____________________________________________________________________________
ageGM_Vector2 ageGM_Vector2::operator*(float num)
{
	return Vector2D(this->x * num, this->y * num);
}

//____________________________________________________________________________
ageGM_Vector2  ageGM_Vector2::operator-(ageGM_Vector2 vVector)
{
	return Vector2D(this->x - vVector.x, this->y - vVector.y);
}

//____________________________________________________________________________
ageGM_Vector2 ageGM_Vector2::operator+(ageGM_Vector2 vVector)
{
	return Vector2D(vVector.x + this->x, vVector.y + this->y);
}

