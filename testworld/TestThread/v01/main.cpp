/*****************************************************************
*																										    *
*									C U B E . C P P 												  *
*________________________________________________________________*
*																											*
*@autor					Tobias Harpering														 * 
*@desciption		  openGL programmier uebung für AiGinE
*	eine abgespeckte grundstrucktur für ein openGL- fenster.		   								*
*@version				v0.0.1																			  *
*																											*
*																											*
*****************************************************************/

/////////////////////////////////////////////////////////////////////////////////////////////
//								I N C L U D E										
//
/////////////////////////////////////////////////////////////////////////////////////////////
#include <iostream.h>
#include <gl/glut.h>
#include "Display.h"
#include "IOControl.h"									

////////////////////
//G l o b e l s
///////////////////

Display _displayObj;


/////////////////////////////////////////////////////////////////////////////////////////////
//										
//@autor			Tobias Harpering
//@desciption
//@param
//@return
////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////
//										M A I N
//@autor			Tobias Harpering
//@desciption	veruch die uebung objektorientiert zu gestallten	
//@param int, char** uebergabeparameter uebername
//@return int exit-status an das system	
////////////////////////////////////////////////////////////////////////////////////////////








/////////////////////////////////////////////////////////////////////////////////////////////
//									I N I T	
//@autor			Tobias Harpering
//@desciption
//@param
//@return
////////////////////////////////////////////////////////////////////////////////////////////



void init(void) {
	Display disObj = new Display();
	_displayObj = disObj;
	_displayObj.init();
}

/////////////////////////////////////////////////////////////////////////////////////////////
//									D I S P L A Y		
//@autor			Tobias Harpering
//@desciption
//@param
//@return
////////////////////////////////////////////////////////////////////////////////////////////

void display(void) {
	_displayObj.display();
	
}


void reshape (int w, int h) {
	
	
}

void keyboard(unsigned char key, int x, int y) {
	
	
	
}













/////////////////////////////////////////////////////////////////////////////////////////////
//								M A I N 										
//@autor			Tobias Harpering
//@desciption 
//@param
//@return
////////////////////////////////////////////////////////////////////////////////////////////


int main(int argc, char** argv)
{
	
	IOControl *control = new IOControl();
	glutInit(&argc, argv);
	// GLUT_DOUBLE fuer den double_buffer
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB| GLUT_DEPTH); //volle farbbereich | depth tiefenbuffer wird initiert

	glutInitWindowSize (600, 600);				//groesse des fensters
	glutInitWindowPosition (100, 100);			//position des fenster
	glutCreateWindow (argv[0]); //-> openGLFenster	//openGL -> Ausgabe / glut -> Eingabe
	init ();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();			   //-> kontrolle wrid an windows abgegeben und auf eingabe behandelt
	return 0;
}