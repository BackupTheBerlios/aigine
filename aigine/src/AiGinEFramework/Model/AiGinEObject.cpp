#include "AiGinEObject.h"
#include <iostream>
using namespace std;

AiGinEObject::AiGinEObject(): Object3D(), Knot() {
}

void AiGinEObject::display()
{
	cout << this << endl;
}
