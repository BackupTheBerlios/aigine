// IOControl.cpp: Implementierung der Klasse IOControl.
//
//////////////////////////////////////////////////////////////////////

#include "IOControl.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

IOControl::IOControl()
{

}

IOControl::~IOControl()
{

}

void IOControl::keyboard(unsigned char key, int x, int y)
{
	 switch (key) {
      case 27:
         exit(0);
         break;
	  case 'l':			//kybort eigenschaften: nach jeder eigenschaft (case) muss display wieder aufgerufen werden
						//um die szene neu zu zeichnen

		  _x += 10.0;
		  display();

		  break;
	  case 'k':
			_x -= 10.0;
			display();
			break;
	  case 'o':

		  _y += 10.0;
		  display();

		  break;
	  case 'i':

		  _z += 10.0;
		  display();

		  break;

   }

}
