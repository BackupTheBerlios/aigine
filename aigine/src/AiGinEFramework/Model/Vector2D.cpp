/* Game Engine Design */

#include "Vector2D.h"








//____________________________________________________________________________
/**
* Constructor
*/
Vector2D::Vector2D()
{
  this->x = 0.0;
  this->y = 0.0;
  
}

//____________________________________________________________________________
/**
* Constructor
*/
Vector2D::Vector2D(float x, float y  ){
	this->x = x;
	this->y = y;
	
	
}


Vector2D Vector2D::operator/(float num)
{
	return Vector2D(this->x / num, this->y / num);
}


//____________________________________________________________________________
Vector2D Vector2D::operator*(float num)
{
	return Vector2D(this->x * num, this->y * num);
}

//____________________________________________________________________________
Vector2D  Vector2D::operator-(Vector2D vVector)
{
	return Vector2D(this->x - vVector.x, this->y - vVector.y);
}

//____________________________________________________________________________
Vector2D Vector2D::operator+(Vector2D vVector)
{
	return Vector2D(vVector.x + this->x, vVector.y + this->y);
}

