/* Game Engine Design 
 * Camera.cpp  
 */

#include <math.h>
#include "Camera.h"





//____________________________________________________________________________
Camera::Camera() {
    this->upVector = new Vector3D(0.0, 1.0, 0.0);
}

//____________________________________________________________________________
Camera::Camera(Vector3D * position, Vector3D * lookAtPosition,
   Vector3D * upVector)
   {
	Translation3D * pos_tmp = (Translation3D*) position; 
       this->setTranslation( pos_tmp ); 
       this->setLookAtPosition( lookAtPosition);
       this->upVector =upVector ;
}

//____________________________________________________________________________


void Camera::setRotation(int mouseX, int mouseY, int * winSize)
{
    // Mittelpunkt des Fensters mit binärem Shift ermitteln
    int middleX = winSize[0] >> 1; // x / 2 = middle x
    int middleY = winSize[1] >> 1;
    // Winkel für oben/unten Achse  0.5 = 180 Grad Drehung
    float angleY = 0.0f;
    // Winkel für rechts/links Achse
    float angleZ = 0.0f;
    static float currentRotX = 0.0f;

    // wenn die Maus im Mittelpunkt ist, hat sich nichts getan => ENDE
    if ((mouseX == middleX) && (mouseY == middleY)) return;

    // setze den Mauszeiger auf den Mittelpunkt des Fensters
    glutWarpPointer(middleX, middleY);

    // Differenz zwischen Mittelpunkt und Mausposition ermitteln und
    // auf eine Wert zwischen 0 und 1 bringen.
    // TODO: Teiler für Steuerung der Mausempfindlichkeit nutzen
    angleY = (float)((middleX - mouseX)) / 1000.0f;
    angleZ = (float)((middleY - mouseY)) / 1000.0f;

    // Zwischenspeichern der z Rotation
    // Here we keep track of the current rotation (for up and down) so that
    // we can restrict the camera from doing a full 360 loop.
    currentRotX -= angleZ;

    // If the current rotation (in radians) is greater than 1.0, we want to cap it.
    if (currentRotX > 1.0f) currentRotX = 1.0f;
    // Check if the rotation is below -1.0, if so we want to make sure it doesn't continue
    else if (currentRotX < -1.0f) currentRotX = -1.0f;
    // Otherwise, we can rotate the view around our position
    else
    {
        // Um den Vektor für die Drehung um die z - Achse zu ermitteln
        // wird das Kreuzprodukt des Kamerarichtungsvektor = Blickpunkt - Position
        // und des Normalenvektors der Kamera gebildet.
        Vector3D temp = * this->getLookAtPosition() - * getTranslation();
//        Vector3D zAxis = Vector3D::cross(& temp, upVector);
        Vector3D zAxis = Vector3DCross(temp, *upVector);

//        zAxis = Vector3D::getNormalVector(zAxis);
        zAxis = Vector3DNormalize(zAxis);

        // Rotation um die beiden Achsen
        this->rotateView(angleZ, zAxis.x, zAxis.y, zAxis.z);
        this->rotateView(angleY, 0, 1, 0); // yAxis

        // Ausgabe der aktuellen Positionen
        this->getTranslation()->print("\ngetTranslation() : ");
        this->getLookAtPosition()->print("lookAtPosition : ");
    }
}

//____________________________________________________________________________


//____________________________________________________________________________
void Camera::moveCamera(float speed)
{
    // Get the current view vector (the direction we are looking)
    Vector3D vector = * this->getLookAtPosition() - * getTranslation();

    // I snuck this change in here!  We now normalize our view vector when
    // moving throughout the world.  This is a MUST that needs to be done.
    // That way you don't move faster than you strafe, since the strafe vector
    // is normalized too.
//    vector = Vector3D::getNormalVector(vector);
     vector = Vector3DNormalize(vector);

    // setze Kameraposition 
    getTranslation()->x = getTranslation()->x + vector.x * speed;
    getTranslation()->z = getTranslation()->z + vector.z * speed;
    // setze Blickpunkt
    this->getLookAtPosition()->x = this->getLookAtPosition()->x + vector.x * speed; 
    this->getLookAtPosition()->z = this->getLookAtPosition()->z + vector.z * speed; 
}

//____________________________________________________________________________
void Camera::strafeCamera(float speed)
{
    Vector3D temp = * this->getLookAtPosition() - *getTranslation();
    // Initialize a variable for the cross product result
//    Vector3D strafe = Vector3D::cross(& temp, upVector);
    Vector3D strafe = Vector3DCross(temp, *upVector);

    // Normalize the strafe vector
//    strafe = Vector3D::getNormalVector(strafe);
    strafe = Vector3DNormalize(strafe);

    // Strafing is quite simple if you understand what the cross product is.
    // If you have 2 vectors (say the up vVector and the view vVector) you can
    // use the cross product formula to get a vVector that is 90 degrees from
    // the 2 vectors.
    // For a better explanation on how this works, check out the OpenGL
    // "Normals" tutorial at our site.
    // In our new Update() function, we set the strafing vector (m_vStrafe).  Due
    // to the fact that we need this vector for many things including the strafing
    // movement and camera rotation (up and down), we just calculate it once.
    //
    // Like our MoveCamera() function, we add the strafing vector to our current position
    // and view.  It's as simple as that.  It has already been calculated in Update().

    // Add the strafe vector to our position
    getTranslation()->x = getTranslation()->x + strafe.x * speed;
    getTranslation()->z = getTranslation()->z + strafe.z * speed;

    // Add the strafe vector to our view
    this->getLookAtPosition()->x = this->getLookAtPosition()->x + strafe.x * speed;
    this->getLookAtPosition()->z = this->getLookAtPosition()->z + strafe.z * speed;
}



//____________________________________________________________________________
void Camera::moveRight(float speed)
{
    strafeCamera(speed);
}

//____________________________________________________________________________
void Camera::moveLeft(float speed)
{
    strafeCamera(-speed);
}

//____________________________________________________________________________
void Camera::moveUp(float speed) 
{ 
    getTranslation()->y = getTranslation()->y + speed;
}

//____________________________________________________________________________
void Camera::moveDown(float speed)
{
    getTranslation()->y = getTranslation()->y - speed;
}

//____________________________________________________________________________
void Camera::moveForward(float speed)
{
    moveCamera(speed);

}

//____________________________________________________________________________
void Camera::moveBack(float speed)
{
    moveCamera(-speed);
}
//____________________________________________________________________________
void Camera::setViewFogMode(bool trigger, float red, float green, float blue,
							float alpha, float density, float start, float end, GLint fogmode, 
							GLint glhint_target,GLint glhint_mode ) {

	// Nebel:   zuerst Nebelfarbe im RGBA Format also {R,G,B,alpha} -->z.Z.Schwarz
	float fogColor[4] = {red,green,blue,alpha};			

	/* : 
	fog mode:The params parameter is a
	single integer or floating-point value that specifies the equation to be
	used to compute the fog blend factor, f. Three symbolic constants are 
	accepted: GL_LINEAR, GL_EXP, and GL_EXP2. 
	The equations corresponding to these symbolic constants are defined in the 
	following Remarks section. The default fog mode is GL_EXP
	*/
	glFogi(GL_FOG_MODE,fogmode);
	
	// unser fogColor wird uebergeben
	glFogfv(GL_FOG_COLOR, fogColor);				

	// Dichte :default ist 1.0 
	glFogf(GL_FOG_DENSITY,density);				

	/* Die Qualitaet des Nebels parameter siehe camera.h 
	*/
	glHint(glhint_target,glhint_mode);					

	// der Anfang des Nebels von der Camera aus!
	glFogf(GL_FOG_START,start);							

	// "das Ende des Sichtbaren Bereiches" e.g. bei 30 beginnt der Nebel und ab 60 ist nichts mehr zusehen
	glFogf(GL_FOG_END, end);						
	
	// und Feuer --> zum abschalten glDisable(GL_FOG);
	if (trigger == true){	glEnable(GL_FOG);}
	else {glDisable(GL_FOG);};
	
}
//____________________________________________________________________________
Vector3D * Camera::getUpVector() { 
	return upVector;
}
//____________________________________________________________________________
/**
 * @author
 * @return
 * @param
 */
//void Camera::attachViewport(Viewport * param) { }

/**
 * @author
 * @return
 * @param
 */
//void Camera::set() { }

/**
 * Destruktor
 * @author
 * @return
 * @param
 */
Camera::~Camera() { }

